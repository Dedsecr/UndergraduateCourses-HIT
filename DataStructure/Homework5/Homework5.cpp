#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;
class AVL
{
private:
    //树节点的存储结构
    struct Node
    {
        int Data, Size, Height;
        Node *LCh, *RCh;
        Node()
        {
            LCh = RCh = NULL;
            Size = Height = 0;
        }
    };
    typedef Node * NodeP;
    //获取树的高度
    int GetHeight(NodeP x)
    {
        if(x == NULL)
            return 0;
        else
            return x->Height;
    }
    //获取树的大小
    int GetSize(NodeP x)
    {
        if(x == NULL)
            return 0;
        else
            return x->Size;
    }
    //更新大小和高度
    void UpdateHS(NodeP x)
    {
        if(x == NULL)
            return;
        x->Height = max(GetHeight(x->LCh), GetHeight(x->RCh)) + 1;
        x->Size = GetSize(x->LCh) + GetSize(x->RCh) + 1;
    }
    //LL旋转
    NodeP LL(NodeP x)
    {
        NodeP Tmp = x->LCh->RCh, Res = x->LCh;
        Res->RCh = x;
        x->LCh = Tmp;

        UpdateHS(x->RCh);
        UpdateHS(x);

        return Res;
    }
    //RR旋转
    NodeP RR(NodeP x)
    {
        NodeP Tmp = x->RCh->LCh, Res = x->RCh;
        Res->LCh = x;
        x->RCh = Tmp;

        UpdateHS(x->LCh);
        UpdateHS(x);

        return Res;
    }
    //LR旋转
    NodeP LR(NodeP x)
    {
        x->LCh = RR(x->LCh);
        return LL(x);
    }
    //RL旋转
    NodeP RL(NodeP x)
    {
        x->RCh = LL(x->RCh);
        return RR(x);
    }
    //保持平衡
    void Maintain(NodeP &x)
    {
        if(GetHeight(x->LCh) == GetHeight(x->RCh) + 2)
        {
            if(GetHeight(x->LCh->LCh) > GetHeight(x->LCh->RCh))
                x = LL(x), cout << "LL\n";
            else
                x = LR(x), cout << "LR\n";
        }
        else if(GetHeight(x->RCh) == GetHeight(x->LCh) + 2)
        {
            if(GetHeight(x->RCh->RCh) > GetHeight(x->RCh->LCh))
                x = RR(x), cout << "RR\n";
            else
                x = RL(x), cout << "RL\n";
        }
        UpdateHS(x);
    }
public:
    
    NodeP Root;
    //插入元素
    void Insert(NodeP &x, int Data)
    {
        if(x == NULL)
        {
            x = new Node;
            x->Data = Data;
            x->Size = 1;
            x->Height = 1;
            return;
        }
        x->Size++;
        if(x->Data > Data) Insert(x->LCh, Data);
        else if(x->Data < Data)
            Insert(x->RCh, Data);
        Maintain(x);
    }
    //删除元素
    int Delete(NodeP &x, int Data)
    {
        x->Size--;
        if(x->Data == Data || (Data < x->Data && x->LCh == NULL) || (Data > x->Data && x->RCh == NULL))
        {
            int Res = x->Data;
            if(x->LCh == NULL) x = x->RCh;
            else if(x->RCh == NULL) x = x->LCh;
            else x->Data = Delete(x->LCh, Data + 1);
            UpdateHS(x);
            return Res;
        }
        int Res;
        if(x->Data > Data) Res = Delete(x->LCh, Data);
        else Res = Delete(x->RCh, Data);
        Maintain(x);
        return Res;
    }
    //获取元素的次序
    int Rank(NodeP x, int Data)
    {
        if(x == NULL)
            return -1e9;
        if(x->Data == Data)
            return 1 + GetSize(x->LCh);
        else if(x->Data < Data)
            return GetSize(x->LCh) + 1 + Rank(x->RCh, Data);
        else
            return Rank(x->LCh, Data);
    }
    void GetSortedDFS(NodeP x, string &Res)
    {
        if(x == NULL)
            return;
        GetSortedDFS(x->LCh, Res);
        Res += to_string(x->Data);
        Res += " ";
        GetSortedDFS(x->RCh, Res);
    }
    //输出排好序的序列
    string GetSorted()
    {
        string Res;
        GetSortedDFS(Root, Res);
        return Res;
    }
};
AVL Tree;
int main()
{
    freopen("Homework5_In.txt", "r", stdin);
    freopen("Homework5_Out.txt", "w", stdout);
    printf("************************\n"
           " 0. Quit                \n"
           " 1. Insert              \n"
           " 2. Delete              \n"
           " 3. Get Rank            \n"
           " 4. Get Sorted Sequence \n"
           "************************\n");
    while(1)
    {
        int OP;
        cin >> OP;
        if(OP == 0)
            break;
        else if(OP == 1)
        {
            int x;
            cin >> x;
            Tree.Insert(Tree.Root, x);
        }
        else if(OP == 2)
        {
            int x;
            cin >> x;
            Tree.Delete(Tree.Root, x);
        }
        else if(OP == 3)
        {
            int x;
            cin >> x;
            cout << Tree.Rank(Tree.Root, x) << '\n';
        }
        else
        {
            cout << Tree.GetSorted() << '\n';
        }
    }
    return 0;
}