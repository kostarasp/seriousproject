#include "Subcategory.h"
#include "Event.h"

Subcategory::Subcategory(string name0,int c,int s):
	Node(name0,c,s)
{
	//cout<<"Subcategory Created : "<<id0<<" "<<name0<<endl;
}

Subcategory::~Subcategory()
{
}


void Subcategory::addNode(){
	string wholeName,name,date,time;
	cout << "Enter the name of the new event\n";
	cin.ignore();
	getline(cin,name,'\n');
	cout << "Enter the date of the event (yy/mm/dd)\n";
	getline(cin,date,'\n');
	cout << "Enter the time of the event (HH:MMpm or am)\n";
	getline(cin,time,'\n');
	wholeName = name + " - " + date + " "+ time;
	getNodesRef().push_back(new Event(wholeName,getCatID(),getSubID(),getNodesRef().size()+1));
}

void Subcategory::reduceIDby1(){
    reduceSubIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->reduceSubIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++){
            getNodesRef()[j]->getNodesRef()[k]->reduceSubIDby1();
            for(int m = 0; m < getNodesRef()[j]->getNodesRef()[k]->getNodesRef().size(); m++)
                getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->reduceSubIDby1();
        }
    }
}

void Subcategory::increaseIDby1(){
    increaseSubIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->increaseSubIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++){
            getNodesRef()[j]->getNodesRef()[k]->increaseSubIDby1();
            for(int m = 0; m < getNodesRef()[j]->getNodesRef()[k]->getNodesRef().size(); m++)
                getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->increaseSubIDby1();
        }
    }
}



void Subcategory::printWholeNode(ofstream& output){
	output << getCatID() <<"."<< getSubID();
	output << " " << getName() <<endl;
}

void Subcategory::printNodeOption(bool isDirector){
	if(getHidden() == false || isDirector == true){
		cout << getSubID();
		cout << " " << getName() <<endl;
	}
}

string Subcategory::getFullID(){
	string s;
	stringstream convert;
	convert << getCatID() <<"."<< getSubID();
	s = convert.str();
	return s;
}