#include <stdio.h>

char board[91]={
	3,3,3,3,3,3,3,3,3,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,0,0,0,0,0,0,0,0,
	3,3,3,3,3,3,3,3,3,3
};
FILE *ef;

void di4();
void di5();
void di6();
void di7();
void di8();
void hv2();
void hv3();
void hv4();
void e2x();
void c33();
void c25();

char dirTurn(char p,char s,char dir)
{
	char o=3-p,i;

	if(board[s]) return 0;
	for(i=0;board[s+=dir]==o;i++);
	if(board[s]==p) return i;
	return 0;
}

char line(char s,char dir,char *es,char mw,char dw)
{
	char p,o,ev=0,m=0;

	for(;board[s]!=3;s+=dir,es++) {
		p=dirTurn(2,s,dir)+dirTurn(2,s,-dir);
		o=dirTurn(1,s,dir)+dirTurn(1,s,-dir);
		if(p && !o) m++;
		else if(!p && o) m--;

		if(board[s]==2) {
			ev-=dw;
			if(!board[s+dir]) ev-=*es;
			if(!board[s-dir]) ev-=*es;
		} else if(board[s]==1) {
			ev+=dw;
			if(!board[s+dir]) ev+=*es;
			if(!board[s-dir]) ev+=*es;
		}
	}
	return ev+m*mw;
}

void main()
{
	ef=fopen("koeval.dat","wb");
	di4();fputc(3,ef);
	di5();fputc(18,ef);
	di6();fputc(5,ef);
	di7();fputc(1,ef);
	di8();fputc(19,ef);
	hv2();fputc(0,ef);
	hv3();fputc(15,ef);
	hv4();fputc(18,ef);
	e2x();fputc(9,ef);
	c33();fputc(15,ef);
	c25();fputc(6,ef);
	fclose(ef);
}