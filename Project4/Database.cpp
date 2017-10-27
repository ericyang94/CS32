#include "Database.h"
#include "http.h"
#include "Tokenizer.h"
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

Database::Database()
{
}

Database::~Database()
{
	//clear m_rows
	m_rows.clear();
	//clear m_fieldIndexes
	for (size_t k = 0; k < m_fieldIndex.size(); k++)
	{
		delete m_fieldIndex[k];
	}
	m_fieldIndex.clear();
	//clear m_schema;
	for (size_t k = 0; k < m_schema.size(); k++)
	{
		delete m_schema[k];
	}
	m_schema.clear();
}

bool Database::specifySchema(const vector<FieldDescriptor>&	schema)
{
	for (unsigned int k = 0; k < m_fieldIndex.size(); k++)	//delete previous rows and indexes
	{
		delete m_fieldIndex[k];
	}

	for (unsigned int k = 0; k < m_schema.size(); k++)	//delete previous schema
	{
		delete m_schema[k];
	}
	m_schema.clear();
	m_rows.clear();
	m_fieldIndex.clear();

	bool atLeastOneIndexed = false;

	for (unsigned int k = 0; k < schema.size(); k++)	//copying schema over to data member vector
	{
		FieldDescriptor* current = new FieldDescriptor;
		current->index = schema[k].index;
		current->name = schema[k].name;
		m_schema.push_back(current);
		MultiMap* map = new MultiMap;
		m_fieldIndex.push_back(map);
		if (schema[k].index == it_indexed)	//sets true if there is at least one indexed field in schema
			atLeastOneIndexed = true;
	}

	return atLeastOneIndexed;
}

bool Database::addRow(const	vector<string>&	rowOfData)
{
	if (rowOfData.size() != m_schema.size())	//check if same number of fields as schema
		return false;
	
	m_rows.push_back(rowOfData);

	for (unsigned int k = 0; k < m_schema.size(); k++)
	{
		if (m_schema[k]->index == it_indexed)	//check if the field is indexed
		{
			m_fieldIndex[k]->insert(rowOfData[k], m_rows.size() - 1);	//insert Node to Multimap of current field
		}
	}

	return true;
}

bool Database::loadFromURL(string url)
{
	string page;
	if (HTTP().get(url, page))
	{
		Tokenizer t(page, "\n");
		string w;
		vector<string> row;
		vector<FieldDescriptor> schema;

		if (t.getNextToken(w))	//get the schema, which is the first line
		{
			FieldDescriptor temp;
			Tokenizer s(w,",");
			string r;
			while (s.getNextToken(r))	//split the string into fields
			{
				if (w[w.size() - 1] == '*')	//check if field is indexed
				{
					temp.index = it_indexed;
					w = w.substr(0, w.size() - 1);
				}
				else temp.index = it_none;

				temp.name = w;
				schema.push_back(temp);	//push field onto schema
			}

			if(!specifySchema(schema))	//create schema
				return false;
		}

		while (t.getNextToken(w))	//get all the rows of data
		{
			Tokenizer s(w,",");
			string r;
			while (s.getNextToken(r))	//split each row into fields
			{
				row.push_back(w);
			}
			if(addRow(row))	//attempt to add the vector of strings to database
				row.clear();
			else
				return false;
		}
		return true;
	}
	else
		return false;
}

bool Database::loadFromFile(string filename)
{
	ifstream page(filename);

	if (!page)	//check if loading was successful
		return false;

	vector<string> row;
	vector<FieldDescriptor> schema;
	string w;

	if (!page)
	{
		getline(page,w);	//get the schema, which is the first line

		FieldDescriptor temp;
		Tokenizer s(w,",");
		string r;
		while (s.getNextToken(r))	//split the string into fields
		{
			if (w[w.size() - 1] == '*')	//check if field is indexed
			{
				temp.index = it_indexed;
				w = w.substr(0, w.size() - 1);
			}
			else temp.index = it_none;

			temp.name = w;
			schema.push_back(temp);	//push field onto schema
		}

		if(!specifySchema(schema))	//create schema
			return false;
	}

	while (!page)	//get all the rows of data
	{
		getline(page,w);
		Tokenizer s(w,",");
		string r;
		while (s.getNextToken(r))	//split each row into fields
		{
			row.push_back(w);
		}
		if(addRow(row))	//attempt to add the vector of strings to database
			row.clear();
		else
			return false;
	}

	return true;
}

int	Database::getNumRows() const 
{ 
	return m_rows.size(); 
}

bool Database::getRow(int rowNum, vector<string>& row)	const
{
	if (rowNum < 0 || rowNum >= getNumRows())
		return false;
	row.clear();

	for (unsigned int k = 0; k < m_rows[rowNum].size(); k++)
	{
		row.push_back(m_rows[rowNum][k]);
	}
	return true;
}

int	Database::search(const vector<SearchCriterion>&	searchCriteria,
	const vector<SortCriterion>& sortCriteria, vector<int>&	results)
{
	if (searchCriteria.size() == 0)	//check if there is search criteria
	{
		results.clear();
		return ERROR_RESULT;
	}

	results.clear();

	vector<int> resultRows;

	for (unsigned int k = 0; k < searchCriteria.size(); k++)
	{
		if (searchCriteria[k].maxValue == "" && searchCriteria[k].minValue == "")	//check for invalid search criteria
		{
			results.clear();
			return ERROR_RESULT;
		}

		bool indexed = false;
		int indexNum = 0;
		for (unsigned int i = 0; i < m_schema.size(); i++)	//check if search criteria is indexed field;
		{
			if (m_schema[i]->name == searchCriteria[k].fieldName)
			{
				if (m_schema[i]->index == it_indexed)
				{
					indexed = true;
					indexNum = i;
				}
				break;
			}
		}
		if (indexed == false)	//not an indexed search
		{
			results.clear();
			return ERROR_RESULT;
		}
		
		MultiMap::Iterator max;
		MultiMap::Iterator min;

		if (searchCriteria[k].minValue == "")	//if no min input
		{
			max = m_fieldIndex[indexNum]->findEqualOrPredecessor(searchCriteria[k].maxValue);
			while(max.valid())
			{
				bool conflict = false;
				for(int j = 0; j != resultRows.size(); j++)
				{
					if (resultRows[j] == max.getValue())
						conflict = true;
				}
				if (conflict != true)
					resultRows.push_back(max.getValue());
				max.prev();
			}
		}
		
		else if (searchCriteria[k].maxValue == "")	//if no max input
		{
			min = m_fieldIndex[indexNum]->findEqualOrSuccessor(searchCriteria[k].minValue);
			while(min.valid())
			{
				bool conflict = false;
				for(int j = 0; j != resultRows.size(); j++)
				{
					if (resultRows[j] == min.getValue())
						conflict = true;
				}
				if (conflict != true)
					resultRows.push_back(min.getValue());
				min.next();
			}	
		}

		else
		{
			max = m_fieldIndex[indexNum]->findEqualOrPredecessor(searchCriteria[k].maxValue);
			min = m_fieldIndex[indexNum]->findEqualOrSuccessor(searchCriteria[k].minValue);
			while(min.valid() && min.getKey() <= max.getKey())
			{
				bool conflict = false;
				for(int j = 0; j != resultRows.size(); j++)
				{
					if (resultRows[j] == min.getValue())
						conflict = true;
				}
				if (conflict != true)
					resultRows.push_back(min.getValue());
				min.next();
			}
		}
	}

	int resultArray[100000];
	for(int r = 0; r != resultRows.size(); r++)
		resultArray[r] = resultRows[r];

	order(resultArray,resultRows.size(), sortCriteria);

	for(int r = 0; r != resultRows.size(); r++)
		results.push_back(resultArray[r]);

	return results.size();
}