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

#### b) c++版本 主要存储单元为 向量 vector<int>  添加元素方法为 .push_back()
    
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

### 思路二：
        可以用一次遍历 + 一次查找实现，
        时间复杂度：O(n) * (O(1)~O(n)) 最坏 O(n^2) 最好 O(n)
        我们可以通过判断target与某一个元素的差值 是否也在数组之中即可。
        
####  a) Python版本 主要存储结构为列表List  查找元素方法 if toFind in nums 得到元素索引方法 j = nums.index(toFind)

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

#### b) c++版本 vector<int>::iterator it=find(nums.begin(),nums.end(),toFind);// 查找元素 同时返回索引   

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

### 思路三：  
    与思路二类似，主要使用使用 map 字典键值对(c++) python中为dict 存储原数组中的元素
    这里利用了 字典的底层存储结构是 
           哈希表 哈希映射 通过哈希表查找元素 
           时间复杂度为O(1) 而空间复杂度为O(n)
           以空间换时间
    而第一个遍历 时间复杂度为 O(n)
    所以总复杂度为 O(n)
### a) Python版本   存储结构为字典 {} dict  这里字典查找复杂度为O(1)

    def twoSum(nums, target):
        #d = {num[i]:i for i in range(len(nums))}# 生成字典 key为数组元素的值 value为元素的索引 复杂度O(n)
        d = {}
        result = []
        for i in range(len(nums)):# 复杂度O(n)
            toFind = target - nums[i] # 获取差值元素(目标元素)
            if toFind in d and i != d[toFind]: # 在字典中并且 不是同一个元素的索引
                 result = [i, d[toFind]]# 返回对应的索引
                 break  # 找到解决方案 结束循环
            d[nums[i]]=i# 生成字典的 键值对元素
        return result   

### b) c++版本 使用 map<int, int> map_ii;映射键值对 存储 和 python的 dict类似

    vector<int> twoSum(vector<int>& nums, int target){
        map<int,int> map_ii;// 原数组的 map字典 哈希表 表示 以空间 换 查找时间
        vector<int> result;// 结果
        for(int i=0; i < nums.size(); i++){
             int toFind = target - nums[i];// 获取差值元素(目标元素)
             // 这里map 支持 find() 的内置方法
             if( map_ii.find(toFind) != map_ii.end() ) {// 找到了目标元素 找不到返回end()迭代器 哈希表找直接映射 复杂度O(1)
               result.push_back(i);// 保存两元素索引
               result.push_back(map_ii[toFind]);//
               //break;
               return result;// 返回
             }
             map_ii[nums[i]] = i;// 构建原数组的 字典表示
        }
        //return result;// 返回
    } 




