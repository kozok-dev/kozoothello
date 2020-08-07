#include <stdio.h>
#include <string.h>

extern char board[];
extern FILE *ef;

char dirTurn(char,char,char);

char typeEdge(char s,char dir)
{
	char c=s+dir,i;

	s=c+dir;

	if(board[s]==2) {
		for(i=0;board[s+=dir]==2;i++);
		if(i==4) {
			if(board[c]==2) return 1;
			if(!board[c]) return 2;
		}
		if(board[c]==2) return 0;
		if(i==3) return 4;
		s=c+dir+dir;
		if(board[s]==1) {
			if(board[s+=dir]) {
				if(board[s+=dir]==2) return -3;
			}
		}
	} else if(board[s]==1) {
		for(i=0;board[s+=dir]==1;i++);
		if(i==4) {
			if(board[c]==1) return -1;
			if(!board[c]) return -2;
		}
		if(board[c]==1) return 0;
		if(i==3) return -4;
		s=c+dir+dir;
		if(board[s]==2) {
			if(board[s+=dir]) {
				if(board[s+=dir]==1) return 3;
			}
		}
	}

	return 0;
}

char discEdge(char p,char s,char dir)
{
	static struct {
		char ptn[8],ev;
	} ptn[]={
		"212",3,
		"2112",-10,
		"2122",-10,
		"21112",-20,
		"21122",-19,
		"21222",-18,
		"211112",-30,
		"211122",-27,
		"211222",-25,
		"212222",-20,
		"2111112",-30,
		"2111122",-30,
		"2111222",-27,
		"2112222",-25,
		"2122222",-23,
		"21111112",-30,
		"21111122",-20,
		"21111222",-10,
		"21112222",0,
		"21122222",0,
		"21222222",0
	};
	char ts=s,i,j,clen,plen;

	for(clen=0;clen<8;clen++,s+=dir) {
		if(!board[s]) break;
	}

	for(i=0;i<21;i++) {
		plen=strlen(ptn[i].ptn);
		if(clen!=plen) continue;
		for(s=ts,j=0;ptn[i].ptn[j];s+=dir,j++) {
			if(p==2) {
				if(board[s]!=ptn[i].ptn[j]-48) break;
			} else {
				if(board[s]!=3-(ptn[i].ptn[j]-48)) break;
			}
		}
		if(j==plen) return ptn[i].ev;
	}
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
		return 20+stable(2,s,dir1)+discEdge(2,s,dir1);
	}
	if(board[s]==1) {
		et=typeEdge(s,dir1);
		if(et==-2 || et==3) return 5;
		if(et==-4) return -8;
		return -20-stable(1,s,dir1)-discEdge(1,s,dir1);
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

	et=typeEdge(s,dir1);
	if(et==1) return 6;
	if(et==2) return 7;
	if(et==3) return 6;
	if(et==4) return 1;
	if(et==-1) return -6;
	if(et==-2) return -7;
	if(et==-3) return -6;
	if(et==-4) return -1;
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