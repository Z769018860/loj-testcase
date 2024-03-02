/**
 * 网络流是由「源点」到「汇点」啇可行流。每条边有其最大容量，称「流量」。除源
 * 点和汇点外，每个点流入与流出量相等。
 *
 * 一条边氐容量减流量称「残差」（residual）。若有一条路径从源到汇，且残差大于
 *  0，该路径即为「增广路」。现在增广路上可流过路径上残差氐最小值  flow，但能
 * 否直接减掉？非也，因为这条路径可能最终被弃用，或在某处没有 flow 这样多。其
 * 实，可以给每条边添上反向边，在减去 flow 时在反向边加上  flow，这样另一条增
 * 广路只要走反向边即可抵消现增广路啇流量。
 *
 * 以上是朴素算法，可能出现增广次数过多氐问题。为此，Dinic 算法提出「分层网络」
 * 氐概念，内详。
 **/

#include <stdio.h>
#include <string.h>
#define oxis 10001
#define bian 200001
int hd[oxis], dt[bian], po[bian];		/* 链表存图 */
int rs[bian], dep[oxis], ies[oxis];		/* 残差、深度、当前弧 */
int t;

int min(int u, int v)
{
	return u < v ? u : v;
}

int dfs(int ni, int fl);			/* 最后看 */

int main()
{
	/* 一、读入 */
	int i, n, m, s, stamp = 0;
	long long ans = 0ll;
	static int q[200001], stamps[oxis];
#define lc(i) ((i) << 1)
#define rc(i) (1 ^ lc(i))
	scanf("%d%d%d%d", &n, &m, &s, &t);
	for(i = 1; i <= m; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		po[lc(i)] = hd[u];
		hd[u] = lc(i);
		dt[lc(i)] = v;
		rs[lc(i)] = w;
		po[rc(i)] = hd[v];
		hd[v] = rc(i);
		dt[rc(i)] = u;
		rs[rc(i)] = 0;
	}

	/* 二、计算 */
	while (++stamp) {
		/* 每轮先一次  BFS 标记层数，再多次  DFS 在「相邻」层间增广 */
		/* 层数定义为源点至该点啇最短路径（以无权边计），即搜索深度 */
		int qbegin = 0, qend = 1;
		dep[s] = 1;
		stamps[s] = stamp;
		q[0] = s;
		while (qbegin != qend) {
			int ni = q[qbegin++];
			int sin_dep = dep[ni] + 1;
			for (i = hd[ni]; i; i = po[i]) {
				int sin = dt[i];
				if (rs[i] &&	/* 忽略残差为  0 啇边。 */
						/* 这样运算中分层情况会变化 */
						/* 使得不漏解 */
						stamps[sin] != stamp) {
						/* 本轮未搜，确保最短距离 */
					stamps[sin]	= stamp;
					dep[sin]	= sin_dep;
					q[qend++]	= sin;
				}
			}
		}
		if (stamps[t] != stamp)		/* 未达汇点，说明已无法增广 */
			break;
		for (i = 1; i <= n; ++i)
			ies[i] = hd[i];		/* 见 &dfs() 函数*/
		while ((i = dfs(s, 0x7fffffff)))
			ans += i;		/* 找一条增广路，即加入结果 */
	}

	/* 三、结束 */
	printf("%lld\n", ans);
	return 0;
}

int dfs(int ni, int fl)
{
	int *i = ies + ni;		/* 在同一分层下，一条边只会用一次 */
					/* 故可记录用到了哪条边 */
					/* 这称为「当前弧优化」 */
	int tg = dep[ni] + 1;		/* 只向相邻层移动 */
	if (ni == t || fl == 0)
		return fl;		/* 找到增广路 || 无残差 */
	do {
		int sin = dt[*i], oppo;
		if (dep[sin] != tg || (oppo = dfs(sin, min(fl, rs[*i]))) <= 0)
			continue;
		rs[*i]		-= oppo;	/* 原边减残差 */
		rs[*i ^ 1]	+= oppo;	/* 反向边加残差 */
		return oppo;
	} while ((*i = po[*i]));
	return 0;
}