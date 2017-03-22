#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>//for formatted output at director's menu
#include <cstdlib>//for atoi
class Node;
class Sportsbook;

using namespace std;

class User//simple user is guest
{
	int userID,type;
	string username,
		   fullname,
		   password,
		   status;
	double balance;
	vector<double> freebets;
public:
	User(int id,string un,string fn,string pw,int tp=0,string st="",
		double balance=0.0,vector<double> freebets = vector <double>() );	//vector constructor -- empty vector
	~User();

	/*accessors for each private data member*/
	int getID(){return userID;}
	string getUsername(){return username;}
	string getStatus(){return status;}
	string getPassword(){return password;}
	string getFullname(){return fullname;}
	int getType(){return type;}
	double getBalance(){return balance;}
	vector<double>& getFreebets(){return freebets;}

	/*mutators for most private data members*/
	void setStatus(string str){status = str;}//needed for director
	void setPassword(string newPassword){password = newPassword;}//needed for Account
	void setUsername(string newUsername){username = newUsername;}//needed for Account
	void increaseBalanceBy(double incr){balance+=incr;}
	void decreaseBalanceBy(double decr){balance-=decr;}

	/*core printing functions*/
	virtual void printInfoToFile(ofstream&);
	virtual void printUserOut();

	/*navigation function*/
	virtual void printOptions(Sportsbook*);//for any type of user(even guest)
	
};
