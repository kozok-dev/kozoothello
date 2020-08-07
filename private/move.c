#include "ko.h"
#define chMove4(p,o,dir) (square[dir]==o && \
	(square[dir*2]==p || (square[dir*2]==o && \
	(square[dir*3]==p || (square[dir*3]==o && \
	(square[dir*4]==p || (square[dir*4]==o && \
	square[dir*5]==p)))))))
#define chMove6(p,o,dir) (square[dir]==o && \
	(square[dir*2]==p || (square[dir*2]==o && \
	(square[dir*3]==p || (square[dir*3]==o && \
	(square[dir*4]==p || (square[dir*4]==o && \
	(square[dir*5]==p || (square[dir*5]==o && \
	(square[dir*6]==p || (square[dir*6]==o && \
	square[dir*7]==p)))))))))))
#define cntFlip4(p,o,dir) if(square[dir]==o) { \
	if(square[dir*2]==p) c++; \
	else if(square[dir*2]==o) { \
		if(square[dir*3]==p) c+=2; \
		else if(square[dir*3]==o) { \
			if(square[dir*4]==p) c+=3; \
			else if(square[dir*4]==o && square[dir*5]==p) c+=4;}}}
#define cntFlip6(p,o,dir) if(square[dir]==o) { \
	if(square[dir*2]==p) c++; \
	else if(square[dir*2]==o) { \
		if(square[dir*3]==p) c+=2; \
		else if(square[dir*3]==o) { \
			if(square[dir*4]==p) c+=3; \
			else if(square[dir*4]==o) { \
				if(square[dir*5]==p) c+=4; \
				else if(square[dir*5]==o) { \
					if(square[dir*6]==p) c+=5; \
					else if(square[dir*6]==o && square[dir*7]==p) c+=6;}}}}}
#define dirFlip(p,o,dir,flip) if chMove##flip(p,o,dir) { \
	s=square+dir; \
	do { \
		*s=p; \
		move->s[++move->c]=s; \
	} while(*(s+=dir)==o);}
#define dirMove(p,o,dir,flip) if chMove##flip(p,o,dir) { \
	s=square+dir; \
	do { \
		move->s[++move->c]=s; \
		move->hc[0]^=hcfd[i=s-board][0]; \
		move->hc[1]^=hcfd[i][1]; \
	} while(*(s+=dir)==o);}

extern char board[],*square;
extern int hcsd[][3][2],hcfd[][2];
extern EMPTY eh;
char flipdir[]={
	0,0,0,0,0,0,0,0,0,
	0,0,0,1,1,1,1,2,2,
	0,0,0,1,1,1,1,2,2,
	0,3,3,4,4,4,4,5,5,
	0,3,3,4,4,4,4,5,5,
	0,3,3,4,4,4,4,5,5,
	0,3,3,4,4,4,4,5,5,
	0,6,6,7,7,7,7,8,8,
	0,6,6,7,7,7,7,8,8
};

char Mobility1()
{
	char c=0;
	EMPTY *ep;

	for(ep=eh.next;ep;ep=ep->next) {
		square=ep->s;
		switch(flipdir[square-board]) {
		case 0:
			c+=chMove6(0,1,1) || chMove6(0,1,9) || chMove6(0,1,10);
			break;
		case 1:
			c+=chMove4(0,1,1) || chMove4(0,1,8) || chMove6(0,1,9) || chMove4(0,1,10) || chMove4(0,1,-1);
			break;
		case 2:
			c+=chMove6(0,1,-1) || chMove6(0,1,8) || chMove6(0,1,9);
			break;
		case 3:
			c+=chMove6(0,1,1) || chMove4(0,1,9) || chMove4(0,1,10) || chMove4(0,1,-8) || chMove4(0,1,-9);
			break;
		case 4:
			c+=chMove4(0,1,1) || chMove4(0,1,8) || chMove4(0,1,9) || chMove4(0,1,10) || chMove4(0,1,-1) || chMove4(0,1,-8) || chMove4(0,1,-9) || chMove4(0,1,-10);
			break;
		case 5:
			c+=chMove4(0,1,8) || chMove4(0,1,9) || chMove6(0,1,-1) || chMove4(0,1,-9) || chMove4(0,1,-10);
			break;
		case 6:
			c+=chMove6(0,1,1) || chMove6(0,1,-8) || chMove6(0,1,-9);
			break;
		case 7:
			c+=chMove4(0,1,1) || chMove4(0,1,-1) || chMove4(0,1,-8) || chMove6(0,1,-9) || chMove4(0,1,-10);
			break;
		default:
			c+=chMove6(0,1,-1) || chMove6(0,1,-9) || chMove6(0,1,-10);
		}
	}
	return c;
}

char Mobility2()
{
	char c=0;
	EMPTY *ep;

	for(ep=eh.next;ep;ep=ep->next) {
		square=ep->s;
		switch(flipdir[square-board]) {
		case 0:
			c+=chMove6(1,0,1) || chMove6(1,0,9) || chMove6(1,0,10);
			break;
		case 1:
			c+=chMove4(1,0,1) || chMove4(1,0,8) || chMove6(1,0,9) || chMove4(1,0,10) || chMove4(1,0,-1);
			break;
		case 2:
			c+=chMove6(1,0,-1) || chMove6(1,0,8) || chMove6(1,0,9);
			break;
		case 3:
			c+=chMove6(1,0,1) || chMove4(1,0,9) || chMove4(1,0,10) || chMove4(1,0,-8) || chMove4(1,0,-9);
			break;
		case 4:
			c+=chMove4(1,0,1) || chMove4(1,0,8) || chMove4(1,0,9) || chMove4(1,0,10) || chMove4(1,0,-1) || chMove4(1,0,-8) || chMove4(1,0,-9) || chMove4(1,0,-10);
			break;
		case 5:
			c+=chMove4(1,0,8) || chMove4(1,0,9) || chMove6(1,0,-1) || chMove4(1,0,-9) || chMove4(1,0,-10);
			break;
		case 6:
			c+=chMove6(1,0,1) || chMove6(1,0,-8) || chMove6(1,0,-9);
			break;
		case 7:
			c+=chMove4(1,0,1) || chMove4(1,0,-1) || chMove4(1,0,-8) || chMove6(1,0,-9) || chMove4(1,0,-10);
			break;
		default:
			c+=chMove6(1,0,-1) || chMove6(1,0,-9) || chMove6(1,0,-10);
		}
	}
	return c;
}

char Turn1()
{
	char c=0;

	switch(flipdir[square-board]) {
	case 0:
		cntFlip6(0,1,1)
		cntFlip6(0,1,9)
		cntFlip6(0,1,10)
		break;
	case 1:
		cntFlip4(0,1,1)
		cntFlip4(0,1,8)
		cntFlip6(0,1,9)
		cntFlip4(0,1,10)
		cntFlip4(0,1,-1)
		break;
	case 2:
		cntFlip6(0,1,-1)
		cntFlip6(0,1,8)
		cntFlip6(0,1,9)
		break;
	case 3:
		cntFlip6(0,1,1)
		cntFlip4(0,1,9)
		cntFlip4(0,1,10)
		cntFlip4(0,1,-8)
		cntFlip4(0,1,-9)
		break;
	case 4:
		cntFlip4(0,1,1)
		cntFlip4(0,1,8)
		cntFlip4(0,1,9)
		cntFlip4(0,1,10)
		cntFlip4(0,1,-1)
		cntFlip4(0,1,-8)
		cntFlip4(0,1,-9)
		cntFlip4(0,1,-10)
		break;
	case 5:
		cntFlip4(0,1,8)
		cntFlip4(0,1,9)
		cntFlip6(0,1,-1)
		cntFlip4(0,1,-9)
		cntFlip4(0,1,-10)
		break;
	case 6:
		cntFlip6(0,1,1)
		cntFlip6(0,1,-8)
		cntFlip6(0,1,-9)
		break;
	case 7:
		cntFlip4(0,1,1)
		cntFlip4(0,1,-1)
		cntFlip4(0,1,-8)
		cntFlip6(0,1,-9)
		cntFlip4(0,1,-10)
		break;
	default:
		cntFlip6(0,1,-1)
		cntFlip6(0,1,-9)
		cntFlip6(0,1,-10)
	}
	return c;
}

char Turn2()
{
	char c=0;

	switch(flipdir[square-board]) {
	case 0:
		cntFlip6(1,0,1)
		cntFlip6(1,0,9)
		cntFlip6(1,0,10)
		break;
	case 1:
		cntFlip4(1,0,1)
		cntFlip4(1,0,8)
		cntFlip6(1,0,9)
		cntFlip4(1,0,10)
		cntFlip4(1,0,-1)
		break;
	case 2:
		cntFlip6(1,0,-1)
		cntFlip6(1,0,8)
		cntFlip6(1,0,9)
		break;
	case 3:
		cntFlip6(1,0,1)
		cntFlip4(1,0,9)
		cntFlip4(1,0,10)
		cntFlip4(1,0,-8)
		cntFlip4(1,0,-9)
		break;
	case 4:
		cntFlip4(1,0,1)
		cntFlip4(1,0,8)
		cntFlip4(1,0,9)
		cntFlip4(1,0,10)
		cntFlip4(1,0,-1)
		cntFlip4(1,0,-8)
		cntFlip4(1,0,-9)
		cntFlip4(1,0,-10)
		break;
	case 5:
		cntFlip4(1,0,8)
		cntFlip4(1,0,9)
		cntFlip6(1,0,-1)
		cntFlip4(1,0,-9)
		cntFlip4(1,0,-10)
		break;
	case 6:
		cntFlip6(1,0,1)
		cntFlip6(1,0,-8)
		cntFlip6(1,0,-9)
		break;
	case 7:
		cntFlip4(1,0,1)
		cntFlip4(1,0,-1)
		cntFlip4(1,0,-8)
		cntFlip6(1,0,-9)
		cntFlip4(1,0,-10)
		break;
	default:
		cntFlip6(1,0,-1)
		cntFlip6(1,0,-9)
		cntFlip6(1,0,-10)
	}
	return c;
}

char Flip1(MOVE *move)
{
	char *s;

	move->c=0;
	switch(flipdir[square-board]) {
	case 0:
		dirFlip(0,1,1,6)
		dirFlip(0,1,9,6)
		dirFlip(0,1,10,6)
		break;
	case 1:
		dirFlip(0,1,1,4)
		dirFlip(0,1,8,4)
		dirFlip(0,1,9,6)
		dirFlip(0,1,10,4)
		dirFlip(0,1,-1,4)
		break;
	case 2:
		dirFlip(0,1,-1,6)
		dirFlip(0,1,8,6)
		dirFlip(0,1,9,6)
		break;
	case 3:
		dirFlip(0,1,1,6)
		dirFlip(0,1,9,4)
		dirFlip(0,1,10,4)
		dirFlip(0,1,-8,4)
		dirFlip(0,1,-9,4)
		break;
	case 4:
		dirFlip(0,1,1,4)
		dirFlip(0,1,8,4)
		dirFlip(0,1,9,4)
		dirFlip(0,1,10,4)
		dirFlip(0,1,-1,4)
		dirFlip(0,1,-8,4)
		dirFlip(0,1,-9,4)
		dirFlip(0,1,-10,4)
		break;
	case 5:
		dirFlip(0,1,8,4)
		dirFlip(0,1,9,4)
		dirFlip(0,1,-1,6)
		dirFlip(0,1,-9,4)
		dirFlip(0,1,-10,4)
		break;
	case 6:
		dirFlip(0,1,1,6)
		dirFlip(0,1,-8,6)
		dirFlip(0,1,-9,6)
		break;
	case 7:
		dirFlip(0,1,1,4)
		dirFlip(0,1,-1,4)
		dirFlip(0,1,-8,4)
		dirFlip(0,1,-9,6)
		dirFlip(0,1,-10,4)
		break;
	default:
		dirFlip(0,1,-1,6)
		dirFlip(0,1,-9,6)
		dirFlip(0,1,-10,6)
	}
	if(move->c) {
		move->s[0]=square;
		*square=0;
	}
	return move->c;
}

char Flip2(MOVE *move)
{
	char *s;

	move->c=0;
	switch(flipdir[square-board]) {
	case 0:
		dirFlip(1,0,1,6)
		dirFlip(1,0,9,6)
		dirFlip(1,0,10,6)
		break;
	case 1:
		dirFlip(1,0,1,4)
		dirFlip(1,0,8,4)
		dirFlip(1,0,9,6)
		dirFlip(1,0,10,4)
		dirFlip(1,0,-1,4)
		break;
	case 2:
		dirFlip(1,0,-1,6)
		dirFlip(1,0,8,6)
		dirFlip(1,0,9,6)
		break;
	case 3:
		dirFlip(1,0,1,6)
		dirFlip(1,0,9,4)
		dirFlip(1,0,10,4)
		dirFlip(1,0,-8,4)
		dirFlip(1,0,-9,4)
		break;
	case 4:
		dirFlip(1,0,1,4)
		dirFlip(1,0,8,4)
		dirFlip(1,0,9,4)
		dirFlip(1,0,10,4)
		dirFlip(1,0,-1,4)
		dirFlip(1,0,-8,4)
		dirFlip(1,0,-9,4)
		dirFlip(1,0,-10,4)
		break;
	case 5:
		dirFlip(1,0,8,4)
		dirFlip(1,0,9,4)
		dirFlip(1,0,-1,6)
		dirFlip(1,0,-9,4)
		dirFlip(1,0,-10,4)
		break;
	case 6:
		dirFlip(1,0,1,6)
		dirFlip(1,0,-8,6)
		dirFlip(1,0,-9,6)
		break;
	case 7:
		dirFlip(1,0,1,4)
		dirFlip(1,0,-1,4)
		dirFlip(1,0,-8,4)
		dirFlip(1,0,-9,6)
		dirFlip(1,0,-10,4)
		break;
	default:
		dirFlip(1,0,-1,6)
		dirFlip(1,0,-9,6)
		dirFlip(1,0,-10,6)
	}
	if(move->c) {
		move->s[0]=square;
		*square=1;
	}
	return move->c;
}

char Move1(MOVE *move)
{
	char i=square-board,*s;

	move->c=0;
	move->hc[0]=hcsd[i][0][0];
	move->hc[1]=hcsd[i][0][1];
	switch(flipdir[i]) {
	case 0:
		dirMove(0,1,1,6)
		dirMove(0,1,9,6)
		dirMove(0,1,10,6)
		break;
	case 1:
		dirMove(0,1,1,4)
		dirMove(0,1,8,4)
		dirMove(0,1,9,6)
		dirMove(0,1,10,4)
		dirMove(0,1,-1,4)
		break;
	case 2:
		dirMove(0,1,-1,6)
		dirMove(0,1,8,6)
		dirMove(0,1,9,6)
		break;
	case 3:
		dirMove(0,1,1,6)
		dirMove(0,1,9,4)
		dirMove(0,1,10,4)
		dirMove(0,1,-8,4)
		dirMove(0,1,-9,4)
		break;
	case 4:
		dirMove(0,1,1,4)
		dirMove(0,1,8,4)
		dirMove(0,1,9,4)
		dirMove(0,1,10,4)
		dirMove(0,1,-1,4)
		dirMove(0,1,-8,4)
		dirMove(0,1,-9,4)
		dirMove(0,1,-10,4)
		break;
	case 5:
		dirMove(0,1,8,4)
		dirMove(0,1,9,4)
		dirMove(0,1,-1,6)
		dirMove(0,1,-9,4)
		dirMove(0,1,-10,4)
		break;
	case 6:
		dirMove(0,1,1,6)
		dirMove(0,1,-8,6)
		dirMove(0,1,-9,6)
		break;
	case 7:
		dirMove(0,1,1,4)
		dirMove(0,1,-1,4)
		dirMove(0,1,-8,4)
		dirMove(0,1,-9,6)
		dirMove(0,1,-10,4)
		break;
	default:
		dirMove(0,1,-1,6)
		dirMove(0,1,-9,6)
		dirMove(0,1,-10,6)
	}
	if(move->c) move->s[0]=square;
	return move->c;
}

char Move2(MOVE *move)
{
	char i=square-board,*s;

	move->c=0;
	move->hc[0]=hcsd[i][1][0];
	move->hc[1]=hcsd[i][1][1];
	switch(flipdir[i]) {
	case 0:
		dirMove(1,0,1,6)
		dirMove(1,0,9,6)
		dirMove(1,0,10,6)
		break;
	case 1:
		dirMove(1,0,1,4)
		dirMove(1,0,8,4)
		dirMove(1,0,9,6)
		dirMove(1,0,10,4)
		dirMove(1,0,-1,4)
		break;
	case 2:
		dirMove(1,0,-1,6)
		dirMove(1,0,8,6)
		dirMove(1,0,9,6)
		break;
	case 3:
		dirMove(1,0,1,6)
		dirMove(1,0,9,4)
		dirMove(1,0,10,4)
		dirMove(1,0,-8,4)
		dirMove(1,0,-9,4)
		break;
	case 4:
		dirMove(1,0,1,4)
		dirMove(1,0,8,4)
		dirMove(1,0,9,4)
		dirMove(1,0,10,4)
		dirMove(1,0,-1,4)
		dirMove(1,0,-8,4)
		dirMove(1,0,-9,4)
		dirMove(1,0,-10,4)
		break;
	case 5:
		dirMove(1,0,8,4)
		dirMove(1,0,9,4)
		dirMove(1,0,-1,6)
		dirMove(1,0,-9,4)
		dirMove(1,0,-10,4)
		break;
	case 6:
		dirMove(1,0,1,6)
		dirMove(1,0,-8,6)
		dirMove(1,0,-9,6)
		break;
	case 7:
		dirMove(1,0,1,4)
		dirMove(1,0,-1,4)
		dirMove(1,0,-8,4)
		dirMove(1,0,-9,6)
		dirMove(1,0,-10,4)
		break;
	default:
		dirMove(1,0,-1,6)
		dirMove(1,0,-9,6)
		dirMove(1,0,-10,6)
	}
	if(move->c) move->s[0]=square;
	return move->c;
}