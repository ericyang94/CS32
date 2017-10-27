#include "MultiMap.h"
#include<string>
using namespace std;

MultiMap::MultiMap()
{
	m_root = NULL;
}

MultiMap::~MultiMap()
{
	clear();
}

void MultiMap::clear()
{
	recursiveClear(m_root);	//call recursive function to delete Nodes starting from root
}

void MultiMap::insert(std::string  key, unsigned int value)
{
	if (m_root == NULL)	//if the tree is empty, insert Node at root
	{
		m_root = new Node(key,value);
		return;
	}

	Node* current = m_root;
	while(true)	// repeat while we haven't inserted the new Node
	{
		if(current->m_key == key)	//if the key is already in the map
		{
			ValueList* temp = current->m_valueList;
			while(temp->m_next != NULL)
				temp = temp->m_next;
			temp->m_next = new ValueList(value);	//add the value to the existing Node's value list
			temp->m_next->m_prev = temp;
			return;
		}

		if (current->m_key > key)	//move to the left child if less than current Node's key
		{
			if(current->m_left != NULL)
				current = current->m_left;
			else
			{
				current->m_left = new Node(key,value);	//add the new Node as left child if there isn't one
				current->m_left->m_parent = current;
				return;
			}
		}

		if (current->m_key < key)	//move to the right child if greater than current Node's key
		{
			if(current->m_right != NULL)
				current = current->m_right;
			else
			{
				current->m_right = new Node(key,value);	//add the new Node as right child if there isn't one
				current->m_right->m_parent = current;
				return;
			}
		}
	}
}

MultiMap::Iterator MultiMap::findEqual(std::string  key) const
{
	Iterator it(m_root);	//use two Iterators for the current Node and previous Node
	Iterator it2(m_root);
	Iterator it3;	//invalid Iterator to return invalid state
	bool next = false; 
	bool prev = false;
	while(it.valid())
	{
		if(it.prev())	//if there is a previous node, set a pointer to it
		{
			it2 = it;
			it.next();
		}
		else
		{
			it = it2;
			it2.prev();	//set previous node pointer invalid if there isn't a previous node
		}
		if(it.getKey()==key)
		{
			if(it2.valid())
			{
				if(it.getKey() == it2.getKey())	//check if the key occurs earlier than the current node
					it.prev();
				else
					return it;
			}
			else return it;
		}
		if(key > it.getKey())
		{
			if (prev == true)
				return it3;
			next = true;
			it.next();
		}
		else
		{
			if (next == true)
				return it3;
			prev = true;
			it.prev();
		}
	}
	return it3;	//return invalid Iterator if key not found
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(std::string  key) const
{
	Iterator it(m_root);	//use two Iterators for the current Node and previous Node
	Iterator it2(m_root);
	Iterator it3;	//invalid Iterator to return invalid state
	while(it.valid())
	{
		if(it.prev())	//if there is a previous node, set a pointer to it
		{
			it2 = it;
			it.next();
		}
		else
		{
			it = it2;
			it2.prev();	//set previous node pointer invalid if there isn't a previous node
		}
		if(it.getKey()>=key)
		{
			if(it2.valid())
			{
				if(it2.getKey() >= key)	//check if there is earlier greater than association
					it.prev();
				else
					return it;
			}
			else return it;
		}
		else
			it.next();
	}
	return it3;	//return invalid Iterator if key not found
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(std::string  key) const
{
	Iterator it(m_root);	//use two Iterators for the current Node and previous Node
	Iterator it2(m_root);
	Iterator it3;	//invalid Iterator to return invalid state
	while(it.valid())
	{
		if(it.next())	//if there is a next node, set a pointer to it
		{
			it2 = it;
			it.prev();
		}
		else
		{
			it = it2;
			it2.next();	//set next node pointer invalid if there isn't a next node
		}
		if(it.getKey()<=key)
		{
			if(it2.valid())
			{
				if(it2.getKey() <= key)	//check if there is later less than association
				{	
					it.next();
				}
				else
				{
					return it;
				}
			}
			else return it;
		}
		else
		{
			it.prev();
		}
	}
	return it3;	//return invalid Iterator if key not found
}

bool MultiMap::Iterator::valid() const
{
	return (m_current != NULL);
}

std::string MultiMap::Iterator::getKey() const
{
	return m_current->m_key;
}

unsigned int MultiMap::Iterator::getValue() const
{
	return m_currentValue->m_value;
}

bool MultiMap::Iterator::next()
{
	if(!valid())	//check if current Iterator is valid
		return false;
	
	if(m_currentValue->m_next != NULL)	//check if the current key has a next value
	{
		m_currentValue = m_currentValue->m_next;
		return true;
	}

	if(m_current->m_right != NULL)	//check if we can move to right child
	{
		m_current = m_current->m_right;

		while (m_current->m_left != NULL)	//go to left most child
			m_current = m_current->m_left;

		m_currentValue = m_current->m_valueList;

		return true;
	}

	while (m_current->m_parent != NULL && m_current!=m_current->m_parent->m_left)	//look for the first child on left of parent
		m_current = m_current->m_parent;

	if(m_current->m_parent == NULL)	//return invalid if at end of tree
	{
		m_current = NULL;
		m_currentValue = NULL;
		return false;
	}

	m_current = m_current->m_parent;
	m_currentValue = m_current->m_valueList;
	return true;
}

bool MultiMap::Iterator::prev()
{
	if(!valid())	//check if current Iterator is valid
		return false;

	if(m_currentValue->m_prev != NULL)	//check if the current key has a prev value
	{
		m_currentValue = m_currentValue->m_prev;
		return true;
	}

	if(m_current->m_left != NULL)	//check if we can move to left child
	{
		m_current = m_current->m_left;

		while (m_current->m_right != NULL)	//go to right most child
			m_current = m_current->m_right;

		m_currentValue = m_current->m_valueList;
		while(m_currentValue->m_next != NULL)
		m_currentValue = m_currentValue->m_next;

		return true;
	}

	while (m_current->m_parent != NULL && m_current!=m_current->m_parent->m_right)	//look for the first child on right of parent
		m_current = m_current->m_parent;

	if(m_current->m_parent == NULL)	//return invalid if at beginning of tree
	{
		m_current = NULL;
		m_currentValue = NULL;
		return false;
	}

	m_current = m_current->m_parent;
	m_currentValue = m_current->m_valueList;
	while(m_currentValue->m_next != NULL)
		m_currentValue = m_currentValue->m_next;
	return true;
}