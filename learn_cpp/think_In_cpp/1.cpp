// hello cpp---------------------------------------
/*
// Saying Hello with C++
#include <iostream> // Stream declarations
using namespace std;

int main() {
  cout << "Hello, World! I am EwenWan."<<endl; 
  cout << "I Have two cats:" << endl;
  cout << "LaJiao, who is "<< 1 <<", and"<< endl;
  cout << "BenBen, who is 2."<< endl;
  cout << "I feel much better!"<<endl;
}
*/


// 文件 单词计数-------------------------------------
/*
//: S02:Words.cpp
#include <iostream>
#include <fstream>// file IO
#include <string>
using namespace std;

int main() {
  ifstream f("1.cpp");
  int nwords = 0;
  string word;

  while (f >> word)
    ++nwords;

  cout << "Number of words = " << nwords << endl;
}
*/


//文件特别单词计数----------------------------------
/*
//: S02:WordCount.cpp
// Counts the occurrences of a word
//
#include <iostream>
#include <fstream>// file IO
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
  if(argc<3) 
   {
     cerr << "usage: ./h int 1.cpp\n";//错误输出流
     return -1;
   }
  string word_need(argv[1]);//直接赋值

  ifstream f(argv[2]);
  int nwords = 0;
  string word;

  while (f >> word)
   if(word == word_need)
    ++nwords;

  cout << "\""<< word_need <<"\" appears " << nwords << " times"<<endl;
}
*/



// 赋值文件到 string 容器vector 反序打印--------------------------------
/*
//: S02:Fillvector.cpp
// Copy an entire file into a vector of string
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
vector<string> v;// string 容器vector
ifstream in("1.cpp");
string line;//每一行
while(getline(in, line))//读取文件的每一行
  v.push_back(line);//添加到容器

// Print backwards:
// 反序打印
int nlines = v.size();//行总数
for(int i = 0; i < nlines; i++) {
  int lineno = nlines-i-1;
  cout << lineno << ": " << v[lineno] << endl;
}
} ///:~

*/



// 用户每按一次Enter按顺序打印文件每一行--------------------------
/*
//: S02:FileView.cpp
// Displays a file a line at a time
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
ifstream in("1.cpp");
string line;
while(getline(in, line)) {
	cout << line;  // No endl!
	cin.get();// 等待用户按Enter键 并换行
 }
} ///:~
*/


// for循环 生成 浮点数容器 并打印--------------------------
/*
//: S02:FloatVector.cpp
// Fills a vector of floats
#include <iostream>
#include <vector>
using namespace std;

int main() {
// 生成容器 Fill vector:
vector<float> v;
for (int i = 0; i < 25; ++i)
	v.push_back(i + 0.5);//填充
// Display
for (int i = 0; i < v.size(); ++i) {
	if (i > 0)//确保第一个数字前不打印空格
	  cout << ' ';
	cout << v[i];
	}
 cout << endl;
}
*/
/* Output:
0.5 1.5 2.5 3.5 4.5 5.5 6.5 7.5 8.5 9.5 10.5 11.5
12.5 13.5 14.5 15.5 16.5 17.5 18.5 19.5 20.5 21.5
22.5 23.5 24.5
*/ ///:~



//创建3个容器 前两个容器内容初始化 后一个容器的内容为前两个容器 对应元素的和
/*
//: S02:FloatVector2.cpp
// Adds vectors
#include <iostream>
#include <vector>
using namespace std;

int main() {
vector<float> v1, v2;
for (int i = 0; i < 25; ++i) {
	v1.push_back(i);
	v2.push_back(25-i-1);
}

// Form sum:
vector<float> v3;//空容器 v3[i] 不存在 使用 push_back 添加元素
for (int i = 0; i < v1.size(); ++i)
	v3.push_back(v1[i] + v2[i]);
// 注意这里

// Form sum:
//  vector<float> v3;
//  v3.resize(v1.size());  // 预定义 容器大小 v3[i] 存在
//  for (int i = 0; i < v1.size(); ++i)
//    v3[i] = v1[i] + v2[i];



// Display:
for (int i = 0; i < v1.size(); ++i) {
if (v1[i] <10)
cout << v1[i] << "  + " << v2[i]
     << " = " << v3[i] << endl;
else if (v2[i]<10)
cout << v1[i] << " + " << v2[i]
     << "  = " << v3[i] << endl;
else 
cout << v1[i] << " + " << v2[i]
     << " = " << v3[i] << endl;
 }
}
*/
/* Output:
0  + 24 = 24
1  + 23 = 24
2  + 22 = 24
3  + 21 = 24
4  + 20 = 24
5  + 19 = 24
6  + 18 = 24
7  + 17 = 24
8  + 16 = 24
9  + 15 = 24
10 + 14 = 24
11 + 13 = 24
12 + 12 = 24
13 + 11 = 24
14 + 10 = 24
15 + 9  = 24
16 + 8  = 24
17 + 7  = 24
18 + 6  = 24
19 + 5  = 24
20 + 4  = 24
21 + 3  = 24
22 + 2  = 24
23 + 1  = 24
24 + 0  = 24
*/ ///:~

// 创建一个容器 打印元素  修改每一个元素变为其原值平方

//: S02:FloatVector3.cpp
// Squares a vector of floats
#include <iostream>
#include <vector>
using namespace std;

int main() {
// Fill and print:
vector<float> v;
for (int i = 0; i < 25; ++i)
	v.push_back(i);
for (int i = 0; i < v.size(); ++i) {
	if (i > 0)
	  cout << ' ';
	cout << v[i];
}
cout << endl;
// Square and print:
for (int i = 0; i < v.size(); ++i)
	v[i] = v[i] * v[i];
for (int i = 0; i < v.size(); ++i) {
	if (i > 0)
	  cout << ' ';
	cout << v[i];
 }
cout << endl;
} ///:~






