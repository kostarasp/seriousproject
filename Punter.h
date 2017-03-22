#pragma once
#include <vector>
using namespace std;
#include "User.h"
class Node;
class Sportsbook;

class Punter : public User
{
	/*this private member is needed so we don't have to pass a pointer
	to Sportsbook in every single function of Punter that needs it*/
	Sportsbook* sb;
	/*sb is initialiazed inside the printOptions function*/
public:
	Punter(int id,string un,string fn,string pw,int tp,string st,
        double balance,vector<double> fbs = vector <double>() );
	~Punter();
	
	/*core navigation functions*/
	void printOptions(Sportsbook*);
	void printAccountMenu();
	
	/*punter's exclusive function*/
	void printPlaceBetMenu(Node*);
	
	/*functions needed for Account menu*/
	void changeUsername();
	void changePassword();
	void increaseBalance();
	void showPunterBets();
	
	/*core printing functions*/
	void printInfoToFile(ofstream&);
	void printUserOut();
	void printFreebets();
};


