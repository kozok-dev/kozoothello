#include "ko.h"

char board[91],p,bm,dd,e,depth,forcecom,br,mr;
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
HWND comcont,prog;
EMPTY empty[63],eh,*mvemp[81];

char WINAPI initKoai()
{
	if(!initBook() || !initHash(131072)) return 0;
	comcont=FindWindowEx(FindWindow(0,"KozoOthello"),0,0,0);
	prog=GetNextWindow(comcont,GW_HWNDNEXT);
	return 1;
}

void WINAPI setCom(char t,char s)
{
	if(!t) br=s;
	else mr=s;
}

void WINAPI setBoard(char p,char i)
{
	board[i]=p;
}

char WINAPI getBM()
{
	return bm;
}

void mainCom(void *d)
{
	char exact,wld,i;
	EMPTY *ep;

	srand(time(0));

	for(bm=e=i=0;i<64;i++) {
		if(board[cp[i]]==2) e++;
	}

	if(p==(e&1)) Book();
	if(!bm) {
		SendMessage(prog,WM_SETTEXT,0,"P");

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

		switch(depth) {
		case 0:depth=1;exact=4;wld=6;break;
		case 1:depth=2;exact=6;wld=8;break;
		case 2:depth=3;exact=10;wld=12;break;
		case 3:depth=4;exact=12;wld=14;break;
		case 4:depth=6;exact=14;wld=16;break;
		case 5:depth=8;exact=16;wld=18;break;
		case 6:depth=9;exact=18;wld=20;break;
		case 7:depth=11;exact=20;wld=22;break;
		default:depth=12;exact=22;wld=24;
		}
		if(e<=exact) depth=-2;
		else if(e<=wld) depth=-1;
		Com(p);
	}
	SendMessage(prog,WM_SETTEXT,0,"");
	SendMessage(comcont,WM_SETTEXT,0,0);
}

void WINAPI callCom(char game,char dep)
{
	p=game;
	depth=dep;
	forcecom=0;
	_beginthread(mainCom,0,0);
}

void WINAPI forceCom()
{
	forcecom=1;
}