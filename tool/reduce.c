#include <stdio.h>
#include <string.h>
#include <direct.h>

void main(int argc,char **argv)
{
	unsigned short i;
	char buf[59049][15],cd[256];
	FILE *f1,*f2;

	if(argc!=2) return;
	strcpy(cd,argv[1]);
	*strrchr(cd,92)=0;
	if(chdir(cd) || !(f1=fopen(argv[1],"rb")) || !(f2=fopen("tmp","wb"))) return;

	for(i=0;i<81;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<81;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<81;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<243;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<243;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<243;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<729;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<729;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<729;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<2187;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<2187;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<2187;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<6561;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<6561;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<6561;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<6561;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<6561;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<6561;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<6561;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<6561;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<6561;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<6561;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<6561;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<6561;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<59049;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<59049;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<59049;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<19683;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<19683;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<19683;i++) fwrite(buf[i]+2,1,13,f2);

	for(i=0;i<59049;i++) fread(buf[i],1,15,f1); fgetc(f1);
	for(i=0;i<59049;i++) buf[i][2]=(buf[i][0]+buf[i][1]+buf[i][2])/3;
	for(i=0;i<59049;i++) fwrite(buf[i]+2,1,13,f2);

	fclose(f1);
	fclose(f2);
	remove(argv[1]);
	rename("tmp",argv[1]);
	remove("tmp");
}