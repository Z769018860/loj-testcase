#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
#include<stdbool.h>

#define space putC(' ')
#define enter putC('\n')
#define mian main

#define ff(a,b,c) for(register int a = (b);a <= (c);++a)
#define fb(a,b,c) for(register int a = (b);a >= (c);--a)
#define fe(a,b) for(register int a = head[b]; ~a ; a = e[a].nxt)
#define mems(a,b) memset(a,b,sizeof(a))

typedef long long ll;

#define N 505
#define M 50005
#define INF 1000000000

#define IS stdin
#define OS stdout
#define IOLIM 10000
char ibuf[IOLIM],obuf[IOLIM];
char *_i = ibuf,*_o = obuf;
inline static void init_IO(void) {
    fread(ibuf,IOLIM,sizeof(char),IS);
}

inline static char getC(void) {
    if(_i == ibuf + IOLIM) {
        mems(ibuf,0);
        fread(ibuf,IOLIM,sizeof(char),IS);
        _i = ibuf;
    }
    return *_i++;
}

inline static void end_IO(void) {
    fwrite(obuf,_o - obuf,sizeof(char),OS);
}

inline static void putC(char ch) {
    if(_o == obuf + IOLIM) {
        fwrite(obuf,_o - obuf,sizeof(char),OS);
        _o = obuf;
    }
    *_o++ = ch;
}

inline static int read(void) {
    int res = 0;char ch = getC();bool m = 0;
    while(!isdigit(ch)) {
        if(ch == '-') m = 1;
        ch = getC();
    }
    while(isdigit(ch)) {
        res = res * 10 + (ch ^ 48);
        ch = getC();
    }
    return m ? -res : res;
}

inline static void write(ll x) {
    if(x < 0) putC('-'),x = -x;
    int sta[20];int top = 0;
    do
        sta[top++] = x % 10,x /= 10;
    while(x);
    while(top) putC(sta[--top] + '0');
}

int head[N],ecnt = -1;
int cur[N];
struct Edge {
	int nxt,to,cap;
}e[M << 1];
inline static void AddEdge(int st,int ed,int cap) {
    e[++ecnt].nxt = head[st],e[ecnt].to = ed,e[ecnt].cap = cap,head[st] = ecnt;
    e[++ecnt].nxt = head[ed],e[ecnt].to = st,head[ed] = ecnt;
}

#define Min(a,b) (a) < (b) ? (a) : (b)
int n,m,s,t;
int dep[N],gap[N];
int que[N];
static void bfs(void) {
	dep[t] = 1,gap[1] = 1;
	int st = 0,ed = 1;
    que[0] = t;
	while(st != ed) {
		int u = que[st++];
		fe(i,u) {
			int v = e[i].to;
			if(dep[v]) continue;
			dep[v] = dep[u] + 1;
			++gap[dep[v]];
			que[ed++] = v;
		}
	}
}

ll ans = 0;

static ll ISAP(int u,ll f) {
	if(u == t || !f) {
		ans += f;
		return f;
	}
	ll maxf = 0,tf;
	for(register int i = cur[u]; ~i && f;i = e[i].nxt) {
		cur[u] = i;int v = e[i].to;
		if(dep[v] != dep[u] - 1 || !e[i].cap) continue;
		tf = ISAP(v,Min(f - maxf,e[i].cap));
		if(tf) {
			maxf += tf;
			e[i].cap -= tf,e[i ^ 1].cap += tf;
		}
		if(maxf == f) return maxf;
	}
    --gap[dep[u]];
    if(!gap[dep[u]]) dep[s] = n + 1;
    ++dep[u],++gap[dep[u]];
	return maxf;
}

static void solve(void) {
	bfs();
	while(dep[s] <= n) {
		memcpy(cur,head,sizeof(head));
		ISAP(s,INF);
	}
	write(ans);
}

int mian(void) {
	init_IO();
	mems(head,-1);
	n = read(),m = read(),s = read(),t = read();
	ff(i,1,m) {
		int st = read(),ed = read(),cap = read();
		AddEdge(st,ed,cap);
	}
	solve();
	end_IO();
	return 0;
}