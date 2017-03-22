#pragma once

#include <string>
#include <vector>

class User;
class Bet;
class Node;

using namespace std;

class Sportsbook
{
	double profit;
	string name;
	double posostoPromithias;
	vector<User*> usersArray;
	vector<Bet*> betsArray;
	Node* hierarchy;
	User* user;
public:
	Sportsbook();
	~Sportsbook();

	/*get basic members of Sportbook*/
	string getName(){return name;}
	double getProfit(){return profit;}
	double getPosostoPromithias(){return posostoPromithias;}
	User* getUser(){return user;}

	/*create account and login functions*/
	int createAccount();/*1->successful creation, 0->username already exists*/
	int login();/*1->successful login, 0->blocked user(unsuccessful login)*/

	/*load from file to program functions*/
	void loadUsersToProgram();
	void loadHierarchyToProgram();
	void loadBetsToProgram();

	/*file-update functions*/
	void updateUsersFile();
	void updateHierarchyFile();
	void updateBetsFile();
	void addLastBetToFile();//this is needed when user is placing a valid bet
	
	/*get vector or pointers to vector functions*/
	Node* getHierarchyPointer(){return hierarchy;}
	vector<Bet*>& getBetsArray(){return betsArray;};
	vector<User*>& getUsersArray(){return usersArray;}

	/*misc functions*/
	void refund(double,User*);/*take money from system to user*/
	bool usernameFound(string);/*true->username found, false->username not found*/
	void showLastBets(int);
	void userWins(double,User*);
	void increaseProfit(double money){profit += money;}
	void initializeProfits();
};



