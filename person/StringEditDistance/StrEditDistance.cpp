#include "StrEditDistance.h"
#include <iostream>
#include <iomanip>

#define MATCH		2
#define MISS_MATCH	-1
#define INSERT		-1
#define DELETE		-1

CStrEditDistance::CStrEditDistance(std::string& vStrRow, std::string& vStrColumn)
{
	process(vStrRow, vStrColumn);
}

CStrEditDistance::~CStrEditDistance(void)
{
}

/***********************************************************************/
//FUNCTION:
void CStrEditDistance::process(const std::string& vStrRow, const std::string& vStrColumn)
{
	int editDis = 0;					//编辑距离
	int row = vStrColumn.length();		
	int column = vStrRow.length();
	const int sizeR = row + 1;
	const int sizeC = column + 1;

	int **pScore = new int*[sizeR];		//二维指针
	for (int i = 0; i <= row; i++)
		pScore[i] = new int[sizeC];

	//初始化第一行和第一列
	for (int c = 0; c <= column; c++)
		pScore[0][c] = 0 - c;
	for (int r = 0; r <= row; r++)
		pScore[r][0] = 0 - r;

	//从v(1,1)开始每列计算
	for (int c = 1; c <= column; c++)
	{
		for (int r = 1; r <= row; r++)
		{
			//计算v(i,j), 其值等于A:v(i-1,j)+insert、B:v(i,j-1)+delet e、C:v(i-1,j-1)+@(i,j)中的最大的一个 
			int valueMatch;
			if (vStrColumn[r-1] == vStrRow[c-1])
 				valueMatch = MATCH;
			else
				valueMatch = MISS_MATCH;   
			int A = pScore[r-1][c] + INSERT;
			int B = pScore[r][c-1] + DELETE;
			int C = pScore[r-1][c-1] + valueMatch;
			pScore[r][c] = getMaxValue(A, B, C);
		}
	}

	//计算编辑距离
	int r = row, c = column;
	while(r > 0 && c > 0)
	{
		if (pScore[r][c]+1 == pScore[r-1][c])	    { editDis++; r--; continue; }
		else if (pScore[r][c]+1 == pScore[r][c-1])  { editDis++; c--; continue; }
		else if (pScore[r][c]+1 == pScore[r-1][c-1]){ editDis++; r--; c--; continue; }
		else { r--; c--; }
	}
	if (r > 0 && c == 0)		editDis += r;
	else if (c > 0 && r == 0)	editDis += c;

	std::cout << std::endl;
	//----------------DEBUG-------------------//
	//打印中间数据
 	for (int i = 0; i <= row; i++)
 	{
 		for (int j = 0; j <= column; j++)
			std::cout << std::setw(2) << pScore[i][j] << "  ";
	    std::cout << std::endl;
 	}
	std::cout << std::endl;

	//设置编辑距离和得分
	setEditDis(editDis);
	setScore(pScore[row][column]);

	for (int i = 0; i <= row; i++)			//释放内存
	{
		delete pScore[i];
		pScore[i] = NULL;
	}
	delete[] pScore;
}