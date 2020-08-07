#include <stdio.h>

extern char board[];
extern FILE *ef;

static char edge()
{
	static char chtbl[][15]={
		1,1,1, 1,1,2, 1,2,2, -10,"\0",
		0,0,1, 0,0,1, 1,1,1, 7,"\0",
		1,0,1, 0,1,1, 1,1,1, 10,"\0",
		0,0,1, 0,0,0, 1,0,1, 8,"\0",
		1,0,1, 1,1,1, 2,2,2, 31,"\0",
		1,1,2, 0,1,2, 1,1,2, 31,"\0",
	};
	char flag,a,b;

	for(a=0;a<6;a++) {
		flag=0;
		for(b=0;b<9;b++) {
			if(chtbl[a][b]==2 || board[10+b/3*9+b%3]==chtbl[a][b]) continue;
			flag=1;
			break;
		}
		if(!flag) return chtbl[a][9];

		flag=0;
		for(b=0;b<9;b++) {
			if(chtbl[a][b]==2 || board[10+b/3*9+b%3]==chtbl[a][b]*2) continue;
			flag=1;
			break;
		}
		if(!flag) return -chtbl[a][9];
	}

	for(a=0,b=0;a<9;a++) {
		if(board[10+a/3*9+a%3]==1) b++;
		else if(board[10+a/3*9+a%3]==2) b--;
	}
	b/=2;
	return b;
}

void c33()
{
	char a,b,c,d,e,f,g,h,i;

	for(a=0;a<3;a++) {
		board[30]=a;
	for(b=0;b<3;b++) {
		board[29]=b;
	for(c=0;c<3;c++) {
		board[28]=c;
	for(d=0;d<3;d++) {
		board[21]=d;
	for(e=0;e<3;e++) {
		board[20]=e;
	for(f=0;f<3;f++) {
		board[19]=f;
	for(g=0;g<3;g++) {
		board[12]=g;
	for(h=0;h<3;h++) {
		board[11]=h;
	for(i=0;i<3;i++) {
		board[10]=i;
		fputc(edge(),ef);
	}}}}}}}}}

	board[10]=board[11]=board[12]=board[19]=board[20]=board[21]=board[28]=board[29]=board[30]=0;
}