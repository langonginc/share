#include <cstdio>
#include <algorithm>

using namespace std;
const int MAXN = 10005;
const int MAXM = 50005;
int n, m;

struct Edge {
    int u, v, w;

    bool operator<(const Edge &other) const {
        return w > other.w;
    }
};

struct Edge2 {
    int v, w, next;
};
Edge ori[MAXM];
Edge2 pool[MAXM];
int head[MAXN];
int nn;

void addEdge(int from, int to, int w) {
    nn++;
    pool[nn].v = to;
    pool[nn].w = w;
    pool[nn].next = head[from];
    head[from] = nn;
};
int ufs[MAXN];

int find(int x) {
    if (ufs[x] == x) return x;
    return ufs[x] = find(ufs[x]);
}

void join(int x, int y) {
    ufs[find(x)] = find(y);
}

int ancestor[MAXN][16];
int capacity[MAXN][16];
int depth[MAXN];

void dfs(int u, int father, int w) {
    depth[u] = depth[father] + 1;
    ancestor[u][0] = father;
    capacity[u][0] = w;
    for (int i = 1; (1 << i) <= depth[u]; ++i) {
        ancestor[u][i] = ancestor[ancestor[u][i - 1]][i - 1];
        capacity[u][i] = min(capacity[u][i - 1], capacity[ancestor[u][i - 1]][i - 1]);
    }
    for (int i = head[u]; i; i = pool[i].next) {
        int v = pool[i].v;
        if (v != father) {
            dfs(v, u, pool[i].w);
        }
    }
}

int lca(int x, int y) {
    if (depth[x] > depth[y]) {
        swap(x, y);
    }
    int cx = MAXM;
    int cy = MAXM;
    for (int i = 15; i >= 0; --i) {
        if (depth[ancestor[y][i]] >= depth[x]) {
            cy = min(cy, capacity[y][i]);
            y = ancestor[y][i];
        }
    }
    if (x == y) return cy;
    for (int i = 15; i >= 0; --i) {
        if (ancestor[x][i] != ancestor[y][i]) {
            cx = min(cx, capacity[x][i]);
            cy = min(cy, capacity[y][i]);
            x = ancestor[x][i];
            y = ancestor[y][i];
        }
    }
    cx = min(cx, capacity[x][0]);
    cy = min(cy, capacity[y][0]);
    return min(cx, cy);
}

int main() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d%d%d", &ori[i].u, &ori[i].v, &ori[i].w);
    }
    sort(ori, ori + m);
    for (int i = 1; i <= n; ++i) {
        ufs[i] = i;
    }
    for (int i = 0; i < m; ++i) {
        if (find(ori[i].u) != find(ori[i].v)) {
            join(ori[i].u, ori[i].v);
            addEdge(ori[i].u, ori[i].v, ori[i].w);
            addEdge(ori[i].v, ori[i].u, ori[i].w);
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (depth[i] == 0) {
            dfs(i, 0, MAXM);
        }
    }
    int q;
    scanf("%d", &q);
    for (int i = 0; i < q; ++i) {
        int x, y;
        scanf("%d%d", &x, &y);
        if (find(x) != find(y)) {
            printf("-1\n");
        } else {
            printf("%d\n", lca(x, y));
        }
    }
    return 0;
}
