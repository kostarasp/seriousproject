#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

class Node
{
	string name;
	int catID,subID,eventID,marketID,selectionID;
	vector <Node*> nodeVec;
	bool hidden;
public:
	Node(string,int=0,int=0,int=0,int=0,int=0,bool=false);
	Node(Node&);
	virtual ~Node();

	/*Accessors*/
	string getName() const {return name;}
	int getCatID() const {return catID;}
    int getSubID() const {return subID;}
    int getEventID() const {return eventID;}
    int getMarketID() const {return marketID;}
    int getSelectionID() const {return selectionID;}
    bool getHidden() const {return hidden;}
    vector <Node*>& getNodesRef(){return nodeVec;}
	vector <Node*>* getNodes() { return &nodeVec;}
	
	/*Mutators*/
	void setName(string s){name = s;}

    /*These functions are needed in case we delete a Node*/
    virtual void reduceIDby1(){};//calls respective functions (depending on deleted node's level)
    void reduceCatIDby1(){catID--;}
	void reduceSubIDby1(){subID--;}
	void reduceEventIDby1(){eventID--;}
	void reduceMarketIDby1(){marketID--;}
	void reduceSelectionIDby1(){selectionID--;}

	virtual void increaseIDby1(){};//calls respective functions (depending on deleted node's level)
    void increaseCatIDby1(){catID++;}
	void increaseSubIDby1(){subID++;}
	void increaseEventIDby1(){eventID++;}
	void increaseMarketIDby1(){marketID++;}
	void increaseSelectionIDby1(){selectionID++;}


	void toggleVisibility();//Director's V option
	void makeCopy(Node*);//Director's C option
	void deleteNode();//Director's D Option
	void printAvailableOptions(bool=false);//if parameter is set to true,hidden options are shown(director)

	/********virtual functions**********/

	virtual Node* makeCopy(){return new Node(*this);}
	virtual void printWholeNode(ofstream&){}//needed to print a node in hierarchy.dat
	virtual void printNodeOption(bool=false){}//prints option on console
	virtual void addNode();//adds a node(Director's N option)
	virtual string getFullID(){}//we mainly need the whole ID of a Node at 2  instances
							   //1. we need to know the whole ID of the node a punter visits for audit.log
							   //2. we need the whole ID of a selection for bets.csv and other times
							   //e.g. when we check whether a bet is won or lost (from bets.csv),
							   //when a market is settled.

	/*virtual functions made for specific classes*/

	/*Selection's exclusive functions*/
	virtual void setSelectionSettled(){}//adds  (settled) to name
	virtual void setSelectionVoided(){}//adds  (voided) to name
	virtual void setResult(string){}//adds (string result) to name
	/*Selection's extra accessors*/
	virtual double getPrice(){}
	virtual int getFract1(){}
	virtual int getFract2(){}
	virtual bool getIsVoided(){}
	virtual bool getIsSettled(){}

	/*Market's exclusive functions*/
	virtual bool isMarketLevel(){return false;}//returns true when we are at market level(last level)
	virtual void setIsSettledTrue(){}
};


