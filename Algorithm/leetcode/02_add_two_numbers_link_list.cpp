/*
题2 两个数求和 Add Two Numbers

给出两个链表，存储非负数，
两个链表都是按倒序方式存储数字（个位，十位，百位……）
要求将两个链表相加并以链表形式返回。

例如：
Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
Explanation: 342 + 465 = 807.
解法思想：

1. 对应位上的数相加时需要考虑低位上的进位(1/0)
2. 还要考虑到两个数位数不相等时，短的数的高位相当于0

*/
#include <iostream>
using namespace std;

// 单向链表结构体
struct ListNode {
	int val;// 值
	ListNode *next;// 指向下一个 节点的指针
	ListNode(int x) : val(x), next(NULL) {}// 结构体初始化函数
};

class Solution {
public:
	// 1
	ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {// 传入的为链表指针 使用->访问成员
		// 新建一个 链表 头结点 内容为0 没什么用处 但是其next指针可以访问我们的结果链表
		ListNode preHead(0);// 结果链表 头指针
		ListNode* p = &preHead;      // 结果链表的 当前节点
		int carry = 0;      // 进位
		while (l1 || l2 || carry) {
			// 求和 对应位上的数相加时需要考虑低位上的进位(1/0)
			// 对应加数位上无数据的话 用0代替
			int sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + carry;
			carry = sum / 10;// 整数除法保留商  为进位 python 为 //
			p->next = new ListNode(sum % 10);//新建节点 保存 余数 = 和-10
			// 迭代
			p = p->next;// 结果的下一个 节点
			// 两个加数的下一个节点
			l1 = l1 ? l1->next : l1;// 不为空的有
			l2 = l2 ? l2->next : l2;
		}
		return preHead.next;// 头节点的下一个节点为 结果的开始
		// 通过这种方式处理头，可以更加简洁的书写链表
	}
	
       // 2
	ListNode *addTwoNumbersMy(ListNode* l1, ListNode* l2) {// 传入的为链表指针 使用->访问成员
		// 新建一个 链表 头结点 内容为0 没什么用处 但是其next指针可以访问我们的结果链表
		ListNode preHead(0);      // 结果链表 头指针
		ListNode* res = &preHead; // 结果链表的 当前节点
		int carry = 0;      // 进位
		while (l1 || l2 || carry) {
			int sum = carry;// 求和先保存 进位值
			if(l1){
				sum += l1->val;// 求和
				l1 = l1->next; // 迭代
			}
			if(l2){
				sum += l2->val;// 求和
				l2 = l2->next; // 迭代
			}
			carry = sum / 10;// 整数除法保留商  为进位 python 为 //
			// carry = (sum>9 ? 1 : 0)
			// liu = (carry==1 ? (sum - 10): sum)
			// res->next = new ListNode(liu);
			res->next = new ListNode(sum % 10);//新建节点 保存 余数 = 和-10
			// 迭代
			res = res->next;// 结果的下一个 节点
		}
		return preHead.next;// 头节点的下一个节点为 结果的开始
		// 通过这种方式处理头，可以更加简洁的书写链表
	}
	
	// 3
	ListNode* addTwoNumbersMy52ms(ListNode* l1, ListNode* l2) {
		ListNode* ret = new ListNode(0);
		ListNode* cur = ret;
		bool enbit = false;
		int n1 = 0, n2 = 0;
		// it1
		int val = l1->val + l2->val;
		if (enbit){
			val++;
		}
		if (val >= 10){
			val -= 10;   // 留数
			enbit = true;// 进位
		}
		cur->val = val;
		while (l1->next || l2->next){
			if (l1->next){
				l1 = l1->next;
				n1 = l1->val;
			} else{
				n1 = 0;
			}
			if (l2->next){
				l2 = l2->next;
				n2 = l2->val;
			} else{
				n2 = 0;
			}

			int val = n1 + n2;
			if (enbit){
				val++;
				enbit = false;
			}
			if (val >= 10){
				val -= 10;
				enbit = true;
			}
			cur->next = new ListNode(val);
			cur = cur->next;
		}

		if (enbit){
			if (cur){
				cur->next = new ListNode(1);
			} else{
				cur = new ListNode(1);
			}
		}
		return ret;
	}
};

int main(int argc, char *argv[]){
	Solution s;
	ListNode* lnp1 = new ListNode(5);
	ListNode* lnp2 = new ListNode(9);
	ListNode* result = s.addTwoNumbersMy(lnp1, lnp2);
	// system("pause");
	while(result){
	 cout<< result->val << endl;
	 result = result->next;
	   }
	return 0;
}
