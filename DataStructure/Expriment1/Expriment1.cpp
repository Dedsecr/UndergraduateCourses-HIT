#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <cmath>
using namespace std;
const int MAXN = 100;
//分数
struct Fraction
{
    int numerator, denominator;
    Fraction(){}
    Fraction(int _n, int _d)
    {
        numerator = _n;
        denominator = _d;
        Simplification();
    }
    Fraction(int _n)
    {
        numerator = _n;
        denominator = 1;
    }
    //求最大公因数
    int gcd(int x, int y)
    {
        if (x == 0)
            return y;
        return gcd(y % x, x);
    }
    //分数化简
    void Simplification()
    {
        int _ = gcd(abs(numerator), abs(denominator));
        numerator /= _;
        denominator /= _;
        if(numerator < 0 && denominator < 0)
            numerator = -numerator, denominator = -denominator;
    }
    bool operator < (const Fraction _) const
    {
        return 1.0 * numerator / denominator < 1.0 * _.numerator / _.denominator;
    }
    bool operator > (const Fraction _) const
    {
        return 1.0 * numerator / denominator > 1.0 * _.numerator / _.denominator;
    }
    Fraction operator * (const Fraction _) const
    {
        Fraction Res;
        Res.numerator = numerator * _.numerator;
        Res.denominator = denominator * _.denominator;
        Res.Simplification();
        return Res;
    }
    Fraction operator / (const Fraction _) const
    {
        Fraction Res;
        Res.numerator = numerator * _.denominator;
        Res.denominator = denominator * _.numerator;
        Res.Simplification();
        return Res; 
    }
    Fraction operator + (const Fraction _) const
    {
        Fraction Res;
        Res.numerator = numerator * _.denominator + denominator * _.numerator;
        Res.denominator = denominator * _.denominator;
        Res.Simplification();
        return Res;
    }
    Fraction operator - (const Fraction _) const
    {
        Fraction Res;
        Res.numerator = numerator * _.denominator - denominator * _.numerator;
        Res.denominator = denominator * _.denominator;
        Res.Simplification();
        return Res;
    }
    //分数转double
    double ToDouble()
    {
        return 1.0 * numerator / denominator;
    }
};
//多项式项
struct EleNum
{
    Fraction coef;
    int expo;
    EleNum(){}
    EleNum(Fraction _c, int _e)
    {
        coef = _c, expo = _e;
    }
    EleNum operator + (const EleNum _) const
    {
        if(expo != _.expo && expo)
        {
            cerr << "Error in " << __LINE__;
            exit(-1);
        }
        EleNum Res;
        Res.coef = coef + _.coef;
        Res.expo = _.expo;
        return Res;
    }
    EleNum operator - (const EleNum _) const
    {
        if(expo != _.expo && expo)
        {
            cerr << "Error in " << __LINE__;
            exit(-1);
        }
        EleNum Res;
        Res.coef = coef - _.coef;
        Res.expo = _.expo;
        return Res;
    }
    EleNum operator * (const EleNum _) const
    {
        EleNum Res;
        Res.coef = coef * _.coef;
        Res.expo = expo + _.expo;
        return Res;
    }
};
//多项式中的项
struct Ele
{
    EleNum Elements;
    int Next;
    Ele() {}
    Ele(EleNum _E, int _N)
    {
        Elements = _E, Next = _N;
    }
};
//多项式
struct Polyn
{
    Ele Elements[MAXN];
    int Available[MAXN];
    int Length, AvailableP, LastP;
    Polyn() 
    {
        LastP = 0;
        Length = 0;
        for (AvailableP = 1; AvailableP < MAXN; ++AvailableP)
            Available[AvailableP] = AvailableP;
        AvailableP--;
    }
    //获取新位置
    int GetNewPos()
    {
        int Res = Available[AvailableP--];
        Elements[Res].Next = 0;
        return Res;
    }
    //插入新项
    void Insert(EleNum element)
    {
        int Pos = GetNewPos();
        Length++;
        Elements[LastP].Next = Pos;
        Elements[Pos].Elements = element;
        Elements[Pos].Next = 0;
        LastP = Pos;
    }
    //回收空间操作
    void Recovery(int Pos)
    {
        Available[++AvailableP] = Pos;
        Elements[Pos].Next = 0;
    }
};
EleNum Tmp[MAXN * MAXN], EleNum_Zero = EleNum(Fraction(0, 1), 0);
//判断是否是数字
bool IsDigit(char x)
{
    return x >= '0' && x <= '9';
}
//判断系数是否合法
bool CoefIsLigal(string x)
{
    int Len = x.length();
    for (int i = 0; i < Len; ++i)
    {
        if ((!IsDigit(x[i]) && x[i] != '/' && x[i] != '-') || (x[i] == '/' && (i == 0 || i == Len - 1)))
            return 0;
    }
    return 1;
}
//判断指数是否合法
bool ExpoIsLigal(string x)
{
    int Len = x.length();
    for (int i = 0; i < Len; ++i)
    {
        if (!IsDigit(x[i]))
            return 0;
    }
    return 1;
}
//判断是否含有分数
int IsFrac(string x)
{
    int Len = x.length();
    for (int i = 0; i < Len; ++i)
    {
        if (x[i] == '/')
            return i + 1;
    }
    return 0;
}
//输入项
bool GetElements(Polyn &polyn)
{
    string input1, input2;
    int input1L, input2L, FracP;
    int numerator, denominator;
    cin >> input1 >> input2;
    if (!CoefIsLigal(input1) || !ExpoIsLigal(input2))
        return 0;
    input1L = input1.length();
    input2L = input2.length();

    FracP = IsFrac(input1);
    if (FracP)
    {
        FracP--;
        int x = 0;
        if(input1[0] == '-')
        {
            for (int i = 1; i < FracP; ++i)
            {
                x = x * 10 + input1[i] - '0';
            }
            x = -x;
        }
        else
        {
            for (int i = 0; i < FracP; ++i)
            {
                x = x * 10 + input1[i] - '0';
            }
        }
        numerator = x;
        x = 0;
        for (int i = FracP + 1; i < input1L; ++i)
        {
            x = x * 10 + input1[i] - '0';
        }
        denominator = x;
    }
    else
    {
        int x = 0;
        if(input1[0] == '-')
        {
            for (int i = 1; i < input1L; ++i)
            {
                x = x * 10 + input1[i] - '0';
            }
            x = -x;
        }
        else
        {
            for (int i = 0; i < input1L; ++i)
            {
                x = x * 10 + input1[i] - '0';
            }
        }
        numerator = x;
        denominator = 1;
    }

    int x = 0;
    for (int i = 0; i < input2L; ++i)
    {
        x = x * 10 + input2[i] - '0';
    }
    polyn.Insert(EleNum(Fraction(numerator, denominator), x));

    return 1;
}
bool Cmp(const EleNum &a, const EleNum &b)
{
    return a.expo > b.expo;
}
//对多项式排序
void Sort(Polyn &polyn)
{
    int Length = polyn.Length;
    int P = 0;
    for (int i = 1; i <= Length; ++i)
    {
        P = polyn.Elements[P].Next;
        Tmp[i] = polyn.Elements[P].Elements;
    }
    sort(Tmp + 1, Tmp + 1 + Length, Cmp);
    P = 0;
    for (int i = 1; i <= Length; ++i)
    {
        P = polyn.Elements[P].Next;
        polyn.Elements[P].Elements = Tmp[i];
    }
}
//输入多项式
Polyn Input(int Length)
{
    Polyn Res;
    for (int i = 1; i <= Length; ++i)
    {
        while (!GetElements(Res))
        {
            puts("Wrong format!");
        }
        Sort(Res);
    }
    return Res;
}
//打印多项式
void Print(Polyn polyn)
{
    bool First = 1;
    for (int Pos = polyn.Elements[0].Next; Pos; Pos = polyn.Elements[Pos].Next)
    {
        if(First)
            First = 0;
        else
            cout << " + ";
        printf("(%d/%d)*x^(%d)", polyn.Elements[Pos].Elements.coef.numerator,
               polyn.Elements[Pos].Elements.coef.denominator,
               polyn.Elements[Pos].Elements.expo);
    }
    puts("");
}
//多项式求和
Polyn Add(Polyn &A, Polyn &B)
{
    Polyn Res;
    int P_A = A.Elements[0].Next, P_B = B.Elements[0].Next;
    while(P_A && P_B)
    {
        if(A.Elements[P_A].Elements.expo > B.Elements[P_B].Elements.expo)
        {
            Res.Insert(A.Elements[P_A].Elements);
            P_A = A.Elements[P_A].Next;
        }
        else if(A.Elements[P_A].Elements.expo < B.Elements[P_B].Elements.expo)
        {
            Res.Insert(B.Elements[P_B].Elements);
            P_B = B.Elements[P_B].Next;
        }
        else
        {
            Res.Insert(A.Elements[P_A].Elements + B.Elements[P_B].Elements);
            P_A = A.Elements[P_A].Next;
            P_B = B.Elements[P_B].Next;
        }
    }
    while(P_A)
    {
        Res.Insert(A.Elements[P_A].Elements);
        P_A = A.Elements[P_A].Next;
    }
    while(P_B)
    {
        Res.Insert(B.Elements[P_B].Elements);
        P_B = B.Elements[P_B].Next;
    }
    //cout << Res.Length << "^^^^^^^^^\n";
    return Res;
}
//多项式相减
Polyn Minus(Polyn &A, Polyn &B)
{
    Polyn Res;
    int P_A = A.Elements[0].Next, P_B = B.Elements[0].Next;
    while(P_A && P_B)
    {
        if(A.Elements[P_A].Elements.expo > B.Elements[P_B].Elements.expo)
        {
            Res.Insert(A.Elements[P_A].Elements);
            P_A = A.Elements[P_A].Next;
        }
        else if(A.Elements[P_A].Elements.expo < B.Elements[P_B].Elements.expo)
        {
            Res.Insert(EleNum_Zero - B.Elements[P_B].Elements);
            P_B = B.Elements[P_B].Next;
        }
        else
        {
            Res.Insert(A.Elements[P_A].Elements - B.Elements[P_B].Elements);
            P_A = A.Elements[P_A].Next;
            P_B = B.Elements[P_B].Next;
        }
    }
    while(P_A)
    {
        Res.Insert(A.Elements[P_A].Elements);
        P_A = A.Elements[P_A].Next;
    }
    while(P_B)
    {
        Res.Insert(EleNum_Zero - B.Elements[P_B].Elements);
        P_B = B.Elements[P_B].Next;
    }
    return Res;
}
//多项式相乘
Polyn Multiply(Polyn &A, Polyn &B)
{
    Polyn Res;
    int TmpP = 0;
    for (int P_A = A.Elements[0].Next; P_A; P_A = A.Elements[P_A].Next)
        for (int P_B = B.Elements[0].Next; P_B; P_B = B.Elements[P_B].Next)
            Tmp[++TmpP] = A.Elements[P_A].Elements * B.Elements[P_B].Elements;
    sort(Tmp + 1, Tmp + 1 + TmpP, Cmp);
    Tmp[0].expo = Tmp[1].expo + 1;
    for (int i = 1; i <= TmpP; ++i)
    {
        if(Tmp[i].expo != Tmp[i-1].expo)
            Res.Insert(Tmp[i]);
        else
            Res.Elements[Res.LastP].Elements = Res.Elements[Res.LastP].Elements + Tmp[i];
    }
    return Res;
}
//获取当前的商
Polyn Get_This_Quotient(EleNum A, EleNum B)
{
    EleNum Now = EleNum(A.coef / B.coef, A.expo - B.expo);
    Polyn Res;
    Res.Insert(Now);
    return Res;
}
//去掉系数为0的项
void Refresh(Polyn &A)
{
    int LasP = 0;
    int Pos = A.Elements[0].Next;
    while(Pos)
    {
        if(A.Elements[Pos].Elements.coef.numerator == 0)
        {
            int Tmp = Pos;
            A.Elements[LasP].Next = A.Elements[Pos].Next;
            Pos = A.Elements[Pos].Next;
            A.Recovery(Tmp);
        }
        else
            LasP = Pos, Pos = A.Elements[Pos].Next;
    }

}
//多项式相除
pair<Polyn, Polyn> Divide(Polyn A, Polyn &B)
{
    pair<Polyn, Polyn> Res;
    Polyn Quotient;
    while(A.Elements[A.Elements[0].Next].Elements.expo >= B.Elements[B.Elements[0].Next].Elements.expo)
    {
        Polyn This_Quotient = Get_This_Quotient(A.Elements[A.Elements[0].Next].Elements, B.Elements[B.Elements[0].Next].Elements);
        Polyn tmp = Multiply(B, This_Quotient);
        Quotient = Add(Quotient, This_Quotient);
        A = Minus(A, tmp);
        Refresh(A);
    }
    Res.first = Quotient;
    Res.second = A;
    return Res;
}
//多项式求导
Polyn Get_Derivative_Function(Polyn &A, int k)
{
    Polyn Res = A;
    for (int Pos = Res.Elements[0].Next; Pos; Pos = Res.Elements[Pos].Next)
    {
        for (int j = 1; j <= k; ++j)
        {
            Res.Elements[Pos].Elements.coef = Res.Elements[Pos].Elements.coef * Fraction(Res.Elements[Pos].Elements.expo, 1);
            Res.Elements[Pos].Elements.expo--;
        }
    }
    Refresh(Res);
    return Res;
}
//多项式求值
double Get_Result(Polyn &A, double x)
{
    double Res = 0;
    for (int Pos = A.Elements[0].Next; Pos; Pos = A.Elements[Pos].Next)
    {
        Res += A.Elements[Pos].Elements.coef.ToDouble() * pow(x, A.Elements[Pos].Elements.expo);
    }
    return Res;
}
int main()
{
    //重定向输入输出至文件
    freopen("Expriment1_In.txt", "r", stdin);
    freopen("Expriment1_Out.txt", "w", stdout);
    int n1, n2, k;
    double x1, x2;

    //输入第一个多项式的项数
    cin >> n1;
    //输入第一个多项式
    Polyn test1 = Input(n1);

    //输入第二个多项式的项数
    cin >> n2;
    //输入第二个多项式
    Polyn test2 = Input(n2);

    cin >> k;
    cin >> x1 >> x2;
    //输出两个多项式
    Print(test1);
    Print(test2);

    //输出相加结果
    cout << "Add:\n";
    Print(Add(test1, test2));
    cout << '\n';

    //输出相减结果
    cout << "Minus:\n";
    Print(Minus(test1, test2));
    cout << '\n';

    //输出相乘结果
    cout << "Multiply:\n";
    Print(Multiply(test1, test2));
    cout << '\n';

    //输出相除结果
    pair<Polyn, Polyn> Ans = Divide(test1, test2);
    cout << "Divide:\n";
    Print(Ans.first);
    Print(Ans.second);
    cout << '\n';

    //输出求导结果
    cout << "Get_Derivative_Function:\n";
    Print(Get_Derivative_Function(test1,1));
    cout << '\n';

    cout << "Get_Derivative_Function:\n";
    Print(Get_Derivative_Function(test2,1));
    cout << '\n';

    //输出求值结果
    cout << "Get_Result:\n";
    cout << Get_Result(test1, x1) << '\n';

    cout << "Get_Result:\n";
    cout << Get_Result(test2, x2) << '\n';
    return 0;
}