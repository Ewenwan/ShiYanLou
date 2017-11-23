#include <iostream>//输入输出流
#include <string>//字符串
#include <vector>//容器
#include <fstream>//文件流
#include <cstdlib>//c标准库
#include <ctime>//时间
 
using namespace std;// standard 标准库 命名空间
 
//COUNT和MAX_NUM分别表示随机数个数和最大值
const long COUNT = 10000;//序列最大数量
const int MAX_NUM = 200;//序列最大值
 
//读文件
bool readFile(vector<int>& input, string fileName)
{
    ifstream infile(fileName.c_str());// 字符串类型读取文件
    if (!infile)
        return false;
    int s;
    while(infile>>s)
    {
        input.push_back(s);//返回 整数存入 容器vector input
    }
    return true;
}
 
//写大量随机测试数据
bool writeTestData(string fileName)
{
    ofstream outfile(fileName.c_str());//字符串 类型 写文件
    if (!outfile)
        return false;
    srand((unsigned)time(NULL));//随机数种子 seed rand 
    for (int i = 0; i < COUNT; i++)//产生随机数的 个数
    {
        if (rand() % 2 == 0)//偶数
            outfile << rand() % MAX_NUM << '\n';//随机数取余最大数 换行
        else
            outfile << ~(rand() % MAX_NUM) << '\n';// 负数
    }
    return true;
}
 
//穷举法  N^3
long maxSubSum1(const vector<int>& a)
{
    long maxSum = 0;
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = i; j < a.size(); j++)
        {
            long thisSum = 0;
 
            for (int k = i; k <= j; k++)
            {
                thisSum += a[k];
            }
            if (thisSum > maxSum)
            maxSum = thisSum;
        }
    }
    return maxSum;
}
 
//也是穷举法，不过减去了上面的一些不必要操作O(n^2)
long maxSubSum2(const vector<int>& a)
{
    long maxSum = 0;
    for (int i = 0; i < a.size(); i++)
    {
        long thisSum = 0;
        for (int j = i; j < a.size(); j++)//后续起点
        {
            thisSum += a[j];
            if (thisSum > maxSum)
                maxSum = thisSum;
        }
    }
    return maxSum;
}
 
//递归法，复杂度是O(nlogn)
long max3(long a, long b, long c)
{
    if (a < b)
    {
        a = b;
    }
    if (a > c)
        return a;
    else
    return c;
}
 
long maxSumRec(const vector<int>& a, int left, int right)
{
    if (left == right)
    {
        if (a[left] > 0)
            return a[left];
        else
            return 0;
    }
    int center = (left + right) / 2;
    long maxLeftSum = maxSumRec(a, left, center);
    long maxRightSum = maxSumRec(a, center+1, right);
 
    //求出以左边对后一个数字结尾的序列最大值
    long maxLeftBorderSum = 0, leftBorderSum = 0;
    for (int i = center; i >= left; i--)
    {
        leftBorderSum += a[i];
        if (leftBorderSum > maxLeftBorderSum)
            maxLeftBorderSum = leftBorderSum;
    }
 
    //求出以右边对后一个数字结尾的序列最大值
    long maxRightBorderSum = 0, rightBorderSum = 0;
    for (int j = center+1; j <= right; j++)
    {
        rightBorderSum += a[j];
        if (rightBorderSum > maxRightBorderSum)
            maxRightBorderSum = rightBorderSum;
    }
 
    return max3(maxLeftSum, maxRightSum,
    maxLeftBorderSum + maxRightBorderSum);
}
 
long maxSubSum3(const vector<int>& a)
{
    return maxSumRec(a, 0, a.size()-1);
}
 
//线性的算法O(N)
long maxSubSum4(const vector<int>& a)
{
    long maxSum = 0, thisSum = 0;
    for (int j = 0; j < a.size(); j++)
    {
        thisSum += a[j];
        if (thisSum > maxSum)
            maxSum = thisSum;
        else if (thisSum < 0)//到这里 序列前面的和小于0 那么前面的可以去掉 ，因为 加上负数都会变小
            thisSum = 0;
    }
    return maxSum;
}
 
int main ()
{
    vector<int> input;
   ///**
    if (!writeTestData("in.txt"))//产生随机序列文件
    {
        cout << "写文件错误" << endl;
    }
   // */
 
    if (readFile(input, "in.txt"))//读取文件
    {
        //cout << maxSubSum1(input) << endl;
        //cout << maxSubSum2(input) << endl;
        cout << maxSubSum3(input) << endl;
        cout << maxSubSum4(input) << endl;
    }
 
    return 0;
}