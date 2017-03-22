#pragma once

#include "Trader.h"

class Node;
class Sportsbook;

class Director : public Trader
{
public:
	Director(int id,string un,string fn,string pw);
	~Director();

	/*core printing functions*/
	void printUserOut();
	void printInfoToFile(ofstream&);

	/*navigation function*/
	void printOptions(Sportsbook*);

	/*director's exclusive functions*/
	void showLastLogs(int);
	void showUsersMenu();
	void showUsers();
	void searchUser();
	void lockUser();
	void renameOption(vector<Node*>*);
	void newOption(Node*);
	void deleteOption(vector<Node*>*);
	void visibilityOption(vector<Node*>*);
	void saveOption();
	void copyOption(Node*);
};


