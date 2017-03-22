#include "Event.h"
#include "Market.h"

Event::Event(string name0,int c,int s,int e):
	Node(name0,c,s,e)
{
	//cout<<"Event Created : "<<id0<<" "<<name0<<endl;
}

Event::~Event()
{
}

void Event::addNode(){
	string newName;
	cout << "Enter the name of the new Market\n";
	cin.ignore();
	getline(cin,newName,'\n');
	getNodesRef().push_back(new Market(newName,getCatID(),getSubID(),getEventID(),getNodesRef().size()+1));
}

void Event::reduceIDby1(){
    reduceEventIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->reduceEventIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++)
            getNodesRef()[j]->getNodesRef()[k]->reduceEventIDby1();
    }
}

void Event::increaseIDby1(){
    increaseEventIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->increaseEventIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++)
            getNodesRef()[j]->getNodesRef()[k]->increaseEventIDby1();
    }
}

void Event::printWholeNode(ofstream& output){
	output << getCatID() <<"."<< getSubID()<<"."<< getEventID();
	output << " " << getName() <<endl;
}

void Event::printNodeOption(bool isDirector){
	if(getHidden() == false || isDirector == true){
		cout << getEventID();
		cout << " " << getName() <<endl;
	}
}

string Event::getFullID(){
	string s;
	stringstream convert;
	convert << getCatID() <<"."<< getSubID()<<"."<<getEventID();
	s = convert.str();
	return s;
}