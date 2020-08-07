#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void di7()
{
	char es[][15]={
		3,3,3,2,3,3,3, 1,0,"\0",
		3,3,2,2,2,3,3, 2,0,"\0",
		3,3,2,2,2,3,3, 3,0,"\0",
		3,2,2,2,2,2,3, 3,0,"\0",
		2,1,1,1,1,1,2, 4,2,"\0",
		2,2,2,1,2,2,2, 4,0,"\0",
		2,2,1,0,1,2,2, 4,0,"\0",
		1,1,0,0,0,1,1, 4,0,"\0",
		1,0,0,-1,0,0,1, 4,0,"\0",
		0,0,-1,-2,-1,0,0, 4,0,"\0"
	},a,b,c,d,e,f,g,k;

	for(a=0;a<3;a++) {
		board[64]=a;
	for(b=0;b<3;b++) {
		board[56]=b;
	for(c=0;c<3;c++) {
		board[48]=c;
	for(d=0;d<3;d++) {
		board[40]=d;
	for(e=0;e<3;e++) {
		board[32]=e;
	for(f=0;f<3;f++) {
		board[24]=f;
	for(g=0;g<3;g++) {
		board[16]=g;
		for(k=0;k<10;k++) fputc(line(16,8,es[k],es[k][7]),ef);
	}}}}}}}

	board[16]=board[24]=board[32]=board[40]=board[48]=board[56]=board[64]=0;
}