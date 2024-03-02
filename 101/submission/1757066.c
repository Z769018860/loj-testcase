#include <stdio.h>
#include <stdlib.h>

int head[100000];
int to[100000];
int next[100000];
int c[100000];
int reg[100000];
int from[100000];
int rev[100000];
int vis[100000];
int cnt = 1;
int n, m, s, t;

void add(int a, int b, int cc, int r)
{
    if (head[a] == 0)
    {
        head[a] = cnt;
    }
    else
    {
        int tt = head[a];
        head[a] = cnt;
        next[cnt] = tt;
    }
    to[cnt] = b;
    c[cnt] = cc;
    rev[cnt] = r;
    cnt++;
}

void add2(int a, int b, int flow)
{
    add(a, b, flow, cnt + 1);
    add(b, a, 0, cnt - 1);
}

int quene[100000];
int min(int aa, int bb)
{
    return aa < bb ? aa : bb;
}

long long get()
{
    int flow = -1;
    // c[s] = 1e9;
    long long res = 0;
    while (flow != 0)
    {
        // printf("getting %d -> %d\n", s, t);
        if (s == 0)
            exit(0);
        int front = 0;
        int tail = 0;
        quene[tail++] = s;
        for (int i = 1; i <= n; i++)
        {
            vis[i] = 0;
            reg[i] = 0;
        }
        reg[s] = 1e9;
        while (front != tail)
        {
            int now = quene[front++];
            if (now == t)
                break;
            int vol = reg[now];
            vis[now] = 1;
            // printf("bfs vis: %d, vol: %d\n", now, vol);
            if (vol != 0)
                for (int i = head[now]; i; i = next[i])
                {
                    int v = to[i];
                    if (!vis[v] && c[i] != 0)
                    {
                        int give = min(vol, c[i]);
                        quene[tail++] = v;
                        vis[v] = 1;
                        reg[v] = give;
                        // printf("registered %d with :%d\n", v, give);
                        from[v] = i;
                    }
                    else
                    {
                        // printf("skipped:%d bc %d %d\n", v, vis[v], c[i]);
                    }
                }
        }
        flow = reg[t];
        if (flow != 0)
        {
            for (int i = rev[from[t]];; i = rev[from[to[i]]])
            {
                // printf("back track to %d -> %d\n", to[i], to[rev[i]]);
                c[i] += flow;
                c[rev[i]] -= flow;
                if (to[i] == s)
                {
                    break;
                }
            }
            res += flow;
        }
    }
    return res;
}

int main()
{
    scanf("%d%d%d%d", &n, &m, &s, &t);
    for (int i = 0; i < m; i++)
    {
        int aa, bb, cc;
        scanf("%d%d%d", &aa, &bb, &cc);
        add2(aa, bb, cc);
    }
    printf("%lld\n", get());
    return 0;
}