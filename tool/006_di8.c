#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void di8()
{
	char es[][15]={
		4,4,3,2,2,3,4,4, 0,0,"\0",
		4,3,3,3,3,3,3,4, 0,0,"\0",
		4,3,2,2,2,2,3,4, 2,0,"\0",
		4,3,2,2,2,2,3,4, 3,0,"\0",
		1,0,0,1,1,0,0,1, 1,0,"\0",
		1,0,0,0,0,0,0,1, 0,0,"\0",
		3,2,1,0,0,1,2,3, 1,0,"\0",
		2,1,0,0,0,0,1,2, 2,0,"\0",
		0,0,0,0,0,0,0,0, 2,0,"\0",
		0,0,-1,-2,-2,-1,0,0, 2,0,"\0"
	},a,b,c,d,e,f,g,h,k;

	for(a=0;a<3;a++) {
		board[73]=a;
	for(b=0;b<3;b++) {
		board[65]=b;
	for(c=0;c<3;c++) {
		board[57]=c;
	for(d=0;d<3;d++) {
		board[49]=d;
	for(e=0;e<3;e++) {
		board[41]=e;
	for(f=0;f<3;f++) {
		board[33]=f;
	for(g=0;g<3;g++) {
		board[25]=g;
	for(h=0;h<3;h++) {
		board[17]=h;
		for(k=0;k<10;k++) fputc(line(17,8,es[k],es[k][8]),ef);
	}}}}}}}}

	board[17]=board[25]=board[33]=board[41]=board[49]=board[57]=board[65]=board[73]=0;
}