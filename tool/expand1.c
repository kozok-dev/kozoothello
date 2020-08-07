#include <stdio.h>
#include <string.h>
#include <direct.h>

void main(int argc,char **argv)
{
	char buf[59050],j;
	unsigned short i;
	FILE *f1,*f2;

	if(argc!=2) return;
	strcpy(buf,argv[1]);
	*strrchr(buf,92)=0;
	if(chdir(buf) || !(f1=fopen(argv[1],"rb")) || !(f2=fopen("tmp","wb"))) return;

	for(i=0;i<81;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<243;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<729;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<2187;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<6561;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<6561;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<6561;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	for(i=0;i<6561;i++) {
		fread(buf,1,10,f1);
		fwrite(buf,1,10,f2);
		for(j=10;j<15;j++) fputc(buf[9],f2);
	} fputc(fgetc(f1),f2);
	fread(buf,1,59050,f1);
	for(i=0;i<59049;i++) {
		for(j=0;j<15;j++) fputc(buf[i],f2);
	}
	fputc(buf[59049],f2);
	fread(buf,1,19684,f1);
	for(i=0;i<19683;i++) {
		for(j=0;j<15;j++) fputc(buf[i],f2);
	}
	fputc(buf[19683],f2);
	fread(buf,1,59050,f1);
	for(i=0;i<59049;i++) {
		for(j=0;j<15;j++) fputc(buf[i],f2);
	}
	fputc(buf[59049],f2);
	fclose(f1);
	fclose(f2);

	remove(argv[1]);
	rename("tmp",argv[1]);
	remove("tmp");
}