#pragma once

#include "User.h"
class Node;
class Sportsbook;


class Trader : public User
{
/*this protected member is needed so we don't have to pass a pointer
to Sportsbook in every single function of Punter that needs it*/
protected:
	Sportsbook* sb;
/*sb is initialiazed inside the printOptions function*/
public:
	Trader(int id,string un,string fn,string pw,int type=2);
	~Trader();
	
	/*navigation function*/
	void printOptions(Sportsbook*);

	/*trader's exclusive functions*/
	void printVoidBetMenu(Node*);//this is used to void a selection
	void printSettleBetMenu(Node*);

	/*core printing functions*/
	void printInfoToFile(ofstream&);
    void printUserOut();
    void giftFreebet();
};
