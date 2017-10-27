//Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
	public:
		Map();
		Map(const Map& other);
		~Map();
		Map& operator=(const Map& other);
		bool empty() const;
		int size() const;
		bool insert(const KeyType& key, const ValueType& value);
		bool update(const KeyType& key, const ValueType& value);
		bool insertOrUpdate(const KeyType& key, const ValueType& value);
		bool erase(const KeyType& key);
		bool contains(const KeyType& key) const;
		bool get(const KeyType& key, ValueType& value) const;
		bool get(int i, KeyType& key, ValueType& value) const;
		void swap(Map& other);
	private:
		struct Node
		{
		public:
			KeyType m_key;
			ValueType m_value;
			Node* m_next;
			Node* m_prev;
		};
		Node* m_head;
		Node* m_tail;
		int m_size;
};

//non-member function declarations
bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif