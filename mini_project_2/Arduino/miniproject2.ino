#define DEBUG
//#define DEBUG_WIFI

#include <WiFiEsp.h>
#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <Wire.h>
#include <DHT.h>

#define AP_SSID "iot1"
#define AP_PASS "iot10000"
#define SERVER_NAME "10.10.141.73"
#define SERVER_PORT 5000
#define LOGID "SNR_ARD"
#define PASSWD "PASSWD"

#define DHTPIN 4
#define WIFIRX 8
#define WIFITX 7
#define POWER_PIN 2
#define SIGNAL_PIN A0

#define SENSOR_MIN 0
#define SENSOR_MAX 521

#define CMD_SIZE 50
#define ARR_CNT 5
#define DHTTYPE DHT11

bool timerIsrFlag = false;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;
boolean cdsFlag = false;

char sendId[10] = "SNR_ARD";
char sendBuf[CMD_SIZE];
char lcdLine1[50];
char lcdLine2[50];

int cds = 0;
unsigned int secCount;
unsigned int myservoTime = 0;

char getSensorId[10];
int sensorTime;
float temp = 0.0;
float humi = 0.0;
int value = 0;
int level = 0;
bool updateTimeFlag = false;

typedef struct {
  int year;
  int month;
  int day;
  int hour;
  int min;
  int sec;
} DATETIME;
DATETIME dateTime = { 0, 0, 0, 12, 0, 0 };
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial wifiSerial(WIFIRX, WIFITX);
WiFiEspClient client;

void setup() {
  Serial.begin(115200);  // 시리얼 모니터용
  wifi_Setup();
  
  MsTimer2::set(1000, timerIsr);  // 1000ms period
  MsTimer2::start();
  pinMode(POWER_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  if (client.available()) {
    socketEvent();
  }

  if (timerIsrFlag) {
    timerIsrFlag = false;
    if (!(secCount % 5)) {
      digitalWrite(POWER_PIN, HIGH);
      value = analogRead(SIGNAL_PIN);
      level = map(value, SENSOR_MIN, SENSOR_MAX, 0, 10); // 4 단계
      digitalWrite(POWER_PIN, LOW);

      humi = dht.readHumidity();
      temp = dht.readTemperature();

      Serial.print("Temperature: ");
      Serial.print(temp);
      Serial.print(" C\tHumidity: ");
      Serial.print(humi);
      Serial.print(" %\tWater Level: ");
      Serial.println(level);

      snprintf(lcdLine2, sizeof(lcdLine2), "T:%2d H:%2d W:%d\n", (int)temp, (int)humi, (int)level); // 강수량까지 추가한 것
      Serial.println(lcdLine2);

      if (!client.connected()) {
        Serial.println("Server Down");
        server_Connect();
      }
    }

    if (sensorTime != 0 && !(secCount % sensorTime)) {
      char tempStr[5];
      char humiStr[5];
      dtostrf(humi, 4, 1, humiStr);
      dtostrf(temp, 4, 1, tempStr);
      // snprintf(sendBuf, sizeof(sendBuf), "[SNR_LIN]%s@%s@%s@%d\n", getSensorId, tempStr, humiStr, level);
      // client.write(sendBuf, strlen(sendBuf));
      // client.flush();
      // snprintf(sendBuf, sizeof(sendBuf), "[SNR_SQL]%s@%s@%s@%d\n", getSensorId, tempStr, humiStr, level);
      // client.write(sendBuf, strlen(sendBuf));
      // client.flush();
      snprintf(sendBuf, sizeof(sendBuf), "[ALLMSG]%s@%s@%s@%d\n", getSensorId, tempStr, humiStr, level);
      client.write(sendBuf, strlen(sendBuf));
      client.flush();
    }
    snprintf(lcdLine1, sizeof(lcdLine1), "%02d.%02d  %02d:%02d:%02d", dateTime.month, dateTime.day, dateTime.hour, dateTime.min, dateTime.sec);
    Serial.println(lcdLine1);
    if (updateTimeFlag) {
      client.print("[GETTIME]\n");
      updateTimeFlag = false;
    }
  }
}

void socketEvent() {
  int i = 0;
  char *pToken;
  char *pArray[ARR_CNT] = { 0 };
  char recvBuf[CMD_SIZE] = { 0 };
  int len;

  sendBuf[0] = '\0';
  len = client.readBytesUntil('\n', recvBuf, CMD_SIZE);
  client.flush();
  Serial.print("recv : ");
  Serial.println(recvBuf);
  
  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL) {
    pArray[i] = pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }

  if ((strlen(pArray[1]) + strlen(pArray[2])) < 16) {
    snprintf(lcdLine2, sizeof(lcdLine2), "%s %s", pArray[1], pArray[2]);
    Serial.println(lcdLine2);
  }
  if (!strncmp(pArray[1], " New", 4))  // New Connected
  {
    Serial.println("Server Connected");
    updateTimeFlag = true;
    return;
  } else if (!strncmp(pArray[1], " Alr", 4)) {
    client.stop();
    server_Connect();
    return;
  }
  else if (!strncmp(pArray[1], "GETSENSOR", 9)) {
    if (pArray[2] != NULL) {
      sensorTime = atoi(pArray[2]);
      strcpy(getSensorId, pArray[0]);
      return;
    } else {
      sensorTime = 0;
      snprintf(sendBuf, sizeof(sendBuf), "[ALLMSG]%s@%d@%d@%d\n", getSensorId, (int)temp, (int)humi, level);
      client.write(sendBuf, strlen(sendBuf));
      client.flush();
      snprintf(sendBuf, sizeof(sendBuf), "[%s]%s@%d@%d@%d\n", pArray[0], pArray[1], (int)temp, (int)humi, level);
    }
  } else if (!strcmp(pArray[0], "GETTIME")) {
    dateTime.year = (pArray[1][0] - '0') * 10 + pArray[1][1] - '0';
    dateTime.month = (pArray[1][3] - '0') * 10 + pArray[1][4] - '0';
    dateTime.day = (pArray[1][6] - '0') * 10 + pArray[1][7] - '0';
    dateTime.hour = (pArray[1][9] - '0') * 10 + pArray[1][10] - '0';
    dateTime.min = (pArray[1][12] - '0') * 10 + pArray[1][13] - '0';
    dateTime.sec = (pArray[1][15] - '0') * 10 + pArray[1][16] - '0';
    Serial.print("Time ");
    Serial.print(dateTime.year);
    Serial.print('.');
    Serial.print(dateTime.month);
    Serial.print('.');
    Serial.print(dateTime.day);
    Serial.print(' ');
    Serial.print(dateTime.hour);
    Serial.print(':');
    Serial.print(dateTime.min);
    Serial.print(':');
    Serial.println(dateTime.sec);
    return;
  } else
    return;
  client.write(sendBuf, strlen(sendBuf));
  client.flush();

  Serial.print(", send : ");
  Serial.println(sendBuf);
}

void timerIsr() {
  timerIsrFlag = true;
  secCount++;
  clock_calc(&dateTime);
}

void clock_calc(DATETIME *dateTime) {
  dateTime->sec++;

  if (dateTime->sec >= 60) {
    dateTime->sec = 0;
    dateTime->min++;

    if (dateTime->min >= 60) {
      dateTime->min = 0;
      dateTime->hour++;
      if (dateTime->hour == 24) {
        dateTime->hour = 0;
        updateTimeFlag = true;
      }
    }
  }
}

void wifi_Setup() {
  wifiSerial.begin(38400);
  wifi_Init();
  server_Connect();
}

void wifi_Init() {
  do {
    WiFi.init(&wifiSerial);
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
    } else
      break;
  } while (1);

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(AP_SSID);
  
  while (WiFi.begin(AP_SSID, AP_PASS) != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(AP_SSID);
  }

  Serial.print("ID: ");
  Serial.println(LOGID);
  Serial.print("IP Address: ");
  IPAddress ip = WiFi.localIP();
  Serial.print(ip[0]);
  Serial.print(".");
  Serial.print(ip[1]);
  Serial.print(".");
  Serial.print(ip[2]);
  Serial.print(".");
  Serial.println(ip[3]);
}

int server_Connect() {
  Serial.println("Starting connection to server...");
  if (client.connect(SERVER_NAME, SERVER_PORT)) {
    Serial.println("Connected to server");
    client.print("[" LOGID ":" PASSWD "]");
  } else {
    Serial.println("Server connection failure");
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
