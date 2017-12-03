#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>
/*
单词查找树，Trie树
字典树遍历 
小Ho，你能不能对于每一个我给出的字符串，都在这个词典里面找到以这个字符串开头的所有单词呢？”
身经百战的小Ho答道：“怎么会不能呢！你每给我一个字符串，我就依次遍历词典里的所有单词，
检查你给我的字符串是不是这个单词的前缀不就是了？”
小Hi笑道：“你啊，还是太年轻了！~假设这本词典里有10万个单词，我询问你一万次，你得要算到哪年哪月去？”
Hihocoder #1014 : Trie树
时间限制:10000ms
单点时限:1000ms
内存限制:256MB

root_node
| 
a(0)    c(2)
|       |
P()     a(0)—l()—l() 
|       |
p()     t()
|
l()
|
e()

apple   cat   call 
*/ 

//AC G++ 409ms 76MB

using namespace std ;
// 树 节点 
typedef struct TrieNode{
    int count ;
    struct TrieNode *next[26] ;//每一个节点都可能指向26个字母中的一个单词 节点 
    bool exit_word ;
}TrieNode ;

// 创建 空树  返回根节点 
TrieNode* createKeyNode(){
    TrieNode *insert_node = (TrieNode *)malloc(sizeof(TrieNode)) ;//开辟内存 
    insert_node->count = 0 ;//经过该节点的单词数量 
    insert_node->exit_word = false ;// 结束字符节点 标识 
    for(int i = 0 ; i < 26 ; i++)insert_node->next[i] = NULL ;//指向节点指针为 空 next[0] 为a字符节点  next[25]为z 字符节点 
    return insert_node ;
}

// 树插入数据   英文字符串 英文单词 
void keyTreeInsert(TrieNode *root_node , const char* insert_str){
    char current_char ;//将单词依次 存入单词查找树中 
    int i = 0 ;
    int j = 0 ;
    TrieNode *current_node = root_node ;//每次插入新单词 都会从头结点开始便利 
    while(insert_str[i]!='\0'){//把所有的字符全部存入 
        current_char = insert_str[i++] ;//当前的字符 
        j = current_char - 'a' ;//0~25  对应a~z26个字母 
        if(current_node->next[j]==NULL)current_node->next[j] = createKeyNode() ;//新单词中对应的字符未出现则 创建一个新节点 
        current_node = current_node->next[j] ;//node后移 
        current_node->count++ ;//经过该节点 单词的数量 计数 
    }
    current_node->exit_word = true ;//该节点为单词的结束字符 
}

// 树查找 元素  所有以指定元素开头的字符串 
int keyTreeSearch(TrieNode *root_node,const char *search_str){
   if(!root_node)return 0 ;//空树 
   int i = 0 ;
   int j = 0 ;
   char current_char ;//当前 字符 
   TrieNode *current_node = root_node ;//从头节点开始搜索 
   while(search_str[i]!='\0'){
       current_char = search_str[i++] ;//当前的字符 
       j = current_char-'a' ;//0~25  对应a~z26个字母 
       if(!current_node->next[j])return 0 ;//未找到 
       current_node = current_node->next[j] ;//依次遍历到最后一个 单词字符节点 
   }
   return current_node->count ;//返回经过 指定字符串最后一个字符 的单词数量 
}

int main(){
    int m,n ;
    int i = 0 ;
    char source_str[11] ;//单词长度限制为 11 
    char target_str[11] ;
    TrieNode *root_node = createKeyNode() ;//根节点 
   // printf("请输入单词数量:\r\n"); 
    scanf("%d" , &n) ;//总单词数量 
    //printf("请依次输入每个单词:\r\n");
    while(i++<n){
        scanf("%s" , source_str) ;
        keyTreeInsert(root_node , source_str);//每次插入都会从头结点依次遍历 经过节点 节点计数+1  未存在的节点进行创建 
    }
    i= 0 ;
    // printf("请输入要查询单词数量:\r\n"); 
    scanf("%d" , &m) ;
    // printf("请输入每一个要查询的:\r\n"); 
    while(i++<m){
        scanf("%s" , target_str) ;
        printf("%d\n",keyTreeSearch(root_node,target_str)) ;
    }
    return 0 ;
}
