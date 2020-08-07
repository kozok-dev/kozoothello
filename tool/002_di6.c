#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void di6()
{
	char es[][15]={
		3,2,1,1,2,3, 2,"\0",
		3,2,1,1,2,3, 3,"\0",
		2,2,1,1,2,2, 3,"\0",
		2,2,1,1,2,2, 4,"\0",
		2,2,1,1,2,2, 4,"\0",
		1,1,1,1,1,1, 4,"\0",
		1,1,1,1,1,1, 4,"\0",
		0,1,1,1,1,0, 4,"\0",
		0,0,1,1,0,0, 4,"\0",
		0,0,0,0,0,0, 4,"\0"
	},a,b,c,d,e,f,k;

	for(a=0;a<3;a++) {
		board[55]=a;
	for(b=0;b<3;b++) {
		board[47]=b;
	for(c=0;c<3;c++) {
		board[39]=c;
	for(d=0;d<3;d++) {
		board[31]=d;
	for(e=0;e<3;e++) {
		board[23]=e;
	for(f=0;f<3;f++) {
		board[15]=f;
		for(k=0;k<10;k++) fputc(line(15,8,es[k],es[k][6]),ef);
	}}}}}}

	board[15]=board[23]=board[31]=board[39]=board[47]=board[55]=0;
}