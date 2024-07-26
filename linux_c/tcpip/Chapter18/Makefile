CC=gcc

TARGET_SRV=chat_serv
TARGET_CLN=chat_clnt

LDFLAGS=-lpthread

all : $(TARGET_SRV) $(TARGET_CLN)

$(TARGET_SRV):$(TARGET_SRV).o
	$(CC) -o $@ $< $(LDFLAGS)

$(TARGET_CLN):$(TARGET_CLN).o
	$(CC) -o $@ $< $(LDFLAGS)

%.o:%.c
	$(CC) -c -o $@ $<

clean:
	rm -f *.o $(TARGET_SRV) $(TARGET_CLN)
