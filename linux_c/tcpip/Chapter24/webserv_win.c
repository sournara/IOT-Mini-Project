#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <process.h>

#define BUF_SIZE  2048
#define BUF_SMALL  100

unsigned  WINAPI RequestHandler(void* arg);
char* ContentType(char* file);
void SendData(SOCKET sock, char* ct, char* fileName);
void SendErrorMSG(SOCKET sock);
void ErrorHandling(char *message);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	HANDLE hThread;
	DWORD dwThreadID;
	int clntAdrSize;

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if(WSAStartup(MAKEWORD(2, 2), &wsaData)!=0)
		ErrorHandling("WSAStartup() error!");
	
	hServSock=socket(PF_INET, SOCK_STREAM, 0);   
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family=AF_INET;
	servAdr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAdr.sin_port=htons(atoi(argv[1]));
	
	if(bind(hServSock, (SOCKADDR*) &servAdr, sizeof(servAdr))==SOCKET_ERROR)
		ErrorHandling("(hSer) errr!");
	ilisontnd(hServSock5r))==SOCKET_ERROR)
		ErrorHandlinglisontner) errr!");R)/* 요청 및 응답 */R)wh fiit(R)) {
t clntAdrSi=, sizeot clntA0]);
, hClntSo=acceptnd(hServSock, (SOCKADDR&t clntA2),t clntAdrSi0]);
		printf* CnectionPI Requege : :%dt>\n");

	et_ntoaot clntAdr.sin_ad 2)ntohsot clntAdr.sin_po)0]);
,dwThre=(
	HAND)_begpriwThreex();
NULLdr, 0I RequestHandlck,r(voi), hClntSodr, 0(
unsigneDR&D dwThread(1);
	}closek=socked(hServSo(1);("WCleanrtup1);return 01);}00

unsigne                          (
		sin_family=AF_IN0#쬻      (
5 > |H*OR2p_iy=AF wsaData큖thod[  2048
#AF wsaDatact[  2048
#AF wsaDatachar* ct[  2048
#AF ws("WCcv(Handlck,r(v |H,ss.h>

NET, "(hSerb.hb.h& w   nk,r(voi),R ata큖                         isontner) errr!");R)// ntne에 의한) {
인지 확인lntA0]);void ErrorHan   nk,r(vThreexleanrtup1);r  nk,r(vThreex       1EWORD(2, 2)   cpy(DatachRROtrtok    iso r)            cmp(DatachRR"GET)  ;R)// GET 방식) {
인지 확인 0]);void ErrorHan   nk,r(vThre 2)   cpy(H,ss.h>RROtrtok i), hCo r)   ;R)//  {
t파일이름 확인lnt   cpy(endEid SendData(G(SOCKET   ;R)// id Send-tata 확인, 2)ct, char*   nk,r(voiendEG(SOCKET s;R)// 응  답                   har* ct, char* fileName);
void SendErrorMSG(SOCKET  0]{	ck,r(v |ZE tocol[]=ontner1.00
0 OK\rsinlck,r(v |Erro.h>
]=oeturer:simple web|Erroer\rsinlck,r(v |cntLen
]=oid Send-length:

\rsinlck,r(v |cntData("WCcv(Handlck,r(v |Datact[  2048
#AFILE*|ErndF(SOhre 2)  :%dt>\cntDatahCoid Send-tata:%s\rsi\rsinoien        (ErndF(SO=fop윱�H,ss.h>RR"wh  errr!")lntA0]);void ErrorHansr(vThreex      EWORD(2t(R))헤더 정보 전송rSi=, Ernd(me);
ZE tocolRROtrlen(ZE tocol)        Ernd(me);
Erro.h>RROtrlen(Erro.h>)        Ernd(me);
cntLenRROtrlen(cntLen)         Ernd(me);
cntDatahCOtrlen(cntData)       t(R)) {
t데이터 전송rSi=, sizeotfgets(),R ata큖      ErndF(SOtarrr!");reexErnd(me);
),R aOtrlen(),R)       t(Rleanrtup1);sr(vThR)// ntne 프로토콜에 의해서lntAdr후 종료       har* ct,  ErrorHandling(char *R)//  윷� 발생시 메시지 전달0]{	ck,r(v |ZE tocol[]=ontner1.004
0 Badmily=AF_\rsinlck,r(v |Erro.h>
]=oeturer:simple web|Erroer\rsinlck,r(v |cntLen
]=oid Send-length:

\rsinlck,r(v |cntData(]=oid Send-tata:text/html\rsi\rsinlck,r(v |cd Send(]=o<html><herv><title>dr=	ErK</title></herv>"ck,       "<body><fd SROR)=+5><br> 윷� 발생!  {
t파일명t cl {
t 逆�)확인!"ck,	   "</fd S></body></html>"hre 2) rnd(me);
ZE tocolRROtrlen(ZE tocol)        Ernd(me);
Erro.h>RROtrlen(Erro.h>)        Ernd(me);
cntLenRROtrlen(cntLen)         Ernd(me);
cntDatahCOtrlen(cntData)        Ernd(me);
cd SendhCOtrlen(cd Send)        leanrtup1);sr(vTh       );
void SendData(SOCKET sock;R)// id Send-Data 구분|H*OR2r(v |exte   on("WCcv(Handlck,r(v |H,ss.h>

NET, "(hSerb   cpy(H,ss.h>RR sock, c	Otrtok H,ss.h>RR".")Serb   cpy(exte   onRROtrtok i), hCo.)         !   cmp(exte   onRR"html")||!   cmp(exte   onRR"htm)  ;reex       "text/htmlnlck,elsereex       "text/pl
{"        har* ;

int main(int * gc, char|H*OR2fputs(gc, chaRROtd/R)     fputc('\n&RROtd/R)     tup(MAKEWO}