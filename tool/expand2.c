#include <stdio.h>
#include <string.h>
#include <direct.h>

void main(int argc,char **argv)
{
	unsigned short i;
	char buf[98416],j;
	FILE *f1,*f2;

	if(argc!=2) return;
	strcpy(buf,argv[1]);
	*strrchr(buf,92)=0;
	if(chdir(buf) || !(f1=fopen(argv[1],"rb")) || !(f2=fopen("tmp","wb"))) return;

	fread(buf,1,1216,f1);
	fwrite(buf,1,1216,f2);
	fread(buf,1,3646,f1);
	fwrite(buf,1,3646,f2);
	fread(buf,1,10936,f1);
	fwrite(buf,1,10936,f2);
	fread(buf,1,32806,f1);
	fwrite(buf,1,32806,f2);
	fread(buf,1,98416,f1);
	fwrite(buf,1,98416,f2);
	fread(buf,1,98416,f1);
	fwrite(buf,1,98416,f2);
	fread(buf,1,98416,f1);
	fwrite(buf,1,98416,f2);
	fread(buf,1,98416,f1);
	fwrite(buf,1,98416,f2);
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