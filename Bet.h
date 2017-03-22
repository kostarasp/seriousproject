#pragma once

#include <fstream>
#include <string>
using namespace std;

class Bet
{
	int bet_id;
	int user_id;
	string node_id;
	double stake;
	char result;
public:
	Bet(int,int,string,double,char='-');
	~Bet();

	/*Accesors*/
	void setResult(char r){result = r;}
	string getNodeID(){return node_id;}
	int getUserID(){return user_id;}
	double getStake(){return stake;}
	char getResult(){return result;}

	/*Printing functions*/
	void printBet(ofstream&);//prints bet in bets.csv
	void printBetOut();//prints bet at console
};

