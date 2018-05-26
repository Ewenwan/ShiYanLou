# 各种 算法
# 例如：求给定序列中 最大子序列的和
[leetcode 博客题解](https://blog.csdn.net/liuxiao214/article/details/76032392)
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
    
    而 python中 的 dict 底层为 哈希表实现  平均时间复杂度为O(1)
    
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


### B寻找和为定值的多个数
[寻找和为定值的多个数](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/02.03.md)

