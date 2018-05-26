# 各种 算法
# 例如：求给定序列中 最大子序列的和
[leetcode 博客题解](https://blog.csdn.net/liuxiao214/article/details/76032392)
> 小詹学Python 微信公众号等

## 1.返回数组中 和为 给定值的 两个数 三个数 四个数
    例如：
    给定数组 nums = [2, 7, 11, 15],目标和 target = 9,
    因为    nums[0] + nums[1] = 2 + 7 = 9,
    返回相应的索引 [0, 1]
### 返回数组中 和为 给定值的 两个数
    思路一：
        第一个思路就是两次遍历，
        找到第一个元素，
        二次遍历其后面的元素，
        如果发现这两个元素的和为target，则保存两元素坐标，并返回。
        
        时间复杂度：O(n^2)
#### Python版本  主要存储结构为列表List  添加元素方法为 .append()
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
            for j in range(i+1, len(nums)):#的后半列
                if nums[i] + nums[j] == target:#条件
                    result.append(i)#保存两元素坐标
                    result.append(j)
                    return result   # 返回

#### c++版本 主要存储单元为 向量 vector<int>  添加元素方法为 .push_back()
    
    // 这里避免拷贝，函数输入使用了 向量的引用 vector<int>& 
    vector<int> twoSum(vector<int>& nums, int target){
        vector<int> result;// 结果
        for(int i=0; i < nums.size(); i++){
           for(j = i+1; j < nums.size(); j++){
             if((nums[i] + nums[j]) == target){
               result.push_back(i);
               result.push_back(j);
               return result;
             }
           }                         
        }   
    }

    
    
