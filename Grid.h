#pragma once

#include <string>
#include <list>
#include <vector>

using namespace std;

enum alignment { unknown, axisAligned, clockwise, counter };

struct Point
{
	Point() { m_fX = 0; m_fY = 0; m_fRowIntercept = 0; m_fColumnIntercept = 0; }
	float m_fX, m_fY;
	float m_fRowIntercept, m_fColumnIntercept;
};

class Grid
{
public:
	Grid();
	bool addPoints(string& strFilePath);

private:
	vector<Point> m_vectorOfPoints;
	list<Point> m_listOfRows;
	list<Point> m_listOfColumns;
	float m_fSlopeRow;
	float m_fSlopeColumn;
	float m_fNumberOfRowsAndColumns;

	bool addPoint(string& strPoint);
	void calculateRowsAndColumns();
	bool isSameLine(float fNum1, float fNum2);
	void createRowsAndColumns(Point& point);
	void displayRowsColumns(bool bIsRow);
};