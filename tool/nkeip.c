#include <stdio.h>
#include <string.h>
#include <direct.h>

char di4[13][81],di5[13][243],di6[13][729],di7[13][2187],di8[13][6561];
char hv2[13][6561],hv3[13][6561],hv4[13][6561],e2x[13][59049],c33[13][19683],c25[13][59049];

char di4f[13][81],di5f[13][243],di6f[13][729],di7f[13][2187],di8f[13][6561];
char hv2f[13][6561],hv3f[13][6561],hv4f[13][6561],e2xf[13][59049],c33f[13][19683],c25f[13][59049];

void prepareEval(char *ptn,char *ptnf,char *p,char cnt)
{
	char o[]={1,0,2},ev,i;
	unsigned short i1,i2,os;

	for(i1=i2=i=0,os=1;i<=cnt;i1+=p[cnt-i]*os,i2+=o[p[cnt-i]]*os,os*=3,i++);
	if(ptnf[i1] || ptnf[i2]) return;
	ev=ptn[i1];
	ptn[i1]=-ptn[i2];
	ptn[i2]=-ev;
	ptnf[i1]=ptnf[i2]=1;
}

void main(int argc,char **argv)
{
	char cd[256],p[10],a,b,c,d,e,f,g,h,i,j,k;
	FILE *ef;

	if(argc!=2) return;
	strcpy(cd,argv[1]);
	*strrchr(cd,92)=0;
	if(chdir(cd) || !(ef=fopen(argv[1],"rb"))) return;

	fread(di4,1,1053,ef);
	fread(di5,1,3159,ef);
	fread(di6,1,9477,ef);
	fread(di7,1,28431,ef);
	fread(di8,1,85293,ef);
	fread(hv2,1,85293,ef);
	fread(hv3,1,85293,ef);
	fread(hv4,1,85293,ef);
	fread(e2x,1,767637,ef);
	fread(c33,1,255879,ef);
	fread(c25,1,767637,ef);
	fclose(ef);

	for(a=0;a<13;a++) {
	for(b=0;b<3;b++) {
		p[0]=b;
	for(c=0;c<3;c++) {
		p[1]=c;
	for(d=0;d<3;d++) {
		p[2]=d;
	for(e=0;e<3;e++) {
		p[3]=e;
		prepareEval(di4[a],di4f[a],p,3);
	for(f=0;f<3;f++) {
		p[4]=f;
		prepareEval(di5[a],di5f[a],p,4);
	for(g=0;g<3;g++) {
		p[5]=g;
		prepareEval(di6[a],di6f[a],p,5);
	for(h=0;h<3;h++) {
		p[6]=h;
		prepareEval(di7[a],di7f[a],p,6);
	for(i=0;i<3;i++) {
		p[7]=i;
		prepareEval(di8[a],di8f[a],p,7);
		prepareEval(hv2[a],hv2f[a],p,7);
		prepareEval(hv3[a],hv3f[a],p,7);
		prepareEval(hv4[a],hv4f[a],p,7);
	for(j=0;j<3;j++) {
		p[8]=j;
		prepareEval(c33[a],c33f[a],p,8);
	for(k=0;k<3;k++) {
		p[9]=k;
		prepareEval(e2x[a],e2xf[a],p,9);
		prepareEval(c25[a],c25f[a],p,9);
	}}}}}}}}}}}

	if(!(ef=fopen(argv[1],"wb"))) return;
	fwrite(di4,1,1053,ef);
	fwrite(di5,1,3159,ef);
	fwrite(di6,1,9477,ef);
	fwrite(di7,1,28431,ef);
	fwrite(di8,1,85293,ef);
	fwrite(hv2,1,85293,ef);
	fwrite(hv3,1,85293,ef);
	fwrite(hv4,1,85293,ef);
	fwrite(e2x,1,767637,ef);
	fwrite(c33,1,255879,ef);
	fwrite(c25,1,767637,ef);
	fclose(ef);
}