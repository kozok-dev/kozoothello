#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>

typedef struct movs {
	struct movs *next;
	char s,ev;
} MOVS;
typedef struct bt {
	struct bt *cdr,*child;
	char s,ev;
} BT;
typedef struct bh {
	struct bh *next;
	unsigned __int64 key;
	MOVS *movs;
} BH;
char board[91],*stack[400],**sp=stack;
char dirmask[]={
	0,0,0,0,0,0,0,0,0,
	0,112,112,-15,-15,-15,-15,-63,-63,
	0,112,112,-15,-15,-15,-15,-63,-63,
	0,124,124,-1,-1,-1,-1,-57,-57,
	0,124,124,-1,-1,-1,-1,-57,-57,
	0,124,124,-1,-1,-1,-1,-57,-57,
	0,124,124,-1,-1,-1,-1,-57,-57,
	0,28,28,31,31,31,31,7,7,
	0,28,28,31,31,31,31,7,7
};
unsigned __int64 key,os[81];
BH *hash[5000];

__forceinline void dirMove(char p,char o,char *s,char dir)
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
			do {
				if(p) key+=os[s-board];
				else key-=os[s-board];
				*s=p;
				*sp++=s;
			} while((s-=dir)!=ms);
		}
	}
}

char Move(char p,char o,char *s)
{
	char i=dirmask[s-board],**bsp=sp;

	if(i&1) dirMove(p,o,s,-1);
	if(i&2) dirMove(p,o,s,-10);
	if(i&4) dirMove(p,o,s,-9);
	if(i&8) dirMove(p,o,s,-8);
	if(i&16) dirMove(p,o,s,1);
	if(i&32) dirMove(p,o,s,10);
	if(i&64) dirMove(p,o,s,9);
	if(i&128) dirMove(p,o,s,8);
	return sp-bsp;
}

char evalBook(BT *bp)
{
	char mev=-100,ev,s;
	BT *wp=bp,*child,*i,*j;

	do {
		if(bp->child) bp->ev=-evalBook(bp->child);
		if(bp->ev>mev) mev=bp->ev;
	} while(bp=bp->cdr);
	for(i=wp;i->cdr;i=i->cdr) {
		for(j=i->cdr;j;j=j->cdr) {
			if(i->ev<j->ev) {
				child=i->child;
				i->child=j->child;
				j->child=child;
				s=i->s;
				i->s=j->s;
				j->s=s;
				ev=i->ev;
				i->ev=j->ev;
				j->ev=ev;
			}
		}
	}
	return mev;
}

MOVS *addPV(BT *tp)
{
	MOVS *mp;

	if(!(mp=(MOVS *)malloc(sizeof(MOVS)))) return 0;
	mp->next=0;
	mp->ev=tp->ev;
	mp->s=tp->s;
	return mp;
}

BH *addHash(BT *tp,BH *cp)
{
	char i;
	MOVS *mp;
	BH *hp;

	for(;cp;cp=cp->next) {
		if(key==cp->key) return 0;
	}

	if(!(hp=(BH *)malloc(sizeof(BH)))) return 0;

	for(hp->next=i=0,hp->key=key;tp;tp=tp->cdr,i++) {
		if(i) {
			for(mp=hp->movs;mp->next;mp=mp->next);
			mp->next=addPV(tp);
		} else {
			hp->movs=addPV(tp);
		}
	}

	return hp;
}

void hashBook(char p,BT *tp)
{
	short i;
	char o=p^1,*j;
	unsigned __int64 k;
	BH *hp;

	if(!tp) return;
	do {
		i=key%5000;
		if(hash[i]) {
			for(hp=hash[i];hp->next;hp=hp->next);
			hp->next=addHash(tp,hash[i]);
		} else {
			hash[i]=addHash(tp,0);
		}

		j=board+tp->s;
		i=Move(p,o,j);
		*j=p;
		k=os[tp->s]<<o;
		key-=k;
		hashBook(o,tp->child);
		if(i&1) {
			**--sp=o;
			if(p) key-=os[*sp-board];
			else key+=os[*sp-board];
			i--;
		}
		while(i) {
			**--sp=o;
			if(p) key-=os[*sp-board];
			else key+=os[*sp-board];
			**--sp=o;
			if(p) key-=os[*sp-board];
			else key+=os[*sp-board];
			i-=2;
		}
		*j=2;
		key+=k;
	} while(tp=tp->cdr);
}

char initBook(char *bf)
{
	char book[192],buf[192],diff,t=0,j;
	short i;
	unsigned __int64 k=1;
	FILE *f;
	BT *bp,*fp;

	if(!(f=fopen(bf,"r"))) return 0;
	if(!(bp=(BT *)malloc(sizeof(BT)))) return 0;
	fp=bp;
	bp->cdr=0;
	bp->s=51;

	for(;;) {
		buf[0]=0;
		fgets(buf,192,f);
		if(!buf[0]) return 0;
		if(buf[0]==9 && buf[1]==9 && buf[2]!='/') {
			t=1;
			break;
		}
		if(buf[0]!=9 && buf[0]!=10 && buf[0]!='/') break;
	}
	if(t==1) {
		sscanf(buf,"\t\t%3d,%s",&diff,book);
	} else if(buf[3]==',') {
		sscanf(buf,"%3d,%s",&diff,book);
		t=1;
	} else {
		sscanf(buf,"%3d %s",&diff,book);
	}
	for(i=0;book[i];i+=2+t) {
		if(t==1 && book[i]=='"') break;
		if(!(bp->child=(BT *)malloc(sizeof(BT)))) return 0;
		bp=bp->child;
		bp->cdr=0;
		bp->s=(book[i+1]-48)*9+(book[i]-96)%9;
	}
	bp->child=0;
	bp->ev=i/(2+t)%2 ? -diff:diff;

	for(;;) {
		for(t=0;;) {
			j=0;
			buf[0]=0;
			fgets(buf,192,f);
			if(!buf[0]) break;
			j=1;
			if(buf[0]==9 && buf[1]==9 && buf[2]!='/') {
				t=1;
				break;
			}
			if(buf[0]!=9 && buf[0]!=10 && buf[0]!='/') break;
		}
		if(!j) break;
		if(t==1) {
			sscanf(buf,"\t\t%3d,%s",&diff,book);
		} else if(buf[3]==',') {
			sscanf(buf,"%3d,%s",&diff,book);
			t=1;
		} else {
			sscanf(buf,"%3d %s",&diff,book);
		}
		bp=fp->child;
		i=j=0;

		while(bp) {
			if(!t && book[i]==0 || t==1 && book[i]=='"') {
				j=1;
				break;
			}
			if(bp->s==(book[i+1]-48)*9+(book[i]-96)%9) {
				bp=bp->child;
				i+=2+t;
			} else {
				if(bp->cdr) bp=bp->cdr;
				else break;
			}
		}
		if(j || !t && book[i]==0 || t==1 && book[i]=='"') continue;

		if(!(bp->cdr=(BT *)malloc(sizeof(BT)))) return 0;
		bp=bp->cdr;
		bp->cdr=0;
		bp->s=(book[i+1]-48)*9+(book[i]-96)%9;
		for(i+=2+t;book[i];i+=2+t) {
			if(t==1 && book[i]=='"') break;
			if(!(bp->child=(BT *)malloc(sizeof(BT)))) return 0;
			bp=bp->child;
			bp->cdr=0;
			bp->s=(book[i+1]-48)*9+(book[i]-96)%9;
		}
		bp->child=0;
		bp->ev=i/(2+t)%2 ? -diff:diff;
	}
	fclose(f);

	evalBook(fp);
	for(i=0;i<5000;i++) hash[i]=0;
	for(i=0;i<10;i++) board[i]=board[i+81]=board[i*9]=3;
	for(i=10;i<81;i++) {
		if(board[i]==3) continue;
		if(i==40) board[40]=1;
		else if(i==41) board[41]=0;
		else if(i==49) board[49]=0;
		else if(i==50) board[50]=1;
		else board[i]=2;
		key+=board[i]*k;
		os[i]=k;
		k*=3;
	}
	hashBook(0,fp);

	return 1;
}

void main(int argc,char **argv)
{
	char cd[256];
	short i;
	FILE *f;
	MOVS *mp;
	BH *hp;

	if(argc!=2) return;
	strcpy(cd,argv[1]);
	*strrchr(cd,92)=0;
	if(chdir(cd) || !initBook(argv[1]) || !(f=fopen("kobook.dat","wb"))) return;

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
}