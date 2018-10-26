/*
 * minEditDis.cpp
 *
 *  @Created on: Jul 10, 2012
 *      @Author: sophia
 *  @Discription: calculate the minimal edit distance between 2 strings
 *
 *  Method : DP (dynamic programming)
 *  D[i,j]: the minimal edit distance for s1的前i个字符和 s2的前j个字符
 *  DP Formulation: D[i,j]=min(D[i-1,j]+1,D[i,j-1]+1,D[i-1,j-1]+flag);//其中if(s1[i]==s2[j])则flag=2,else flag=0;
 *
 */

#include"iostream"
#include"stdio.h"
#include"string.h"
using namespace std;

#define N 100
#define INF 100000000
#define min(a,b) a<b?a:b

int dis[N][N];
char s1[N],s2[N];
char s11[N],s22[N];

int minStrEditDis(char* s1, char* s2);


int main()
{
    int editDis=0;
	while(scanf("%s%s",&s11,&s22)!=EOF)
	{
		editDis=minStrEditDis(s11,s22);
		printf("min edit distance is: %d\n",editDis);
	}
	return 0;
}

int minStrEditDis(char* s1, char* s2){
	int i,j,k;
	int n,m;                   //length of the two string
	n=strlen(s1);m=strlen(s2);
		for(i=0;i<=n+1;i++)
			for(j=0;j<=m+1;j++)
				dis[i][j]=INF;
		dis[0][0]=0;

		for(i=0;i<=n;i++)
			for(j=0;j<=m;j++)
			{
				if(i>0) dis[i][j] = min(dis[i][j],dis[i-1][j]+1); //delete
				if(j>0) dis[i][j] = min(dis[i][j],dis[i][j-1]+1);//insert

				//substitute
				if(i>0&&j>0)
				{
					if(s1[i-1]!=s2[j-1])
						dis[i][j] = min(dis[i][j],dis[i-1][j-1]+2);
					else
						dis[i][j] = min(dis[i][j],dis[i-1][j-1]);
				}
			}
        return dis[n][m];
	//	printf("min edit distance is: %d\n",dis[n][m]);
}

