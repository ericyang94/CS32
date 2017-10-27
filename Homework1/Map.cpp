#include "Map.h"
#include <string>
using namespace std;

Map::Map()
{
	m_counter = 0;
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
	if (m_counter != DEFAULT_MAX_ITEMS)
	{
		for (int k = 0; k != m_counter; k++)
		{
			if (m_duo[k].m_key == key)
				return false;			
		}
		m_duo[m_counter].m_key = key;
		m_duo[m_counter].m_value = value;
		m_counter++;
		return true;
	}
	else return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int k = 0; k != m_counter; k++)
	{
		if (m_duo[k].m_key == key)
		{
			m_duo[k].m_value = value;
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
		if (m_duo[k].m_key == key)
		{
			for (int i = k; i != m_counter - 1; i++)
			{
				m_duo[i].m_key = m_duo[i + 1].m_key;
				m_duo[i].m_value = m_duo[i + 1].m_value;
			}
			m_counter--;
			return true;
		}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (int k = 0; k != m_counter; k++)
		if (m_duo[k].m_key == key)
			return true;
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	for (int k = 0; k != m_counter; k++)
		if (m_duo[k].m_key == key)
		{
			value = m_duo[k].m_value;
			return true;
		}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i >= 0 && i < m_counter)
	{
		value = m_duo[i].m_value;
		key = m_duo[i].m_key;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
{
	KeyType x;
	ValueType y;
	int z;
	int k = 0;
	for (; k != DEFAULT_MAX_ITEMS && k != m_counter && k != other.m_counter; k++)
	{
		x = m_duo[k].m_key;
		y = m_duo[k].m_value;
		m_duo[k].m_key = other.m_duo[k].m_key;
		m_duo[k].m_value = other.m_duo[k].m_value;
		other.m_duo[k].m_key = x;
		other.m_duo[k].m_value = y;
	}
	for (; k != DEFAULT_MAX_ITEMS && k != m_counter; k++)
	{
		m_duo[k].m_key = other.m_duo[k].m_key;
		m_duo[k].m_value = other.m_duo[k].m_value;
	}
	for (; k != DEFAULT_MAX_ITEMS && k != other.m_counter; k++)
	{
		other.m_duo[k].m_key = m_duo[k].m_key;
		other.m_duo[k].m_value = m_duo[k].m_value;
	}

	z = m_counter;
	m_counter = other.m_counter;
	other.m_counter = z;
}