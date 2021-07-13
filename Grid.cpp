#include "Grid.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

Grid::Grid()
{
	m_fSlopeRow = 0;
	m_fSlopeColumn = 0;
	m_fNumberOfRowsAndColumns = 0;
}

bool Grid::addPoints(string& strFilePath)
{
	string strLine;
	ifstream inFile;
	int iNumberOfPoints = 0;
	
	inFile.open(strFilePath);
	if (!inFile)
	{
		cout << "Failed to open " << strFilePath << "\n";
		return false;
	}

	while (getline(inFile, strLine))
	{
		if (!addPoint(strLine))
		{
			cout << "Failed to add point " << strLine << "\n";
			inFile.close();
			return false;
		}
		else
		{
			iNumberOfPoints++;			
		}

	}
	inFile.close();

	//Check to make sure the Grid is square	
	m_fNumberOfRowsAndColumns = sqrt(iNumberOfPoints);
	if (floor(m_fNumberOfRowsAndColumns) != m_fNumberOfRowsAndColumns)
	{
		cout << "The grid is not square!\n";
		return false;
	}

	calculateRowsAndColumns();
		
    return true;
}

bool Grid::addPoint(string& strPoint)
{	
	stringstream stringStream(strPoint);
	string segment;	
	vector<string> vector;
	Point point;

	while (getline(stringStream, segment, ','))
	{
		vector.push_back(segment);
	}

	if (vector.size() != 2)
		return false;

	point.m_fX = stof(vector[0]);
	point.m_fY = stof(vector[1]);

	m_vectorOfPoints.push_back(point);
	
	return true;
}

void Grid::calculateRowsAndColumns()
{
	int iMinXPointIndex = 0;
	int iMaxXPointIndex = 0;
	int iMinYPointIndex = 0;
	int iMaxYPointIndex = 0;	
	float fMinX = 0;
	float fMaxX = 0;
	float fMinY = 0;
	float fMaxY = 0;
	int iCurrentIndex = 0;
	alignment aligned = unknown;
	float fOldX = 0;	
	float fAngle = 0;

	//Determine if alignment is axisAligned along with the leftmost, rightmost, topmost, and downmost
	//points
	
	for (auto const& i : m_vectorOfPoints) {
		if (iCurrentIndex == 0)
		{
			fMinX = i.m_fX;
			fMaxX = i.m_fX;
			fMinY = i.m_fY;
			fMaxY = i.m_fY;

			fOldX = i.m_fX;
		}
		else
		{
			if (i.m_fX < fMinX)
			{
				fMinX = i.m_fX;
				iMinXPointIndex = iCurrentIndex;
			}

			if (i.m_fX > fMaxX)
			{
				fMaxX = i.m_fX;
				iMaxXPointIndex = iCurrentIndex;
			}

			if (i.m_fY < fMinY)
			{
				fMinY = i.m_fY;
				iMinYPointIndex = iCurrentIndex;
			}

			if (i.m_fY > fMaxY)
			{
				fMaxY = i.m_fY;
				iMaxYPointIndex = iCurrentIndex;
			}

			if (fOldX == i.m_fX)
			{
				aligned = axisAligned;
			}
		}

		iCurrentIndex++;
	}

	/*cout << "LEFT: " << fMinX << "\n";
	cout << "POSITION: " << iMinXPointIndex << "\n";

	cout << "RIGHT: " << fMaxX << "\n";
	cout << "POSITION: " << iMaxXPointIndex << "\n";

	cout << "DOWN: " << fMinY << "\n";
	cout << "POSITION: " << iMinYPointIndex << "\n";
	
	cout << "UP: " << fMaxY << "\n";
	cout << "POSITION: " << iMaxYPointIndex << "\n";*/

	if (aligned == axisAligned)
	{
		m_fSlopeColumn = 0;
		m_fSlopeRow = 0;

		//cout << "ALIGNED: axisAligned, " << "Slope Row: " << m_fSlopeRow << ", Slope Column: " << m_fSlopeColumn << "\n";
	}
	else
	{
		if (m_vectorOfPoints[iMinXPointIndex].m_fY < m_vectorOfPoints[iMaxXPointIndex].m_fY)
		{
			aligned = clockwise;
			
			m_fSlopeColumn = ((m_vectorOfPoints[iMaxYPointIndex].m_fY - m_vectorOfPoints[iMinXPointIndex].m_fY) / (m_vectorOfPoints[iMaxYPointIndex].m_fX - m_vectorOfPoints[iMinXPointIndex].m_fX));
			m_fSlopeRow = ((m_vectorOfPoints[iMinYPointIndex].m_fY - m_vectorOfPoints[iMinXPointIndex].m_fY) / (m_vectorOfPoints[iMinYPointIndex].m_fX - m_vectorOfPoints[iMinXPointIndex].m_fX));

			//cout << "ALIGNED: clockwise, " << "Slope Row: " << m_fSlopeRow << ", Slope Column: " << m_fSlopeColumn << "\n";
		}
		else
		{
			aligned = counter;			

			m_fSlopeRow = ((m_vectorOfPoints[iMaxYPointIndex].m_fY - m_vectorOfPoints[iMinXPointIndex].m_fY) / (m_vectorOfPoints[iMaxYPointIndex].m_fX - m_vectorOfPoints[iMinXPointIndex].m_fX));			
			m_fSlopeColumn = ((m_vectorOfPoints[iMinYPointIndex].m_fY - m_vectorOfPoints[iMinXPointIndex].m_fY) / (m_vectorOfPoints[iMinYPointIndex].m_fX - m_vectorOfPoints[iMinXPointIndex].m_fX));

			//cout << "ALIGNED: counter, " << "Slope Row: " << m_fSlopeRow << ", Slope Column: " << m_fSlopeColumn << "\n";
		}		
	}

	for (auto &i : m_vectorOfPoints)
	{		
		if (m_fSlopeRow == 0)
		{
			i.m_fRowIntercept = -i.m_fY;
		}
		else
		{
			i.m_fRowIntercept = -(i.m_fY - (m_fSlopeRow * i.m_fX));
		}

		if (m_fSlopeColumn == 0)
		{
			i.m_fColumnIntercept = i.m_fX;
		}
		else if (aligned == counter)
		{
			i.m_fColumnIntercept = i.m_fY - (m_fSlopeColumn * i.m_fX);
		}
		else
		{
			i.m_fColumnIntercept = -(i.m_fY - (m_fSlopeColumn * i.m_fX));
		}		

		//cout << "x: " << i.m_fX << ", y: " << i.m_fY << ", Intercept Row: " << i.m_fRowIntercept << ", Intercept Column: " << i.m_fColumnIntercept << "\n";

		createRowsAndColumns(i);
	}

	displayRowsColumns(true);
	displayRowsColumns(false);

	fAngle = atan(m_fSlopeRow) * (180 / 3.14159265358979323846);
	cout << "Alpha=" << fAngle << " degrees";
}

bool Grid::isSameLine(float fNum1, float fNum2)
{
	float fBigNum = 0;
	float fSmallNum = 0;
	float fResult = 0;

	if (fNum1 == fNum2)
	{
		return true;
	}		
	else
	{
		if (fNum1 > fNum2)
		{
			fBigNum = fNum1;
			fSmallNum = fNum2;
		}
		else
		{
			fBigNum = fNum2;
			fSmallNum = fNum1;
		}

		fResult = (fBigNum - fSmallNum) / fBigNum;
		fResult = abs(fResult);

		if (fResult < 0.01)
			return true;
		else
			return false;
	}
}

void Grid::createRowsAndColumns(Point& point)
{
	bool bInsertedPoint = false;

	if (m_listOfRows.empty())
	{
		m_listOfRows.push_back(point);
	}
	else
	{
		bInsertedPoint = false;
		for (list<Point>::iterator it = m_listOfRows.begin(); it != m_listOfRows.end(); ++it) {
			if (isSameLine(point.m_fRowIntercept, it->m_fRowIntercept))
			{
				if (point.m_fColumnIntercept < it->m_fColumnIntercept)
				{
					m_listOfRows.insert(it, point);
					bInsertedPoint = true;
					break;
				}
			}
			else if (point.m_fRowIntercept < it->m_fRowIntercept)
			{
				m_listOfRows.insert(it, point);
				bInsertedPoint = true;
				break;
			}
		}

		if (!bInsertedPoint)
		{
			m_listOfRows.push_back(point);
		}
	}

	if (m_listOfColumns.empty())
	{
		m_listOfColumns.push_back(point);
	}
	else
	{
		bInsertedPoint = false;
		for (list<Point>::iterator it = m_listOfColumns.begin(); it != m_listOfColumns.end(); ++it) {
			if (isSameLine(point.m_fColumnIntercept, it->m_fColumnIntercept))
			{
				if (point.m_fRowIntercept < it->m_fRowIntercept)
				{
					m_listOfColumns.insert(it, point);
					bInsertedPoint = true;
					break;
				}
			}
			else if (point.m_fColumnIntercept < it->m_fColumnIntercept)
			{
				m_listOfColumns.insert(it, point);
				bInsertedPoint = true;
				break;
			}
		}

		if (!bInsertedPoint)
		{
			m_listOfColumns.push_back(point);
		}
	}
}

void Grid::displayRowsColumns(bool bIsRow)
{
	int iCount = 1;
	int iNumber = 0;
	bool bNew = true;
	bool bPrint = false;
	vector<Point> tempVector;
	list<Point>* pList = NULL;
	string strLabel;

	if (bIsRow)
	{
		pList = &m_listOfRows;
		strLabel = "Row ";
	}
	else
	{
		pList = &m_listOfColumns;
		strLabel = "Col ";
	}

	for (auto& point : *pList)
	{
		tempVector.push_back(point);

		if ((iCount % (int)m_fNumberOfRowsAndColumns) == 0)
		{
			bPrint = true;
		}

		if (bPrint) {
			bPrint = false;
			bNew = true;
			for (auto& groupPoint : tempVector)
			{
				if (bNew)
				{
					bNew = false;
					cout << strLabel << iNumber << ": " << groupPoint.m_fX << "," << groupPoint.m_fY;
				}
				else
				{
					cout << " - " << groupPoint.m_fX << "," << groupPoint.m_fY;
				}
			}

			cout << "\n";
			iNumber++;
			tempVector.clear();
		}

		iCount++;
	}
}
