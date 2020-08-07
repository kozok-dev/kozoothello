#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void di5()
{
	char es[][15]={
		4,5,3,5,4, 1,5,"\0",
		4,3,3,3,4, 2,0,"\0",
		4,3,2,3,4, 2,0,"\0",
		3,2,2,2,3, 2,0,"\0",
		2,2,2,2,2, 3,0,"\0",
		2,2,1,2,2, 3,0,"\0",
		2,1,0,1,2, 4,0,"\0",
		1,0,0,0,1, 4,0,"\0",
		0,0,0,0,0, 4,0,"\0",
		0,0,0,0,0, 4,0,"\0"
	},a,b,c,d,e,k;

	for(a=0;a<3;a++) {
		board[46]=a;
	for(b=0;b<3;b++) {
		board[38]=b;
	for(c=0;c<3;c++) {
		board[30]=c;
	for(d=0;d<3;d++) {
		board[22]=d;
	for(e=0;e<3;e++) {
		board[14]=e;
		for(k=0;k<10;k++) fputc(line(14,8,es[k],es[k][5]),ef);
	}}}}}

	board[14]=board[22]=board[30]=board[38]=board[46]=0;
}