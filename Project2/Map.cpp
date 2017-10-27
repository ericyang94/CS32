//Map.cpp

#include "Map.h"

Map::Map()
{
	m_head = m_tail = NULL;
	m_size = 0;
}

Map::Map(const Map& other)
{
	m_size = other.m_size;
	m_head = m_tail = NULL;

	for(Node* b = other.m_head; b != NULL; b = b->m_next)	//go through every Node in other map
	{
		Node* New = new Node;	//dynamically create a Node copied from other map
		New->m_key = b->m_key;
		New->m_value = b->m_value;
		New->m_next = NULL;
		New->m_prev = NULL;
		m_tail = New;	//place at end of list
		if (m_head == NULL)	//if the list is empty
			m_head = New;
		else
		{
			Node* a = m_head;
			while (a->m_next != NULL)	//go through Nodes in new list until last Node
				a = a->m_next;
			a->m_next = New;
			New->m_prev = a;
		}
	}
}

Map::~Map()
{
    Node* temp = m_head;
	while(temp != NULL)
	{
		Node* next;
		next = temp->m_next;
		delete temp;
		temp = next;
	}
}

Map& Map::operator=(const Map& other)
{
    if (this != &other)
    {
		Node* temp = m_head;	//delete all the nodes in original map first
		while(temp != NULL)
		{
			Node* next;
			next = temp->m_next;
			delete temp;
			temp = next;
		}

		m_size = other.m_size;
		m_head = m_tail = NULL;

		for(Node* b = other.m_head; b != NULL; b = b->m_next)	//use same operations as copy constructor
		{
			Node* New = new Node;
			New->m_key = b->m_key;
			New->m_value = b->m_value;
			New->m_next = NULL;
			New->m_prev = NULL;
			m_tail = New;
			if (m_head == NULL)
				m_head = New;
			else
			{
				Node* a = m_head;
				while (a->m_next != NULL)
					a = a->m_next;
				a->m_next = New;
				New->m_prev = a;
			}
		}
    }
    return *this;
}

bool Map::empty() const
{
    return (m_size == 0);
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	for (Node* a = m_head; a != NULL; a = a->m_next)	//check if the key already exists
	{
		if (key == a->m_key)
			return false;
	}

	Node* New = new Node;	//creates a new Node dynamically and places it at the end of the list
	New->m_key = key;
	New->m_value = value;
	New->m_next = NULL;
	New->m_prev = m_tail;
	if (m_tail != NULL)
		m_tail->m_next = New;
	m_tail = New;
	if (m_head == NULL)
		m_head = New;
	m_size++;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	Node* a = m_head; 
	
	while (a != NULL)	//find the Node that has the key in parameter
	{
		if (key == a->m_key)
			break;
		a = a->m_next;
	}
	if (a == NULL)	//return false if the key wasn't found
		return false;

	a->m_value = value;
	return true;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (update(key,value))	//try to update the list first
		return true;
	else
		return (insert(key,value));	//if updating fails, insert at end
}

bool Map::erase(const KeyType& key)
{
	Node* a = m_head;
	
	while (a != NULL)	//look for through list for the matching key
	{
		if (key == a->m_key)
			break;
		a = a->m_next;
	}
	if (a == NULL)	//return false if the key wasn't found
		return false;
	if (a == m_head)	//check if first Node
		m_head = m_head->m_next;
	else
		a->m_prev->m_next = a->m_next;
	if (a == m_tail)
		m_tail = m_tail->m_prev;	//check if last Node
	else
		a->m_next->m_prev = a->m_prev;
	delete a;
    m_size--;
    return true;
}

bool Map::contains(const KeyType& key) const
{
	for (Node* a = m_head; a != NULL; a = a->m_next)	//look through every Node for matching key
	{
		if (key == a->m_key)
			return true;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	Node* a = m_head;
	
	while (a != NULL)
	{
		if (key == a->m_key)
			break;
		a = a->m_next;
	}
	if (a == NULL)
		return false;

	value = a->m_value;
	return true;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i < 0  ||  i >= m_size)	//check for valid parameter
        return false;

	Node* a = m_head;

	for (int k = i; k != 0; k--)	//go to (i+1)th Node from beginning of list
		a = a->m_next;
    key = a->m_key;
    value = a->m_value;
    return true;
}

void Map::swap(Map& other)
{
	Node* tempHead = m_head;	//swap the m_head pointers using temporary pointer
	m_head = other.m_head;
	other.m_head = tempHead;
	
	Node* tempTail = m_tail;	//swap the m_tail pointers using temporary pointer
	m_tail = other.m_tail;
	other.m_tail = tempTail;

	int t = m_size;	////swap the size
	m_size = other.m_size;
	other.m_size = t;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
	bool isTrue = true;	//true if there are no key pairs with different values

	KeyType tempKey;
	ValueType tempValue1, tempValue2;

	result = m1;	//erase all result Nodes and insert all Nodes from m1 to result

	for(int k = 0; k!= m2.size(); k++)	//go through all Nodes in m2
	{
		m2.get(k, tempKey, tempValue1);
		if (result.contains(tempKey))
		{
			result.get(tempKey, tempValue2);
			if (tempValue1 != tempValue2)	//check if the values are different for the same key
			{
				isTrue = false;	//set false because values differ and erase Node from result
				result.erase(tempKey);
			}
		}
		else
			result.insert(tempKey, tempValue1);	//insert Node into result if key not in m1
	}

	return isTrue;
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
	KeyType tempKey;
	ValueType tempValue;

	result = m1;	//erase all result Nodes and insert all Nodes from m1 to result

	for(int k = 0; k!= m2.size(); k++)	//go through all Nodes in m2
	{
		m2.get(k, tempKey, tempValue);
		if (result.contains(tempKey))	//erase the Node if keys match
			result.erase(tempKey);
	}
}