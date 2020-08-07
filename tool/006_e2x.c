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
		if(i==3) return -4;
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
		if(i==3) return 4;
		s=c+dir+dir;
		if(board[s]==1) {
			if(board[s+=dir]) {
				if(board[s+=dir]==2) return -3;
			}
		}
	}

	return 0;
}

char discEdge(char p,char s,int dir)
{
	char c=s,r=0,i;

	for(i=1;i<8;i++) {
		if(!board[c+=dir]) break;
		if(!r && board[c]==p) r=i;
	}
	if(!r || i<=2) return 0;
	if(i==3) return -3;
	if(i==4) {
		if(r==1) return 6;
		if(r==2) return 3;
	}
	if(i==5) {
		if(r==1) return 20;
		if(r==2) return 6;
		if(r==3) return 3;
	}
	if(i==6) {
		if(r==1) return 30;
		if(r==2) return 20;
		if(r==3) return 6;
		if(r==4) return 3;
	}
	if(i==7) {
		if(r==1) return 25;
		if(r==2) return 20;
		if(r==3) return 10;
		if(r==4) return 7;
		if(r==5) return 4;
	}
	if(r==1) return 15;
	if(r==2) return 10;
	if(r==3) return 6;
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
		if(et==2 || et==-3) return -5;
		if(et==4) return 8;
		return 20+stable(2,s,dir1)-discEdge(1,s,dir1);
	}
	if(board[s]==1) {
		et=typeEdge(s,dir1);
		if(et==-2 || et==3) return 5;
		if(et==-4) return -8;
		return -20-stable(1,s,dir1)+discEdge(2,s,dir1);
	}

	if(dirTurn(2,s,dir1)) {
		et=typeEdge(s,dir1);
		if(et==2 || et==-3) return -5;
		if(et==4) return 8;
		return 20;
	}
	if(dirTurn(1,s,dir1)) {
		et=typeEdge(s,dir1);
		if(et==-2 || et==3) return 5;
		if(et==-4) return -8;
		return -20;
	}

	if(board[s+dir1+dir2]==2) {
		et=typeEdge(s,dir1);
		if(et==-2 || et==3) return 5;
		return -19;
	}
	if(board[s+dir1+dir2]==1) {
		et=typeEdge(s,dir1);
		if(et==2 || et==-3) return -5;
		return 19;
	}

	if(board[s+dir1]==1 && !board[s+dir1+dir1]) return 10;
	if(board[s+dir1]==2 && !board[s+dir1+dir1]) return -10;

	return 0;
}

void e2x()
{
	char a,b,c,d,e,f,g,h,i,j;

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
		fputc(corner(10,1,9)+corner(17,-1,9),ef);
	}}}}}}}}}}

	board[10]=board[11]=board[12]=board[13]=board[14]=board[15]=board[16]=board[17]=board[20]=board[25]=0;
}