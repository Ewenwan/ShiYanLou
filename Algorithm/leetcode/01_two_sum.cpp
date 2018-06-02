    #include<hash_map>
    #include <iostream>
    #include <vector>
    using namespace std;
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
    
int main(){
        vector<int> vi{1,2,4,7,11};
        vector<int> result = twoSum(vi,13);
        for(auto id : result)
           cout << vi[id] << endl;
        return 0;
}  
