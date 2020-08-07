#include "ko.h"

extern char board[],bm,e,br;
char *stack[400],**sp=stack;
unsigned __int64 key,os[81];
BH *hash[5000];

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
	mp->s=tp->s;
	mp->ev=tp->ev;
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
	int i;
	char o=p^1,*j;
	__int64 k;
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
		i=HMove(p,o,j);
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

void delBT(BT *bp)
{
	if(!bp) return;
	delBT(bp->child);
	delBT(bp->cdr);
	free((void *)bp);
}

char initBook()
{
	static char book[][45]={
#include "book.txt"
		0,0,"\0"
	};
	short i;
	char j;
	__int64 k=1;
	BT *bp,*fp;

	if(!(bp=(BT *)malloc(sizeof(BT)))) return 0;
	fp=bp;
	bp->cdr=0;
	bp->s=51;

	for(i=1;book[0][i];i++) {
		if(!(bp->child=(BT *)malloc(sizeof(BT)))) return 0;
		bp=bp->child;
		bp->cdr=0;
		bp->s=book[0][i];
	}
	bp->child=0;
	bp->ev=i&1 ? book[i][0]:-book[i][0];

	for(i=1;book[i][1];i++) {
		bp=fp->child;
		j=1;

		while(bp) {
			if(bp->s==book[i][j]) {
				bp=bp->child;
				j++;
			} else {
				if(bp->cdr) bp=bp->cdr;
				else break;
			}
		}

		if(!(bp->cdr=(BT *)malloc(sizeof(BT)))) return 0;
		bp=bp->cdr;
		bp->cdr=0;
		bp->s=book[i][j++];
		while(book[i][j]) {
			if(!(bp->child=(BT *)malloc(sizeof(BT)))) return 0;
			bp=bp->child;
			bp->cdr=0;
			bp->s=book[i][j++];
		}
		bp->child=0;
		bp->ev=j&1 ? book[i][0]:-book[i][0];
	}
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
	delBT(fp);

	return 1;
}

void Book()
{
	char i,j,k;
	MOVS *mp;
	BH *bp;

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
		for(bp=hash[key%5000],j=0;bp;bp=bp->next) {
			if(key==bp->key) {
				j=1;
				break;
			}
		}
		if(!j) continue;
		if(e==60) {
			j=rand()&3;
			if(i<4) {
				if(!j) bm=51;
				else if(j==1) bm=59;
				else if(j==2) bm=39;
				else bm=31;
			} else {
				if(!j) bm=32;
				else if(j==1) bm=42;
				else if(j==2) bm=58;
				else bm=48;
			}
		} else {
			for(mp=bp->movs,k=0;mp;mp=mp->next) {
				if(k && rand()&1) continue;
				j=mp->s;
				if(i==1) j=j%9*9+j/9;
				else if(i==2) j=(9-j/9)*9+9-j%9;
				else if(i==3) j=(9-j%9)*9+9-j/9;
				else if(i==4) j=j%9*9+9-j/9;
				else if(i==5) j=(9-j/9)*9+j%9;
				else if(i==6) j=(9-j%9)*9+j/9;
				else if(i==7) j=j/9*9+9-j%9;
				k=1;
				if(bp->movs->ev-br<=mp->ev) bm=j;
				else break;
			}
		}
		return;
	}
}