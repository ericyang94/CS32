#include "SalaryMap.h"
#include <string>
#include <iostream>
using namespace std;

SalaryMap::SalaryMap()
{
}

bool SalaryMap::add(std::string name, double salary)
{
	return(salary >= 0 && m_map.insert(name,salary));
}

bool SalaryMap::raise(std::string name, double pct)
{
	if(pct >= -100 && m_map.contains(name))
	{
		double salary;
		m_map.get(name,salary);
		m_map.update(name, salary * (100 + pct) / 100);
		return true;
	}
	return false;
}

double SalaryMap::salary(std::string name) const
{
	double salary;
	if (m_map.get(name, salary))
		return salary;
	else return -1;
}

int SalaryMap::size() const
{
	return m_map.size();
}

void SalaryMap::print() const
{
	string y;
	double x;
	for (int k = 0; k != m_map.size(); k++)
	{
		m_map.get(k,y,x);
		cout << y << " " << x << endl;
	}
}