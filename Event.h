#pragma once

#include "Node.h"

class Event : public Node
{
public:
	Event(string,int,int,int);
	Event( Event& cp):Node(cp){}
	~Event();

	void addNode();
	void reduceIDby1();
	void increaseIDby1();
	void printWholeNode(ofstream&);
	void printNodeOption(bool=false);
	string getFullID();
	virtual Event* makeCopy(){return new Event(*this);}
};

