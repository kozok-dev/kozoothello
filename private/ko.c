#include "ko.h"

char board[91],pv[16],ap[256],p,dd,e,depth,ub,bop,br,mr,bmo,forcecom;
short me,es[81];
int evcnt;
char cp[]={
	10,17,73,80,
	31,32,39,42,48,51,58,59,
	30,33,57,60,
	22,23,38,43,47,52,67,68,
	12,15,28,35,55,62,75,78,
	13,14,37,44,46,53,76,77,
	21,24,29,34,56,61,66,69,
	11,16,19,26,64,71,74,79,
	20,25,65,70,
	40,41,49,50
};
HWND comcont,lpv,lev;
EMPTY empty[63],eh,*mvemp[81];

void WINAPI initKoai(char *vbap)
{
	strcpy(ap,vbap);
	initBook();
	comcont=FindWindowEx(FindWindow(0,"KozoOthello"),0,0,0);
	lpv=GetNextWindow(comcont,GW_HWNDNEXT);
	lev=GetNextWindow(lpv,GW_HWNDNEXT);
	WSAStartup(MAKEWORD(1,1),0);
}

void WINAPI setCom(char t,char s)
{
	if(!t) bop=s;
	else if(t==1) br=s;
	else mr=s;
}

void WINAPI setBoard(char p,char i)
{
	board[i]=p;
}

int WINAPI getCom(char t,char i)
{
	if(!t) return pv[0];
	if(t==1) return me;
	if(t==2) return es[i];
	return evcnt;
}

void mainCom(void *d)
{
	char tb[91],i;
	EMPTY *ep;

	srand(time(0));

	for(me=-3000,e=evcnt=i=0;i<16;i++) pv[i]=0;
	for(i=10;i<81;i++) {
		if(board[i]==2) e++;
		es[i]=-3000;
	}

	memcpy(tb,board,91);
	if(ub && p==(e&1)) Book(p);
	if((bmo || !bmo && bop) && pv[0]) {
		comMsg(100);
		SendMessage(comcont,WM_SETTEXT,0,0);
		return;
	} else {
		ub=0;
	}
	if(!bmo && bop) {
		comMsg(100);
		SendMessage(comcont,WM_SETTEXT,0,0);
		return;
	}
	memcpy(board,tb,91);

	if(!ub || !bmo) {
		for(ep=&eh,dd=e=i=0;i<64;i++) {
			if(!board[cp[i]]) {
				if(p) {
					board[cp[i]]=1;
					dd--;
				} else {
					dd++;
				}
			} else if(board[cp[i]]==1) {
				if(p) {
					board[cp[i]]=0;
					dd++;
				} else {
					dd--;
				}
			} else if(board[cp[i]]==2) {
				ep->next=&empty[e];
				empty[e++].prev=ep;
				ep=ep->next;
				ep->s=board+cp[i];
				mvemp[cp[i]]=ep;
			}
		}
		ep->next=0;

		Com(p);
	}
}

void WINAPI callCom(char vbp,char dep,char opt,char st)
{
	p=vbp;
	depth=dep;
	ub=opt&1;
	bmo=opt&2;
	forcecom=0;
	if(st) mainCom(0);
	else _beginthread(mainCom,0,0);
}

void WINAPI forceCom()
{
	forcecom=1;
}

void WINAPI sendSSTP(char *msg,char *opt)
{
	char sendbuf[256],accept[64],acceptbuf[64],nbr;
	struct hostent *hostentry;
	struct sockaddr_in serverin;
	SOCKET sock;
	DWORD serveraddr;
	HANDLE mutex;

	if(!(mutex=OpenMutex(MUTEX_ALL_ACCESS,0,"sakura"))) return;
	CloseHandle(mutex);
	if((sock=socket(PF_INET,SOCK_STREAM,0))==INVALID_SOCKET) return;
	if((serveraddr=inet_addr("localhost"))==-1) {
		if(!(hostentry=gethostbyname("localhost"))) {
			closesocket(sock);
			return;
		} else {
			serveraddr=*((DWORD *)((hostentry->h_addr_list)[0]));
		}
	}
	serverin.sin_family=AF_INET;
	serverin.sin_addr.s_addr=serveraddr;
	serverin.sin_port=htons(9801);
	ZeroMemory(serverin.sin_zero,sizeof(serverin.sin_zero));
	if(connect(sock,(struct sockaddr*)&serverin,sizeof(serverin))==SOCKET_ERROR) {
		closesocket(sock);
		return;
	}

	strcpy(sendbuf,"SEND SSTP/1.1\r\nSender: KO\r\n");
	strcat(sendbuf,"Script: ");
	strcat(sendbuf,msg);
	strcat(sendbuf,"\\e\r\n");
	if(opt && strlen(opt)) {
		strcat(sendbuf,"Options: ");
		strcat(sendbuf,opt);
		strcat(sendbuf,"\r\n");
	}
	strcat(sendbuf,"Charset: SJIS\r\n\r\n");
	if(send(sock,sendbuf,strlen(sendbuf),0)==SOCKET_ERROR) {
		shutdown(sock,2);
		closesocket(sock);
		return;
	}

	accept[0]=0;
	while(1) {
		nbr=recv(sock,acceptbuf,sizeof(acceptbuf),0);
		if(!nbr || nbr==SOCKET_ERROR) break;
		acceptbuf[nbr]=0;
		strcat(accept,acceptbuf);
	}

	shutdown(sock,2);
	closesocket(sock);
}

void WINAPI comClick(char t)
{
	short x,y,sw;
	HWND win;
	RECT rect;

	if(t==1) {
		win=FindWindowEx(FindWindowEx(FindWindow(0,"Thell for Win32"),0,"Thell BoardBase Class",0),0,0,0);

		GetWindowRect(win,&rect);
		sw=(rect.right-rect.left)/8;
		x=(pv[0]%9-1)*sw+sw/2;
		y=(pv[0]/9-1)*sw+sw/2;
	} else if(t==2) {
		win=FindWindowEx(FindWindow(0,"BearRev Version 1.33"),0,0,0);

		GetWindowRect(win,&rect);
		x=rect.right-rect.left;
		y=rect.bottom-rect.top;
		if(x>y) x=y;
		sw=(x-21)/8;
		x=(pv[0]%9-1)*sw+sw/2;
		y=(pv[0]/9-1)*sw+sw/2;
	} else if(t==3) {
		win=FindWindowEx(FindWindow(0,"ƒ’ƒZƒ“¹ê - –¼l"),0,0,0);

		GetWindowRect(win,&rect);
		x=rect.right-rect.left;
		y=rect.bottom-rect.top;
		if(x>y) x=y;
		sw=x/10;
		x=pv[0]%9*sw+sw/2;
		y=pv[0]/9*sw+sw/2;
	} else {
		if(!(win=FindWindow(0,"Nabe's Reversi - 8x8 Reversi"))) return;

		GetWindowRect(win,&rect);
		x=(pv[0]%9-1)*37+78;
		y=(pv[0]/9-1)*37+116;
		SetCursorPos(rect.left+x,rect.top+y);
	}
	PostMessage(win,WM_LBUTTONDOWN,0,(y<<16)+x);
}