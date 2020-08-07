#include <stdio.h>

extern char board[];
extern FILE *ef;

char dirTurn(char,char,char);

char typeEdge(char s,char dir)
{
	char c=s+dir,i;

	s=c+dir;

	if(board[s]==1) {
		for(i=0;board[s+=dir]==1;i++);
		if(i==4) {
			if(board[c]==1) return -1;
			if(!board[c]) return -2;
		}
		s=c+dir+dir;
		if(board[s]==2) {
			if(board[s+=dir]) {
				if(board[s+=dir]==1) return 3;
			}
		}
	} else if(board[s]==2) {
		for(i=0;board[s+=dir]==2;i++);
		if(i==4) {
			if(board[c]==2) return 1;
			if(!board[c]) return 2;
		}
		s=c+dir+dir;
		if(board[s]==1) {
			if(board[s+=dir]) {
				if(board[s+=dir]==2) return -3;
			}
		}
	}

	return 0;
}

char discEdge(char p,char s,char dir)
{
	if(board[s+dir]==p && board[s+dir+dir]==3-p) return 3;
	if(board[s+dir]==p && board[s+dir+dir]==p && board[s+dir*3]==3-p) return 7;
	if(board[s+dir]==p && board[s+dir+dir]==p && board[s+dir*3]==p && board[s+dir*4]==3-p) return 10;
	if(board[s+dir]==p && board[s+dir+dir]==p && board[s+dir*3]==p && board[s+dir*4]==p && board[s+dir*5]==3-p) return 15;
	if(board[s+dir]==p && board[s+dir+dir]==p && board[s+dir*3]==p && board[s+dir*4]==p && board[s+dir*5]==p && board[s+dir*6]==3-p) return 18;
	if(board[s+dir]==p && board[s+dir+dir]==p && board[s+dir*3]==p && board[s+dir*4]==p && board[s+dir*5]==p && board[s+dir*6]==p && board[s+dir*7]==3-p) return 20;
	return 0;
}

char stable(char p,char s,char dir)
{
	char i;

	for(i=0;board[s+=dir]==p;i+=5);
	return i;
}

char corner(char s,char dir1,char dir2)
{
	char et;

	if(board[s]==2) {
		et=typeEdge(s,dir1);
		if(et==2 && et==-3) {
			if(dirTurn(1,s+dir1,dir1)) return -5;
		}
		return 15+stable(2,s,dir1)-discEdge(1,s,dir1);
	}
	if(board[s]==1) {
		et=typeEdge(s,dir1);
		if(et==-2 && et==3) {
			if(dirTurn(2,s+dir1,dir1)) return 5;
		}
		return -15-stable(1,s,dir1)+discEdge(2,s,dir1);
	}

	if(dirTurn(2,s,dir1)) {
		et=typeEdge(s,dir1);
		if(et==2 && et==-3) return -5;
		return 13;
	}
	if(dirTurn(1,s,dir1)) {
		et=typeEdge(s,dir1);
		if(et==-2 && et==3) return 5;
		return -13;
	}

	if(board[s+dir1+dir2]==2) {
		et=typeEdge(s,dir1);
		if(et==-2 && et==3) return 5;
		return -15;
	}
	if(board[s+dir1+dir2]==1) {
		et=typeEdge(s,dir1);
		if(et==2 && et==-3) return -5;
		return 15;
	}

	if(board[s+dir1]==1 && !board[s+dir1+dir1]) return 10;
	if(board[s+dir1]==2 && !board[s+dir1+dir1]) return -10;

	et=typeEdge(s,dir1);
	if(et==1) return 7;
	if(et==-1) return -7;
	if(et==2) return 8;
	if(et==-2) return -8;
	if(et==3) return 3;
	if(et==-3) return -3;

	return 0;
}

void e2x()
{
	char p,o,m,a,b,c,d,e,f,g,h,i,j,k;

	for(a=0;a<3;a++) {
		board[25]=a;
	for(b=0;b<3;b++) {
		board[20]=b;
	for(c=0;c<3;c++) {
		board[17]=c;
	for(d=0;d<3;d++) {
		board[16]=d;
	for(e=0;e<3;e++) {
		board[15]=e;
	for(f=0;f<3;f++) {
		board[14]=f;
	for(g=0;g<3;g++) {
		board[13]=g;
	for(h=0;h<3;h++) {
		board[12]=h;
	for(i=0;i<3;i++) {
		board[11]=i;
	for(j=0;j<3;j++) {
		board[10]=j;
		for(m=0,k=10;k<=17;k++) {
			p=dirTurn(2,k,1)+dirTurn(2,k,-1);
			o=dirTurn(1,k,1)+dirTurn(1,k,-1);
			if(p && !o) m+=2;
			else if(!p && o) m-=2;
		}
		fputc(corner(10,1,9)+corner(17,-1,9)+m,ef);
	}}}}}}}}}}

	board[10]=board[11]=board[12]=board[13]=board[14]=board[15]=board[16]=board[17]=board[20]=board[25]=0;
}