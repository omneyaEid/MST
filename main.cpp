#include <bits/stdc++.h>
using namespace std;
struct Edge{
    int x, y, cost;
    Edge(int X, int Y, int Cost){
        x = X;
        y = Y;
        cost = Cost;
    }
    bool operator < (const Edge b) const{return cost != b.cost ? cost < b.cost : (x < b.x || (x == b.x && y < b.y));}
};
struct DSU{
    vector<int> p, r;
    DSU(int size){
        r.assign(size, 1);
        p.resize(size);
        for (int i = 0; i < size; i++)
            p[i] = i;
    }
    DSU() {}
    int get(int x){ return p[x] == x ? x : (p[x] = get(p[x])); }
    bool unite(int x, int y){
        x = get(x);
        y = get(y);
        if (x == y)
            return false;
        if (r[x] < r[y])
            swap(x,y);
        p[y] = x;
        r[x] += r[y];
        return true;
    }
};
vector<pair<Edge,int> > edges;
vector<string> answer;
DSU dsu;
vector<vector<int> > g;
vector<int> u, d, l;
int n, m, globalCounter;
void dfs(int v, int p = -1, int depth = 0){
    l[v] = d[v] = depth;
    u[v] = globalCounter+1;
    for (int i = 0; i < (int)g[v].size(); i++){
        if (g[v][i] == p)continue;
        int id = g[v][i], x = dsu.get(edges[id].first.x), y = dsu.get(edges[id].first.y), to = x == v ? y : x;
        if (u[to]>globalCounter)
            l[v] = min(l[v], d[to]);
        else{
            dfs(to, g[v][i], depth+1);
            l[v] = min(l[v], l[to]);
        }
        if (l[to] > d[v]){
            answer[edges[g[v][i]].second] = "any";
        }
    }
}
void solve(){
    cin >> n >> m;
    d.resize(n);
    l.resize(n);
    u.resize(n);
    g.assign(n, vector<int> ());
    answer.assign(m, "at least one");
    dsu = DSU(n);
    for (int i = 0; i < m; i++){
        int x, y, z;
        cin >> x >> y >> z;
        x--;
        y--;
        edges.push_back(make_pair(Edge(x,y,z),i));
    }
    sort(edges.begin(), edges.end());
    for (int i = 0; i < m;){
        int k = i;
        while (k < (int)edges.size() && edges[k].first.cost == edges[i].first.cost)
            k++;
        for (int j = i; j < k; j++){
            int x, y;
            x = dsu.get(edges[j].first.x);
            y = dsu.get(edges[j].first.y);
            if (x != y){
                g[x].push_back(j);
                g[y].push_back(j);
            }
            else
                answer[edges[j].second] = "none";
        }
        for (int j = i; j < k; j++){
            int x, y;
            x = dsu.get(edges[j].first.x);
            y = dsu.get(edges[j].first.y);
            if (u[x]<=globalCounter)
                dfs(x);
            if (u[y]<=globalCounter)
                dfs(y);
        }
        for (int j = i; j < k; j++){
            int x, y;
            x = dsu.get(edges[j].first.x);
            y = dsu.get(edges[j].first.y);
            dsu.unite(x, y);
            g[x].clear();
            g[y].clear();
        }
        i = k;
        globalCounter++;
    }
    for (int i = 0; i < m; i++)
        cout << answer[i] << "\n";
}
int main(){
    solve();
    return 0;
}
