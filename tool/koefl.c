#include <stdio.h>
#include <string.h>
#include <direct.h>
#define N 13
#define B 0.07
#define MC 100
#define MI 0.9

float d4[N][81],d5[N][243],d6[N][729],d7[N][2187],d8[N][6561];
float h2[N][6561],h3[N][6561],h4[N][6561];
float e2[N][59049],c3[N][19683],c2[N][59049];

float d4e[N][81],d5e[N][243],d6e[N][729],d7e[N][2187],d8e[N][6561];
float h2e[N][6561],h3e[N][6561],h4e[N][6561];
float e2e[N][59049],c3e[N][19683],c2e[N][59049];

unsigned int d4c[N][81],d5c[N][243],d6c[N][729],d7c[N][2187],d8c[N][6561];
unsigned int h2c[N][6561],h3c[N][6561],h4c[N][6561];
unsigned int e2c[N][59049],c3c[N][19683],c2c[N][59049];

char board[91],game[60],gs;

void initEval()
{
	char i;
	unsigned short j;
	FILE *f;

	if(f=fopen("koeval.dat","rb")) {
		for(i=0;i<N;i++) for(j=0;j<81;j++) d4[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<243;j++) d5[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<729;j++) d6[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<2187;j++) d7[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<6561;j++) d8[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<6561;j++) h2[i][j]=(float)(char)fgetc(f);
		printf(".");
		for(i=0;i<N;i++) for(j=0;j<6561;j++) h3[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<6561;j++) h4[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<59049;j++) e2[i][j]=(float)(char)fgetc(f);
		printf(".");
		for(i=0;i<N;i++) for(j=0;j<19683;j++) c3[i][j]=(float)(char)fgetc(f);
		for(i=0;i<N;i++) for(j=0;j<59049;j++) c2[i][j]=(float)(char)fgetc(f);
		fclose(f);
	} else {
		printf("..");
		for(i=0;i<N;i++) {
			for(j=0;j<81;j++) d4[i][j]=0;
			for(j=0;j<243;j++) d5[i][j]=0;
			for(j=0;j<729;j++) d6[i][j]=0;
			for(j=0;j<2187;j++) d7[i][j]=0;
			for(j=0;j<6561;j++) d8[i][j]=h2[i][j]=h3[i][j]=h4[i][j]=0;
			for(j=0;j<59049;j++) e2[i][j]=c2[i][j]=0;
			for(j=0;j<19683;j++) c3[i][j]=0;
		}
	}

	printf(".");
	for(i=0;i<N;i++) {
		for(j=0;j<81;j++) d4e[i][j]=d4c[i][j]=0;
		for(j=0;j<243;j++) d5e[i][j]=d5c[i][j]=0;
		for(j=0;j<729;j++) d6e[i][j]=d6c[i][j]=0;
		for(j=0;j<2187;j++) d7e[i][j]=d7c[i][j]=0;
		for(j=0;j<6561;j++) d8e[i][j]=d8c[i][j]=h2e[i][j]=h2c[i][j]=h3e[i][j]=h3c[i][j]=h4e[i][j]=h4c[i][j]=0;
		for(j=0;j<59049;j++) e2e[i][j]=e2c[i][j]=c2e[i][j]=c2c[i][j]=0;
		for(j=0;j<19683;j++) c3e[i][j]=c3c[i][j]=0;
	}
}

__inline char dirMove(char p,char o,char *s,char dir)
{
	char *ms=s;

	s+=dir;
	if(*s==o) {
		s+=dir;
		if(*s==o) {
			s+=dir;
			if(*s==o) {
				s+=dir;
				if(*s==o) {
					s+=dir;
					if(*s==o) {
						s+=dir;
						if(*s==o) s+=dir;
					}
				}
			}
		}
		if(*s==p) {
			s-=dir;
			do *s=p; while((s-=dir)!=ms);
			return 1;
		}
	}
	return 0;
}

char Move(char p,char *s)
{
	char o=p^1,i;

	i=dirMove(p,o,s,-1);
	i+=dirMove(p,o,s,-10);
	i+=dirMove(p,o,s,-9);
	i+=dirMove(p,o,s,-8);
	i+=dirMove(p,o,s,1);
	i+=dirMove(p,o,s,10);
	i+=dirMove(p,o,s,9);
	i+=dirMove(p,o,s,8);
	return i;
}

__inline char Rnd(float ev)
{
	if(ev>64) return 64;
	if(ev<-64) return -64;
	return (char)(ev+(ev>0 ? 0.5:-0.5));
}

float Eval()
{
	float ev=d4[gs][board[13]+board[21]*3+board[29]*9+board[37]*27];
	ev+=d4[gs][board[53]+board[61]*3+board[69]*9+board[77]*27];
	ev+=d4[gs][board[14]+board[24]*3+board[34]*9+board[44]*27];
	ev+=d4[gs][board[46]+board[56]*3+board[66]*9+board[76]*27];
	ev+=d5[gs][board[14]+board[22]*3+board[30]*9+board[38]*27+board[46]*81];
	ev+=d5[gs][board[44]+board[52]*3+board[60]*9+board[68]*27+board[76]*81];
	ev+=d5[gs][board[13]+board[23]*3+board[33]*9+board[43]*27+board[53]*81];
	ev+=d5[gs][board[37]+board[47]*3+board[57]*9+board[67]*27+board[77]*81];
	ev+=d6[gs][board[15]+board[23]*3+board[31]*9+board[39]*27+board[47]*81+board[55]*243];
	ev+=d6[gs][board[35]+board[43]*3+board[51]*9+board[59]*27+board[67]*81+board[75]*243];
	ev+=d6[gs][board[12]+board[22]*3+board[32]*9+board[42]*27+board[52]*81+board[62]*243];
	ev+=d6[gs][board[28]+board[38]*3+board[48]*9+board[58]*27+board[68]*81+board[78]*243];
	ev+=d7[gs][board[16]+board[24]*3+board[32]*9+board[40]*27+board[48]*81+board[56]*243+board[64]*729];
	ev+=d7[gs][board[26]+board[34]*3+board[42]*9+board[50]*27+board[58]*81+board[66]*243+board[74]*729];
	ev+=d7[gs][board[11]+board[21]*3+board[31]*9+board[41]*27+board[51]*81+board[61]*243+board[71]*729];
	ev+=d7[gs][board[19]+board[29]*3+board[39]*9+board[49]*27+board[59]*81+board[69]*243+board[79]*729];
	ev+=d8[gs][board[17]+board[25]*3+board[33]*9+board[41]*27+board[49]*81+board[57]*243+board[65]*729+board[73]*2187];
	ev+=d8[gs][board[10]+board[20]*3+board[30]*9+board[40]*27+board[50]*81+board[60]*243+board[70]*729+board[80]*2187];
	ev+=h2[gs][board[19]+board[20]*3+board[21]*9+board[22]*27+board[23]*81+board[24]*243+board[25]*729+board[26]*2187];
	ev+=h2[gs][board[64]+board[65]*3+board[66]*9+board[67]*27+board[68]*81+board[69]*243+board[70]*729+board[71]*2187];
	ev+=h2[gs][board[11]+board[20]*3+board[29]*9+board[38]*27+board[47]*81+board[56]*243+board[65]*729+board[74]*2187];
	ev+=h2[gs][board[16]+board[25]*3+board[34]*9+board[43]*27+board[52]*81+board[61]*243+board[70]*729+board[79]*2187];
	ev+=h3[gs][board[28]+board[29]*3+board[30]*9+board[31]*27+board[32]*81+board[33]*243+board[34]*729+board[35]*2187];
	ev+=h3[gs][board[55]+board[56]*3+board[57]*9+board[58]*27+board[59]*81+board[60]*243+board[61]*729+board[62]*2187];
	ev+=h3[gs][board[12]+board[21]*3+board[30]*9+board[39]*27+board[48]*81+board[57]*243+board[66]*729+board[75]*2187];
	ev+=h3[gs][board[15]+board[24]*3+board[33]*9+board[42]*27+board[51]*81+board[60]*243+board[69]*729+board[78]*2187];
	ev+=h4[gs][board[37]+board[38]*3+board[39]*9+board[40]*27+board[41]*81+board[42]*243+board[43]*729+board[44]*2187];
	ev+=h4[gs][board[46]+board[47]*3+board[48]*9+board[49]*27+board[50]*81+board[51]*243+board[52]*729+board[53]*2187];
	ev+=h4[gs][board[13]+board[22]*3+board[31]*9+board[40]*27+board[49]*81+board[58]*243+board[67]*729+board[76]*2187];
	ev+=h4[gs][board[14]+board[23]*3+board[32]*9+board[41]*27+board[50]*81+board[59]*243+board[68]*729+board[77]*2187];
	ev+=e2[gs][board[10]+board[11]*3+board[12]*9+board[13]*27+board[14]*81+board[15]*243+board[16]*729+board[17]*2187+board[20]*6561+board[25]*19683];
	ev+=e2[gs][board[17]+board[26]*3+board[35]*9+board[44]*27+board[53]*81+board[62]*243+board[71]*729+board[80]*2187+board[25]*6561+board[70]*19683];
	ev+=e2[gs][board[73]+board[64]*3+board[55]*9+board[46]*27+board[37]*81+board[28]*243+board[19]*729+board[10]*2187+board[65]*6561+board[20]*19683];
	ev+=e2[gs][board[80]+board[79]*3+board[78]*9+board[77]*27+board[76]*81+board[75]*243+board[74]*729+board[73]*2187+board[70]*6561+board[65]*19683];
	ev+=c3[gs][board[10]+board[11]*3+board[12]*9+board[19]*27+board[20]*81+board[21]*243+board[28]*729+board[29]*2187+board[30]*6561];
	ev+=c3[gs][board[17]+board[16]*3+board[15]*9+board[26]*27+board[25]*81+board[24]*243+board[35]*729+board[34]*2187+board[33]*6561];
	ev+=c3[gs][board[73]+board[74]*3+board[75]*9+board[64]*27+board[65]*81+board[66]*243+board[55]*729+board[56]*2187+board[57]*6561];
	ev+=c3[gs][board[80]+board[79]*3+board[78]*9+board[71]*27+board[70]*81+board[69]*243+board[62]*729+board[61]*2187+board[60]*6561];
	ev+=c2[gs][board[10]+board[11]*3+board[12]*9+board[13]*27+board[14]*81+board[19]*243+board[20]*729+board[21]*2187+board[22]*6561+board[23]*19683];
	ev+=c2[gs][board[10]+board[19]*3+board[28]*9+board[37]*27+board[46]*81+board[11]*243+board[20]*729+board[29]*2187+board[38]*6561+board[47]*19683];
	ev+=c2[gs][board[17]+board[16]*3+board[15]*9+board[14]*27+board[13]*81+board[26]*243+board[25]*729+board[24]*2187+board[23]*6561+board[22]*19683];
	ev+=c2[gs][board[17]+board[26]*3+board[35]*9+board[44]*27+board[53]*81+board[16]*243+board[25]*729+board[34]*2187+board[43]*6561+board[52]*19683];
	ev+=c2[gs][board[73]+board[74]*3+board[75]*9+board[76]*27+board[77]*81+board[64]*243+board[65]*729+board[66]*2187+board[67]*6561+board[68]*19683];
	ev+=c2[gs][board[73]+board[64]*3+board[55]*9+board[46]*27+board[37]*81+board[74]*243+board[65]*729+board[56]*2187+board[47]*6561+board[38]*19683];
	ev+=c2[gs][board[80]+board[79]*3+board[78]*9+board[77]*27+board[76]*81+board[71]*243+board[70]*729+board[69]*2187+board[68]*6561+board[67]*19683];
	ev+=c2[gs][board[80]+board[71]*3+board[62]*9+board[53]*27+board[44]*81+board[79]*243+board[70]*729+board[61]*2187+board[52]*6561+board[43]*19683];
	return ev;
}

void setError(float w)
{
	static char d4i[][4]={{13,21,29,37},{53,61,69,77},{14,24,34,44},{46,56,66,76}};
	static char d5i[][5]={{14,22,30,38,46},{44,52,60,68,76},{13,23,33,43,53},{37,47,57,67,77}};
	static char d6i[][6]={{15,23,31,39,47,55},{35,43,51,59,67,75},{12,22,32,42,52,62},{28,38,48,58,68,78}};
	static char d7i[][7]={{16,24,32,40,48,56,64},{26,34,42,50,58,66,74},{11,21,31,41,51,61,71},{19,29,39,49,59,69,79}};
	static char d8i[][8]={{17,25,33,41,49,57,65,73},{10,20,30,40,50,60,70,80}};
	static char h2i[][8]={{19,20,21,22,23,24,25,26},{64,65,66,67,68,69,70,71},{11,20,29,38,47,56,65,74},{16,25,34,43,52,61,70,79}};
	static char h3i[][8]={{28,29,30,31,32,33,34,35},{55,56,57,58,59,60,61,62},{12,21,30,39,48,57,66,75},{15,24,33,42,51,60,69,78}};
	static char h4i[][8]={{37,38,39,40,41,42,43,44},{46,47,48,49,50,51,52,53},{13,22,31,40,49,58,67,76},{14,23,32,41,50,59,68,77}};
	static char e2i[][10]={{10,11,12,13,14,15,16,17,20,25},{17,26,35,44,53,62,71,80,25,70},{73,64,55,46,37,28,19,10,65,20},{80,79,78,77,76,75,74,73,70,65}};
	static char c3i[][9]={{10,11,12,19,20,21,28,29,30},{17,16,15,26,25,24,35,34,33},{73,74,75,64,65,66,55,56,57},{80,79,78,71,70,69,62,61,60}};
	static char c2i[][10]={{10,11,12,13,14,19,20,21,22,23},{10,19,28,37,46,11,20,29,38,47},{17,16,15,14,13,26,25,24,23,22},{17,26,35,44,53,16,25,34,43,52},{73,74,75,76,77,64,65,66,67,68},{73,64,55,46,37,74,65,56,47,38},{80,79,78,77,76,71,70,69,68,67},{70,71,62,53,44,79,70,61,52,43}};
	static unsigned short os[]={1,3,9,27,81,243,729,2187,6561,19683};
	unsigned short i1,i2;
	char i,j;

	for(i=0;i<2;i++) {
		if(i1=board[d8i[i][0]]+board[d8i[i][1]]*3+board[d8i[i][2]]*9+board[d8i[i][3]]*27+board[d8i[i][4]]*81+board[d8i[i][5]]*243+board[d8i[i][6]]*729+board[d8i[i][7]]*2187) {
			d8e[gs][i1]+=w;
			d8c[gs][i1]++;
			for(i2=j=0;j<8;j++) {
				if(board[d8i[i][j]]!=2) i2+=(board[d8i[i][j]]^1)*os[j];
			}
			d8e[gs][i2]-=w;
			d8c[gs][i2]++;
			if(i1!=(i2=board[d8i[i][7]]+board[d8i[i][6]]*3+board[d8i[i][5]]*9+board[d8i[i][4]]*27+board[d8i[i][3]]*81+board[d8i[i][2]]*243+board[d8i[i][1]]*729+board[d8i[i][0]]*2187)) {
				d8e[gs][i2]+=w;
				d8c[gs][i2]++;
				for(i1=0,j=7;j>=0;j--) {
					if(board[d8i[i][j]]!=2) i1+=(board[d8i[i][j]]^1)*os[j];
				}
				d8e[gs][i1]-=w;
				d8c[gs][i1]++;
			}
		}
	}
	for(i=0;i<4;i++) {
		if(i1=board[d4i[i][0]]+board[d4i[i][1]]*3+board[d4i[i][2]]*9+board[d4i[i][3]]*27) {
			d4e[gs][i1]+=w;
			d4c[gs][i1]++;
			for(i2=j=0;j<4;j++) {
				if(board[d4i[i][j]]!=2) i2+=(board[d4i[i][j]]^1)*os[j];
			}
			d4e[gs][i2]-=w;
			d4c[gs][i2]++;
			if(i1!=(i2=board[d4i[i][3]]+board[d4i[i][2]]*3+board[d4i[i][1]]*9+board[d4i[i][0]]*27)) {
				d4e[gs][i2]+=w;
				d4c[gs][i2]++;
				for(i1=0,j=3;j>=0;j--) {
					if(board[d4i[i][j]]!=2) i1+=(board[d4i[i][j]]^1)*os[j];
				}
				d4e[gs][i1]-=w;
				d4c[gs][i1]++;
			}
		}
		if(i1=board[d5i[i][0]]+board[d5i[i][1]]*3+board[d5i[i][2]]*9+board[d5i[i][3]]*27+board[d5i[i][4]]*81) {
			d5e[gs][i1]+=w;
			d5c[gs][i1]++;
			for(i2=j=0;j<5;j++) {
				if(board[d5i[i][j]]!=2) i2+=(board[d5i[i][j]]^1)*os[j];
			}
			d5e[gs][i2]-=w;
			d5c[gs][i2]++;
			if(i1!=(i2=board[d5i[i][4]]+board[d5i[i][3]]*3+board[d5i[i][2]]*9+board[d5i[i][1]]*27+board[d5i[i][0]]*81)) {
				d5e[gs][i2]+=w;
				d5c[gs][i2]++;
				for(i1=0,j=4;j>=0;j--) {
					if(board[d5i[i][j]]!=2) i1+=(board[d5i[i][j]]^1)*os[j];
				}
				d5e[gs][i1]-=w;
				d5c[gs][i1]++;
			}
		}
		if(i1=board[d6i[i][0]]+board[d6i[i][1]]*3+board[d6i[i][2]]*9+board[d6i[i][3]]*27+board[d6i[i][4]]*81+board[d6i[i][5]]*243) {
			d6e[gs][i1]+=w;
			d6c[gs][i1]++;
			for(i2=j=0;j<6;j++) {
				if(board[d6i[i][j]]!=2) i2+=(board[d6i[i][j]]^1)*os[j];
			}
			d6e[gs][i2]-=w;
			d6c[gs][i2]++;
			if(i1!=(i2=board[d6i[i][5]]+board[d6i[i][4]]*3+board[d6i[i][3]]*9+board[d6i[i][2]]*27+board[d6i[i][1]]*81+board[d6i[i][0]]*243)) {
				d6e[gs][i2]+=w;
				d6c[gs][i2]++;
				for(i1=0,j=5;j>=0;j--) {
					if(board[d6i[i][j]]!=2) i1+=(board[d6i[i][j]]^1)*os[j];
				}
				d6e[gs][i1]-=w;
				d6c[gs][i1]++;
			}
		}
		if(i1=board[d7i[i][0]]+board[d7i[i][1]]*3+board[d7i[i][2]]*9+board[d7i[i][3]]*27+board[d7i[i][4]]*81+board[d7i[i][5]]*243+board[d7i[i][6]]*729) {
			d7e[gs][i1]+=w;
			d7c[gs][i1]++;
			for(i2=j=0;j<7;j++) {
				if(board[d7i[i][j]]!=2) i2+=(board[d7i[i][j]]^1)*os[j];
			}
			d7e[gs][i2]-=w;
			d7c[gs][i2]++;
			if(i1!=(i2=board[d7i[i][6]]+board[d7i[i][5]]*3+board[d7i[i][4]]*9+board[d7i[i][3]]*27+board[d7i[i][2]]*81+board[d7i[i][1]]*243+board[d7i[i][0]]*729)) {
				d7e[gs][i2]+=w;
				d7c[gs][i2]++;
				for(i1=0,j=6;j>=0;j--) {
					if(board[d7i[i][j]]!=2) i1+=(board[d7i[i][j]]^1)*os[j];
				}
				d7e[gs][i1]-=w;
				d7c[gs][i1]++;
			}
		}
		if(i1=board[h2i[i][0]]+board[h2i[i][1]]*3+board[h2i[i][2]]*9+board[h2i[i][3]]*27+board[h2i[i][4]]*81+board[h2i[i][5]]*243+board[h2i[i][6]]*729+board[h2i[i][7]]*2187) {
			h2e[gs][i1]+=w;
			h2c[gs][i1]++;
			for(i2=j=0;j<8;j++) {
				if(board[h2i[i][j]]!=2) i2+=(board[h2i[i][j]]^1)*os[j];
			}
			h2e[gs][i2]-=w;
			h2c[gs][i2]++;
			if(i1!=(i2=board[h2i[i][7]]+board[h2i[i][6]]*3+board[h2i[i][5]]*9+board[h2i[i][4]]*27+board[h2i[i][3]]*81+board[h2i[i][2]]*243+board[h2i[i][1]]*729+board[h2i[i][0]]*2187)) {
				h2e[gs][i2]+=w;
				h2c[gs][i2]++;
				for(i1=0,j=7;j>=0;j--) {
					if(board[h2i[i][j]]!=2) i1+=(board[h2i[i][j]]^1)*os[j];
				}
				h2e[gs][i1]-=w;
				h2c[gs][i1]++;
			}
		}
		if(i1=board[h3i[i][0]]+board[h3i[i][1]]*3+board[h3i[i][2]]*9+board[h3i[i][3]]*27+board[h3i[i][4]]*81+board[h3i[i][5]]*243+board[h3i[i][6]]*729+board[h3i[i][7]]*2187) {
			h3e[gs][i1]+=w;
			h3c[gs][i1]++;
			for(i2=j=0;j<8;j++) {
				if(board[h3i[i][j]]!=2) i2+=(board[h3i[i][j]]^1)*os[j];
			}
			h3e[gs][i2]-=w;
			h3c[gs][i2]++;
			if(i1!=(i2=board[h3i[i][7]]+board[h3i[i][6]]*3+board[h3i[i][5]]*9+board[h3i[i][4]]*27+board[h3i[i][3]]*81+board[h3i[i][2]]*243+board[h3i[i][1]]*729+board[h3i[i][0]]*2187)) {
				h3e[gs][i2]+=w;
				h3c[gs][i2]++;
				for(i1=0,j=7;j>=0;j--) {
					if(board[h3i[i][j]]!=2) i1+=(board[h3i[i][j]]^1)*os[j];
				}
				h3e[gs][i1]-=w;
				h3c[gs][i1]++;
			}
		}
		if(i1=board[h4i[i][0]]+board[h4i[i][1]]*3+board[h4i[i][2]]*9+board[h4i[i][3]]*27+board[h4i[i][4]]*81+board[h4i[i][5]]*243+board[h4i[i][6]]*729+board[h4i[i][7]]*2187) {
			h4e[gs][i1]+=w;
			h4c[gs][i1]++;
			for(i2=j=0;j<8;j++) {
				if(board[h4i[i][j]]!=2) i2+=(board[h4i[i][j]]^1)*os[j];
			}
			h4e[gs][i2]-=w;
			h4c[gs][i2]++;
			if(i1!=(i2=board[h4i[i][7]]+board[h4i[i][6]]*3+board[h4i[i][5]]*9+board[h4i[i][4]]*27+board[h4i[i][3]]*81+board[h4i[i][2]]*243+board[h4i[i][1]]*729+board[h4i[i][0]]*2187)) {
				h4e[gs][i2]+=w;
				h4c[gs][i2]++;
				for(i1=0,j=7;j>=0;j--) {
					if(board[h4i[i][j]]!=2) i1+=(board[h4i[i][j]]^1)*os[j];
				}
				h4e[gs][i1]-=w;
				h4c[gs][i1]++;
			}
		}
		if(i1=board[e2i[i][0]]+board[e2i[i][1]]*3+board[e2i[i][2]]*9+board[e2i[i][3]]*27+board[e2i[i][4]]*81+board[e2i[i][5]]*243+board[e2i[i][6]]*729+board[e2i[i][7]]*2187+board[e2i[i][8]]*6561+board[e2i[i][9]]*19683) {
			e2e[gs][i1]+=w;
			e2c[gs][i1]++;
			for(i2=j=0;j<10;j++) {
				if(board[e2i[i][j]]!=2) i2+=(board[e2i[i][j]]^1)*os[j];
			}
			e2e[gs][i2]-=w;
			e2c[gs][i2]++;
			if(i1!=(i2=board[e2i[i][7]]+board[e2i[i][6]]*3+board[e2i[i][5]]*9+board[e2i[i][4]]*27+board[e2i[i][3]]*81+board[e2i[i][2]]*243+board[e2i[i][1]]*729+board[e2i[i][0]]*2187+board[e2i[i][9]]*6561+board[e2i[i][8]]*19683)) {
				e2e[gs][i2]+=w;
				e2c[gs][i2]++;
				for(i1=0,j=7;j>=0;j--) {
					if(board[e2i[i][j]]!=2) i1+=(board[e2i[i][j]]^1)*os[j];
				}
				if(board[e2i[i][9]]!=2) i1+=(board[e2i[i][9]]^1)*6561;
				if(board[e2i[i][8]]!=2) i1+=(board[e2i[i][8]]^1)*19683;
				e2e[gs][i1]-=w;
				e2c[gs][i1]++;
			}
		}
		if(i1=board[c3i[i][0]]+board[c3i[i][1]]*3+board[c3i[i][2]]*9+board[c3i[i][3]]*27+board[c3i[i][4]]*81+board[c3i[i][5]]*243+board[c3i[i][6]]*729+board[c3i[i][7]]*2187+board[c3i[i][8]]*6561) {
			c3e[gs][i1]+=w;
			c3c[gs][i1]++;
			for(i2=j=0;j<10;j++) {
				if(board[c3i[i][j]]!=2) i2+=(board[c3i[i][j]]^1)*os[j];
			}
			c3e[gs][i2]-=w;
			c3c[gs][i2]++;
			if(i1!=(i2=board[c3i[i][0]]+board[c3i[i][3]]*3+board[c3i[i][6]]*9+board[c3i[i][1]]*27+board[c3i[i][4]]*81+board[c3i[i][7]]*243+board[c3i[i][2]]*729+board[c3i[i][5]]*2187+board[c3i[i][8]]*6561)) {
				c3e[gs][i2]+=w;
				c3c[gs][i2]++;
				for(i1=j=0;j<9;j++) {
					if(board[c3i[i][j%3*3+j/3]]!=2) i1+=(board[c3i[i][j%3*3+j/3]]^1)*os[j];
				}
				c3e[gs][i1]-=w;
				c3c[gs][i1]++;
			}
		}
	}
	for(i=0;i<8;i++) {
		if(i1=board[c2i[i][0]]+board[c2i[i][1]]*3+board[c2i[i][2]]*9+board[c2i[i][3]]*27+board[c2i[i][4]]*81+board[c2i[i][5]]*243+board[c2i[i][6]]*729+board[c2i[i][7]]*2187+board[c2i[i][8]]*6561+board[c2i[i][9]]*19683) {
			c2e[gs][i1]+=w;
			c2c[gs][i1]++;
			for(i1=j=0;j<10;j++) {
				if(board[c2i[i][j]]!=2) i1+=(board[c2i[i][j]]^1)*os[j];
			}
			c2e[gs][i1]-=w;
			c2c[gs][i1]++;
		}
	}
}

void updateEval()
{
	char i;
	unsigned short j;

	for(i=0;i<N;i++) {
		for(j=0;j<81;j++) if(d4c[i][j]>=MC) d4[i][j]+=B*d4e[i][j]/(float)d4c[i][j];
		for(j=0;j<243;j++) if(d5c[i][j]>=MC) d5[i][j]+=B*d5e[i][j]/(float)d5c[i][j];
		for(j=0;j<729;j++) if(d6c[i][j]>=MC) d6[i][j]+=B*d6e[i][j]/(float)d6c[i][j];
		for(j=0;j<2187;j++) if(d7c[i][j]>=MC) d7[i][j]+=B*d7e[i][j]/(float)d7c[i][j];
		for(j=0;j<6561;j++) {
			if(d8c[i][j]>=MC) d8[i][j]+=B*d8e[i][j]/(float)d8c[i][j];
			if(h2c[i][j]>=MC) h2[i][j]+=B*h2e[i][j]/(float)h2c[i][j];
			if(h3c[i][j]>=MC) h3[i][j]+=B*h3e[i][j]/(float)h3c[i][j];
			if(h4c[i][j]>=MC) h4[i][j]+=B*h4e[i][j]/(float)h4c[i][j];
		}
		for(j=0;j<59049;j++) {
			if(e2c[i][j]>=MC) e2[i][j]+=B*e2e[i][j]/(float)e2c[i][j];
			if(c2c[i][j]>=MC) c2[i][j]+=B*c2e[i][j]/(float)c2c[i][j];
		}
		for(j=0;j<19683;j++) if(c3c[i][j]>=MC) c3[i][j]+=B*c3e[i][j]/(float)c3c[i][j];
	}
}

void modelError()
{
	char i;
	unsigned short j;

	for(i=0;i<N;i++) {
		for(j=0;j<81;j++) d4e[i][j]*=MI;
		for(j=0;j<243;j++) d5e[i][j]*=MI;
		for(j=0;j<729;j++) d6e[i][j]*=MI;
		for(j=0;j<2187;j++) d7e[i][j]*=MI;
		for(j=0;j<6561;j++) {
			d8e[i][j]*=MI;
			h2e[i][j]*=MI;
			h3e[i][j]*=MI;
			h4e[i][j]*=MI;
		}
		for(j=0;j<59049;j++) {
			e2e[i][j]*=MI;
			c2e[i][j]*=MI;
		}
		for(j=0;j<19683;j++) c3e[i][j]*=MI;
	}
}

void prepareEval(float *ptn,char *p,char cnt)
{
	char o[]={1,0,2},i;
	unsigned short i1,i2,i3,os;
	float ev;

	if(!cnt) {
		i1=p[9]+p[8]*3+p[7]*9+p[6]*27+p[5]*81+p[4]*243+p[3]*729+p[2]*2187+p[1]*6561+p[0]*19683;
		i2=p[2]+p[3]*3+p[4]*9+p[5]*27+p[6]*81+p[7]*243+p[8]*729+p[9]*2187+p[0]*6561+p[1]*19683;
		ptn[i1]=ptn[i2]=(ptn[i1]+ptn[i2])/2;

		i3=o[p[9]]+o[p[8]]*3+o[p[7]]*9+o[p[6]]*27+o[p[5]]*81+o[p[4]]*243+o[p[3]]*729+o[p[2]]*2187+o[p[1]]*6561+o[p[0]]*19683;
		ev=(ptn[i1]-ptn[i3])/2;
		ptn[i1]=ev;
		ptn[i3]=-ev;

		i3=o[p[2]]+o[p[3]]*3+o[p[4]]*9+o[p[5]]*27+o[p[6]]*81+o[p[7]]*243+o[p[8]]*729+o[p[9]]*2187+o[p[0]]*6561+o[p[1]]*19683;
		ev=(ptn[i2]-ptn[i3])/2;
		ptn[i2]=ev;
		ptn[i3]=-ev;
	} else if(cnt==1) {
		i1=p[8]+p[7]*3+p[6]*9+p[5]*27+p[4]*81+p[3]*243+p[2]*729+p[1]*2187+p[0]*6561;
		i2=p[8]+p[5]*3+p[2]*9+p[7]*27+p[4]*81+p[1]*243+p[6]*729+p[3]*2187+p[0]*6561;
		ptn[i1]=ptn[i2]=(ptn[i1]+ptn[i2])/2;

		i3=o[p[8]]+o[p[7]]*3+o[p[6]]*9+o[p[5]]*27+o[p[4]]*81+o[p[3]]*243+o[p[2]]*729+o[p[1]]*2187+o[p[0]]*6561;
		ev=(ptn[i1]-ptn[i3])/2;
		ptn[i1]=ev;
		ptn[i3]=-ev;

		i3=o[p[8]]+o[p[5]]*3+o[p[2]]*9+o[p[7]]*27+o[p[4]]*81+o[p[1]]*243+o[p[6]]*729+o[p[3]]*2187+o[p[0]]*6561;
		ev=(ptn[i2]-ptn[i3])/2;
		ptn[i2]=ev;
		ptn[i3]=-ev;
	} else if(cnt==2) {
		i1=p[9]+p[8]*3+p[7]*9+p[6]*27+p[5]*81+p[4]*243+p[3]*729+p[2]*2187+p[1]*6561+p[0]*19683;
		i2=o[p[9]]+o[p[8]]*3+o[p[7]]*9+o[p[6]]*27+o[p[5]]*81+o[p[4]]*243+o[p[3]]*729+o[p[2]]*2187+o[p[1]]*6561+o[p[0]]*19683;
		ev=(ptn[i1]-ptn[i2])/2;
		ptn[i1]=ev;
		ptn[i2]=-ev;
	} else {
		for(i1=i2=i=0,os=1;i<=cnt;i1+=p[cnt-i]*os,i2+=p[i]*os,os*=3,i++);
		ptn[i1]=ptn[i2]=(ptn[i1]+ptn[i2])/2;

		for(i3=i=0,os=1;i<=cnt;i3+=o[p[cnt-i]]*os,os*=3,i++);
		ev=(ptn[i1]-ptn[i3])/2;
		ptn[i1]=ev;
		ptn[i3]=-ev;

		for(i3=i=0,os=1;i<=cnt;i3+=o[p[i]]*os,os*=3,i++);
		ev=(ptn[i2]-ptn[i3])/2;
		ptn[i2]=ev;
		ptn[i3]=-ev;
	}
}

void preparePtn()
{
	char p[10],a,b,c,d,e,f,g,h,i,j,k;

	for(a=0;a<N;a++) {
	for(b=0;b<3;b++) {
		p[0]=b;
	for(c=0;c<3;c++) {
		p[1]=c;
	for(d=0;d<3;d++) {
		p[2]=d;
	for(e=0;e<3;e++) {
		p[3]=e;
		prepareEval(d4[a],p,3);
	for(f=0;f<3;f++) {
		p[4]=f;
		prepareEval(d5[a],p,4);
	for(g=0;g<3;g++) {
		p[5]=g;
		prepareEval(d6[a],p,5);
	for(h=0;h<3;h++) {
		p[6]=h;
		prepareEval(d7[a],p,6);
	for(i=0;i<3;i++) {
		p[7]=i;
		prepareEval(d8[a],p,7);
		prepareEval(h2[a],p,7);
		prepareEval(h3[a],p,7);
		prepareEval(h4[a],p,7);
	for(j=0;j<3;j++) {
		p[8]=j;
		prepareEval(c3[a],p,1);
	for(k=0;k<3;k++) {
		p[9]=k;
		prepareEval(e2[a],p,0);
		prepareEval(c2[a],p,2);
	}}}}}}}}}}}
}

void writeEval()
{
	char i;
	unsigned short j;
	FILE *f;

	if(!(f=fopen("koeval.dat","wb"))) return;
	for(i=0;i<N;i++) for(j=0;j<81;j++) fputc(Rnd(d4[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<243;j++) fputc(Rnd(d5[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<729;j++) fputc(Rnd(d6[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<2187;j++) fputc(Rnd(d7[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<6561;j++) fputc(Rnd(d8[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<6561;j++) fputc(Rnd(h2[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<6561;j++) fputc(Rnd(h3[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<6561;j++) fputc(Rnd(h4[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<59049;j++) fputc(Rnd(e2[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<19683;j++) fputc(Rnd(c3[i][j]),f);
	for(i=0;i<N;i++) for(j=0;j<59049;j++) fputc(Rnd(c2[i][j]),f);
	fclose(f);
}

void main(int argc,char **argv)
{
	char buf[256],diff,blen,p;
	int epoch,tlc,lc,nl,max=0,t,i,j;
	FILE *f;

	if(argc!=2) return;
	strcpy(buf,argv[1]);
	*strrchr(buf,92)=0;
	if(chdir(buf)) return;

	for(i=0;i<10;i++) board[i]=board[i+81]=board[i*9]=3;
	printf("Game file: %s\nLearning late: %.2f\nEpoch: ",argv[1],B);
	scanf("%d",&epoch);
	printf("MinCount: %d\nModel error: %.2f\nLearning count: ",MC,MI);
	scanf("%d",&tlc);

	for(lc=1;lc<=tlc;lc++) {
		if(!(f=fopen(argv[1],"r"))) return;
		printf("\b\b\b\b\b\b\b       \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bInitializing");
		initEval();
		printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bLearn %3d:        <-Next %6d\b\b\b\b\b\b\b\b\b\b\b\b\b\b",lc,epoch);
		t=0;
		nl=1;

		while(fscanf(f,"%3d %s",&diff,buf)!=-1) {
			if(lc>1 && t>=max) break;
			printf("\b\b\b\b\b\b%6d",++t);

			for(blen=i=0;buf[i];i+=2,blen++) game[blen]=(buf[i+1]-48)*9+buf[i]-96;

			for(i=10;i<81;i++) {
				if(board[i]!=3) board[i]=2;
			}
			board[41]=board[49]=0;
			board[40]=board[50]=1;

			for(p=i=0;i<blen;i++) {
				if((gs=(i-9)/4)<0) gs=0;
				if(gs>12) gs=12;
				setError((float)diff-Eval()/(float)10.1);
				if(t%epoch==0) {
					updateEval();
					modelError();
					if(!i) {
						printf(" U            \b\b\b\b\b\b\b\b\b\b\b\b\b\b");
						if(lc==1) max=t;
					} else if(i+1==blen) {
						j=epoch*++nl;
						if(lc>1 && j==max) printf(" <-Last ");
						else printf(" <-Next ");
						printf("%6d\b\b\b\b\b\b\b\b\b\b\b\b\b\b",j);
					}
				}

				for(j=0;j<2;j++) {
					if(Move(p,board+game[i])) break;
					p^=1;
				}
				if(j==2) break;
				board[game[i]]=p;
				p^=1;
			}
		}

		fclose(f);
		printf(" P            \b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		preparePtn();
		printf(" W");
		writeEval();
	}
	printf("\b\b\b\b    \b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bLearn finished!\n");
}