#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void hv4()
{
	char es[][15]={
		3,2,2,1,1,2,2,3, 0,0,"\0",
		3,3,3,3,3,3,3,3, 0,0,"\0",
		3,3,2,2,2,2,3,3, 3,0,"\0",
		3,3,2,2,2,2,3,3, 4,0,"\0",
		1,4,3,3,3,3,4,1, 4,0,"\0",
		1,3,3,2,2,3,3,1, 4,0,"\0",
		2,2,2,2,2,2,2,2, 5,0,"\0",
		2,2,1,0,0,1,2,2, 5,0,"\0",
		0,1,1,1,1,1,1,0, 5,0,"\0",
		0,0,-2,-2,-2,-2,0,0, 5,0,"\0"
	},a,b,c,d,e,f,g,h,k;

	for(a=0;a<3;a++) {
		board[44]=a;
	for(b=0;b<3;b++) {
		board[43]=b;
	for(c=0;c<3;c++) {
		board[42]=c;
	for(d=0;d<3;d++) {
		board[41]=d;
	for(e=0;e<3;e++) {
		board[40]=e;
	for(f=0;f<3;f++) {
		board[39]=f;
	for(g=0;g<3;g++) {
		board[38]=g;
	for(h=0;h<3;h++) {
		board[37]=h;
		for(k=0;k<10;k++) fputc(line(37,1,es[k],es[k][8]),ef);
	}}}}}}}}

	board[37]=board[38]=board[39]=board[40]=board[41]=board[42]=board[43]=board[44]=0;
}