#pragma once
#include <string>
using namespace std;

#include "Node.h"

class Subcategory : public Node
{
public:
	Subcategory(string,int,int);
	Subcategory(Subcategory& cp):Node(cp){}
	~Subcategory();

	void addNode();
	void reduceIDby1();
	void increaseIDby1();
	void printWholeNode(ofstream&);
	void printNodeOption(bool=false);
	string getFullID();
	virtual Subcategory* makeCopy(){return new Subcategory(*this);}
};
