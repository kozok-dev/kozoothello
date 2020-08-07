#include <stdio.h>

extern char board[];
extern FILE *ef;

char line(char,char,char *,char);

void hv2()
{
	char es[][15]={
		0,1,3,5,5,3,1,0, 1,"\0",
		0,1,3,5,5,3,1,0, 1,"\0",
		0,1,3,5,5,3,1,0, 1,"\0",
		0,1,3,5,5,3,1,0, 1,"\0",
		0,1,3,4,4,3,1,0, 1,"\0",
		0,1,2,4,4,2,1,0, 1,"\0",
		0,0,2,4,4,2,0,0, 2,"\0",
		0,0,1,3,3,1,0,0, 2,"\0",
		0,0,1,2,2,1,0,0, 2,"\0",
		0,0,0,1,1,0,0,0, 2,"\0"
	},a,b,c,d,e,f,g,h,k;

	for(a=0;a<3;a++) {
		board[26]=a;
	for(b=0;b<3;b++) {
		board[25]=b;
	for(c=0;c<3;c++) {
		board[24]=c;
	for(d=0;d<3;d++) {
		board[23]=d;
	for(e=0;e<3;e++) {
		board[22]=e;
	for(f=0;f<3;f++) {
		board[21]=f;
	for(g=0;g<3;g++) {
		board[20]=g;
	for(h=0;h<3;h++) {
		board[19]=h;
		for(k=0;k<10;k++) fputc(line(19,1,es[k],es[k][8]),ef);
	}}}}}}}}

	board[19]=board[20]=board[21]=board[22]=board[23]=board[24]=board[25]=board[26]=0;
}