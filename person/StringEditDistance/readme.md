# 字符串编辑距离

[Fast C++ CSV Parser 参考](https://github.com/ben-strasser/fast-cpp-csv-parser)

    编辑距离，又称Levenshtein距离，WIKI，是指两个字串之间，由一个转成另一个所需的最少编辑操作次数。
    许可的编辑操作包括：将一个字符替换成另一个字符，插入一个字符，删除一个字符。
    俄罗斯科学家Vladimir Levenshtein在1965年提出这个概念。
    可以用来词相似度的比较。

```python
def normal_leven(str1, str2):
    len_str1 = len(str1) + 1
    len_str2 = len(str2) + 1

    matrix = [0 for n in range(len_str1 * len_str2)]

    for i in range(len_str1):
        matrix[i] = i
    for j in range(0, len(matrix), len_str1):
        if j % len_str1 == 0:
            matrix[j] = j // len_str1

    for i in range(1, len_str1):
        for j in range(1, len_str2):
            if str1[i - 1] == str2[j - 1]:
                cost = 0
            else:
                cost = 1
            matrix[j * len_str1 + i] = min(matrix[(j - 1) * len_str1 + i] + 1,
                                           matrix[j * len_str1 + (i - 1)] + 1,
                                           matrix[(j - 1) * len_str1 + (i - 1)] + cost)

    return matrix[-1]

if __name__ == '__main__':
    s1 = 'abcde'
    s2 = 'adcdef'
    print normal_leven(s1, s2)
```
输出：2，即abcde与adcdef的编辑距离是2



