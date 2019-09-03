# bash查找内容并修改 sed  for while


```sh

#!/bin/bash

##################检查MOD_algo/apps/model_cvt_package/quantize_nets.sh脚本的正确性

# 保存 bash 开头的行
quantize_nets_sh="./MOD_algo/apps/model_cvt_package/quantize_nets.sh"
key_line=$(grep ^"bash" $quantize_nets_sh) 
echo "$key_line" > key_line.txt
key_line_count=0
key_line_arr=() #字符串数组
while read line;do
    #echo $line
    key_line_arr[$key_line_count]=$line
    ((key_line_count+=1)) #计数
done < key_line.txt
rm key_line.txt

# 去掉model_index= 保留模型修改id列表
model_index=`cat $quantize_nets_sh | grep ^"model_index=" | sed 's/model_index=//g'`
model_index_count=0
key_model_index_arr=()
for id in $model_index; do
    key_model_index_arr[model_index_count]=$id
    ((model_index_count+=1))
done

if [ $model_index_count -ne $key_line_count ];then
    echo "make sure model_index num equals to convert model num"
    exit 0
fi

#i=0
#echo ${key_line_arr[$i]}
#echo ${key_model_index_arr[$i]}
#exit 0

############## 解析quantize_nets.sh 获取所需修改的模型列表 并修改 MOD_algo/apps/hik_hisi_test_plat/hisi_demo/para.ini文件
#查找目标模型名字
key_model_arr=()
for((i=0;i<$key_line_count;i++));do
    #echo $i
    #echo ${key_line_arr[$i]}
    item_count=0
    for item in ${key_line_arr[$i]};do
        ((item_count+=1))
        if [ $item_count -eq 14 ]; then
            key_model_arr[$i]=$item
        fi
    done
done

#i=0
#echo ${key_model_arr[$i]}
#exit 0

# 查找目标行号
para_ini="./MOD_algo/apps/hik_hisi_test_plat/hisi_demo/para.ini"
mod_model_num=`cat $para_ini | grep ^"mod_model_num = " | sed 's/mod_model_num = //g'`
mod_model_count=0
mod_model_line_num_list=()
sed -n '/mod_model_path/=' $para_ini > line_num.txt
while read line;do
    #echo $line
    mod_model_line_num_list[$mod_model_count]=$line
    ((mod_model_count+=1)) #计数
done < line_num.txt
rm line_num.txt

if [ $mod_model_count -ne $mod_model_num ]; then
    echo "make sure mod_model_num num equals to mod_model_path num"
    exit 0
fi


#i=0
#echo ${mod_model_line_num_list[((${key_model_index_arr[$i]}-1))]}

#exit 0

#修改配置文件目标行内容
for((i=0;i<$model_index_count;i++));do
    # 目标行
    #echo ${key_model_index_arr[$i]}
    key_line_num=${mod_model_line_num_list[((${key_model_index_arr[$i]}-1))]}
    #echo $key_line_num
    # 目标行新内容
    key_line_contex="mod_model_path = ./models/"${key_model_arr[$i]}
    #echo $key_line_contex
    # 替换内容
    sed -i "${key_line_num}c${key_line_contex}" $para_ini 
done





####### 将quantize_nets.sh 运行后生成的MOD_algo/apps/model_cvt_package/output/bin文件拷贝到MOD_algo/apps/hik_hisi_test_plat/hisi_demo/models下
bin_model_path="./MOD_algo/apps/model_cvt_package/output"
dest_bin_model_path="./MOD_algo/apps/hik_hisi_test_plat/hisi_demo/models/"
#修改配置文件目标行内容
for((i=0;i<$model_index_count;i++));do
    # 目标模型路径
    src_bin_model_path=${bin_model_path}"/"${key_model_arr[$i]}
    cp $src_bin_model_path $dest_bin_model_path
done













```
