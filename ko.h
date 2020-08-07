#include <stdio.h>
#include <time.h>
#include <process.h>
#include <windows.h>
#define a1 10
#define b1 11
#define c1 12
#define d1 13
#define e1 14
#define f1 15
#define g1 16
#define h1 17
#define a2 19
#define b2 20
#define c2 21
#define d2 22
#define e2 23
#define f2 24
#define g2 25
#define h2 26
#define a3 28
#define b3 29
#define c3 30
#define d3 31
#define e3 32
#define f3 33
#define g3 34
#define h3 35
#define a4 37
#define b4 38
#define c4 39
#define d4 40
#define e4 41
#define f4 42
#define g4 43
#define h4 44
#define a5 46
#define b5 47
#define c5 48
#define d5 49
#define e5 50
#define f5 51
#define g5 52
#define h5 53
#define a6 55
#define b6 56
#define c6 57
#define d6 58
#define e6 59
#define f6 60
#define g6 61
#define h6 62
#define a7 64
#define b7 65
#define c7 66
#define d7 67
#define e7 68
#define f7 69
#define g7 70
#define h7 71
#define a8 73
#define b8 74
#define c8 75
#define d8 76
#define e8 77
#define f8 78
#define g8 79
#define h8 80
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
typedef struct bt {
	char s,ev;
	struct bt *cdr,*child;
} BT;
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
	char dep;
	short a,b;
} HASH;
typedef struct {
	HASH d,n;
} HE;

char Mobility1();
char Mobility2();
char Turn1();
char Turn2();
char Flip1(MOVE *);
char Flip2(MOVE *);
char Move1(MOVE *);
char Move2(MOVE *);
char HMove(char,char,char *);

char initBook();
void Book();

char initHash(int);
HASH *getHash(char);
void updateHash(char,short,short,short);
void Com(char);
short Eval();
short ABmax1(char,char,char,short,short);
short ABmin1(char,char,char,short,short);
char ABmax2(char,char,char,char,char);
char ABmin2(char,char,char,char,char);
short PVSmax1(char,char,char,short,short);
short PVSmin1(char,char,char,short,short);
char PVSmax2(char,char,char,char,char);
char PVSmin2(char,char,char,char,char);