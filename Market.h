#pragma once

#include "Node.h"

class Market : public Node
{
	bool isSettled;//needed in order to know if a market is settled
public:
	Market(string,int,int,int,int);
	Market( Market& cp):Node(cp),isSettled(cp.getIsSettled()){}
	~Market();

	bool isMarketLevel(){return true;}
	bool getIsSettled(){return isSettled;}
	
	void addNode();
	void reduceIDby1();
	void increaseIDby1();
	void setIsSettledTrue();
	void printWholeNode(ofstream&);
	void printNodeOption(bool=false);
	string getFullID();
	virtual Market* makeCopy(){return new Market(*this);}
};



