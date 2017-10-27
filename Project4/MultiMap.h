#ifndef MULTIMAP_DEFINED
#define MULTIMAP_DEFINED

#include <string>

class MultiMap
{
private:
	struct ValueList
	{
		ValueList(unsigned int value)
		{
			m_value = value;
			m_next = NULL;
			m_prev = NULL;
		}

		unsigned int m_value;
		ValueList* m_next;
		ValueList* m_prev;
	};

	struct Node
	{
		Node(std::string key, unsigned int value)
		{
			m_key = key;
			m_valueList = new ValueList(value);
			m_left = NULL;
			m_right = NULL;
			m_parent = NULL;
		}

		~Node()
		{
			while (m_valueList!=NULL)	// deletes all values associated with this Node
			{
				ValueList* temp = m_valueList;
				m_valueList = m_valueList->m_next;
				delete temp;
			}
		}

		std::string m_key;
		Node* m_left;
		Node* m_right;
		Node* m_parent;
		ValueList* m_valueList;
	};
	
	//  To  prevent  Multimaps  from  being  copied  or  assigned
	MultiMap& operator=(const MultiMap&  rhs);
	Node* m_root;

	void recursiveClear(Node* currentNode)	//delete Nodes using recursion
	{
		if (currentNode==NULL)
			return;
		recursiveClear(currentNode->m_left);	//delete left Node
		recursiveClear(currentNode->m_right);	//delete right Node

		delete currentNode;
	}
public:
	class Iterator
    {
    public:
		Iterator()
		{
			m_current = NULL;
			m_currentValue = NULL;
		}
        Iterator(Node* node)
		{
			m_current = node;
			m_currentValue = node->m_valueList;
		}
		bool valid() const;
        std::string  getKey() const;
        unsigned int getValue() const;
		bool next();
		bool prev();
	private:
		Node* m_current;
		ValueList* m_currentValue;
	};
       
	MultiMap();
    ~MultiMap();
	void clear();
	void insert(std::string  key, unsigned int value);
    Iterator  findEqual(std::string  key) const;
    Iterator  findEqualOrSuccessor(std::string  key) const;
    Iterator  findEqualOrPredecessor(std::string  key) const;  
};

#endif