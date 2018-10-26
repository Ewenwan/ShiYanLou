#include "StrEditDistance.h"
#include <iostream>
#include <stdlib.h>
#define MAX_SIZE	100

int main(int, char **)
{
	char sRow[MAX_SIZE];
	char sColumn[MAX_SIZE];

	std::cout << "input row string : ";
	std::cin.getline(sRow, MAX_SIZE);
	std::cout << "input column str : ";
	std::cin.getline(sColumn, MAX_SIZE);
 
	std::string strRow(sRow), strColumn(sColumn);
  	CStrEditDistance *pStrEdit = new CStrEditDistance(strRow, strColumn);
	std::cout << "The score is : " << pStrEdit->getScore() << std::endl;
	std::cout << "The edit distance is : " << pStrEdit->getEditDis() << std::endl;
  	delete pStrEdit;

	system("pause");
	return 0;
}
