#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

void csvline_populate(vector<string> &record, const string& line, char delimiter);
void  GetString( const string& Str, vector<string> &Value );


int main(int argc, char *argv[])
{
	
    vector<string> row;
    vector<string> label;
    string line;
    string temp_str;
    ifstream in("sour_lable.csv"); //声明文件流 
    if (in.fail())  { cout << "File not found" <<endl; return 0; }
     long t=1;
    while(getline(in, line)  && in.good() )
    {
          csvline_populate(row, line, ',');
          cout<< t<<"\t";
          cout<< row[1] << "\t";
          temp_str=row[1];
         // char* c;
         // const int len = temp_str.length();
         // c =new char[len+1];
         // strcpy(c,temp_str.c_str());
          //cout<<temp_str.size()<<"|";
         // char *tokenPtr=strtok(c,"");
          GetString(temp_str,label);
          //vector<string>::iterator it;
          //cout<<label.size()<<"|";
         for (int it=0;it<label.size();it++)
           cout<<label[it]<<"|";
         // while(tokenPtr!=NULL)　{
          //    cout<<tokenPtr<<"|";
          //    tokenPtr=strtok(NULL,"");
          //      }
          cout << endl;
          t++;
    
    }
    in.close();
    return 0;
}

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
       
        c = line[linepos];       //每个字符 
       
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
void  GetString( const string& Str, vector<string> &Value )
{
   string Temp="";                          //分割的字符串 
   int strlength=Str.length();              //源字符串长度 
   int Index= 0;
   Value.clear();
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
         Value.push_back(Temp);
         Temp="";                    //清空上一次保存的临时字符串 
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
