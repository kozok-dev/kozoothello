#include "ko.h"

extern char board[],e,depth,gs,forcecom,*square;
extern int hc[],hcsp[],evcnt;
extern EMPTY eh,*mvemp[];
extern MPC mpc[][10];

short PVSmax1(char dep,char dd,char pp,short a,short b)
{
	short mev,ev,at,bt;
	char s,i;
	EMPTY *ep;
	MOVE move[35],tmp,*mp,*mtp;
	HASH *hash;

	if(dd-e+depth-dep==-64) {
		evcnt++;
		return -2064;
	}
	if(dep<4) return ABmax1(dep,dd,0,a,b);
	if(forcecom) return Eval();

	s=0;
	at=a;
	bt=b;

	if(hash=getHash(dep)) {
		if(hash->b<b) {
			b=hash->b;
			if(a>=b) return b;
		}
		if(hash->a>a) {
			a=hash->a;
			if(a>=b) return a;
		}
		s=hash->s;
	}

	mev=-3000;

	if(dep<14) {
		short bound;
		MPC *mpcp=&mpc[e-depth+dep][dep-4];

		if(mpcp->dep) {
			bound=(short)(mpcp->ia*(double)b+mpcp->b);
			if(ABmax1(mpcp->dep,dd,0,bound-1,bound)>=bound) {
				mev=b;
			} else {
				bound=(short)(mpcp->ia*(double)a+mpcp->a);
				if(ABmax1(mpcp->dep,dd,0,bound,bound+1)<=bound) mev=a;
			}
		}
	}

	if(mev==-3000) {
		for(mp=move,ep=eh.next;ep;ep=ep->next) {
			square=ep->s;
			if(!Move1(mp)) continue;
			Move(0)
			mp->ev=Eval();
			Undo(1)
			for(mtp=mp;mtp!=move;mtp--) {
				if(mtp[-1].ev<mtp->ev) {
					tmp=mtp[-1];
					mtp[-1]=*mtp;
					*mtp=tmp;
				}
			}
			mp++;
		}

		if(mp==move) {
			if(pp) {
				if(dd>0) mev=dd+e-depth+dep+2000;
				else if(dd<0) mev=dd-e+depth-dep-2000;
				else mev=0;
				s=0;
				at=-3000;
				bt=3000;
				evcnt++;
			} else {
				hc[0]^=hcsp[0];
				hc[1]^=hcsp[1];
				mev=PVSmin1(dep,dd,1,a,b);
				hc[0]^=hcsp[0];
				hc[1]^=hcsp[1];
				s=1;
			}
		} else {
			mp->c=0;
			mp=move;
			do {
				ep=mvemp[mp->s[0]-board];

				Move(0)
				ep->prev->next=ep->next;
				if(ep->next) ep->next->prev=ep->prev;
				hc[0]^=mp->hc[0];
				hc[1]^=mp->hc[1];

				if(mev==-3000) {
					ev=PVSmin1(dep-1,dd+(mp->c<<1)+1,0,a,b);
				} else {
					ev=PVSmin1(dep-1,i=dd+(mp->c<<1)+1,0,a,a+1);
					if(ev>a && ev<b) ev=PVSmin1(dep-1,i,0,ev,b);
				}

				Undo(1)
				ep->prev->next=ep;
				if(ep->next) ep->next->prev=ep;
				hc[0]^=mp->hc[0];
				hc[1]^=mp->hc[1];

				if(ev>mev) {
					mev=ev;
					s=mp->s[0]-board;
					if(ev>a) {
						a=ev;
						if(ev>=b) break;
					}
				}
			} while((++mp)->c);
		}
	}

	if(!forcecom) updateHash(dep,s,mev,at,bt);
	return mev;
}

short PVSmin1(char dep,char dd,char pp,short a,short b)
{
	short mev,ev,at,bt;
	char s,i;
	EMPTY *ep;
	MOVE move[35],tmp,*mp,*mtp;
	HASH *hash;

	if(dd+e-depth+dep==64) {
		evcnt++;
		return 2064;
	}
	if(dep<4) return ABmin1(dep,dd,0,a,b);

	s=0;
	at=a;
	bt=b;

	if(hash=getHash(dep)) {
		if(hash->b<b) {
			b=hash->b;
			if(a>=b) return b;
		}
		if(hash->a>a) {
			a=hash->a;
			if(a>=b) return a;
		}
		s=hash->s;
	}

	mev=3000;

	if(dep<14) {
		short bound;
		MPC *mpcp=&mpc[e-depth+dep][dep-4];

		if(mpcp->dep) {
			bound=(short)(mpcp->ia*(double)b+mpcp->b);
			if(ABmax1(mpcp->dep,dd,0,bound-1,bound)>=bound) {
				mev=b;
			} else {
				bound=(short)(mpcp->ia*(double)a+mpcp->a);
				if(ABmax1(mpcp->dep,dd,0,bound,bound+1)<=bound) mev=a;
			}
		}
	}

	if(mev==3000) {
		for(mp=move,ep=eh.next;ep;ep=ep->next) {
			square=ep->s;
			if(!Move2(mp)) continue;
			Move(1)
			mp->ev=Eval();
			Undo(0)
			for(mtp=mp;mtp!=move;mtp--) {
				if(mtp[-1].ev>mtp->ev) {
					tmp=mtp[-1];
					mtp[-1]=*mtp;
					*mtp=tmp;
				}
			}
			mp++;
		}

		if(mp==move) {
			if(pp) {
				if(dd>0) mev=dd+e-depth+dep+2000;
				else if(dd<0) mev=dd-e+depth-dep-2000;
				else mev=0;
				s=0;
				at=-3000;
				bt=3000;
				evcnt++;
			} else {
				hc[0]^=hcsp[0];
				hc[1]^=hcsp[1];
				mev=PVSmax1(dep,dd,1,a,b);
				hc[0]^=hcsp[0];
				hc[1]^=hcsp[1];
				s=1;
			}
		} else {
			mp->c=0;
			mp=move;
			do {
				ep=mvemp[mp->s[0]-board];

				Move(1)
				ep->prev->next=ep->next;
				if(ep->next) ep->next->prev=ep->prev;
				hc[0]^=mp->hc[0];
				hc[1]^=mp->hc[1];

				if(mev==3000) {
					ev=PVSmax1(dep-1,dd-(mp->c<<1)-1,0,a,b);
				} else {
					ev=PVSmax1(dep-1,i=dd-(mp->c<<1)-1,0,b-1,b);
					if(ev>a && ev<b) ev=PVSmax1(dep-1,i,0,a,ev);
				}

				Undo(0)
				ep->prev->next=ep;
				if(ep->next) ep->next->prev=ep;
				hc[0]^=mp->hc[0];
				hc[1]^=mp->hc[1];

				if(ev<mev) {
					mev=ev;
					s=mp->s[0]-board;
					if(ev<b) {
						b=ev;
						if(ev<=a) break;
					}
				}
			} while((++mp)->c);
		}
	}

	if(!forcecom) updateHash(dep,s,mev,at,bt);
	return mev;
}

char PVSmax2(char dep,char dd,char pp,char a,char b)
{
	char mev,ev,s,at,bt,i;
	EMPTY *ep,*bep;
	MOVE move[35],tmp,*mp,*mtp;
	HASH *hash;

	if(dep==6) return ABmax2(dep,dd,0,a,b);
	if(forcecom) {
		gs=(60-dep-9)/4;
		return Eval();
	}

	s=0;
	at=a;
	bt=b;

	if(hash=getHash(dep)) {
		if(hash->b<b) {
			b=hash->b;
			if(a>=b) return b;
		}
		if(hash->a>a) {
			a=hash->a;
			if(a>=b) return a;
		}
		s=hash->s;
	}

	for(mev=-100,mp=move,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Move1(mp)) continue;
		Move(0)
		bep->next=ep->next;
		mp->ev=Mobility2();
		Undo(1)
		bep->next=ep;
		for(mtp=mp;mtp!=move;mtp--) {
			if(mtp[-1].ev>mtp->ev) {
				tmp=mtp[-1];
				mtp[-1]=*mtp;
				*mtp=tmp;
			}
		}
		mp++;
	}

	if(mp==move) {
		if(pp) {
			if(dd>0) mev=dd+dep;
			else if(dd<0) mev=dd-dep;
			else mev=0;
			s=0;
			at=-100;
			bt=100;
			evcnt++;
		} else {
			hc[0]^=hcsp[0];
			hc[1]^=hcsp[1];
			mev=PVSmin2(dep,dd,1,a,b);
			hc[0]^=hcsp[0];
			hc[1]^=hcsp[1];
			s=1;
		}
	} else {
		mp->c=0;
		mp=move;
		do {
			ep=mvemp[mp->s[0]-board];

			Move(0)
			ep->prev->next=ep->next;
			if(ep->next) ep->next->prev=ep->prev;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];

			if(mev==-100) {
				ev=PVSmin2(dep-1,dd+(mp->c<<1)+1,0,a,b);
			} else {
				ev=PVSmin2(dep-1,i=dd+(mp->c<<1)+1,0,a,a+1);
				if(ev>a && ev<b) ev=PVSmin2(dep-1,i,0,ev,b);
			}

			Undo(1)
			ep->prev->next=ep;
			if(ep->next) ep->next->prev=ep;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];

			if(ev>mev) {
				mev=ev;
				s=mp->s[0]-board;
				if(ev>a) {
					a=ev;
					if(ev>=b) break;
				}
			}
		} while((++mp)->c);
	}

	if(!forcecom) updateHash(dep,s,mev,at,bt);
	return mev;
}

char PVSmin2(char dep,char dd,char pp,char a,char b)
{
	char mev,ev,s,at,bt,i;
	EMPTY *ep,*bep;
	MOVE move[35],tmp,*mp,*mtp;
	HASH *hash;

	if(dep==6) return ABmin2(dep,dd,0,a,b);

	s=0;
	at=a;
	bt=b;

	if(hash=getHash(dep)) {
		if(hash->b<b) {
			b=hash->b;
			if(a>=b) return b;
		}
		if(hash->a>a) {
			a=hash->a;
			if(a>=b) return a;
		}
		s=hash->s;
	}

	for(mev=100,mp=move,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Move2(mp)) continue;
		Move(1)
		bep->next=ep->next;
		mp->ev=Mobility1();
		Undo(0)
		bep->next=ep;
		for(mtp=mp;mtp!=move;mtp--) {
			if(mtp[-1].ev>mtp->ev) {
				tmp=mtp[-1];
				mtp[-1]=*mtp;
				*mtp=tmp;
			}
		}
		mp++;
	}

	if(mp==move) {
		if(pp) {
			if(dd>0) mev=dd+dep;
			else if(dd<0) mev=dd-dep;
			else mev=0;
			s=0;
			at=-100;
			bt=100;
			evcnt++;
		} else {
			hc[0]^=hcsp[0];
			hc[1]^=hcsp[1];
			mev=PVSmax2(dep,dd,1,a,b);
			hc[0]^=hcsp[0];
			hc[1]^=hcsp[1];
			s=1;
		}
	} else {
		mp->c=0;
		mp=move;
		do {
			ep=mvemp[mp->s[0]-board];

			Move(1)
			ep->prev->next=ep->next;
			if(ep->next) ep->next->prev=ep->prev;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];

			if(mev==100) {
				ev=PVSmax2(dep-1,dd-(mp->c<<1)-1,0,a,b);
			} else {
				ev=PVSmax2(dep-1,i=dd-(mp->c<<1)-1,0,b-1,b);
				if(ev>a && ev<b) ev=PVSmax2(dep-1,i,0,a,ev);
			}

			Undo(0)
			ep->prev->next=ep;
			if(ep->next) ep->next->prev=ep;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];

			if(ev<mev) {
				mev=ev;
				s=mp->s[0]-board;
				if(ev<b) {
					b=ev;
					if(ev<=a) break;
				}
			}
		} while((++mp)->c);
	}

	if(!forcecom) updateHash(dep,s,mev,at,bt);
	return mev;
}