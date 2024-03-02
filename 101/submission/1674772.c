#include <stdio.h>
#include <stdbool.h>
long long min(long long a, long long b) {
	return a < b ? a : b;
}
static int n, m, head[205], nxt[10005], to[10005], c[10005], d[205], tot = 1, s, t, cur[205], q[205], hh, tt;
void addedge(int u, int v, int w) {
	nxt[++tot] = head[u], head[u] = tot, to[tot] = v, c[tot] = w;
	nxt[++tot] = head[v], head[v] = tot, to[tot] = u, c[tot] = 0;
}
bool bfs() {
	int i;
	for (i = 1; i <= n; ++i)
		d[i] = -1, cur[i] = head[i];
	hh = 1;
	tt = 1;
	q[1] = s;
	d[s] = 0;
	while (hh <= tt) {
		int u = q[hh++];
		for (i = head[u]; i; i = nxt[i]) {
			int v = to[i];
			if (d[v] == -1 && c[i]) {
				d[v] = d[u] + 1;
				q[++tt] = v;
				if (v == t)
					return true;
			}
		}
	}
	return false;
}
long long dfs(int u, long long lim) {
	if (u == t)
		return lim;
	long long flow = 0;
	int i;
	for (i = cur[u]; i && flow < lim; i = nxt[i]) {
		cur[u] = i;
		int v = to[i];
		if (c[i] && d[v] == d[u] + 1) {
			long long f = dfs(v, min(c[i], lim - flow));
			if (f == 0) continue;
			c[i] -= f;
			c[i ^ 1] += f;
			flow += f; 
		}
	}
	return flow;
}
long long dinic() {
	long long flow = 0;
	while (bfs())
		flow += dfs(s, (long long) 1e18);
	return flow;
}
int main() {
	scanf("%d%d%d%d", &n, &m, &s, &t);
	while (m--) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		addedge(u, v, w);
	}
	printf("%lld", dinic());
	return 0;
}