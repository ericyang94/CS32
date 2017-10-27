#ifndef DATABASE_DEFINED
#define DATABASE_DEFINED

#include "MultiMap.h"
#include <string>
#include <vector>

class Database
{
public:
	enum IndexType	{ it_none, it_indexed };
	enum OrderingType	{ ot_ascending, ot_descending };

	struct	FieldDescriptor
	{
		std::string	name;
		IndexType index;
	};
	
	struct SearchCriterion
	{
		std::string	fieldName;
		std::string	minValue;
		std::string	maxValue;
	};
	
	struct SortCriterion
	{
		std::string	fieldName;
		OrderingType ordering;
	};
	
	static const int	ERROR_RESULT = -1;
	
	Database();
	~Database();
	bool specifySchema(const std::vector<FieldDescriptor>& schema);
	bool addRow(const	std::vector<std::string>& rowOfData);
	bool loadFromURL(std::string url);
	bool loadFromFile(std::string filename);
	int	getNumRows() const;
	bool getRow(int rowNum, std::vector<std::string>& row) const;
	int	search(const std::vector<SearchCriterion>& searchCriteria,
				const std::vector<SortCriterion>& sortCriteria,
				std::vector<int>& results);
	
private:
	// To prevent Databases from being copied or assigned, declare these members
	// private and do not implement them.
	Database(const Database& other);
	Database& operator=(const Database&	rhs);
	std::vector<FieldDescriptor*> m_schema;
	std::vector<std::vector<std::string> > m_rows;
	std::vector<MultiMap*> m_fieldIndex;

	int doesABelongBeforeB(const int& a, const int& b, const std::vector<SortCriterion>& sortCriteria)
	{
		for(int k = 0; k != sortCriteria.size(); k++)
		{
			for(int i = 0; i != m_schema.size(); i++)
			{
				if(m_schema[i]->name == sortCriteria[k].fieldName)
				{
					if (m_rows[a][i] < m_rows[b][i] && sortCriteria[k].ordering == ot_ascending)
						return 2;
					if (m_rows[a][i] > m_rows[b][i] && sortCriteria[k].ordering == ot_ascending)
						return 1;
					if (m_rows[a][i] < m_rows[b][i] && sortCriteria[k].ordering == ot_descending)
						return 1;
					if (m_rows[a][i] > m_rows[b][i] && sortCriteria[k].ordering == ot_descending)
						return 2;
					break;
				}
			}
		}
		return 0;
	}

	void order(int a[], int n, const std::vector<SortCriterion>& sortCriteria)
	{
	    if (n <= 1)
			return;
		int firstNotGreater;
		int firstLess;
		int splitter = a[n/2];
		split(a,n,splitter,firstNotGreater,firstLess, sortCriteria);
		order(a,firstNotGreater, sortCriteria);
		order(a+firstLess, n-firstLess, sortCriteria);
	}

	void split(int a[], int n, int splitter, int& firstNotGreater, int& firstLess, const std::vector<SortCriterion>& sortCriteria)
	{
	    if (n < 0)
	        n = 0;

	    firstNotGreater = 0;
	    firstLess = n;
	    int firstUnknown = 0;
	    while (firstUnknown < firstLess)
	    {
                if (doesABelongBeforeB(a[firstUnknown], splitter, sortCriteria) == 1)
                {
                    firstLess--;
                    exchange(a[firstUnknown], a[firstLess]);
                }
                else
                {
                    if (doesABelongBeforeB(a[firstUnknown], splitter, sortCriteria) == 2)
                    {
                        exchange(a[firstNotGreater], a[firstUnknown]);
                        firstNotGreater++;
                    }
                    firstUnknown++;
                }
	    }
	}

	void exchange(int& x, int& y)
	{
	    int t = x;
	    x = y;
	    y = t;
	}
};

#endif