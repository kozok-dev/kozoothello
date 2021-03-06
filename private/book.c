#include "ko.h"

extern char board[],pv[],ap[],e,br,bmo,*square;
extern short me,es[];
unsigned __int64 key,os[81];
FILE *bf;
BH *hash[5000];

MOVS *addPV()
{
	char s;
	MOVS *mp;

	if(!(s=fgetc(bf)) || feof(bf) || !(mp=(MOVS *)malloc(sizeof(MOVS)))) return 0;

	mp->next=0;
	mp->s=s;
	mp->ev=fgetc(bf);
	return mp;
}

BH *addHash()
{
	char i;
	MOVS *mp;
	BH *hp;

	if(!(hp=(BH *)malloc(sizeof(BH)))) return 0;

	for(hp->next=i=0,hp->key=key;;i++) {
		if(i) {
			for(mp=hp->movs;mp->next;mp=mp->next);
			if(!(mp->next=addPV())) break;
		} else {
			if(!(hp->movs=addPV())) break;
		}
	}
	return hp;
}

void initBook()
{
	short i;
	__int64 k=1;
	BH *hp;

	for(i=0;i<5000;i++) hash[i]=0;
	if(!chdir(ap) && (bf=fopen("kobook.dat","rb"))) {
		for(;;) {
			fread(&key,8,1,bf);
			if(feof(bf)) break;
			i=key%5000;
			if(hash[i]) {
				for(hp=hash[i];hp->next;hp=hp->next);
				hp->next=addHash();
			} else {
				hash[i]=addHash();
			}
		}
		fclose(bf);
	}

	for(i=0;i<10;i++) board[i]=board[i+81]=board[i*9]=3;
	for(i=10;i<81;i++) {
		if(board[i]==3) continue;
		board[i]=2;
		os[i]=k;
		k*=3;
	}
}

MOVS *learnPV(char s,char ev)
{
	MOVS *mp;

	if(!(mp=(MOVS *)malloc(sizeof(MOVS)))) return 0;

	mp->next=0;
	mp->s=s;
	mp->ev=ev;
	return mp;
}

BH *learnHash(char s,char ev)
{
	BH *hp;

	if(!(hp=(BH *)malloc(sizeof(BH)))) return 0;

	hp->next=0;
	hp->key=key;
	hp->movs=learnPV(s,ev);
	return hp;
}

void WINAPI addBook(char s,char ev)
{
	short i;
	char j=0;
	MOVS *mp,*tp;
	BH *hp;

	for(key=0,i=10;i<81;i++) {
		if(board[i] && board[i]!=3) key+=board[i]*os[i];
	}
	i=key%5000;
	for(hp=hash[i];hp;hp=hp->next) {
		if(key==hp->key) {
			j=1;
			break;
		}
	}
	if(j) {
		for(mp=hp->movs,j=0;mp;mp=mp->next) {
			if(s==mp->s) {
				mp->ev=ev;
				j=1;
				break;
			}
		}
		if(!j) {
			for(mp=hp->movs;mp->next;mp=mp->next);
			mp->next=learnPV(s,ev);
		}
		for(mp=hp->movs;mp->next;mp=mp->next) {
			for(tp=mp->next;tp;tp=tp->next) {
				if(mp->ev<tp->ev) {
					i=mp->s;
					mp->s=tp->s;
					tp->s=i;
					i=mp->ev;
					mp->ev=tp->ev;
					tp->ev=i;
				}
			}
		}
	} else {
		if(hash[i]) {
			for(hp=hash[i];hp->next;hp=hp->next);
			hp->next=learnHash(s,ev);
		} else {
			hash[i]=learnHash(s,ev);
		}
	}
}

char WINAPI writeBook()
{
	short i;
	FILE *f;
	MOVS *mp;
	BH *hp;

	if(chdir(ap) || !(f=fopen("kobook.dat","wb"))) return 0;

	for(i=0;i<5000;i++) {
		for(hp=hash[i];hp;hp=hp->next) {
			fwrite(&hp->key,8,1,f);
			for(mp=hp->movs;mp;mp=mp->next) {
				fputc(mp->s,f);
				fputc(mp->ev,f);
			}
			fputc(0,f);
		}
	}
	fclose(f);
	return 1;
}

void Book(char p)
{
	char b,i,j=0,k;
	MOVS *mp;
	BH *bp;
	MOVE move;

	for(b=i=0;i<8;i++) {
		for(;b<15;b++) {
			for(key=0,j=10;j<81;j++) {
				if(!board[j] || board[j]==3) continue;
				if(!i) key+=board[j]*os[j];
				else if(i==1) key+=board[j]*os[j%9*9+j/9];
				else if(i==2) key+=board[j]*os[(9-j/9)*9+9-j%9];
				else if(i==3) key+=board[j]*os[(9-j%9)*9+9-j/9];
				else if(i==4) key+=board[j]*os[(9-j%9)*9+j/9];
				else if(i==5) key+=board[j]*os[(9-j/9)*9+j%9];
				else if(i==6) key+=board[j]*os[j%9*9+9-j/9];
				else key+=board[j]*os[j/9*9+9-j%9];
			}
			for(bp=hash[key%5000],j=0;bp;bp=bp->next) {
				if(key==bp->key) {
					j=1;
					break;
				}
			}
			if(!j) break;
			if(e==60) {
				j=rand()&3;
				if(i<4) {
					if(!j) pv[0]=51;
					else if(j==1) pv[0]=59;
					else if(j==2) pv[0]=39;
					else pv[0]=31;
					me=es[51]=es[59]=es[39]=es[31]=bp->movs->ev+1500;
				} else {
					if(!j) pv[0]=32;
					else if(j==1) pv[0]=42;
					else if(j==2) pv[0]=58;
					else pv[0]=48;
					me=es[32]=es[42]=es[58]=es[48]=bp->movs->ev+1500;
				}
			} else {
				for(mp=bp->movs,k=0;mp;mp=mp->next) {
					if(bmo && (b && bp->movs->ev>mp->ev || !b && bp->movs->ev-br>mp->ev)) continue;
					if(bmo && k && rand()&1) continue;
					k=1;
					j=mp->s;
					if(i==1) j=j%9*9+j/9;
					else if(i==2) j=(9-j/9)*9+9-j%9;
					else if(i==3) j=(9-j%9)*9+9-j/9;
					else if(i==4) j=j%9*9+9-j/9;
					else if(i==5) j=(9-j/9)*9+j%9;
					else if(i==6) j=(9-j%9)*9+j/9;
					else if(i==7) j=j/9*9+9-j%9;
					pv[b]=j;
					if(!b) me=es[pv[0]]=mp->ev+1500;
				}
			}
			square=board+pv[b];
			if(p) Flip2(&move);
			else Flip1(&move);
			e=0;
			p^=1;
		}
	}
}

char WINAPI getBook(char t,char n,char g)
{
	static struct {
		__int64 key;
		char p,book[21],board[51];
	} book[]={
		0x41e69a3b9da348a3,1,{60,0},{40,1,41,0,49,0,50,1,51,0,60,1,0},
		0x3b70f2eecb35767c,0,{60,59,42,70,0},{40,1,41,1,42,1,49,0,50,0,51,1,59,0,60,0,70,0,0},
		0x6de8ab4297678fbc,0,{60,59,42,61,0},{40,1,41,1,42,1,49,0,50,0,51,1,59,0,60,0,61,0,0},
		0x56c1e919a34ba37e,0,{60,59,42,61,48,43,52,33,32,39,0},{32,1,33,0,39,0,40,0,41,0,42,0,43,0,48,1,49,0,50,1,51,0,52,1,59,0,60,0,61,0,0},
		0x5570b5646573a3dc,0,{60,59,42,52,0},{40,1,41,1,42,1,49,0,50,0,51,0,52,0,59,0,60,1,0},
		0x859bfc077cf8ebec,0,{60,59,42,52,68,69,48,32,33,39,31,21,0},{21,0,31,0,32,1,33,1,39,0,40,0,41,0,42,0,48,1,49,0,50,0,51,0,52,0,59,0,60,0,68,1,69,0,0},
		0xc67f0e8ebda52658,0,{60,59,42,52,68,32,0},{32,0,40,1,41,1,42,0,49,0,50,1,51,0,52,0,59,1,60,1,68,1,0},
		0x6c691bddb7c511e6,0,{60,59,42,52,68,67,0},{40,1,41,1,42,1,49,0,50,1,51,0,52,0,59,0,60,1,67,0,68,1,0},
		0xef2dbc6c76f0f93e,1,{60,59,42,52,58,0},{40,1,41,1,42,1,49,1,50,1,51,0,52,0,58,1,59,1,60,1,0},
		0x7aed9fd6ae1ead1c,0,{60,59,42,43,0},{40,1,41,1,42,1,43,0,49,0,50,0,51,0,59,0,60,1,0},
		0x812e92b3fa3b853c,0,{60,59,42,34,0},{34,0,40,1,41,1,42,0,49,0,50,0,51,1,59,0,60,1,0},
		0x7aed9fc5a1b86a37,0,{60,59,42,34,33,43,0},{33,1,34,0,40,1,41,1,42,1,43,0,49,0,50,0,51,0,59,0,60,1,0},
		0x18d65b75802713cd,1,{60,59,42,34,58,0},{34,0,40,1,41,1,42,0,49,1,50,0,51,1,58,1,59,1,60,1,0},
		0x812ebc5dcdaa3cae,0,{60,59,42,32,0},{32,0,40,1,41,0,42,1,49,0,50,0,51,1,59,0,60,1,0},
		0x1aebc365df279210,1,{60,59,42,32,58,0},{32,0,40,1,41,0,42,1,49,1,50,1,51,1,58,1,59,1,60,1,0},
		0x5e3bdae12dd8f04c,0,{60,59,42,32,48,52,0},{32,0,40,1,41,0,42,0,48,1,49,1,50,1,51,1,52,0,59,0,60,1,0},
		0x4af491a253ab4d8d,0,{60,59,42,32,48,52,34,43,33,61,0},{32,0,33,1,34,1,40,1,41,1,42,1,43,0,48,1,49,1,50,1,51,1,52,0,59,0,60,0,61,0,0},
		0x5d889957f0159ce6,0,{60,59,42,32,48,52,34,43,33,39,0},{32,0,33,1,34,1,39,0,40,0,41,0,42,0,43,0,48,1,49,0,50,1,51,1,52,0,59,0,60,1,0},
		0x1f60cbe84b90e75a,0,{60,59,42,32,48,52,33,43,61,34,58,69,53,44,35,68,0},{32,0,33,0,34,0,35,1,40,1,41,0,42,0,43,1,44,1,48,1,49,1,50,0,51,1,52,0,53,1,58,1,59,0,60,0,61,1,68,0,69,0,0},
		0x02ff2d74002686dd,0,{60,59,42,32,48,52,58,61,0},{32,0,40,1,41,0,42,0,48,1,49,1,50,1,51,0,52,0,58,1,59,1,60,1,61,0,0},
		0x8308af46471f5169,0,{60,59,42,32,48,39,0},{32,0,39,0,40,0,41,0,42,1,48,1,49,0,50,1,51,1,59,0,60,1,0},
		0xf2020896f4aac1c9,1,{60,59,42,32,48,39,68,0},{32,0,39,0,40,0,41,0,42,1,48,1,49,0,50,1,51,1,59,1,60,1,68,1,0},
		0xf20053a82bd252c4,0,{60,59,42,32,48,39,68,43,0},{32,0,39,0,40,0,41,0,42,0,43,0,48,1,49,0,50,1,51,1,59,1,60,1,68,1,0},
		0xf19f5db7efc7970b,1,{60,59,42,32,48,39,68,47,23,0},{23,1,32,1,39,0,40,0,41,1,42,1,47,0,48,0,49,0,50,1,51,1,59,1,60,1,68,1,0},
		0x8308c4105dc276f9,1,{60,59,42,32,48,39,31,33,23,0},{23,1,31,1,32,1,33,0,39,0,40,0,41,1,42,1,48,1,49,0,50,1,51,1,59,0,60,1,0},
		0x8308c413f9794652,0,{60,59,42,32,48,39,31,30,0},{30,0,31,0,32,0,39,0,40,0,41,1,42,1,48,1,49,0,50,1,51,1,59,0,60,1,0},
		0x812eca3e313250b4,0,{60,59,42,30,0},{30,0,40,0,41,1,42,1,49,0,50,0,51,1,59,0,60,1,0},
		0xe7cd31da7861f333,0,{60,59,42,30,67,33,0},{30,0,33,0,40,0,41,0,42,1,49,0,50,0,51,1,59,1,60,1,67,1,0},
		0x1ab0954cd055a356,1,{60,59,42,30,58,33,39,48,38,0},{30,0,33,0,38,1,39,1,40,1,41,1,42,1,48,1,49,1,50,1,51,1,58,1,59,1,60,1,0},
		0x812eceddd7375086,1,{60,59,42,30,39,0},{30,0,39,1,40,1,41,1,42,1,49,0,50,0,51,1,59,0,60,1,0},
		0x801c13cbfe78fe2e,1,{60,59,42,61,57,0},{40,1,41,1,42,1,49,1,50,0,51,1,57,1,59,0,60,0,61,0,0},
		0x48260fb3fb803812,1,{60,31,42,0},{31,0,40,0,41,0,42,1,49,0,50,1,51,1,60,1,0},
		0x48260b14e0089ff6,1,{60,39,42,0},{39,0,40,0,41,0,42,1,49,0,50,1,51,1,60,1,0},
		0x67245c55452ddac3,1,{42,0},{40,1,41,1,42,1,49,0,50,0,51,0,0},
		0x9c5b40b49e2df545,1,{58,0},{40,1,41,0,49,1,50,0,51,0,58,1,0},
		0xaf7b397bc26755a3,0,{58,48,42,32,57,33,0},{32,0,33,0,40,0,41,1,42,0,48,0,49,1,50,1,51,0,57,1,58,1,0},
		0xe88408d8e5ca4db8,0,{58,48,42,32,57,59,0},{32,0,40,0,41,0,42,1,48,0,49,1,50,0,51,0,57,1,58,1,59,0,0},
		0x29e1a7ae824ebd49,1,{58,48,42,32,57,59,60,0},{32,0,40,0,41,0,42,1,48,0,49,1,50,0,51,1,57,1,58,1,59,1,60,1,0},
		0xeba990b5cf620ed0,0,{58,48,42,32,57,67,0},{32,0,40,0,41,1,42,1,48,0,49,0,50,1,51,0,57,1,58,0,67,0,0},
		0x2e0f161e85e7aa38,1,{58,48,42,32,57,31,60,59,67,0},{31,0,32,0,40,0,41,0,42,1,48,0,49,1,50,0,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x2e0d612fbd0f3b33,0,{58,48,42,32,57,31,60,59,67,43,0},{31,0,32,0,40,0,41,0,42,0,43,0,48,0,49,1,50,0,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x2e489eaedd1cedd3,1,{58,48,42,32,57,31,60,59,67,43,39,0},{31,0,32,0,39,1,40,0,41,0,42,0,43,0,48,1,49,1,50,0,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x3c15e7ed02af8d6e,0,{58,48,42,32,57,31,60,59,67,43,39,61,0},{31,0,32,0,39,1,40,0,41,0,42,0,43,0,48,1,49,1,50,0,51,0,57,1,58,1,59,1,60,1,61,0,67,1,0},
		0xdd08ac67284d7a72,1,{58,48,42,32,57,31,60,59,67,43,39,52,30,69,22,68,24,0},{22,1,24,1,30,1,31,1,32,1,39,1,40,1,41,0,42,0,43,0,48,1,49,1,50,0,51,0,52,0,57,1,58,1,59,0,60,0,67,1,68,0,69,0,0},
		0x2e4a1523c7c7ef13,1,{58,48,42,32,57,31,60,59,67,34,39,0},{31,0,32,0,34,0,39,1,40,0,41,0,42,0,48,1,49,1,50,0,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x04a14895a4807f18,1,{58,48,42,32,57,31,60,59,67,34,39,52,30,38,23,0},{23,1,30,1,31,0,32,1,34,0,38,0,39,0,40,0,41,1,42,0,48,1,49,1,50,1,51,0,52,0,57,1,58,1,59,1,60,1,67,1,0},
		0x2e4a1149681919e4,0,{58,48,42,32,57,31,60,59,67,34,39,38,0},{31,0,32,0,34,0,38,0,39,0,40,0,41,0,42,0,48,1,49,1,50,0,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x305f58cf289d4f10,1,{58,48,42,32,57,31,60,59,67,34,39,38,30,0},{30,1,31,0,32,0,34,0,38,0,39,1,40,1,41,0,42,0,48,1,49,1,50,1,51,1,57,1,58,1,59,1,60,1,67,1,0},
		0x19339a188157b82a,0,{58,48,42,32,57,31,60,59,67,68,0},{31,0,32,0,40,0,41,0,42,1,48,0,49,1,50,0,51,1,57,1,58,0,59,0,60,1,67,1,68,0,0},
		0xaf7b77e8e899be33,1,{58,48,42,32,57,31,33,0},{31,0,32,0,33,1,40,0,41,1,42,1,48,0,49,1,50,1,51,0,57,1,58,1,0},
		0x9d47fa9525965a65,0,{58,48,42,31,0},{31,0,40,0,41,0,42,1,48,0,49,0,50,1,51,0,58,1,0},
		0x91f0fdc4c54893a3,1,{58,39,52,0},{39,0,40,0,41,0,49,1,50,1,51,1,52,1,58,1,0},
		0xb442909107d9a9b6,0,{58,39,52,57,0},{39,0,40,0,41,0,49,0,50,1,51,1,52,1,57,0,58,1,0},
		0xf0abfc6a2a63e8f6,1,{58,39,52,57,48,67,31,38,30,32,33,0},{30,1,31,1,32,1,33,1,38,0,39,0,40,0,41,1,48,0,49,0,50,1,51,1,52,1,57,0,58,0,67,0,0},
		0xd6c760a9d380c51b,0,{58,39,31,59,0},{31,1,39,0,40,1,41,0,49,0,50,0,51,0,58,1,59,0,0},
		0x9b32fd09f39144eb,0,{58,39,31,48,0},{31,1,39,0,40,1,41,0,48,0,49,0,50,0,51,0,58,1,0},
		0xaf7b734a12670ca0,1,{58,39,31,48,42,32,33,21,57,0},{21,0,31,0,32,1,33,1,39,0,40,0,41,1,42,1,48,0,49,1,50,1,51,0,57,1,58,1,0},
		0x9d8350b9c1b21a49,0,{58,39,31,48,42,32,33,21,38,29,0},{21,0,29,0,31,0,32,1,33,1,38,1,39,0,40,1,41,1,42,1,48,1,49,0,50,1,51,0,58,1,0},
		0x91f102643d1e70e3,1,{58,30,52,0},{30,0,40,0,41,0,49,1,50,1,51,1,52,1,58,1,0},
		0x9c5b35250bab9f4b,0,{58,39,31,30,0},{30,0,31,1,39,0,40,0,41,0,49,1,50,0,51,0,58,1,0},
		0x4db4a2bfb7d57312,0,{58,39,31,30,42,60,0},{30,0,31,1,39,0,40,0,41,0,42,1,49,1,50,0,51,0,58,1,60,0,0},
		0x434a6b5efdfc90fd,1,{58,39,31,30,42,60,52,0},{30,0,31,1,39,0,40,0,41,0,42,1,49,1,50,1,51,1,52,1,58,1,60,0,0},
		0x4349aff45d65ae44,1,{58,39,31,30,42,60,52,32,33,43,35,34,24,0},{24,1,30,0,31,0,32,0,33,1,34,0,35,1,39,0,40,0,41,0,42,1,43,1,49,1,50,1,51,1,52,1,58,1,60,0,0},
		0x4db4b78bb36f9f90,1,{58,39,31,30,42,60,33,0},{30,0,31,1,33,1,39,0,40,0,41,1,42,1,49,1,50,0,51,0,58,1,60,0,0},
		0x9bccbe25082ad9f5,0,{58,39,31,30,42,60,33,59,68,67,61,78,69,52,62,44,43,35,53,71,0},{30,0,31,1,33,1,35,0,39,0,40,0,41,1,42,1,43,1,44,0,49,0,50,0,51,0,52,1,53,0,58,0,59,0,60,0,61,0,62,0,67,0,68,0,69,1,71,0,78,0,0},
		0x4db4a2bb636a3562,0,{58,39,31,30,42,60,33,32,0},{30,0,31,0,32,0,33,1,39,0,40,0,41,0,42,1,49,1,50,0,51,0,58,1,60,0,0},
		0x4db4c008289fc744,1,{58,39,31,30,42,60,38,0},{30,0,31,1,38,1,39,1,40,1,41,1,42,1,49,1,50,0,51,0,58,1,60,0,0},
		0xd6c71b4eeb06a53b,0,{58,39,31,30,42,59,0},{30,0,31,1,39,0,40,0,41,0,42,1,49,0,50,0,51,0,58,1,59,0,0},
		0x9e7034fe710afd8c,0,{58,39,31,30,42,32,0},{30,0,31,0,32,0,39,0,40,0,41,0,42,1,49,1,50,1,51,0,58,1,0},
		0x4d1def460c9be99c,0,{58,39,31,30,42,48,38,47,57,33,59,32,0},{30,0,31,0,32,0,33,0,38,1,39,1,40,1,41,1,42,0,47,0,48,1,49,1,50,1,51,0,57,1,58,1,59,1,0},
		0xc9aea1088d28fee1,1,{58,39,31,30,42,48,38,47,57,33,59,32,61,60,52,67,34,0},{30,0,31,0,32,0,33,0,34,1,38,1,39,1,40,0,41,1,42,1,47,0,48,1,49,0,50,1,51,1,52,1,57,0,58,0,59,1,60,0,61,1,67,0,0},
		0x9b32b7aebe1c1e1b,0,{58,39,31,30,42,48,29,21,0},{21,0,29,1,30,0,31,0,39,0,40,0,41,0,42,1,48,0,49,0,50,0,51,0,58,1,0},
		0x9b6e14c621f6a4d6,1,{58,39,31,30,42,48,29,21,38,0},{21,0,29,1,30,0,31,0,38,1,39,1,40,1,41,1,42,1,48,1,49,0,50,0,51,0,58,1,0},
		0x39736b6c6736c32e,1,{58,39,31,30,42,48,29,21,59,0},{21,0,29,1,30,0,31,0,39,1,40,0,41,0,42,1,48,0,49,1,50,0,51,0,58,1,59,1,0},
		0x9c5b3774ee132aba,1,{58,39,31,30,29,0},{29,1,30,1,31,1,39,1,40,0,41,0,49,1,50,0,51,0,58,1,0},
		0x9e70530dd7335713,1,{58,39,31,30,29,22,42,0},{22,0,29,1,30,1,31,0,39,1,40,1,41,1,42,1,49,1,50,1,51,0,58,1,0}
	};
	char i,j;

	if(!t) {
		for(i=0;i<8;i++) {
			for(key=0,j=10;j<81;j++) {
				if(!board[j] || board[j]==3) continue;
				if(!i) key+=board[j]*os[j];
				else if(i==1) key+=board[j]*os[j%9*9+j/9];
				else if(i==2) key+=board[j]*os[(9-j/9)*9+9-j%9];
				else if(i==3) key+=board[j]*os[(9-j%9)*9+9-j/9];
				else if(i==4) key+=board[j]*os[(9-j%9)*9+j/9];
				else if(i==5) key+=board[j]*os[(9-j/9)*9+j%9];
				else if(i==6) key+=board[j]*os[j%9*9+9-j/9];
				else key+=board[j]*os[j/9*9+9-j%9];
			}
			for(j=0;j<76;j++) {
				if(key==book[j].key && g==book[j].p) return j+1;
			}
		}
		return 0;
	}
	if(t==1) {
		if(g) return book[n].book[g-1];
		return 51;
	}
	return book[n].board[g];
}