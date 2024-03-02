#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RN 50005

typedef int I;
typedef char C;
typedef long long L;

#define SWAP(T, a, b) { T t = a; a = b; b = t; }

// Pools

typedef struct
{
    I nxt;
    I to;
    I cap;
    I cost;
}
Network;

Network net_pool[RN * 2];
I       ncnt = 1;

#define nnode(x) net_pool[x]
#define nnxt(x)  nnode(x).nxt
#define nto(x)   nnode(x).to
#define ncap(x)  nnode(x).cap
#define ncost(x) nnode(x).cost

// Network Simplex

I head[RN], fa[RN], fe[RN], pi[RN], mark[RN], cycle[RN];
I ti;

static inline void addEdge(I u, I v, I f, I c)
{
    nnode(++ncnt) = (Network){head[u], v, f, c};
    head[u] = ncnt;
    nnode(++ncnt) = (Network){head[v], u, 0, -c};
    head[v] = ncnt;
}

void initTree(I x)
{
    mark[x] = 1;
    for (I i = head[x]; i; i = nnxt(i))
    {
        I v = nto(i);
        if (!mark[v])
        {
            fa[v] = x, fe[v] = i;
            initTree(v);
        }
    }
}

I phi(I x)
{
    if (mark[x] == ti) return pi[x];
    return mark[x] = ti, pi[x] = phi(fa[x]) - ncost(fe[x]);
}

void pushFlow(I e, L *cost)
{
    I pen = nto(e ^ 1), lca = nto(e);
    while (pen) mark[pen] = ti + 1, pen = fa[pen];
    while (mark[lca] != ti + 1)     lca = fa[lca];
    
    I e2 = 0, f = ncap(e), path = 2, clen = 0;
    for (I i = nto(e ^ 1); i != lca; i = fa[i])
    {
        cycle[++clen] = fe[i];
        if (ncap(fe[i]) < f) f = ncap(fe[e2 = i] ^ (path = 0));
    } 
    for (I i = nto(e); i != lca; i = fa[i])
    {
        cycle[++clen] = fe[i] ^ 1;
        if (ncap(fe[i] ^ 1) <= f) f = ncap(fe[e2 = i] ^ (path = 1));
    }
    cycle[++clen] = e;
    
    for (I i = 1; i <= clen; i++)
    {
        ncap(cycle[i]) -= f;
        ncap(cycle[i] ^ 1) += f;
        *cost += 1ll * f * ncost(cycle[i]);
    }
    if (path == 2) return;
    
    I laste = e ^ path, last = nto(laste), cur = nto(laste ^ 1);
    while (last != e2)
    {
        laste ^= 1;
        SWAP(I, fe[cur], laste);
        SWAP(I, fa[cur], last);
        SWAP(I, cur, last);
    }
}

I simplex(I st, I ed, L *cost)
{
    addEdge(ed, st, 0x3f3f3f3f, 0xc0c0c0c0);
    initTree(ed);
    fa[ed] = *cost = 0, mark[ed] = ti = 2;
    for (I i = 3, pre = 2; i != pre; i = i == ncnt ? 2 : i + 1)
    {
        if (ncap(i) > 0 && ncost(i) < phi(nto(i ^ 1)) - phi(nto(i)))
        {
            pushFlow(i, cost);
            mark[ed] = ti = ti + 2, pre = i;
        }
    }
    ncnt -= 2;
    return ncap(ncnt + 2);
}

// Main

int main(void)
{
    I n, m, s, t;
    scanf("%d%d", &n, &m);
    for (I i = 1; i <= m; i++)
    {
        I u, v, f, c;
        scanf("%d%d%d%d", &u, &v, &f, &c);
        addEdge(u, v, f, c);
    }
    L cost;
    I flow = simplex(1, n, &cost);
    printf("%d %lld", flow, cost + 1ll * flow * (-0xc0c0c0c0));
    return 0;
}