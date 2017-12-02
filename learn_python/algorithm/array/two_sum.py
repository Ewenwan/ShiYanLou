#-*- coding:etf-8 -*-
"""
Given an array of integers, return indices of the two numbers
such that they add up to a specific target.

You may assume that each input would have exactly one solution,
and you may not use the same element twice.

Example:
    Given nums = [2, 7, 11, 15], target = 9,

    Because nums[0] + nums[1] = 2 + 7 = 9,
    return [0, 1].
"""

'''
给定一个整数数组和一个目标整数，返回相加之后可以得到目标整数的元素下标
'''
def two_sum(nums:"List[int]", target:"int")->"List[int]":
    dic = {}
    for i, num in enumerate(nums):
        if num in dic:
            return [dic[num], i]# 1 2
        else:
            dic[target - num] = i#3:0 4:1


if __name__ == "__main__":
    arr = [3,2,4]
    target = 6
    res = two_sum(arr, target)
    print(res)
