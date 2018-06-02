# 各种 算法

# 数组系列
# 例如：求给定序列中 最大子序列的和

[leetcode 博客题解](https://blog.csdn.net/liuxiao214/article/details/76032392)

[编程之法：面试和算法心得](https://github.com/julycoding/The-Art-Of-Programming-By-July)

[CLRS算法](https://github.com/Ewenwan/CLRS)

> 小詹学Python 微信公众号等


## 1.返回数组中 和为 给定值的 两个数 三个数 四个数
    例如：
    给定数组 nums = [2, 7, 11, 15],目标和 target = 9,
    因为    nums[0] + nums[1] = 2 + 7 = 9,
    返回相应的索引 [0, 1]
### A 返回数组中 和为 给定值的 两个数
### 思路一：
        第一个思路就是两次遍历，
        找到第一个元素，
        二次遍历其后面的元素，
        如果发现这两个元素的和为target，则保存两元素索引，并返回。
        
        时间复杂度：O(n^2)
#### a) Python版本  主要存储结构为列表List  添加元素方法为 .append()

```python
"""
输入 input:
   nums:   List[int]
   target: int
输出 output:
   result: List[int]
"""
def twoSum(nums, target):
    result = [] # 结果 List
    for i in range(len(nums)):#每一行
        for j in range(i+1, len(nums)):#第一个元素后面的  后半列
            if nums[i] + nums[j] == target:# 条件
                result.append(i)# 保存两元素索引
                result.append(j)
                return result   # 返回
```

#### b) c++版本 主要存储单元为 向量 vector<int>  添加元素方法为 .push_back()
    
```c++ 
// 这里避免拷贝，函数输入使用了 向量的引用 vector<int>& 
vector<int> twoSum(vector<int>& nums, int target){
    vector<int> result;// 结果
    for(int i=0; i < nums.size(); i++){
       for(j = i+1; j < nums.size(); j++){// 第一个元素后面的 后半列 
         if((nums[i] + nums[j]) == target){// 条件
           result.push_back(i);// 保存两元素索引
           result.push_back(j);
           return result;// 返回
         }
       }                         
    }   
}
```

### 思路二：
        可以用一次遍历 + 一次查找实现，
        时间复杂度：O(n) * (O(1)~O(n)) 最坏 O(n^2) 最好 O(n)
        如果数列有序，查找使用二分查找，平均复杂度 log(n),总复杂度 O(n*log(n))
        我们可以通过判断target与某一个元素的差值 是否也在数组之中即可。
        
####  a) Python版本 主要存储结构为列表List  查找元素方法 if toFind in nums 得到元素索引方法 j = nums.index(toFind)
```python
    def twoSum(nums, target):
        result = []
        for i in range(len(nums)):
            toFind = target - nums[i]# 获取差值元素(目标元素)
            if toFind in nums:#查找算法   最坏 O(n) 最好 O(1)
                j = nums.index(toFind)# 获取元素的索引
                if i != j:# 防止找了同一个元素
                    result.append(i)
                    result.append(j)
                    return result
```
#### b) c++版本 vector<int>::iterator it=find(nums.begin(),nums.end(),toFind);// 查找元素 同时返回索引   
    
```c++ 
    #include <iostream>
    #include <vector>
    #include <algorithm>
    using namespace std;

    // 不同于map（map有find方法），标准库中  vector本身是没有find这一方法，其find是依靠algorithm来实现的
    // 这里避免拷贝，函数输入使用了 向量的引用 vector<int>& 
    vector<int> twoSum(vector<int>& nums, int target){
        vector<int> result;// 结果
        for(int i=0; i < nums.size(); i++){
             int toFind = target - nums[i];// 获取差值元素(目标元素)
             vector<int>::iterator it = find(nums.begin(),nums.end(),toFind);// 查找元素 同时返回索引
             if( it != nums.end()){// 找到了目标元素 从后向前查找 找不到返回end()迭代器
               result.push_back(i);// 保存两元素索引
               result.push_back((int)(it - nums.begin()));// 迭代器 转化成 索引
               return result;// 返回
             }
        }
    }         

    int main(){
        vector<int> vi{1,2,4,7,11};
        vector<int> result = twoSum(vi,13);
        for(auto id : result)
           cout << vi[id] << endl;
        return 0;
    }     
```

### 思路三：  
    与思路二类似，主要使用使用 map 字典键值对(c++) python中为dict 存储原数组中的元素
    这里利用了 map 字典的底层存储结构是 
           红黑二叉树 (有序字典)
           查找元素 平均时间复杂度为O(log(n))  
           以空间换时间
           
           也有 hasp_map<>结构 用哈希函数实现(无序字典)
           查找元素 平均时间复杂度为O(1)  
           以空间换时间
    而第一个遍历 时间复杂度为 O(n)
    使用map时，总复杂度为 O(n*log(n))
    使用hasp_map时，总复杂度为 O(n)
    空间复杂度 O(n)
    
    而 python中 的 dict 底层为 哈希表实现  平均时间复杂度为O(1)
    总复杂度为 O(n)
    
### a) Python版本   存储结构为字典 {} dict 哈希函实现  这里字典查找复杂度为O(1)
```python
    def twoSum(nums, target):
        #d = {num[i]:i for i in range(len(nums))}# 生成字典 key为数组元素的值 value为元素的索引 复杂度O(n)
        d = {}
        result = []
        for i in range(len(nums)):# 复杂度O(n)
            toFind = target - nums[i] # 获取差值元素(目标元素)
            if toFind in d and i != d[toFind]: # 在字典中并且 不是同一个元素的索引  平均时间复杂度为O(1)
                 result = [i, d[toFind]]# 返回对应的索引
                 break  # 找到解决方案 结束循环
            d[nums[i]]=i# 生成字典的 键值对元素
        return result   
```
### b) c++版本 使用 map<int, int> map_ii; 映射键值对 红黑二叉树实现存储   
```c++ 
    // 红黑二叉树实现的 有序字典 map实现 
    #include<map>
    vector<int> twoSum(vector<int>& nums, int target){
        map<int,int> map_ii;// 原数组的 map字典 红黑二叉树实现
        vector<int> result;// 结果
        for(int i=0; i < nums.size(); i++){// 复杂度O(n)
             int toFind = target - nums[i];// 获取差值元素(目标元素)
             // 这里map 支持 find() 的内置方法
             if( map_ii.find(toFind) != map_ii.end() ) {// 找到了目标元素 找不到返回end()迭代器 哈希表找直接映射 复杂度O(log(n)) 
               result.push_back(i);// 保存两元素索引
               result.push_back(map_ii[toFind]);//
               //break;
               return result;// 返回
             }
             map_ii[nums[i]] = i;// 构建原数组的 字典表示
        }
        //return result;// 返回
    } 
```
### c) c++版本 使用 哈希函数实现的 无序字典 hash_map实现  使用count() 统计元素出现与否
```c++ 
    #include<hash_map>
    //问题是： ‘hash_map’ was not declared in this scope。
    //解决 加一个  using namespace __gnu_cxx;
    using namespace __gnu_cxx;
    vector<int> twoSum(vector<int>& nums, int target){
        hash_map<int,int> hash_map_ii;// 原数组的 map字典 哈希表 表示 以空间 换 查找时间
        vector<int> result;// 结果
        for(int i=0; i < nums.size(); i++){// 复杂度O(n)
             int toFind = target - nums[i];// 获取差值元素(目标元素)
             // 这里hash_map 支持 count() 的内置方法
             if( hash_map_ii.count(toFind)) {// 要么是1(即出现了该元素),要么是0(即没出现这样的元素) 
               result.push_back(i);// 保存两元素索引
               result.push_back(hash_map_ii[toFind]);//
               //break;
               return result;// 返回
             }
            hash_map_ii[nums[i]] = i;// 构建原数组的 字典表示
        }
        return {};//不写的话 编译器会自动添加
        //return result;// 返回
    } 
```
### 思路四： 
    先排序，然后左右夹逼。
    排序 O(n*log(n))    排序后 就与原来的数组的索引不一样了
    左右夹逼 O(n) 
    最终时间复杂度 O(n*log(n))
    如果数组有序，总时间复杂度 O(n)
    空间复杂度O(1)
    
    这里不能返回原数组中的索引
    这里只能返回数字
    后可以使用find在原数组中查找索引
```c
vector<int> twoSum(vector<int>& nums, int target){
// sort(nums.begin(), nums.end()) 如果数组无序，先排序，时间复杂度 O(n*log(n))
vector<int> result;// 结果
// 两个指针
int beg=0;
int end=nums.size()-1;
while(beg < end){// 从首尾位置向中间扫描夹逼，
  long currSum = nums[beg]+nums[end];
  if(currSum == target) {
      result.push_back(nums[beg]);
      result.push_back(nums[end]);
      return result;
     }
   else{
        if (currSum < target)// 和小了
         beg++;// 小数变大
        else end--;// 和大了 大数变小
     }
  }
}
```

### 总结
    0. 暴力两次遍历,时间复杂度O(n^2),空间复杂度O(1);
    1. 第一遍遍历，第二次查找(
                           (红黑树结构map查找时间复杂度O(log(n)) , 空间复杂度O(n);
                            hash表hash_map查找O(1),              空间复杂度O(n);
                            二分查找(数组需要有序) 查找时间复杂度O(log(n)) , 排序，时间复杂度 O(n*log(n)), 空间复杂度O(1);
                            )
    2.先排序后，从首尾位置向中间扫描夹逼，有序O(n)，无序O(n*log(n) + n)=O(n*log(n))，空间复杂度都为O(1)。




## B寻找和为定值的多个数
[寻找和为定值的多个数](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/02.03.md)

### 和为定值的 3个数
    解法：先排序，再遍历一个数，后再 在首尾位置左右夹逼选出另外两个数
    先排序 复杂度O(n*log(n))
    遍历第一个数 O(n)
    左右夹逼最后两个数 O(n)
    复杂度 MAX(O( n*log(n)), O(n^2) ) ， 空间复杂度O(1) 
    

### 和为定值的 4个数
    解法：先排序，再遍历4-2个数，后再 在首尾位置左右夹逼选出另外两个数
    先排序 复杂度O(n*log(n))
    遍历第一个数 O(n)
    遍历第二个数 O(n)
    左右夹逼最后两个数 O(n)
    复杂度 MAX(O( n*log(n)), O(n^3) ) ，  空间复杂度O(1) 
    
    使用 一个hashmap存储两个数的和，可以降低一些复杂度
    时间复杂度O(n^2)  空间复杂度O(n^2)
### 和为定值的 k个数
    解法：先排序，再遍历k-2个数，后再 在首尾位置左右夹逼选出最后两个数
    先排序 复杂度O(n*log(n))
    遍历第一个数 O(n)
    遍历第二个数 O(n)
    ...
    遍历第k-2个数 O(n)
    左右夹逼最后两个数 O(n)
    复杂度 MAX(O( n*log(n)), O(n^(k-1)) )
    
    
### 通用解法(大问题分解成小问题) 使用0-1背包问题 求解法
    有点问题
    找k个数，和为target
    排序后的数组 nums
    从后向前选择数：
        如果选择最后一个数nums[n],  那么需要从前n-1个数中找出k-1个数使得和为target-nums[n]；
        如果不选择最后一个数nums[n],那么需要从前n-1个数中找出k个数使得和为  target


# 链表系列

# 题2 两个数求和 Add Two Numbers

    给出两个链表，存储非负数，
    两个链表都是按倒序方式存储数字（个位，十位，百位……）
    要求将两个链表相加并以链表形式返回。
    
    例如：
    Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
    Output: 7 -> 0 -> 8
    Explanation: 342 + 465 = 807.
    
## 解法思想：
    1. 对应位上的数相加时需要考虑低位上的进位(1/0)
    2. 还要考虑到两个数位数不相等时，短的数的高位相当于0
    
## python解法
```python
# Definition for singly-linked list.
# class ListNode:
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution:
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        # 新建结果保存链表 头节点保存的内容无用 但是其 指针dummy.next可以保存我们的链表
        p = dummy = ListNode(-1)
        carry = 0 # 初始进位为0
        # 计算两个数的共有位的和
        while l1 and l2:
            # 对应位上的数相加时需要考虑低位上的进位(1/0)
            p.next = ListNode(l1.val + l2.val + carry)
            # carry  = p.next.val // 10 #  商为进位
            # python 的 ? : 语句 
            # carry=(1 if(p.next.val>9) else 0)
            if p.next.val>9 : 
                carry = 1
            else: 
                carry = 0
            # p.next.val %= 10 # 余数为当前位上的数
            if carry == 1: 
                p.next.val = p.next.val - 10
            # 结果指向
            p = p.next # 指向下一个节点
            # 两个求和数指向下一个节点
            l1 = l1.next
            l2 = l2.next
        # 计算长的数
        rest = l1 or l2 # 合并为长的 或运算获取 长的高位上的数3位数和5为数相加 获取5位数的第4位和第5位
        while rest:
            p.next = ListNode(rest.val + carry)#仅有高位上的数 和 低位的进位
            carry  = p.next.val // 10 #  商为进位
            p.next.val %= 10 # 余数为当前位上的数
            # 结果指向
            p = p.next # 指向下一个节点 
            # 长的下一个节点
            rest = rest.next
        if carry: #最后还有可能进位
            p.next = ListNode(1)
        return dummy.next
```
### 两个循环合并  python
```python
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        head = ListNode(0)#新建一个头结点 同来保存结果 链表
        add = head # 结果链表的当前节点
        sum = 0    # 和
        # 只要两个加数 还有未 计算的位 以及还有进位
        while (l1 != None or l2 != None or int(sum / 10)):
            # 对应加数位上无数据的话 用0代替
            sum = (l1.val if (l1 != None) else 0) + (l2.val if (l2 != None) else 0) + int(sum / 10)
            # 结果位 为  求和 - 10 即求和对10的余数
            add.next = ListNode(sum % 10)
            # 迭代
            add = add.next#结果的下一个节点
            l1 = l1.next if (l1 != None) else l1# 加数的下一个节点 不为空的有
            l2 = l2.next if (l2 != None) else l2# 
        return head.next
```
## 对应  c++ 版本
```asm
    // 单向链表结构体
    struct ListNode {
        int val;// 值
        ListNode *next;// 指向下一个 节点的指针
        ListNode(int x) : val(x), next(NULL) {}// 结构体初始化函数
    };
    
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
```

### 两个循环合并简版 python
```python
    def addTwoNumbers(self, l1, l2):
        """
        :type l1: ListNode
        :type l2: ListNode
        :rtype: ListNode
        """
        head=ListNode(0)#新建一个头结点 同来保存结果 链表
        l3=head# 结果链表的当前节点
        v=0#进位
        while l1 or l2 or v:
            v1=v# 求和值
            if l1:
                v1+=l1.val# 求和
                l1=l1.next# 迭代
            if l2:
                v1+=l2.val# 求和
                l2=l2.next# 迭代
            v=v1//10# 商   进位
            v2=v1%10# 余数 留数
            l3.next=ListNode(v2)# 结果保存
            l3=l3.next # 结果迭代
        return head.next
```
###  c++ 解法
```asm
    // 单向链表结构体
    struct ListNode {
        int val;// 值
        ListNode *next;// 指向下一个 节点的指针
        ListNode(int x) : val(x), next(NULL) {}// 结构体初始化函数
    };
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
```
