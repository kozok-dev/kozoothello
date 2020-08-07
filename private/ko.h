#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <process.h>
#include <windows.h>
#define Move(p) switch(mp->c) { \
	case 18:*mp->s[18]=p; \
	case 17:*mp->s[17]=p; \
	case 16:*mp->s[16]=p; \
	case 15:*mp->s[15]=p; \
	case 14:*mp->s[14]=p; \
	case 13:*mp->s[13]=p; \
	case 12:*mp->s[12]=p; \
	case 11:*mp->s[11]=p; \
	case 10:*mp->s[10]=p; \
	case 9:*mp->s[9]=p; \
	case 8:*mp->s[8]=p; \
	case 7:*mp->s[7]=p; \
	case 6:*mp->s[6]=p; \
	case 5:*mp->s[5]=p; \
	case 4:*mp->s[4]=p; \
	case 3:*mp->s[3]=p; \
	case 2:*mp->s[2]=p; \
	case 1:*mp->s[1]=p; \
	default:*mp->s[0]=p; \
	}
#define Undo(p) switch(mp->c) { \
	case 18:*mp->s[18]=p; \
	case 17:*mp->s[17]=p; \
	case 16:*mp->s[16]=p; \
	case 15:*mp->s[15]=p; \
	case 14:*mp->s[14]=p; \
	case 13:*mp->s[13]=p; \
	case 12:*mp->s[12]=p; \
	case 11:*mp->s[11]=p; \
	case 10:*mp->s[10]=p; \
	case 9:*mp->s[9]=p; \
	case 8:*mp->s[8]=p; \
	case 7:*mp->s[7]=p; \
	case 6:*mp->s[6]=p; \
	case 5:*mp->s[5]=p; \
	case 4:*mp->s[4]=p; \
	case 3:*mp->s[3]=p; \
	case 2:*mp->s[2]=p; \
	case 1:*mp->s[1]=p; \
	default:*mp->s[0]=2; \
	}

typedef struct movs {
	char s,ev;
	struct movs *next;
} MOVS;
typedef struct bh {
	__int64 key;
	MOVS *movs;
	struct bh *next;
} BH;
typedef struct empty {
	char *s;
	struct empty *prev,*next;
} EMPTY;
typedef struct {
	char *s[20],c;
	short ev;
	int hc[2];
} MOVE;
typedef struct {
	int lock;
	char s,dep;
	short a,b;
} HASH;
typedef struct {
	HASH d,n;
} HE;
typedef struct {
	double ia,a,b;
	char dep;
} MPC;

char Mobility1();
char Mobility2();
char Turn1();
char Turn2();
char Flip1(MOVE *);
char Flip2(MOVE *);
char Move1(MOVE *);
char Move2(MOVE *);

void initBook();
void Book(char);

HASH *getHash(char);
void updateHash(char,char,short,short,short);
void Com(char);
void comMsg(char);

short Eval();
short ABmax1(char,char,char,short,short);
short ABmin1(char,char,char,short,short);
char ABmax2(char,char,char,char,char);
char ABmin2(char,char,char,char,char);
short PVSmax1(char,char,char,short,short);
short PVSmin1(char,char,char,short,short);
char PVSmax2(char,char,char,char,char);
char PVSmin2(char,char,char,char,char);