#include "ko.h"

extern char e,depth,*square;
extern EMPTY eh;

short ABmax1(char dep,char dd,char pp,short a,short b)
{
	short mev,ev;
	EMPTY *ep,*bep;
	MOVE mp[1];

	if(dd-e+depth-dep==-64) return -2064;
	if(!dep) return Eval();

	for(mev=-3000,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Flip1(mp)) continue;
		bep->next=ep->next;

		ev=ABmin1(dep-1,dd+(mp->c<<1)+1,0,a,b);

		Undo(1)
		bep->next=ep;

		if(ev>mev) {
			mev=ev;
			if(ev>a) {
				a=ev;
				if(ev>=b) return mev;
			}
		}
	}

	if(mev!=-3000) return mev;
	if(pp) {
		if(dd>0) return dd+e-depth+dep+2000;
		if(dd<0) return dd-e+depth-dep-2000;
		return 0;
	}
	return ABmin1(dep,dd,1,a,b);
}

short ABmin1(char dep,char dd,char pp,short a,short b)
{
	short mev,ev;
	EMPTY *ep,*bep;
	MOVE mp[1];

	if(dd+e-depth+dep==64) return 2064;
	if(!dep) return Eval();

	for(mev=3000,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Flip2(mp)) continue;
		bep->next=ep->next;

		ev=ABmax1(dep-1,dd-(mp->c<<1)-1,0,a,b);

		Undo(0)
		bep->next=ep;

		if(ev<mev) {
			mev=ev;
			if(ev<b) {
				b=ev;
				if(ev<=a) return mev;
			}
		}
	}

	if(mev!=3000) return mev;
	if(pp) {
		if(dd>0) return dd+e-depth+dep+2000;
		if(dd<0) return dd-e+depth-dep-2000;
		return 0;
	}
	return ABmax1(dep,dd,1,a,b);
}

char ABmax2(char dep,char dd,char pp,char a,char b)
{
	char mev=-100,ev,i;
	EMPTY *ep,*bep;
	MOVE mp[1];

	for(bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Flip1(mp)) continue;
		bep->next=ep->next;

		if(dep==2) {
			square=eh.next->s;
			if(i=Turn2()) {
				ev=dd+((mp->c-i)<<1);
			} else if(i=Turn1()) {
				ev=dd+((mp->c+i)<<1)+2;
			} else {
				ev=dd+(mp->c<<1)+1;
				if(ev>0) ev++;
				else if(ev<0) ev--;
				else ev=0;
			}
		} else {
			ev=ABmin2(dep-1,dd+(mp->c<<1)+1,0,a,b);
		}

		Undo(1)
		bep->next=ep;

		if(ev>mev) {
			mev=ev;
			if(ev>a) {
				a=ev;
				if(ev>=b) return mev;
			}
		}
	}

	if(mev!=-100) return mev;
	if(pp) {
		if(dd>0) return dd+dep;
		if(dd<0) return dd-dep;
		return 0;
	}
	return ABmin2(dep,dd,1,a,b);
}

char ABmin2(char dep,char dd,char pp,char a,char b)
{
	char mev=100,ev,i;
	EMPTY *ep,*bep;
	MOVE mp[1];

	for(bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Flip2(mp)) continue;
		bep->next=ep->next;

		if(dep==2) {
			square=eh.next->s;
			if(i=Turn1()) {
				ev=dd-((mp->c-i)<<1);
			} else if(i=Turn2()) {
				ev=dd-((mp->c+i)<<1)-2;
			} else {
				ev=dd-(mp->c<<1)-1;
				if(ev>0) ev++;
				else if(ev<0) ev--;
				else ev=0;
			}
		} else {
			ev=ABmax2(dep-1,dd-(mp->c<<1)-1,0,a,b);
		}

		Undo(0)
		bep->next=ep;

		if(ev<mev) {
			mev=ev;
			if(ev<b) {
				b=ev;
				if(ev<=a) return mev;
			}
		}
	}

	if(mev!=100) return mev;
	if(pp) {
		if(dd>0) return dd+dep;
		if(dd<0) return dd-dep;
		return 0;
	}
	return ABmax2(dep,dd,1,a,b);
}