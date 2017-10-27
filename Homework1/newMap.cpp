#include "newMap.h"
#include <string>
using namespace std;

Map::Map()
{
	m_counter = 0;
	m_max = DEFAULT_MAX_ITEMS;
	m_duo = new Duo[m_max];
}

Map::Map(int capacity)
{
	m_counter = 0;
	m_max = capacity;
	m_duo = new Duo[m_max];
}

Map::Map(const Map &oldMap)
{
	m_counter = oldMap.m_counter;
	m_max = oldMap.m_max;
	m_duo = new Duo[m_max];
	for (int k = 0; k != m_max; k++)
		m_duo[k] = oldMap.m_duo[k];
}

Map &Map::operator=(const Map &other)
{
	if (&other == this)
		return(*this);
	delete [] m_duo;
	m_counter = other.m_counter;
	m_max = other.m_max;
	m_duo = new Duo[m_max];
	for (int k = 0; k != m_max; k++)
		m_duo[k] = other.m_duo[k];
	return(*this);
}

Map::~Map()
{
    delete [] m_duo;
}

bool Map::empty() const
{
	if (m_counter == 0)
		return true;
	else return false;
}

int Map::size() const
{
	return m_counter;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (m_counter != m_max)
	{
		for (int k = 0; k != m_counter; k++)
		{
			if ((m_duo+k)->m_key == key)
				return false;			
		}
		(m_duo + m_counter)->m_key = key;
		(m_duo + m_counter)->m_value = value;
		m_counter++;
		return true;
	}
	else return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int k = 0; k != m_counter; k++)
	{
		if ((m_duo+k)->m_key == key)
		{
			(m_duo+k)->m_value = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key,value) == true)
		return true;
	else return insert(key,value);
}

bool Map::erase(const KeyType& key)
{
	for (int k = 0; k != m_counter; k++)
		if ((m_duo+k)->m_key == key)
		{
			for (int i = k; i != m_counter - 1; i++)
			{
				*(m_duo+i) = *(m_duo+i+1);
			}
			delete (m_duo+m_counter);
			m_counter--;
			return true;
		}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (int k = 0; k != m_counter; k++)
		if ((m_duo+k)->m_key == key)
			return true;
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int k = 0; k != m_counter; k++)
		if ((m_duo+k)->m_key == key)
		{
			value = (m_duo+k)->m_value;
			return true;
		}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < m_counter)
	{
		value = (m_duo+i)->m_value;
		key = (m_duo+i)->m_key;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
{
	Duo* temp;
	int z;

	temp = m_duo;
	m_duo = other.m_duo;
	other.m_duo = temp;

	z = m_max;
	m_max = other.m_max;
	other.m_max = z;

	z = m_counter;
	m_counter = other.m_counter;
	other.m_counter = z;
}