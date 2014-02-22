#include <bits/stdc++.h>
#define MAXW 12
#define MAXH 11
#define P_II pair<int,int>
#define V_PII vector<pair<int,int> >
#define M_P make_pair
using namespace std;
char maze[][MAXH]={
        "**********",
        "*...*....*",
        "*.*...*.S*",
        "*.********",
        "*.*......*",
        "*.*.****.*",
        "*.*.*....*",
        "*.*.*.****",
        "*.*.*.*E.*",
        "*...*....*",
        "**********"};
int g_dir[4][2]={{1,0},{0,1},{0,-1},{-1,0}};
struct Node
{
    P_II pos;
    int g;
    int f;
    P_II parent;
    Node(){parent.first=-1;}
    Node(P_II _pos,int _g,int _f,P_II _parent):pos(_pos),g(_g),f(_f),parent(_parent){}
    bool haveParent()
    {
        return parent.first != -1;
    }
    P_II getDirection()
    {
        return M_P((pos.first-parent.first)/max(abs(pos.first-parent.first),1),
                         (pos.second-parent.second)/max(abs(pos.second-parent.second),1));
    }
};
int g_f[MAXW][MAXH];
bool operator < (const Node &a, const Node &b)
{
    return a.f < b.f;
}
class Maze
{
private:
    bool inque[MAXW][MAXH];
    bool vis[MAXW][MAXH];
public:
    void clearQue()
    {
        memset(inque,false,sizeof(inque));
    }
    void pushQue(P_II t)
    {
        inque[t.first][t.second]=true;
    }
    void popQue(P_II t)
    {
        inque[t.first][t.second]=false;
    }
    bool isInQue(P_II t)
    {
        return inque[t.first][t.second];
    }
    void clearVis()
    {
        memset(vis,false,sizeof(vis));
    }
    void pushVis(P_II t)
    {
        vis[t.first][t.second]=true;
    }
    void popVis(P_II t)
    {
        vis[t.first][t.second]=false;
    }
    bool isInVis(P_II t)
    {
        return vis[t.first][t.second];
    }

    bool checkMaze(int x,int y)
    {
        if (x < 0 || x > MAXW || y < 0 || y > MAXH)
            return false;
        return maze[x][y] != '*';
    }
};
class NodeList
{
private:
    vector<Node> l;
public:
    void clear()
    {
        l.clear();
    }
    void push(Node t)
    {
        l.push_back(t);
        sort(l.begin(),l.end());
    }
    void pop()
    {
        vector<Node> ::iterator k=l.begin();
        l.erase(k);
    }
    Node top()
    {
        return l[0];
    }
    void update(Node t)
    {
        for (int i=0; i<l.size(); i++)
        {
            if (l[i].pos.first == t.pos.first &&
                l[i].pos.second == t.pos.second)
            {
                l[i].f=min(l[i].f,t.f);
            }
        }
    }
    bool empty()
    {
        return l.size() == 0;
    }
};
class PathFinder
{
private:
    NodeList pri_List;
    P_II sta,end;
    Maze mazer;
    int mem_g[MAXW][MAXH];
    P_II path[MAXW][MAXH];
public:
    void getStartPos()
    {
        int i,j;
        for (i=0; i<MAXH; i++)
            for (j=0; j<MAXW; j++)
                if (maze[i][j] == 'S')
                    sta=M_P(i,j);
    }
    void getEndPos()
    {
        int i,j;
        for (i=0; i<MAXH; i++)
            for (j=0; j<MAXW; j++)
                if (maze[i][j] == 'E')
                    end=M_P(i,j);
    }
    void pqClear()
    {
        pri_List.clear();
    }
    void init()
    {
        pqClear();
        getStartPos();
        getEndPos();
        mazer.clearQue();
        mazer.clearVis();
        memset(mem_g,0x3f,sizeof(mem_g));
    }

    V_PII neighbourPrune(Node c)
    {
        V_PII t;
        int i;
        P_II dir;
        t.clear();
        if (c.haveParent())
        {
            dir=c.getDirection();
            if (dir.first != 0 && dir.second != 0)
            {
                if (mazer.checkMaze(c.pos.first,c.pos.second+dir.second))
                        t.push_back(M_P(c.pos.first,c.pos.second+dir.second));

                if (mazer.checkMaze(c.pos.first+dir.first,c.pos.second))
                        t.push_back(M_P(c.pos.first+dir.first,c.pos.second));

                if (mazer.checkMaze(c.pos.first,c.pos.second+dir.second)
                ||  mazer.checkMaze(c.pos.first+dir.first,c.pos.second))
                        t.push_back(M_P(c.pos.first+dir.first,c.pos.second+dir.second));

                if (!mazer.checkMaze(c.pos.first-dir.first,c.pos.second)
                &&  mazer.checkMaze(c.pos.first,c.pos.second+dir.second))
                        t.push_back(M_P(c.pos.first-dir.first,c.pos.second+dir.second));

                if (!mazer.checkMaze(c.pos.first,c.pos.second-dir.second)
                &&  mazer.checkMaze(c.pos.first+dir.first,c.pos.second))
                        t.push_back(M_P(c.pos.first+dir.first,c.pos.second-dir.second));
            }
            else
            {
                if (dir.first == 0)
                {
                    if (mazer.checkMaze(c.pos.first,c.pos.second+dir.second))
                    {
                        if (mazer.checkMaze(c.pos.first,c.pos.second+dir.second))
                                t.push_back(M_P(c.pos.first,c.pos.second+dir.second));

                        if (!mazer.checkMaze(c.pos.first+1,c.pos.second))
                                t.push_back(M_P(c.pos.first+1,c.pos.second+dir.second));

                        if (!mazer.checkMaze(c.pos.first-1,c.pos.second))
                                t.push_back(M_P(c.pos.first-1,c.pos.second+dir.second));
                    }
                }
                else
                {
                    if (mazer.checkMaze(c.pos.first+dir.first,c.pos.second))
                    {
                        if (mazer.checkMaze(c.pos.first+dir.first,c.pos.second))
                                t.push_back(M_P(c.pos.first+dir.first,c.pos.second));

                        if (!mazer.checkMaze(c.pos.first,c.pos.second+1))
                                t.push_back(M_P(c.pos.first+dir.first,c.pos.second+1));

                        if (!mazer.checkMaze(c.pos.first,c.pos.second-1))
                                t.push_back(M_P(c.pos.first+dir.first,c.pos.second-1));
                    }
                }
            }
        }
        else
        {
            for (i=0; i<4; i++)
            {
                if (mazer.checkMaze(c.pos.first+g_dir[i][0],c.pos.second+g_dir[i][1]))
                        t.push_back(M_P(c.pos.first+g_dir[i][0],c.pos.second+g_dir[i][1]));
            }

            if (mazer.checkMaze(c.pos.first+1,c.pos.second) || mazer.checkMaze(c.pos.first,c.pos.second+1))
            {
                if (mazer.checkMaze(c.pos.first+1,c.pos.second+1))
                    t.push_back(M_P(c.pos.first+1,c.pos.second+1));
            }

            if (mazer.checkMaze(c.pos.first+1,c.pos.second) || mazer.checkMaze(c.pos.first,c.pos.second-1))
            {
                if (mazer.checkMaze(c.pos.first+1,c.pos.second-1))
                {

                    t.push_back(M_P(c.pos.first+1,c.pos.second-1));
                }
            }
            if (mazer.checkMaze(c.pos.first-1,c.pos.second) || mazer.checkMaze(c.pos.first,c.pos.second+1))
            {
                if (mazer.checkMaze(c.pos.first-1,c.pos.second+1))
                    t.push_back(M_P(c.pos.first-1,c.pos.second+1));
            }
            if (mazer.checkMaze(c.pos.first-1,c.pos.second) || mazer.checkMaze(c.pos.first,c.pos.second-1))
            {
                if (mazer.checkMaze(c.pos.first-1,c.pos.second-1))
                    t.push_back(M_P(c.pos.first-1,c.pos.second-1));
            }
        }
        for (i=0; i<t.size(); i++)
        {
            printf("%d %d\n",t[i].first,t[i].second);
        }
        return t;
    }
    P_II checkJump(P_II now,P_II pre)
    {
        P_II dir=M_P(now.first-pre.first,now.second-pre.second);
        P_II tmp=M_P(-1,-1);
        if (mazer.checkMaze(now.first,now.second) == false)
            return tmp;
        if (now == end)
            return now;
        if (dir.first != 0 && dir.second != 0)
        {
            if ((mazer.checkMaze(now.first-dir.first,now.second+dir.second)
             && !mazer.checkMaze(now.first-dir.first,now.second))
            ||  (mazer.checkMaze(now.first+dir.first,now.second-dir.second)
             && !mazer.checkMaze(now.first,now.second-dir.second)))
                return now;
        }
        else
        {
            if (dir.first != 0)
            {
                if ((mazer.checkMaze(now.first+dir.first,now.second+1)
                 && !mazer.checkMaze(now.first,now.second+1))
                ||  (mazer.checkMaze(now.first+dir.first,now.second-1)
                 && !mazer.checkMaze(now.first,now.second-1)))
                    return now;
            }
            else
            {
                if ((mazer.checkMaze(now.first+1,now.second+dir.second)
                 && !mazer.checkMaze(now.first+1,now.second))
                ||  (mazer.checkMaze(now.first-1,now.second+dir.second)
                 && !mazer.checkMaze(now.first-1,now.second)))
                    return now;
            }
        }
        if (dir.first != 0 && dir.second != 0)
        {
            tmp=checkJump(M_P(now.first+dir.first,now.second),now);
            P_II t2=checkJump(M_P(now.first,now.second+dir.second),now);
            if (tmp.first != -1 ||  t2.first != -1)
                return now;
        }
        if (mazer.checkMaze(now.first+dir.first,now.second)
        ||  mazer.checkMaze(now.first,now.second+dir.second))
        {
            tmp=checkJump(M_P(now.first+dir.first,now.second+dir.second),now);
            if (tmp.first != -1)
                return tmp;
        }
        return tmp;
    }
    void expandSuccessors(Node c)
    {
        int i,d,g,h;
        Node t;
        P_II jp;
        V_PII nbs=neighbourPrune(c);
        for (i=0; i<nbs.size(); i++)
        {
            jp=checkJump(nbs[i],c.pos);
            if (jp.first != -1)
            {
                if (mazer.isInVis(jp) == true)
                    continue;
                d=abs(jp.first-c.pos.first)+abs(jp.second-c.pos.second);
                g=c.g+d;
                if (mazer.isInQue(jp) == false || g < mem_g[jp.first][jp.second])
                {
                    mem_g[jp.first][jp.second]=g;
                    t.g=g;
                    h=abs(jp.first-end.first)+abs(jp.second-end.second);
                    t.f=t.g+h;
                    t.pos=jp;
                   // printf("%d %d %d %d %d %d\n",t.pos.first,t.pos.second,t.parent.first,t.parent.second,c.pos.first,c.pos.second);
                    t.parent=c.pos;


                    if (mazer.isInQue(jp) == false)
                    {
                        path[jp.first][jp.second]=c.pos;
                        pri_List.push(t);
                        mazer.pushQue(t.pos);
                    }
                    else
                    {
                        pri_List.update(t);
                    }
                }

            }
        }
    }
    void backtrace(int x,int y)
    {
        if (x != sta.first && y != sta.second)
        {
            printf("%d %d\n",x,y);
            backtrace(path[x][y].first,path[x][y].second);
        }
        else
        {
            printf("%d %d\n",x,y);
            return ;
        }
    }
    int findPath()
    {
        init();
        Node s=Node(sta,0,0,make_pair(-1,-1)),tmp;
        pri_List.push(s);
        mazer.pushQue(s.pos);
        while (!pri_List.empty())
        {
            tmp=pri_List.top();
            printf("%d %d %d %d %d\n",tmp.pos.first,tmp.pos.second,tmp.parent.first,tmp.parent.second,tmp.f);
            mazer.pushVis(tmp.pos);
            pri_List.pop();
            if (tmp.pos == end)
            {
                backtrace(end.first,end.second);
                return 1;
            }
            expandSuccessors(tmp);
        }
        return 0;
    }
};
int main()
{
    PathFinder pf;
    cout<<pf.findPath()<<endl;
}
