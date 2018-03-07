// Sort.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

class Sort
{
public:
	void InsertSort(vector<int> data)
	{
		for (int i = 1; i < data.size(); i++)
		{
			int num = data[i];
			int j;
			for (j = i-1; j >= 0; j--)
			{
				if (data[j] > num)
					data[j + 1] = data[j];
				else
				{
					break;
				}
			}
			data[j + 1] = num;
		}
		for (int i = 0; i < data.size(); i++)
		{
			cout << data[i] << " ";
		}
	}
	



	void SelectSort(vector<int> data)
	{
		vector<int> sortdata;
		vector<int>::iterator min;
		while (!data.empty())
		{
			for (vector<int>::iterator it = data.begin(); it != data.end(); it++)
			{
				if (it == data.begin())
				{
					min = it;
					continue;
				}
				else
				{
					if (*min > *it)
					{
						min = it;
					}
				}
			}
			sortdata.push_back(*min);
			data.erase(min);
		}
		for (int i = 0; i < sortdata.size(); i++)
		{
			cout << sortdata[i] << " ";
		}
	}



	void BubbleSort(vector<int> data)
	{
		bool flag = false;
		do 
		{
			flag = false;
			for (int i = 0; i != data.size()-1; i++)
			{
				if (data[i + 1] < data[i])
				{
					flag = true;
					int num = data[i];
					data[i] = data[i + 1];
					data[i + 1] = num;
				}
			}
		} while (flag);
		for (int i = 0; i < data.size(); i++)
		{
			cout << data[i] << " ";
		}
	}







	void QuickSort(vector<int> data, int begin, int end)
	{
		Quick(data, begin, end);
		Print(data);
	}



	void HeapSort(vector<int> data)
	{
		for (int i = (data.size() - 1) / 2; i >= 0; i--)
		{
			Adjust(data, i, data.size());
		}
		for (int i = data.size() - 1; i >= 0; i--)
		{
			int num = data[0];
			data[0] = data[i];
			data[i] = num;
			Adjust(data, 0, i);
		}
		Print(data);
	}


private:


	void Quick(vector<int> &data, int begin, int end)
	{
		if (begin < end)
		{
			int i, j;
			while (true)
			{
				for (i = begin + 1; i < end; i++)
				{
					if (data[i] > data[begin])
						break;
				}
				for (j = end - 1; j > begin; j--)
				{
					if (data[j] < data[begin])
						break;
				}
				if (i < j)
				{
					int num = data[i];
					data[i] = data[j];
					data[j] = num;
				}
				else
				{
					break;
				}
			}
				int num = data[begin];
				data[begin] = data[j];
				data[j] = num;
				Quick(data, begin, j);
				Quick(data, j + 1, end);
		}
	}



	void Print(vector<int> data)
	{
		for (int i = 0; i < data.size(); i++)
		{
			cout << data[i] << " ";
		}
	}



	void Adjust(vector<int> &data, int i, int size)
	{
		int j = i * 2 + 1;
		int num = data[i];
		while (j < size)
		{
			if (j < size - 1 && data[j] < data[j + 1])	//找到两个子节点中最大的数
				j++;
			if (num >= data[j])	//如果根节点比子节点大，则堆积完成
				break;
			data[(j - 1) / 2] = data[j];
			j = j * 2 + 1;
		}
		data[(j - 1) / 2] = num;
	}


};




int main()
{
	vector<int> data;
	int size;
	cout << "size:";
	cin >> size;
	int num;
	for (int i = 0; i < size; i++)
	{
		cin >> num;
		data.push_back(num);
	}
	Sort sort;
	cout << "插入：";
	sort.InsertSort(data);
	cout << endl;

	cout << "选择：";
	sort.SelectSort(data);
	cout << endl;

	cout << "冒泡：";
	sort.BubbleSort(data);
	cout << endl;

	cout << "快排：";
	sort.QuickSort(data, 0, size);
	cout << endl;

	cout << "堆排：";
	sort.HeapSort(data);
	cout << endl;
	system("pause");
    return 0;
}

