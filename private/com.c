#include "ko.h"

extern char board[],pv[],dd,e,depth,ub,bop,mr,bmo;
extern short me,es[];
extern int evcnt;
extern HWND comcont,lpv,lev;
extern EMPTY eh,*mvemp[];
char di4[13][81],di5[13][243],di6[13][729],di7[13][2187],di8[13][6561];
char hv2[13][6561],hv3[13][6561],hv4[13][6561],e2x[13][59049],c33[13][19683],c25[13][59049];
char gs,clt,dispev,*square;
int hc[2],hcsp[2],hcsd[91][3][2],hcfd[91][2];
HE *he;
MPC mpc[62][10];

void WINAPI clearHash(s)
{
	static int size;
	int i;

	if(s) size=s;
	for(i=0;i<size;i++) he[i].d.lock=he[i].d.dep=he[i].n.lock=he[i].n.dep=0;
}

void WINAPI initEval(char *ef)
{
	char cf[256];
	int d1,d2,emp;
	double ia,a,b,r;
	FILE *f;
	MPC *mpcp;

	if(!(f=fopen(ef,"rb"))) return;
	fread(di4,1,1053,f);
	fread(di5,1,3159,f);
	fread(di6,1,9477,f);
	fread(di7,1,28431,f);
	fread(di8,1,85293,f);
	fread(hv2,1,85293,f);
	fread(hv3,1,85293,f);
	fread(hv4,1,85293,f);
	fread(e2x,1,767637,f);
	fread(c33,1,255879,f);
	fread(c25,1,767637,f);
	fclose(f);

	clearHash(0);

	for(d1=0;d1<62;d1++) {
		for(d2=0;d2<10;d2++) mpc[d1][d2].dep=0;
	}
	strcpy(cf,ef);
	*strrchr(cf,46)=0;
	sprintf(cf,"%s.txt",cf);
	if(!(f=fopen(cf,"r"))) return;
	while(fscanf(f,"%d %2d %2d %8lf %11lf %10lf %8lf",&d1,&d2,&emp,&ia,&a,&b,&r)!=-1) {
		mpcp=&mpc[emp][d2-4];
		mpcp->ia=ia;
		mpcp->a=a;
		mpcp->b=b;
		mpcp->dep=d1;
	}
	fclose(f);
}

int Rnd()
{
	static int r[]={58989,57068,5};
	int s[3];

	s[0]=r[0]*58989+11;
	s[1]=r[0]*57068+(s[0]>>16);
	s[2]=r[1]*57068+r[2]*58989+r[0]*5+(s[1]>>16);
	s[1]=r[1]*58989+(s[1]&65535);
	s[2]+=s[1]>>16;
	r[0]=s[0]&65535;
	r[1]=s[1]&65535;
	r[2]=s[2]&65535;
	return r[1]+(r[2]<<16);
}

char WINAPI initHash(int s)
{
	char i,j;
	int mask[2];

	switch(s) {
		case 0:s=65536;break;
		case 1:s=131072;break;
		case 2:s=262144;break;
		case 3:s=524288;break;
		default:s=1048576;
	}

	if(!(he=(HE *)realloc(he,s*sizeof(HE)))) return 0;

	mask[0]=s-1;
	mask[1]=-1;
	for(i=0;i<2;i++) {
		hcsp[i]=Rnd()&mask[i];
		for(j=10;j<81;j++) {
			if(board[j]==3) continue;
			hcsd[j][0][i]=Rnd()&mask[i];
			hcsd[j][1][i]=Rnd()&mask[i];
			hcsd[j][2][i]=0;
			hcfd[j][i]=hcsd[j][0][i]^hcsd[j][1][i];
			hcsd[j][0][i]^=hcsp[i];
			hcsd[j][1][i]^=hcsp[i];
		}
	}

	clearHash(s);

	return 1;
}

HASH *getHash(char dep)
{
	HE *hel=he+hc[0];

	if(hc[1]==hel->d.lock && hel->d.dep>=dep) return &hel->d;
	if(hc[1]==hel->n.lock && hel->n.dep>=dep) return &hel->n;
	return 0;
}

void updateHash(char dep,char s,short ev,short a,short b)
{
	HE *hel=he+hc[0];
	HASH *d=&hel->d,*n=&hel->n;

	if(hc[1]==d->lock) {
		if(ev>a && ev>d->a) d->a=ev;
		if(ev<b && ev<d->b) d->b=ev;
		d->s=s;
	} else if(hc[1]==n->lock) {
		if(ev>a && ev>n->a) n->a=ev;
		if(ev<b && ev<n->b) n->b=ev;
		n->s=s;
	} else if(d->dep<dep) {
		if(d->dep>n->dep) *n=*d;
		d->lock=hc[1];
		d->dep=dep;
		if(ev>a) d->a=ev;
		else d->a=-3000;
		if(ev<b) d->b=ev;
		else d->b=3000;
		d->s=s;
	} else {
		n->lock=hc[1];
		n->dep=dep;
		if(ev>a) n->a=ev;
		else n->a=-3000;
		if(ev<b) n->b=ev;
		else n->b=3000;
		n->s=s;
	}
}

void setPV(char p,char dep)
{
	char o;
	MOVE mp[1];
	HASH *hash;

	if(dep>14 || !(hash=getHash(0)) || !hash->s) return;
	o=p^1;
	if(hash->s>1) {
		if(dep) pv[dep]=hash->s;
		square=board+hash->s;
		if(p) Move2(mp);
		else Move1(mp);
		Move(p)
		hc[0]^=mp->hc[0];
		hc[1]^=mp->hc[1];
		setPV(o,dep+1);
		Undo(o)
		hc[0]^=mp->hc[0];
		hc[1]^=mp->hc[1];
	} else {
		hc[0]^=hcsp[0];
		hc[1]^=hcsp[1];
		setPV(o,dep);
		hc[0]^=hcsp[0];
		hc[1]^=hcsp[1];
	}
}

void Com(char p)
{
	short ev;
	char tm=0,m=0,mef=0,i;
	EMPTY *ep,*bep;
	MOVE move[35],tmp,*mp=move,*mtp;

	if(p==(e&1)) {
		hc[0]=hc[1]=0;
	} else {
		hc[0]=hcsp[0];
		hc[1]=hcsp[1];
	}
	for(i=10;i<81;i++) {
		if(board[i]==3) continue;
		hc[0]^=hcsd[i][board[i]][0];
		hc[1]^=hcsd[i][board[i]][1];
	}

	if(depth<0) gs=(60-e+6-9)/4;
	else gs=(60-e+4-9)/4;
	if(gs<0) gs=0;
	else if(gs>12) gs=12;
	for(bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Move1(mp)) continue;
		if(e>10 && (depth>5 || depth<0)) {
			Move(0)
			bep->next=ep->next;
			if(depth<0) mp->ev=ABmin1(5,dd+(mp->c<<1)+1,0,-2064,2064);
			else mp->ev=ABmin1(3,dd+(mp->c<<1)+1,0,-2064,2064);
			Undo(1)
			bep->next=ep;
			for(mtp=mp;mtp!=move;mtp--) {
				if(mtp[-1].ev<mtp->ev) {
					tmp=mtp[-1];
					mtp[-1]=*mtp;
					*mtp=tmp;
				}
			}
		}
		mp++;
		tm++;
	}
	mp->c=0;
	mp=move;
	if(!ub) pv[0]=mp->s[0]-board;
	if(depth>=0) {
		if((gs=(60-e+depth-9)/4)<0) gs=0;
		if(gs>12) gs=12;
	}
	evcnt=0;

	if(!depth) {
		me=Eval();
		comMsg(100);
		SendMessage(comcont,WM_SETTEXT,0,0);
		return;
	}

	comMsg(0);
	me=-3000;
	do {
		ep=mvemp[mp->s[0]-board];

		if(bmo) {
			Move(0)
			ep->prev->next=ep->next;
			if(ep->next) ep->next->prev=ep->prev;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];
			i=dd+(mp->c<<1)+1;

			if(me==-3000) {
				if(depth==-2) {
					if((me=PVSmin2(e-1,i,0,-64,64))==64) mef=1;
				} else if(depth==-1) {
					if((me=PVSmin2(e-1,i,0,-1,1))>0) mef=1;
				} else {
					if((me=PVSmin1(depth-1,i,0,-2064,2064))==2064) mef=1;
					else if(me<2000 && me>-2000) me+=rand()%mr-mr/2;
				}
			} else {
				if(depth==-2) {
					if((ev=PVSmin2(e-1,i,0,me,me+1))==64) {
						pv[0]=mp->s[0]-board;
						me=64;
						mef=1;
					} else if(me<ev) {
						pv[0]=mp->s[0]-board;
						if((me=PVSmin2(e-1,i,0,ev,64))==64) mef=1;
					}
				} else if(depth==-1) {
					if((ev=PVSmin2(e-1,i,0,me,1))>me) {
						pv[0]=mp->s[0]-board;
						if((me=ev)>0) mef=1;
					}
				} else {
					if((ev=PVSmin1(depth-1,i,0,me,me+1))==2064) {
						pv[0]=mp->s[0]-board;
						me=2064;
						mef=1;
					} else if(me<ev) {
						pv[0]=mp->s[0]-board;
						if((me=PVSmin1(depth-1,i,0,ev,2064))==2064) mef=1;
						else if(me<2000 && me>-2000) me+=rand()%mr-mr/2;
					}
				}
			}
		} else {
			if(es[mp->s[0]-board]>1400) goto nm;

			Move(0)
			ep->prev->next=ep->next;
			if(ep->next) ep->next->prev=ep->prev;
			hc[0]^=mp->hc[0];
			hc[1]^=mp->hc[1];
			i=dd+(mp->c<<1)+1;

			if(depth==-2) ev=PVSmin2(e-1,i,0,-64,64);
			else if(depth==-1) ev=PVSmin2(e-1,i,0,-1,1);
			else ev=PVSmin1(depth-1,i,0,-2064,2064);
			if(me<ev) {
				me=ev;
				if(!ub) pv[0]=mp->s[0]-board;
			}
			es[mp->s[0]-board]=ev;
		}
		Undo(1)
		ep->prev->next=ep;
		if(ep->next) ep->next->prev=ep;
		hc[0]^=mp->hc[0];
		hc[1]^=mp->hc[1];

nm:
		m++;
		if(mef) comMsg(100);
		else comMsg((int)((float)m/(float)tm*100+0.5));
	} while(!mef && (++mp)->c);
	SendMessage(comcont,WM_SETTEXT,0,0);
}

short Eval()
{
	short ev=di4[gs][board[13]+board[21]*3+board[29]*9+board[37]*27];
	ev+=di4[gs][board[53]+board[61]*3+board[69]*9+board[77]*27];
	ev+=di4[gs][board[14]+board[24]*3+board[34]*9+board[44]*27];
	ev+=di4[gs][board[46]+board[56]*3+board[66]*9+board[76]*27];
	ev+=di5[gs][board[14]+board[22]*3+board[30]*9+board[38]*27+board[46]*81];
	ev+=di5[gs][board[44]+board[52]*3+board[60]*9+board[68]*27+board[76]*81];
	ev+=di5[gs][board[13]+board[23]*3+board[33]*9+board[43]*27+board[53]*81];
	ev+=di5[gs][board[37]+board[47]*3+board[57]*9+board[67]*27+board[77]*81];
	ev+=di6[gs][board[15]+board[23]*3+board[31]*9+board[39]*27+board[47]*81+board[55]*243];
	ev+=di6[gs][board[35]+board[43]*3+board[51]*9+board[59]*27+board[67]*81+board[75]*243];
	ev+=di6[gs][board[12]+board[22]*3+board[32]*9+board[42]*27+board[52]*81+board[62]*243];
	ev+=di6[gs][board[28]+board[38]*3+board[48]*9+board[58]*27+board[68]*81+board[78]*243];
	ev+=di7[gs][board[16]+board[24]*3+board[32]*9+board[40]*27+board[48]*81+board[56]*243+board[64]*729];
	ev+=di7[gs][board[26]+board[34]*3+board[42]*9+board[50]*27+board[58]*81+board[66]*243+board[74]*729];
	ev+=di7[gs][board[11]+board[21]*3+board[31]*9+board[41]*27+board[51]*81+board[61]*243+board[71]*729];
	ev+=di7[gs][board[19]+board[29]*3+board[39]*9+board[49]*27+board[59]*81+board[69]*243+board[79]*729];
	ev+=di8[gs][board[17]+board[25]*3+board[33]*9+board[41]*27+board[49]*81+board[57]*243+board[65]*729+board[73]*2187];
	ev+=di8[gs][board[10]+board[20]*3+board[30]*9+board[40]*27+board[50]*81+board[60]*243+board[70]*729+board[80]*2187];
	ev+=hv2[gs][board[19]+board[20]*3+board[21]*9+board[22]*27+board[23]*81+board[24]*243+board[25]*729+board[26]*2187];
	ev+=hv2[gs][board[64]+board[65]*3+board[66]*9+board[67]*27+board[68]*81+board[69]*243+board[70]*729+board[71]*2187];
	ev+=hv2[gs][board[11]+board[20]*3+board[29]*9+board[38]*27+board[47]*81+board[56]*243+board[65]*729+board[74]*2187];
	ev+=hv2[gs][board[16]+board[25]*3+board[34]*9+board[43]*27+board[52]*81+board[61]*243+board[70]*729+board[79]*2187];
	ev+=hv3[gs][board[28]+board[29]*3+board[30]*9+board[31]*27+board[32]*81+board[33]*243+board[34]*729+board[35]*2187];
	ev+=hv3[gs][board[55]+board[56]*3+board[57]*9+board[58]*27+board[59]*81+board[60]*243+board[61]*729+board[62]*2187];
	ev+=hv3[gs][board[12]+board[21]*3+board[30]*9+board[39]*27+board[48]*81+board[57]*243+board[66]*729+board[75]*2187];
	ev+=hv3[gs][board[15]+board[24]*3+board[33]*9+board[42]*27+board[51]*81+board[60]*243+board[69]*729+board[78]*2187];
	ev+=hv4[gs][board[37]+board[38]*3+board[39]*9+board[40]*27+board[41]*81+board[42]*243+board[43]*729+board[44]*2187];
	ev+=hv4[gs][board[46]+board[47]*3+board[48]*9+board[49]*27+board[50]*81+board[51]*243+board[52]*729+board[53]*2187];
	ev+=hv4[gs][board[13]+board[22]*3+board[31]*9+board[40]*27+board[49]*81+board[58]*243+board[67]*729+board[76]*2187];
	ev+=hv4[gs][board[14]+board[23]*3+board[32]*9+board[41]*27+board[50]*81+board[59]*243+board[68]*729+board[77]*2187];
	ev+=e2x[gs][board[10]+board[11]*3+board[12]*9+board[13]*27+board[14]*81+board[15]*243+board[16]*729+board[17]*2187+board[20]*6561+board[25]*19683];
	ev+=e2x[gs][board[17]+board[26]*3+board[35]*9+board[44]*27+board[53]*81+board[62]*243+board[71]*729+board[80]*2187+board[25]*6561+board[70]*19683];
	ev+=e2x[gs][board[73]+board[64]*3+board[55]*9+board[46]*27+board[37]*81+board[28]*243+board[19]*729+board[10]*2187+board[65]*6561+board[20]*19683];
	ev+=e2x[gs][board[80]+board[79]*3+board[78]*9+board[77]*27+board[76]*81+board[75]*243+board[74]*729+board[73]*2187+board[70]*6561+board[65]*19683];
	ev+=c33[gs][board[10]+board[11]*3+board[12]*9+board[19]*27+board[20]*81+board[21]*243+board[28]*729+board[29]*2187+board[30]*6561];
	ev+=c33[gs][board[17]+board[16]*3+board[15]*9+board[26]*27+board[25]*81+board[24]*243+board[35]*729+board[34]*2187+board[33]*6561];
	ev+=c33[gs][board[73]+board[74]*3+board[75]*9+board[64]*27+board[65]*81+board[66]*243+board[55]*729+board[56]*2187+board[57]*6561];
	ev+=c33[gs][board[80]+board[79]*3+board[78]*9+board[71]*27+board[70]*81+board[69]*243+board[62]*729+board[61]*2187+board[60]*6561];
	ev+=c25[gs][board[10]+board[11]*3+board[12]*9+board[13]*27+board[14]*81+board[19]*243+board[20]*729+board[21]*2187+board[22]*6561+board[23]*19683];
	ev+=c25[gs][board[10]+board[19]*3+board[28]*9+board[37]*27+board[46]*81+board[11]*243+board[20]*729+board[29]*2187+board[38]*6561+board[47]*19683];
	ev+=c25[gs][board[17]+board[16]*3+board[15]*9+board[14]*27+board[13]*81+board[26]*243+board[25]*729+board[24]*2187+board[23]*6561+board[22]*19683];
	ev+=c25[gs][board[17]+board[26]*3+board[35]*9+board[44]*27+board[53]*81+board[16]*243+board[25]*729+board[34]*2187+board[43]*6561+board[52]*19683];
	ev+=c25[gs][board[73]+board[74]*3+board[75]*9+board[76]*27+board[77]*81+board[64]*243+board[65]*729+board[66]*2187+board[67]*6561+board[68]*19683];
	ev+=c25[gs][board[73]+board[64]*3+board[55]*9+board[46]*27+board[37]*81+board[74]*243+board[65]*729+board[56]*2187+board[47]*6561+board[38]*19683];
	ev+=c25[gs][board[80]+board[79]*3+board[78]*9+board[77]*27+board[76]*81+board[71]*243+board[70]*729+board[69]*2187+board[68]*6561+board[67]*19683];
	ev+=c25[gs][board[80]+board[71]*3+board[62]*9+board[53]*27+board[44]*81+board[79]*243+board[70]*729+board[61]*2187+board[52]*6561+board[43]*19683];
	return ev;
}

char WINAPI getComDepType(float *mev)
{
	short mes=me;

	if(depth==-2) {
		if(mes>1400) mes-=1500;
		*mev=(float)mes;
	} else if(depth==-1) {
		if(mes>1400) mes-=1500;
		*mev=(float)mes;
	} else if(mes>2000) {
		*mev=(float)mes-2000;
	} else if(mes<-2000) {
		*mev=(float)mes+2000;
	} else if(mes>1400) {
		*mev=((float)mes-1500)/1.01;
	} else {
		*mev=(float)mes/10.1;
	}

	return clt;
}

void WINAPI setDispEval(char s)
{
	dispev=s;
}

void comMsg(char prog)
{
	short mev=me;
	char buf[46],i,j;

	if(mev==-3000) {
		SendMessage(lev,WM_SETTEXT,0,"");
		return;
	}
	if(!prog) {
		SendMessage(lev,WM_SETTEXT,0,"0“");
		return;
	}

	if(depth==-2) {
		if(mev>1400) {
			mev-=1500;
			i=0;
		} else {
			i=3;
		}
		sprintf(buf,"%+d",mev);
	} else if(depth==-1) {
		if(mev>1400) {
			mev-=1500;
			i=0;
		} else {
			i=2;
		}
		if(mev>0) sprintf(buf,"Win");
		else if(mev<0) sprintf(buf,"Loss");
		else sprintf(buf,"Draw");
	} else if(mev>2000) {
		sprintf(buf,"%+d",mev-2000);
		i=3;
	} else if(mev<-2000) {
		sprintf(buf,"%d",mev+2000);
		i=3;
	} else {
		if(mev>1400) {
			sprintf(buf,"%+.2f",((float)mev-1500)/1.01);
			i=0;
		} else {
			sprintf(buf,"%+.2f",(float)mev/10.1);
			i=1;
		}
	}
	if(!i) strcat(buf," (B)");
	clt=i;

	if(prog<100 && i && (strcmp(buf,"Win") || !bmo)) {
		if(dispev || !bmo) sprintf(buf,"%s %d“",buf,prog);
		else sprintf(buf,"%d“",prog);
	} else if(!dispev && bmo) {
		buf[0]=0;
	}
	SendMessage(lev,WM_SETTEXT,0,buf);

	if(!ub) setPV(0,0);
	for(i=0;i<15 && (j=pv[i]);i++) sprintf(buf+i*3,"%c%d ",j%9+96,j/9);
	buf[strlen(buf)-1]=0;
	SendMessage(lpv,WM_SETTEXT,0,buf);
}