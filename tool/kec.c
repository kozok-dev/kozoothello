#include <stdio.h>
#include <string.h>
#include <direct.h>

void main(int argc,char **argv)
{
	char cd[256];
	int i;
	FILE *f1,*f2;

	if(argc!=2) return;
	strcpy(cd,argv[1]);
	*strrchr(cd,92)=0;
	if(chdir(cd) || !(f1=fopen(argv[1],"rb")) || !(f2=fopen("koeval.c","w"))) return;

	fprintf(f2,"char di4[][81]={");
	for(i=0;i<1053;i++) {
		if(i%81==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==1052) fputc(125,f2);
		else if(i%81==80) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char di5[][243]={");
	for(i=0;i<3159;i++) {
		if(i%243==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==3158) fputc(125,f2);
		else if(i%243==242) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char di6[][729]={");
	for(i=0;i<9477;i++) {
		if(i%729==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==9476) fputc(125,f2);
		else if(i%729==728) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char di7[][2187]={");
	for(i=0;i<28431;i++) {
		if(i%2187==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==28430) fputc(125,f2);
		else if(i%2187==2186) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char di8[][6561]={");
	for(i=0;i<85293;i++) {
		if(i%6561==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==85292) fputc(125,f2);
		else if(i%6561==6560) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char hv2[][6561]={");
	for(i=0;i<85293;i++) {
		if(i%6561==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==85292) fputc(125,f2);
		else if(i%6561==6560) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char hv3[][6561]={");
	for(i=0;i<85293;i++) {
		if(i%6561==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==85292) fputc(125,f2);
		else if(i%6561==6560) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char hv4[][6561]={");
	for(i=0;i<85293;i++) {
		if(i%6561==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==85292) fputc(125,f2);
		else if(i%6561==6560) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char e2x[][59049]={");
	for(i=0;i<767637;i++) {
		if(i%59049==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==767636) fputc(125,f2);
		else if(i%59049==59048) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char c33[][19683]={");
	for(i=0;i<255879;i++) {
		if(i%19683==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==255878) fputc(125,f2);
		else if(i%19683==19682) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};\n");

	fprintf(f2,"char c25[][59049]={");
	for(i=0;i<767637;i++) {
		if(i%59049==0) fputc(123,f2);
		fprintf(f2,"%d",(char)fgetc(f1));
		if(i==767636) fputc(125,f2);
		else if(i%59049==59048) fprintf(f2,"},");
		else fputc(44,f2);
	}
	fprintf(f2,"};");

	fclose(f1);
	fclose(f2);
}