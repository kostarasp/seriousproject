#include "Market.h"
#include "Selection.h"

Market::Market(string name0,int c,int s,int e,int m):
	Node(name0,c,s,e,m),isSettled(false)
{
	if(name0.find("(settled)") == 0)
		this->isSettled = true;
}

Market::~Market()
{
}

void Market::addNode(){
	string name;
	int X,Y;
	cout << "Enter the name of the new selection\n";
	cin.ignore();
	getline(cin,name,'\n');
	cout << "Yield format : X/Y\nEnter X and Y values\n";
	cout << "X : ";
	cin >> X;
	cout << "Y : ";
	cin >> Y;
	getNodesRef().push_back(new Selection(name,getCatID(),getSubID(),getEventID(),getMarketID(),getNodesRef().size()+1,X,Y));
}

void Market::reduceIDby1(){
    reduceMarketIDby1();
    for (int j = 0; j < getNodesRef().size(); j++)
        getNodesRef()[j]->reduceMarketIDby1();
}

void Market::increaseIDby1(){
    increaseMarketIDby1();
    for (int j = 0; j < getNodesRef().size(); j++)
        getNodesRef()[j]->increaseMarketIDby1();
}



void Market::setIsSettledTrue(){
	setName("(settled) " + getName());
	isSettled = true;
}

void Market::printWholeNode(ofstream& output){
	output << getCatID() <<"."<< getSubID()<<"."<< getEventID()<<"."<<getMarketID();
	output << " " << getName() <<endl;
}


void Market::printNodeOption(bool isDirector){
	if(getHidden() == false || isDirector == true){
		cout << getMarketID();
		cout << " " << getName() <<endl;
	}
}

string Market::getFullID(){
	string s;
	stringstream convert;
	convert << getCatID() <<"."<< getSubID()<<"."<<getEventID()<<"."<<getMarketID();
	s = convert.str();
	return s;
}