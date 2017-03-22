#pragma once
#include "Node.h"
class Category : public Node
{
public:
	Category(string,int);
	Category(Category& cp):Node(cp){}
	~Category();

	void addNode();
	void reduceIDby1();
	void increaseIDby1();
	void printWholeNode(ofstream&);
	void printNodeOption(bool=false);
	string getFullID();
	virtual Category* makeCopy(){return new Category(*this);}
};


