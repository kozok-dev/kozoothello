#include <stdio.h>
#include <string.h>
#include <direct.h>

char di4[81][13],di5[243][13],di6[729][13],di7[2187][13],di8[6561][13];
char hv2[6561][13],hv3[6561][13],hv4[6561][13],e2x[59049][13],c33[19683][13],c25[59049][13];

void main(int argc,char **argv)
{
	char cd[256],p[]={1,2,0},a,b,c,d,e,f,g,h,i,j,k;
	FILE *f1,*f2;

	if(argc!=2) return;
	strcpy(cd,argv[1]);
	*strrchr(cd,92)=0;
	if(chdir(cd) || !(f1=fopen(argv[1],"rb")) || !(f2=fopen("tmp","wb"))) return;

	fread(di4,1,1053,f1);
	fread(di5,1,3159,f1);
	fread(di6,1,9477,f1);
	fread(di7,1,28431,f1);
	fread(di8,1,85293,f1);
	fread(hv2,1,85293,f1);
	fread(hv3,1,85293,f1);
	fread(hv4,1,85293,f1);
	fread(e2x,1,767637,f1);
	fread(c33,1,255879,f1);
	fread(c25,1,767637,f1);

	for(e=0;e<13;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-di4[p[a]+p[b]*3+p[c]*9+p[d]*27][e],f2);
	for(f=0;f<13;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-di5[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81][f],f2);
	for(g=0;g<13;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-di6[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243][g],f2);
	for(h=0;h<13;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-di7[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729][h],f2);
	for(i=0;i<13;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-di8[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187][i],f2);
	for(i=0;i<13;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-hv2[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187][i],f2);
	for(i=0;i<13;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-hv3[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187][i],f2);
	for(i=0;i<13;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-hv4[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187][i],f2);
	for(k=0;k<13;k++) for(j=0;j<3;j++) for(i=0;i<3;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-e2x[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187+p[i]*6561+p[j]*19683][k],f2);
	for(j=0;j<13;j++) for(i=0;i<3;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-c33[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187+p[i]*6561][j],f2);
	for(k=0;k<13;k++) for(j=0;j<3;j++) for(i=0;i<3;i++) for(h=0;h<3;h++) for(g=0;g<3;g++) for(f=0;f<3;f++) for(e=0;e<3;e++) for(d=0;d<3;d++) for(c=0;c<3;c++) for(b=0;b<3;b++) for(a=0;a<3;a++) fputc(-c25[p[a]+p[b]*3+p[c]*9+p[d]*27+p[e]*81+p[f]*243+p[g]*729+p[h]*2187+p[i]*6561+p[j]*19683][k],f2);

	fclose(f1);
	fclose(f2);
	remove(argv[1]);
	rename("tmp",argv[1]);
	remove("tmp");
}