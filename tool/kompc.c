#include <stdio.h>
#include <string.h>
#include <math.h>
#include <io.h>
#include <direct.h>

typedef struct empty {
	struct empty *prev,*next;
	char *s;
} EMPTY;
typedef struct {
	short ev;
	EMPTY *empty;
} EMPEV;
typedef struct {
	double ia,a,b;
	char dep;
} MPC;
char board[91],dd,e,depth,gs,*square,*stack[400],**sp=stack;
char di4[13][81],di5[13][243],di6[13][729],di7[13][2187],di8[13][6561];
char hv2[13][6561],hv3[13][6561],hv4[13][6561],e2x[13][59049],c33[13][19683],c25[13][59049];
char cp[]={
	10,17,73,80,
	31,32,39,42,48,51,58,59,
	30,33,57,60,
	22,23,38,43,47,52,67,68,
	12,15,28,35,55,62,75,78,
	13,14,37,44,46,53,76,77,
	21,24,29,34,56,61,66,69,
	11,16,19,26,64,71,74,79,
	20,25,65,70,
	40,41,49,50
},dirmask[]={
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
short stat[1000][61][10][2];
EMPTY empty[63],eh;
MPC mpc[62][10];

__forceinline char dirFlip(char p,char o,char dir)
{
	char *s=square+dir;

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
		if(*s==p) return 1;
	}
	return 0;
}

char Flip(char p,char o)
{
	char i=dirmask[square-board];

	if(i&1 && dirFlip(p,o,-1)) return 1;
	if(i&2 && dirFlip(p,o,-10)) return 1;
	if(i&4 && dirFlip(p,o,-9)) return 1;
	if(i&8 && dirFlip(p,o,-8)) return 1;
	if(i&16 && dirFlip(p,o,1)) return 1;
	if(i&32 && dirFlip(p,o,10)) return 1;
	if(i&64 && dirFlip(p,o,9)) return 1;
	if(i&128 && dirFlip(p,o,8)) return 1;
	return 0;
}

__forceinline void dirMove(char p,char o,char dir)
{
	char *s=square+dir;

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
				*s=p;
				*sp++=s;
			} while((s-=dir)!=square);
		}
	}
}

char Move(char p,char o)
{
	char i=dirmask[square-board],**bsp=sp;

	if(i&1) dirMove(p,o,-1);
	if(i&2) dirMove(p,o,-10);
	if(i&4) dirMove(p,o,-9);
	if(i&8) dirMove(p,o,-8);
	if(i&16) dirMove(p,o,1);
	if(i&32) dirMove(p,o,10);
	if(i&64) dirMove(p,o,9);
	if(i&128) dirMove(p,o,8);
	return sp-bsp;
}

short Eval(char p)
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
	return p ? -ev:ev;
}

short AB(char p,char dep,char dd,char pp,short a,short b)
{
	short ev;
	char o,i;
	EMPTY *ep,*bep;

	if(!dep) return Eval(p);

	for(o=p^1,ev=-3000,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!(i=Move(p,o))) continue;
		*ep->s=p;
		bep->next=ep->next;
		ev=-AB(o,dep-1,-dd-(i<<1)-1,0,-b,-a);
		if(i&1){**--sp=o;i--;}while(i){**--sp=o;**--sp=o;i-=2;}
		*ep->s=2;
		bep->next=ep;
		if(a<ev) a=ev;
		if(a>=b) return a;
	}

	if(ev!=-3000) return a;
	if(pp) {
		if(dd>0) return dd+e-depth+dep+2000;
		if(dd<0) return dd-e+depth-dep-2000;
		return 0;
	}
	return -AB(o,dep,-dd,1,-b,-a);
}

short PVS(char m,char dep,char dd,char pp,short a,short b)
{
	short mev,ev;
	char o,i,d;
	EMPTY *ep;
	EMPEV nm[35],tmp,*p;

	if(dep<4) return AB(m,dep,dd,0,a,b);

	if(dep<14) {
		short bound;
		MPC *mpcp=&mpc[e-depth+dep][dep-4];

		if(mpcp->dep) {
			bound=(short)(mpcp->ia*(double)b+mpcp->b);
			if(AB(m,mpcp->dep,dd,0,bound-1,bound)>=bound) return b;
			bound=(short)(mpcp->ia*(double)a+mpcp->a);
			if(AB(m,mpcp->dep,dd,0,bound,bound+1)<=bound) return a;
		}
	}

	for(o=m^1,mev=-3000,p=nm,ep=eh.next;ep;ep=ep->next) {
		square=ep->s;
		if(!(i=Move(m,o))) continue;
		*ep->s=m;
		p->empty=ep;
		p->ev=Eval(m);
		if(i&1){**--sp=o;i--;}while(i){**--sp=o;**--sp=o;i-=2;}
		*ep->s=2;
		for(i=p-nm;i;i--) {
			if(nm[i-1].ev<nm[i].ev) {
				tmp=nm[i-1];
				nm[i-1]=nm[i];
				nm[i]=tmp;
			}
		}
		p++;
	}

	if(p==nm) {
		if(pp) {
			if(dd>0) return dd+e-depth+dep+2000;
			if(dd<0) return dd-e+depth-dep-2000;
			return 0;
		}
		return -PVS(o,dep,-dd,1,-b,-a);
	}

	p->empty=0;
	p=nm;
	do {
		ep=p->empty;
		square=ep->s;
		i=Move(m,o);
		*ep->s=m;
		ep->prev->next=ep->next;
		if(ep->next) ep->next->prev=ep->prev;

		if(mev==-3000) {
			ev=-PVS(o,dep-1,-dd-(i<<1)-1,0,-b,-a);
		} else {
			d=-dd-(i<<1)-1;
			ev=-PVS(o,dep-1,d,0,-a-1,-a);
			if(ev>a && ev<b) ev=-PVS(o,dep-1,d,0,-b,-ev);
		}

		if(i&1){**--sp=o;i--;}while(i){**--sp=o;**--sp=o;i-=2;}
		*ep->s=2;
		ep->prev->next=ep;
		if(ep->next) ep->next->prev=ep;
		if(ev>mev) {
			mev=ev;
			if(ev>a) {
				a=ev;
				if(ev>=b) return mev;
			}
		}
	} while((++p)->empty);
	return mev;
}

short Com(char rp)
{
	short mev=-3000,ev;
	char d,i,mef=0;
	EMPTY *ep,*bep;
	EMPEV nm[35],tmp,*p;

	if((gs=(60-e+4-9)/4)<0) gs=0;
	if(gs>12) gs=12;
	for(p=nm,bep=&eh,ep=bep->next;ep;bep=ep,ep=ep->next) {
		square=ep->s;
		if(!Flip(0,1)) continue;
		p->empty=ep;
		if(e>10 && depth>5) {
			i=Move(0,1);
			*ep->s=0;
			bep->next=ep->next;
			p->ev=-AB(1,3,-dd-(i<<1)-1,0,-2064,2064);
			if(i&1){**--sp=1;i--;}while(i){**--sp=1;**--sp=1;i-=2;}
			*ep->s=2;
			bep->next=ep;
			for(i=p-nm;i;i--) {
				if(nm[i-1].ev<nm[i].ev) {
					tmp=nm[i-1];
					nm[i-1]=nm[i];
					nm[i]=tmp;
				}
			}
		}
		p++;
	}
	p->empty=0;
	p=nm;
	if((gs=(60-e+depth-9)/4)<0) gs=0;
	if(gs>12) gs=12;

	ep=p->empty;
	do {
		square=ep->s;
		i=Move(0,1);
		*ep->s=0;
		ep->prev->next=ep->next;
		if(ep->next) ep->next->prev=ep->prev;

		if(mev==-3000) {
			if((mev=-PVS(1,depth-1,-dd-(i<<1)-1,0,-2064,2064))==2064) mef=1;
		} else {
			d=-dd-(i<<1)-1;
			if((ev=-PVS(1,depth-1,d,0,-mev-1,-mev))==2064) mef=1;
			else if(mev<ev && (mev=-PVS(1,depth-1,d,0,-2064,-ev))==2064) mef=1;
		}

		if(i&1){**--sp=1;i--;}while(i){**--sp=1;**--sp=1;i-=2;}
		*ep->s=2;
		ep->prev->next=ep;
		if(ep->next) ep->next->prev=ep;
	} while(!mef && (ep=(++p)->empty));

	return rp ? -mev:mev;
}

void main(int argc,char **argv)
{
	char cd[]={2,1,2,3,4,3,4,3,4,5},game[61],buf[256],p,dep,i,j;
	short ev1,ev2,cgc,gc,gl;
	int fn;
	double sum_x,sum_y,sum_xy,avg_x,avg_y,avg_xy,sigma_x,sigma_y,a,b,r,t;
	FILE *f;
	EMPTY *ep;

	if(argc!=2) return;
	strcpy(buf,argv[1]);
	*strrchr(buf,92)=0;
	if(chdir(buf) || !(f=fopen(argv[1],"rb"))) return;

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

	strcpy(buf,argv[1]);
	*strrchr(buf,46)=0;
	sprintf(buf,"%s.txt",buf);
	if(f=fopen(buf,"r")) {
		int d1,d2,emp;
		MPC *mpcp;

		while(fscanf(f,"%d %2d %2d %8lf %11lf %10lf %8lf",&d1,&d2,&emp,&t,&a,&b,&r)!=-1) {
			mpcp=&mpc[emp][d2-4];
			mpcp->ia=t;
			mpcp->a=a;
			mpcp->b=b;
			mpcp->dep=d1;
		}
		fclose(f);

		printf("MPC-table: exist\n");
	} else {
		printf("MPC-table: not exist\n");
	}

	if(!(f=fopen("game.txt","r"))) return;

	printf("Depth: ");
	scanf("%d",&dep);
	printf("Calculate game count: ");
	scanf("%d",&cgc);

	for(gc=0;gc<cgc;gc++) {
		for(i=0;i<61;i++) {
			for(j=0;j<11;j++) stat[gc][i][j][0]=stat[gc][i][j][1]=-3000;
		}
	}

	for(i=0;i<10;i++) board[i]=board[i+81]=board[i*9]=3;

	printf("   0");
	for(gc=0;gc<cgc;) {
		buf[0]=0;
		fgets(buf,256,f);
		if(!buf[0]) break;
		if(buf[i=strlen(buf)-1]==10) buf[i]=0;

		for(i=0;buf[i];i+=2) game[i/2]=buf[i]-96+(buf[i+1]-48)*9;
		game[i/2]=0;

		for(i=10;i<81;i++) {
			if(board[i]!=3) board[i]=2;
		}
		board[41]=board[49]=0;
		board[40]=board[50]=1;

		for(p=i=0;game[i];p^=1,i++) {
			printf(" %2d",i);
			square=board+game[i];
			if(!Flip(p,p^1)) p^=1;

			for(ep=&eh,dd=e=j=0;j<64;j++) {
				if(!board[cp[j]]) {
					if(p) {
						board[cp[j]]=1;
						dd--;
					} else {
						dd++;
					}
				} else if(board[cp[j]]==1) {
					if(p) {
						board[cp[j]]=0;
						dd++;
					} else {
						dd--;
					}
				} else if(board[cp[j]]==2) {
					ep->next=&empty[e];
					empty[e++].prev=ep;
					ep=ep->next;
					ep->s=board+cp[j];
				}
			}
			ep->next=0;

			for(j=0;i<49 && i>11+j && j<dep-3;j++) {
				depth=cd[j];
				if((ev1=Com(p))>2000 || ev1<-2000) break;
				depth=j+4;
				printf(" %2d",depth);
				ev2=Com(p);
				printf("\b\b\b");
				if(ev2>2000 || ev2<-2000) break;

				stat[gc][60-i][j][0]=ev1;
				stat[gc][60-i][j][1]=ev2;
			}

			if(p) {
				for(j=10;j<81;j++) {
					if(!board[j]) board[j]=1;
					else if(board[j]==1) board[j]=0;
				}
			}

			square=board+game[i];
			Move(p,p^1);
			board[game[i]]=p;
			sp=stack;
			printf("\b\b\b");
		}

		printf("\b\b\b\b%4d",++gc);
	}

	fclose(f);

	*strrchr(argv[1],46)=0;
	sprintf(argv[1],"%s.txt",strrchr(argv[1],92)+1);
	if(!(f=fopen(argv[1],"w"))) return;

	for(i=0;i<dep-3;i++) {
		for(j=0;j<61;j++) {
			for(sum_x=sum_y=sum_xy=sigma_x=sigma_y=gl=gc=0;gc<cgc;gc++) {
				if(stat[gc][j][i][0]==-3000) continue;
				sum_x+=(double)stat[gc][j][i][0];
				sum_y+=(double)stat[gc][j][i][1];
				sum_xy+=(double)stat[gc][j][i][0]*(double)stat[gc][j][i][1];
				sigma_x+=pow((double)stat[gc][j][i][0],2);
				sigma_y+=pow((double)stat[gc][j][i][1],2);
				gl++;
			}

			if(gl) {
				avg_x=sum_x/(double)gl;
				avg_y=sum_y/(double)gl;
				avg_xy=sum_xy/(double)gl;
				sigma_x=sqrt(sigma_x/(double)gl-pow(avg_x,2));
				sigma_y=sqrt(sigma_y/(double)gl-pow(avg_y,2));
				r=(sigma_x && sigma_y) ? (sum_xy/(double)gl-avg_x*avg_y)/sigma_x/sigma_y:0;

				for(sum_x=sum_xy=gc=0;gc<cgc;gc++) {
					if(stat[gc][j][i][0]==-3000) continue;
					sum_x+=pow((double)stat[gc][j][i][0]-avg_x,2);
					sum_xy+=((double)stat[gc][j][i][0]-avg_x)*((double)stat[gc][j][i][1]-avg_y);
				}
				a=sum_x ? sum_xy/sum_x:0;
				b=avg_y-a*avg_x;
			} else {
				a=b=r=0;
			}

			if(r>0.8) {
				t=j>30 ? 1.0:1.5;
				fprintf(f,"%d %2d %2d %8lf %11lf %10lf %8lf\n",cd[i],i+4,j,1/a,(-t*sigma_x-b)/a+0.5,(t*sigma_x-b)/a+0.5,r);
			}
		}
	}

	fclose(f);

	if(!(f=fopen(argv[1],"a"))) return;
	fn=fileno(f);
	chsize(fn,filelength(fn)-2);
	fclose(f);
}