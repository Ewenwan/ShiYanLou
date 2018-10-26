#pragma once
#include <string>

class CStrEditDistance
{
public:
	CStrEditDistance(std::string& vStrRow, std::string& vStrColumn);
	~CStrEditDistance(void);

	int  getScore()				{ return m_Score;   }
	int  getEditDis()			{ return m_EditDis; }
	void setEditDis(int vDis)	{ m_EditDis = vDis; }
	void setScore(int vScore)	{ m_Score = vScore; }
private:
	void process(const std::string& vStrRow, const std::string& vStrColumn);
	int getMaxValue(int a, int b, int c)
	{
		if (a < b){ if (b < c) return c; return b; }
		else { if (b > c) return a; return a < c ? c : a; }
	}
private:
	int			m_EditDis;
	int			m_Score;
};