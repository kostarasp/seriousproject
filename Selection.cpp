#include "Selection.h"

Selection::Selection(string name0,int c,int s,int e,int m,int sel,int f1,int f2):
	Node(name0,c,s,e,m,sel),fract1(f1),fract2(f2),price( ( (double)fract1/(double)fract2 ) + 1),
	isVoided(false)
{
	if(getName().find("(voided)") == 0)
		this->isVoided = true;
}

Selection::~Selection()
{
}


void Selection::printNodeOption(bool isDirector){
	cout << this->getSelectionID() << " " << this->getName()<< " -- ";
	if(mode == "fractional")
		cout <<fract1 << "/" << fract2<<endl;
	else if(mode == "decimal")
		cout << price << endl;
}



string Selection::mode = "fractional";

void Selection::toggleMode(){
	if(mode == "fractional")
		mode = "decimal";
	else if(mode == "decimal")
		mode = "fractional";
}

void Selection::setSelectionVoided(){
	setName("(voided) " + getName());
	isVoided = true;
}

void Selection::setResult(string result){
	setName("("+result+") " + getName());
}

void Selection::reduceIDby1(){
    reduceSelectionIDby1();
}

void Selection::increaseIDby1(){
    increaseSelectionIDby1();
}

void Selection::printWholeNode(ofstream& output){
	output << getCatID() <<"."<< getSubID()<<"."<< getEventID()<<"."<<getMarketID()<<"."<<getSelectionID();
	output << " " << getName() <<endl;
}

string Selection::getFullID(){
	string s;
	stringstream convert;
	convert << getCatID() <<"."<< getSubID()<<"."<<getEventID()<<"."<<getMarketID()<<"."<<getSelectionID();
	s = convert.str();
	return s;
}
