#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void di4()
{
	char es[][15]={
		5,5,5,5, 0,0,"\0",
		4,5,5,4, 0,0,"\0",
		4,5,5,4, 1,0,"\0",
		4,4,4,4, 1,0,"\0",
		4,4,4,4, 1,2,"\0",
		3,4,4,3, 1,0,"\0",
		3,4,4,3, 1,0,"\0",
		3,3,3,3, 1,3,"\0",
		2,3,3,2, 2,0,"\0",
		0,0,0,0, 2,0,"\0"
	},a,b,c,d,k;

	for(a=0;a<3;a++) {
		board[37]=a;
	for(b=0;b<3;b++) {
		board[29]=b;
	for(c=0;c<3;c++) {
		board[21]=c;
	for(d=0;d<3;d++) {
		board[13]=d;
		for(k=0;k<10;k++) fputc(line(13,8,es[k],es[k][4]),ef);
	}}}}

	board[13]=board[21]=board[29]=board[37]=0;
}