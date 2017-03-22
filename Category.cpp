#include "Category.h"
#include "Node.h"
#include "Subcategory.h"
#include <iostream>


Category::Category(string name0,int id0):
	Node(name0,id0)
{
}

Category::~Category()
{
}

void Category::addNode(){
	string newName;
	cout << "Enter the name of the new subcategory\n";
	cin.ignore();
	getline(cin,newName,'\n');
	getNodesRef().push_back(new Subcategory(newName,getCatID(),getNodesRef().size()+1));
}

void Category::reduceIDby1(){
    reduceCatIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->reduceCatIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++){
            getNodesRef()[j]->getNodesRef()[k]->reduceCatIDby1();
            for(int m = 0; m < getNodesRef()[j]->getNodesRef()[k]->getNodesRef().size(); m++){
                getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->reduceCatIDby1();
                for(int n = 0; n < getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef().size(); n++)
                    getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->reduceCatIDby1();
            }
        }
    }
}

void Category::increaseIDby1(){
    increaseCatIDby1();
    for (int j = 0; j < getNodesRef().size(); j++){
        getNodesRef()[j]->increaseCatIDby1();
        for(int k = 0; k <  getNodesRef()[j]->getNodesRef().size(); k++){
            getNodesRef()[j]->getNodesRef()[k]->increaseCatIDby1();
            for(int m = 0; m < getNodesRef()[j]->getNodesRef()[k]->getNodesRef().size(); m++){
                getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->increaseCatIDby1();
                for(int n = 0; n < getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef().size(); n++)
                    getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->increaseCatIDby1();
            }
        }
    }
}

void Category::printWholeNode(ofstream& output){
	output << getCatID();
	output << " " << getName() <<endl;
}

void Category::printNodeOption(bool isDirector){
	if(getHidden() == false || isDirector == true){
		cout << getCatID();
		cout << " " << getName() <<endl;
	}
}

string Category::getFullID(){
	string s;
	stringstream convert;
	convert << getCatID();
	s = convert.str();
	return s;
}