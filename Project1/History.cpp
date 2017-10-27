#include <iostream>
using namespace std;

#include "History.h"
#include "globals.h"

History::History(int nRows, int nCols)
{
	m_row = nRows;
	m_col = nCols;
	for (int r = 1; r <= nRows; r++)
		for (int c = 1; c <= nCols; c++)
			displayGrid[r-1][c-1] = '.';
}

bool History::record(int r, int c)
{
    if (r >= 1  &&  r <= m_row  &&  c >= 1  &&  c <= m_col)
	{
		if (displayGrid[r-1][c-1] == '.')
			displayGrid[r-1][c-1] = 'A';
		else if (displayGrid[r-1][c-1] >= 'A' && displayGrid[r-1][c-1] <= 'Z')
			displayGrid[r-1][c-1]++;
		return true;
	}
	else return false;
}

void History::display() const
{
	clearScreen();

    for (int r = 1; r <= m_row; r++)
    {
        for (int c = 1; c <= m_col; c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}