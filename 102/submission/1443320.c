#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RN 250000

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
I ncnt = 1;

#define nnode(x) net_pool[x]
#define nnxt(x)  nnode(x).nxt
#define nto(x)   nnode(x).to
#define ncap(x)  nnode(x).cap
#define ncost(x) nnode(x).cost

// Network Simplex

I head[RN], fa[RN], fe[RN], pi[RN], mark[RN], buf[RN];
I ti, nc;

static inline void addEdge(I u, I v, I f, I c)
{
    nnode(++ncnt) = (Network){ head[u], v, f, c };
    head[u] = ncnt;
    nnode(++ncnt) = (Network){ head[v], u, 0, -c };
    head[v] = ncnt;
}

void initTree(I x)
{
    nc++, mark[x] = 1;
    for (I i = head[x]; i; i = nnxt(i))
    {
        I v = nto(i);
        if (!mark[v] && ncap(i))
        {
            fa[v] = x, fe[v] = i;
            pi[v] = pi[x] - ncost(i);
            initTree(v);
        }
    }
}

static inline I phi(I x)
{
    I top = 0;
    while (mark[x] != ti) mark[buf[top++] = x] = ti, x = fa[x];
    while (top--) x = buf[top], pi[x] = pi[fa[x]] - ncost(fe[x]);
    return pi[x];
}

void pushFlow(I e, L *cost)
{
	I u = nto(e ^ 1), v = nto(e), l = nc, r = nc;
	ti++;
	while (u)             buf[++r] = fe[u], mark[u] = ti, u = fa[u];
	while (mark[v] != ti) buf[--l] = fe[v] ^ 1, mark[v] = ti, v = fa[v];
	buf[nc] = e;
	
	I e2 = l;
	for (I i = l; buf[i] != fe[v]; i++)
	{
	    if (ncap(buf[e2]) > ncap(buf[i])) e2 = i;
	}
	I f = ncap(buf[e2]);
	for (I i = l; buf[i] != fe[v]; i++)
	{
		ncap(buf[i]) -= f, ncap(buf[i] ^ 1) += f;
		*cost += 1ll * ncost(buf[i]) * f;
	}
	if (e2 == nc) return;
	
	I x = e ^ (e2 < nc), y = nto(x), z = nto(x ^ 1);
	while (x != (buf[e2] ^ (e2 < nc)))
	{
		x ^= 1;
		pi[z] = pi[y] - ncost(x);
		SWAP(I, x, fe[z]); SWAP(I, y, fa[z]); SWAP(I, y, z);
	}
}

I simplex(I st, I ed, L *cost)
{
	I lhead = head[st], lhead2 = head[ed];
	addEdge(ed, st, 0x3f3f3f3f, -0x3f3f3f3f);
	initTree(ed);
	mark[ed] = ti = 1, fa[ed] = *cost = 0;
	for (I i = 4, pre = 2; i != pre; i = i == ncnt - 1 ? 2 : i + 2)
	{
		if (!mark[nto(i)] || !mark[nto(i ^ 1)])
			continue;
		I delta = phi(nto(i ^ 1)) - phi(nto(i));
		if (ncost(i) < delta && ncap(i))
			pushFlow(pre = i, cost);
		if (ncost(i) > delta && ncap(i ^ 1))
			pushFlow((pre = i) ^ 1, cost);
	}
	head[st] = lhead, head[ed] = lhead2, ncnt -= 2;
	return ncap(ncnt + 2);
}

// Main

int main(void)
{
	I n, m, s, t;
	scanf("%d%d", &n, &m);
	s = 1, t = n;
	for (I i = 1; i <= m; i++)
	{
		I u, v, f, c;
		scanf("%d%d%d%d", &u, &v, &f, &c);
		addEdge(u, v, f, c);
	}
	L cost;
	I flow = simplex(s, t, &cost);
	printf("%d %lld\n", flow, cost + 1ll * flow * 0x3f3f3f3f);
	return 0;
}