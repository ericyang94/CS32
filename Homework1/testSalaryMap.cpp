#include "SalaryMap.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int main()
{
    SalaryMap m;
	double salary = 8.00;
	string name[3] = {"tom", "steve", "kevin"};
    m.add(name[0], salary);
	m.add(name[1], salary);
	m.add(name[2], salary);
	m.raise (name[0], 10);
	m.raise (name[1], -10);
	m.print();
	cout<< "There are "<< m.size() << " employees." <<endl;
}