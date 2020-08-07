#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void hv3()
{
	char es[][15]={
		4,4,3,3,3,3,4,4, 3,"\0",
		4,4,3,2,2,3,4,4, 3,"\0",
		4,3,3,2,2,3,3,4, 3,"\0",
		3,3,3,2,2,3,3,3, 3,"\0",
		3,3,3,2,2,3,3,3, 4,"\0",
		3,3,3,2,2,3,3,3, 4,"\0",
		3,3,2,1,1,2,3,3, 4,"\0",
		3,2,1,0,0,1,2,3, 4,"\0",
		1,0,0,-1,-1,0,0,1, 4,"\0",
		1,0,0,-1,-1,0,0,1, 5,"\0"
	},a,b,c,d,e,f,g,h,k;

	for(a=0;a<3;a++) {
		board[35]=a;
	for(b=0;b<3;b++) {
		board[34]=b;
	for(c=0;c<3;c++) {
		board[33]=c;
	for(d=0;d<3;d++) {
		board[32]=d;
	for(e=0;e<3;e++) {
		board[31]=e;
	for(f=0;f<3;f++) {
		board[30]=f;
	for(g=0;g<3;g++) {
		board[29]=g;
	for(h=0;h<3;h++) {
		board[28]=h;
		for(k=0;k<10;k++) fputc(line(28,1,es[k],es[k][8]),ef);
	}}}}}}}}

	board[28]=board[29]=board[30]=board[31]=board[32]=board[33]=board[34]=board[35]=0;
}