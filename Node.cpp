#include "Node.h"

#include "Category.h"


Node::Node(string name0,int c,int s,int e,int m,int sel,bool hidden):
	name(name0),catID(c),subID(s),eventID(e),marketID(m),selectionID(sel),hidden(false)
{
	if(name0.find("[hidden]") == 0)
		this->hidden = true;
}

Node::Node(Node& cp):name("[hidden]" + cp.getName() + " -- Copied"),catID(cp.getCatID()),subID(cp.getSubID()),eventID(cp.getEventID()),marketID(cp.getMarketID()),selectionID(cp.getSelectionID()),
hidden(true)
{
	
        for (int i=0; i<cp.getNodesRef().size(); i++){
            nodeVec.push_back(cp.getNodesRef()[i]->makeCopy()); }
}

Node::~Node()
{
}


/*This is the core navigation function for any user(showing available nodes to visit)*/
void Node::printAvailableOptions(bool isDirector){
	if(nodeVec.size() == 0)
		cout << "There aren't any available options here! Return to home!\n";
	if(isDirector == false)
		for(int i=0;i<nodeVec.size();i++)//print options
        	nodeVec[i]->printNodeOption();
    else//isDirector == true
    	for(int i=0;i<nodeVec.size();i++)//print options
        	nodeVec[i]->printNodeOption(true);
    cout << endl;
}

void Node::toggleVisibility(){//needed for Director's V
	if(hidden == false){
		hidden = true;
		name = "[hidden]" + name;
	}
	else{
		hidden = false;
		name.erase(0,8);//erase [hidden]
	}
	for(int i=0;i<nodeVec.size();i++)//toggling visibility (recursively) of subnodes
		nodeVec[i]->toggleVisibility();
}

void Node::addNode(){
	string newName;
	cout << "Enter the name of the new category\n";
	cin.ignore();
	getline(cin,newName,'\n');
	nodeVec.push_back(new Category(newName,nodeVec.size()+1));
}

void Node::deleteNode(){//first we delete the subnodes of a node and then the node itsself
    for(int i=0;i<nodeVec.size();i++){
        nodeVec[i]->deleteNode();//first delete(so we don't have memory leak)
        nodeVec.erase(nodeVec.begin()+i);//then erase from vector
    }
    delete this;
}

/*void Node::makeCopy(Node* originalNode){
	nodeVec.push_back(new Category(*originalNode));
}*/

/*string Node::getFullID(){
	string s;
	stringstream convert;
	if(selectionID != 0)
		convert << catID <<"."<< subID<<"."<< eventID<<"."<< marketID<<"."<< selectionID;
	else if(marketID !=0 && selectionID == 0)
		convert << catID <<"."<< subID<<"."<< eventID<<"."<< marketID;
	else if(eventID!=0 && marketID ==0 && selectionID == 0)
		convert << catID <<"."<< subID<<"."<< eventID;
	else if(subID!=0 && eventID==0 && marketID ==0 && selectionID == 0)
		convert << catID <<"."<< subID;
	else
		convert << catID;
	s = convert.str();
	return s;
}*/
