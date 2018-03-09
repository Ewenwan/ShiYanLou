/*
 *  minEditDis.cpp
 *  计算两个字符串间的编辑距离的LDscore 
 *  万有文
 *  1631684@tongji.edu.cn 
 */


#include"stdio.h"
#include"string.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#define N 10
#define INF 100000000
#define min(a,b) a<b?a:b

//int dis[N][N];
//char s1[N],s2[N];
//char s11[N],s21[N];
//char s12[N],s22[N];
int dist =0;
int labellenght=0;
float LDSCORE=0; 

int minStrEditDis(char* s1, char* s2);
void csvline_populate(vector<string> &record, const string& line, char delimiter);
void  GetString( const string& Str, string &Value );


int main(int argc, char *argv[])
{
    vector<string> row1,row2;//存储每一行 字符串 逗号间的 字符串的 容器 
    string label1,label2;// 1,2,3,...,9,10,11,12,13这种标签转换成  1,2,3,...,9,A,B,C,D
    string line1,line2;// 文件的每一行 
    string temp_str;//  row1,row2; 中的单个 字符串 
    
    // 文件为 cvs文件 
    ifstream sour("sour_lable.csv"); //声明文件流 
    ifstream predict("M_V_N.csv");
    //ifstream predict("D:\\LW\\Sample_code_6_16_2012\\LD score\\M_V_N.csv");   //dtw_19 声明文件流 
   
    
    if (sour.fail())  { cout << "Source File not found" <<endl; return 0; }
    if (predict.fail())  { cout << "Predict File not found" <<endl; return 0; }
    long t=1;
    // 得到 两个文件的 每一行 对每一行进行比较
	// devel01_11	 9 4 4 9

     while(getline(sour, line1)  && sour.good()&&getline(predict, line2)  && predict.good() )
    {
    	 // cout <<  line1 << endl; // 	devel01_1,10,, 
    	 // cout << line2 <<endl;
    	 // break;
          csvline_populate(row1, line1, ',');//  按逗号将字符串分成 vector<string>
          csvline_populate(row2, line2, ',');
       // cout << row1[1] <<endl;   // devel01_1,10,,  ------> {"devel01_1", "10"}
       // devel01_11	 9 4 4 9 ------> {"devel01_1", "10 4 4 9"}
       // if(t == 20) break;
		  
          cout<< t <<"\t";
          cout<< row1[1] << "\t";//为  源标签 
          temp_str=row1[1];
          GetString(temp_str,label1);// 1,2,3,...,9,10,11,12,13这种标签转换成  1,2,3,...,9,A,B,C,D
          // 10 4 4 9 ------>>  A449 
          for (int it=0;it<label1.size();it++)
          cout<<label1[it]<<"|";// 输出 源  标签信息 
          cout<<"\t";
          
          
          cout<< row2[1] << "\t";
          temp_str=row2[1];
          GetString(temp_str,label2);
          for (int it=0;it<label2.size();it++)// 识别标签 
          cout<<label2[it]<<"|";
          
          //string2char* 字符数组 
          // string 类型转成 char* 字符数组 
            char* c1;
            const int len1 = label1.length();
            c1 =new char[len1+1];
            strcpy(c1,label1.c_str());
            
            char* c2;
            const int len2 = label2.length();
            c2 =new char[len2+1];
            strcpy(c2,label2.c_str());
            
            int d=minStrEditDis(c1,c2); // 计算两个 字符串之间的编辑距离 
            labellenght +=label1.size();//  总 源标签长度 
            dist+=d;// 总标记距离 
            //distance+=dist;
          cout << '\t' << "edit dis: "<< d << endl;
          ++t;    
    }
    // 关闭文件 
    sour.close();
    predict.close();
    // 计算 LDscore 
    LDSCORE=float(dist)/labellenght;
    cout<<"======LDscore is："<<endl;
	cout<< LDSCORE <<endl;
    return 0;
    //int editDis=0;
	//	editDis=minStrEditDis(s11,s21);
	//	printf("min edit distance is: %d\n",editDis);
}

/*
计算两个字符串之间的编辑距离   
从s2字符串 到 s1字符串 需要多少次 插入、删除、替换操作 
*/
int minStrEditDis(char* s1, char* s2){
	int i,j;
	
	// 两个字符串的长度 
	int n,m;         
	n=strlen(s1);
	m=strlen(s2);	
	int dis[n+1][m+1] = {INF}};// 创建一个二维数组 
	dis[0][0]=0;// 第一个 认为相等 距离为0 

	for(i=1;i<=n;i++)
		for(j=1;j<=m;j++)
		{
			 dis[i][j] = min(dis[i][j],dis[i-1][j]+1); //delete 删除 
			 dis[i][j] = min(dis[i][j],dis[i][j-1]+1);//insert  插入 

			//替换 
			if(s1[i-1] != s2[j-1])// 不 相等 距离是 上一匹配到dis[i-1][j-1]的距离 + 1 
				dis[i][j] = min(dis[i][j],dis[i-1][j-1]+1);// 替换算两个 dis[i][j] = min(dis[i][j],dis[i-1][j-1]+2); 	
			else // 相等 的话，距离还是  上一匹配到dis[i-1][j-1]为止的距离 
				dis[i][j] = min(dis[i][j],dis[i-1][j-1]);
		}
    return dis[n][m];
}
/*
int min_(int a,int b,int c)  
{  
    int tmp=a<b ? a : b;  
    return tmp<c ? tmp:c;  
}

int minStrEditDis(char* s1, char* s2){
	// 两个字符串的长度 
	int n,m;                   //length of the two string
	n=strlen(s1);
	m=strlen(s2);
	
	int d[n+1][m+1];// 创建一个二维数组 
	d[0][0]=0;
	int i,j,k;
    for(i=0;i<=n;i++)  
        d[i][0]=i;  
    for(j=0;j<=m;j++)  
        d[0][j]=j;  
    for(i=1;i<=n;i++)  
    {  
        for(j=1;j<=m;j++)  
        {  
            int cost = (s1[i]==s2[j])? 0 : 1;// 替换 相等 操作位0  不相等操作为1 
            int deletion=d[i-1][j]+1; // 删除 
            int insertion=d[i][j-1]+1;// 插入 
            int substitution=d[i-1][j-1] + cost;  
            d[i][j]=min_(deletion,insertion,substitution);  
        }  
    } 
    return d[n][m];
} 
 */
    
//  按逗号将字符串分成 vector<string>
// devel01_1,10,,  ------> {"devel01_1", "10"}
// devel01_11	 9 4 4 9 ------> {"devel01_1", "10 4 4 9"}
void csvline_populate(vector<string> &record, const string& line, char delimiter)
{
    int linepos=0;               //字符串的索引 
    int inquotes=false;          //引号 
    char c;                      //字符串的每个元素 char 
    int i;
    int linemax=line.length();   //每行字符串的总长度 
    string curstring;
    record.clear();
       
    while(line[linepos]!=0 && linepos < linemax)//字符串的每个元素 
    {
       
        c = line[linepos];       //字符串中的 每个字符 
       
        if (!inquotes && curstring.length()==0 && c=='"')//第一个引号 
        {
            //beginquotechar
            inquotes=true;//是引号 
        }
        else if (inquotes && c=='"')//后面的引号 
        {
            //quotechar
            if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )//双引号 
            {
                //encountered 2 double quotes in a row (resolves to 1 double quote)
                curstring.push_back(c);
                linepos++;
            }
            else
            {
                //endquotechar
                inquotes=false;
            }
        }
        else if (!inquotes && c==delimiter)
        {
            //end of field
            record.push_back( curstring );
            curstring="";
        }
        else if (!inquotes && (c=='\r' || c=='\n') )
        {
            record.push_back( curstring );
            return;
        }
        else
        {
            curstring.push_back(c);
        }
        linepos++;
    }
    record.push_back( curstring );
    return;
}

//提取出字符串的内容，根据空格拆分并存入容器中去：
//Str是源字符串，Value存放根据目标字符串的空格提取出来的每个字符串内容。
// "10 4 4 9" ---------> A449
void  GetString( const string& Str, string &Value )
{
   string Temp="";                          //分割的字符串 
   int strlength=Str.length();              //源字符串长度 
   int Index= 0;
   Value="";
   char  Tmp=' '; 
   while(Str[Index]!=0 && Index < strlength)//字符串的每个元素 
    {
      Tmp = Str[Index];               //字符串中的每个字符 
      if( Tmp==' '|| Index == strlength-1)  //空格的话或者到最后一个字符 
      {
         if( Index == strlength-1) Temp.push_back(Tmp);
         if(Temp!="")       //遇到空格了，如果 Temp有字符的话就把Temp放入容器里 
         {
         	if (Temp=="10") Temp="A";
         	if (Temp=="11") Temp="B";
         	if (Temp=="12") Temp="C";
         	if (Temp=="13") Temp="D";
         	if (Temp=="14") Temp="E";
         Value=Value+Temp;
         Temp="";         //清空上一次保存的临时字符串 
         }
        Index++; 
        continue;
      }
      else {
      Temp.push_back(Tmp);
      Index++; 
      continue;
	  }
   }
   return;
}
