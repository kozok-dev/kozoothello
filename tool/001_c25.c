#include <stdio.h>

extern char board[];
extern FILE *ef;

static char edge(char w)
{
	char ev=0,a,b,i;

	if(board[10]==2) {
		for(i=10,a=0;i<15 && board[i]==2;i++,a++);
		for(i=19,b=0;i<24 && board[i]==2 && b<a-1;i++,b++,ev+=w*(b+1));
		return ev;
	} else if(board[10]==1) {
		for(i=10,a=0;i<15 && board[i]==1;i++,a++);
		for(i=19,b=0;i<24 && board[i]==1 && b<a-1;i++,b++,ev-=w*(b+1));
		return ev;
	}
	return 0;
}

void c25()
{
	char a,b,c,d,e,f,g,h,i,j;

	for(a=0;a<3;a++) {
		board[23]=a;
	for(b=0;b<3;b++) {
		board[22]=b;
	for(c=0;c<3;c++) {
		board[21]=c;
	for(d=0;d<3;d++) {
		board[20]=d;
	for(e=0;e<3;e++) {
		board[19]=e;
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
		fputc(edge(3),ef);
	}}}}}}}}}}

	board[10]=board[11]=board[12]=board[13]=board[14]=board[19]=board[20]=board[21]=board[22]=board[23]=0;
}