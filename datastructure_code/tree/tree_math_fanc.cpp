#include <iostream>  
#include <cstdio>  
#include <cstring>  
#include <string>  
  
using namespace std;  
  
char str[1000];  
char op[1000];  
int lch[1000], rch[1000];  
int no = 0;  
  
int build_tree(char *st, int beg, int end){  
    int i, j, k, p;  
    int u;  
    int sum = 0;  
    int flag = 1;  
    //printf("%d %d\n",beg,end);  
    for(i = beg; i < end; i ++){  
        if(!(st[i] <= '9' && st[i] >= '0')){  
            flag = 0;  
            break;  
        }  
    }  
    if(flag){  
        //printf("*%d***%d**",beg,end);  
        j = 1;  
        for(i = end - 1; i >= beg; i --){  
            sum += ((int)st[i] - '0') * j;  
            j *= 10;  
        }  
        u= ++no;  
        lch[u] = 0;  
        rch[u] = 0;  
        op[u] = sum;  
        //printf("#%d#",sum);  
        return u;  
    }  
    int c1 = -1;  
    int c2 = -1;  
    p = 0;  
    for(i = beg; i < end; i ++){  
        switch(st[i]){  
            case '(': p ++; break;  
            case ')': p --; break;  
            case '+': case '-': if(!p) c1 = i; break;  
            case '*': case '/': if(!p) c2 = i; break;  
        }  
    }  
    //cout << c1 <<endl;  
    if(c1 < 0) c1 = c2;  
    if(c1 < 0) return build_tree(str, beg + 1, end - 1 );  
    u = ++no;  
    lch[u] = build_tree(st, beg, c1);  
    rch[u] = build_tree(st, c1+1, end);  
    op[u] = st[c1];  
    return u;  
}  
  
float calculate(char *st, int beg){  
    if(lch[beg] == 0 && rch[beg] == 0)  
        return (float)st[beg];  
    else switch (st[beg]){  
        case '+': return calculate(op, lch[beg]) + calculate(op, rch[beg]); break;  
        case '-': return calculate(op, lch[beg]) - calculate(op, rch[beg]); break;  
        case '*': return calculate(op, lch[beg]) * calculate(op, rch[beg]); break;  
        case '/': return calculate(op, lch[beg]) / calculate(op, rch[beg]); break;  
    }  
    return 0;  
}  
  
int main(){  
    int i;  
    while(scanf("%[^\n]", str) != EOF ){  
        getchar();  
        memset(op, 0, sizeof(op));  
        //cout << str <<endl;  
        no = 0;  
        build_tree(str, 0, strlen(str));  
        printf("%.2f\n", calculate(op, 1));  
        memset(str, 0, sizeof(str));  
    }  
    return 0;  
} 
