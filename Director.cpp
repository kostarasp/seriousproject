#include "Director.h"
#include "Node.h"
#include "Bet.h"
#include "Selection.h"
#include "Sportsbook.h"
#include <iostream>
using namespace std;

bool isSaved = true;//director functions change this and there is a warning at exit if not saved

Director::Director(int id,string un,string fn,string pw):
	Trader(id,un,fn,pw,3)
{
}

Director::~Director()
{
}

void Director::printInfoToFile(ofstream& output){
	output 	 << getID() <<"|"
			 << getUsername() << "|"
			 << getFullname() << "|"
			 << getPassword() << "|"
			 << getType()<<"|||\n";	
}

void Director::printUserOut(){
	cout << setw(7) << getID() <<"|"
		 << setw(15) << getUsername() << "|"
		 << setw(30) << getFullname() << "|"
		 << setw(15) << getPassword() << "|"
		 << setw(4) << getType() <<"|"
		 << setw(31)<<"|"
		 << setw(11)<<"|"<<endl;
}

void Director::printOptions(Sportsbook* sportsbookPointer){
	cout << "You have logged in as a Director!\n";
	this->sb = sportsbookPointer;//we initialize sb here
    Node* homeNode = sb->getHierarchyPointer();//this variable is needed for H option
    string sysName = sb->getName();
    string location = "/" + sysName;//initialise location
    string choice;//choice is string because of various inputs
    int intChoice;//int choice is atoi(choice) in case user's choice is a number
    /*this variable (curNode) is needed to navigate through different levels of hierarchy*/
    Node* curNode = homeNode;
    while(1){
        cout << "=======================================\n";
        cout << "Location : " << location <<endl;
        cout << "Yield mode : "<< Selection::getMode() << endl;
        cout << "\nChoose:\n";
        cout << "ID of the node you want to visit\n"
             << "H (Home), to return to home screen\n"
             << "T (Toggle), to switch price mode\n"
             << "B (Bets), to view last 20 bets\n"
             << "F (Freebets), to gift a coupon to a user\n";
             if(curNode->isMarketLevel()){
                cout << "O (vOid), to void a bet.\n";
                cout << "E (sEttle), to settle a bet.\n";
             }
             cout << "X (Exit), to exit the system\n\n"
             << "Management : \n"
             << "L (Log), to view the last 25 actions\n"
             << "U (Users), to view users information\n"
             << "R (Rename), to rename a node\n"
             << "N (New), to create a new Node\n"
             << "C (Copy), to copy a node\n"
             << "D (Delete), to delete a node\n"
             << "V (Visibility), to change visibility of a node\n"
             << "S (Save), to synchronize system files\n"<<endl;
        curNode->printAvailableOptions(true);
        cin.clear();//buffer flush because it may cause infinite loop
        cin >> choice;
        
        switch(choice.at(0)){
            case 'H':
                curNode = homeNode;
                location = "/" + sysName;
                continue;
            case 'T':
            	Selection::toggleMode();
                continue;
            case 'X':
                if(!isSaved){
                    char saveChoice;
                    cout<< "You didn't save your changes. Exit?[y/n]"<<endl;
                    cin >> saveChoice;
                    if(saveChoice == 'n' || saveChoice == 'N')
                        continue;
                    else if (saveChoice == 'y' || saveChoice == 'Y'){
                        cout << "Exiting "<< sysName << endl;
                        return;
                    }

                }
                else {
                    cout << "Exiting "<< sysName << endl;
                    return;//exits function
                }
            case 'B':
            	sb->showLastBets(20);
            	continue;
            case 'O':
                if(curNode->isMarketLevel()==false)
                    break;
                printVoidBetMenu(curNode);
                continue;
            case 'E':
                if(curNode->isMarketLevel()==false)
                    break;
                printSettleBetMenu(curNode);
                continue;
            case 'F':
                giftFreebet();
            	continue;	
            case 'L':
            	showLastLogs(25);
            	continue;	
            case 'U':
                showUsersMenu();
            	continue;
            case 'R':
            	renameOption(curNode->getNodes());
            	continue;
            case 'N':
            	newOption(curNode);
            	continue;
            case 'C':
            	copyOption(curNode);
            	continue;
            case 'D':
                deleteOption(curNode->getNodes());
            	continue;
            case 'V':
                visibilityOption(curNode->getNodes());
                break;
            case 'S':
                saveOption();
                isSaved = true;
                continue;
            default:
                break;
        }
        intChoice = atoi(choice.c_str());//convert input from string to int
        if(intChoice <= 0 || intChoice > curNode->getNodesRef().size()){
            cout << "Wrong input. Please try again.\n";
            continue;
        }
        else{
            if(curNode->isMarketLevel()){//we can't move any further if we are at market level
                cout << "You can't move any further.\n";
                continue;
            }
            location = location + "/" + curNode->getNodesRef()[intChoice-1]->getName();//adjust new location
            curNode = curNode->getNodesRef()[intChoice-1];//adjust curNode to the node which user chose
        }
    }
}


void Director::copyOption(Node* tempNode){
    int choice;
    if(tempNode->getNodesRef().size()==0){
        cout <<"There's nothing to copy here!\n";
        return;
    }
    cout << "Nodes of current level : \n";
    for(int i=0;i<tempNode->getNodesRef().size();i++)//print options
        tempNode->getNodesRef()[i]->printNodeOption(true);
    cout << "Choose the ID of the node you want to copy\n";
    cin >> choice;
    if(choice <=0 || choice >tempNode->getNodesRef().size()){
        cout << "Invalid option.\n";
        return;
    }
    //deep copying --calling virtual makeCopy that calls respective copy construcors and at Node(Node&) deep copying Nodes to vector<Node*>
    tempNode->getNodesRef().push_back(tempNode->getNodesRef()[choice-1]->makeCopy()); 
    for (int i = choice; i <  tempNode->getNodesRef().size(); i++ )//then increase ID shown 
        tempNode->getNodesRef()[tempNode->getNodesRef().size()-1]->increaseIDby1();
    
    isSaved = false;
}

void Director::deleteOption(vector<Node*>* tempLevel){
    int choice;
    if(tempLevel->size()==0){
        cout <<"There's nothing to delete here!\n";
        return;
    }
    cout << "Nodes of current level : \n";
    for(int i=0;i<tempLevel->size();i++)//print options
        tempLevel->at(i)->printNodeOption(true);
    cout << "Choose the ID of the node you want to delete\n";
    cin >> choice;
    if(choice <=0 || choice >tempLevel->size()){
        cout << "Invalid option.\n";
        return;
    }
    // na valw elegxo gia cancel abort
    tempLevel->at(choice-1)->deleteNode();
    tempLevel->erase(tempLevel->begin()+choice-1);
    for(int i = choice-1;i<tempLevel->size();i++)
        tempLevel->at(i)->reduceIDby1();
    isSaved = false;
}


void Director::newOption(Node* tempNode){
    int choice;
    string newName;
    cout << "Nodes of current level : \n";
    for(int i=0;i<tempNode->getNodesRef().size();i++)//print options
        tempNode->getNodesRef()[i]->printNodeOption();
    tempNode->addNode();
    isSaved = false;
}

void Director::saveOption(){//bets are automatically saved
    sb->updateHierarchyFile();
    sb->updateUsersFile();
}

void Director::visibilityOption(vector<Node*>* tempLevel){
    int choice;
    cout << "Nodes of current level : \n";
    for(int i=0;i<tempLevel->size();i++)//print options
        tempLevel->at(i)->printNodeOption(true);
    cout << "Choose the ID of the node you want to change visibility\n";
    cin >> choice;
    if(choice <=0 || choice >tempLevel->size()){
        cout << "Invalid option.\n";
        return;
    }
    tempLevel->at(choice-1)->toggleVisibility();
    isSaved = false;

}

void Director::renameOption(vector<Node*>* tempLevel){
	int choice;
	string newName;
	cout << "Nodes of current level : \n";
	for(int i=0;i<tempLevel->size();i++)//print options
    	tempLevel->at(i)->printNodeOption();
    cout << "Choose the ID of the node you want to rename\n";
    cin >> choice;
    if(choice <=0 || choice >tempLevel->size()){
    	cout << "Invalid option.\n";
    	return;
    }
    cout << "Please enter the new name of the node with ID "<< choice<<endl;
    cin.ignore();
    getline(cin,newName,'\n');
    tempLevel->at(choice-1)->setName(newName);
    cout << "Node with ID " <<choice<<" has been renamed to "<<newName<<endl;
    isSaved = false;
}



void Director::showLastLogs(int sz){
	vector<string> stringVec;
	ifstream inputLog("audit.log");
	string tempString;
	while(inputLog){
		getline(inputLog,tempString);
		if(stringVec.size() <= sz)
			stringVec.push_back(tempString);
		else{
			stringVec.erase(stringVec.begin());
			stringVec.push_back(tempString);
		}
	}
    cout << "Showing the most recent down to last : "<<endl;
	for(int i = stringVec.size()-1; i >= 0; i--)
		cout << stringVec[i] << endl;
	inputLog.close();
}

void Director::showUsersMenu(){
	while(1){
		cout << "***********************\n";
	    cout << "Users menu choices :\n"
	         << "V (View), to show users' array\n"
	         << "S (Search), to search a user\n"
	         << "L (Lock), to block or unblock a user\n"
	         << "N (returN), to return to director's menu\n";
	    string choice;//we read string because of case of wrong input
	    cin >> choice;
	    if(choice.size()!=1){
	    	cout << "Wrong input\n";
	    	continue;
	    }
	    switch(choice.at(0)){
	    	case 'V':
	    		showUsers();
	    		continue;
	        case 'S':
	        	searchUser();
	        	continue;
	        case 'L':
	        	lockUser();
	        	continue;
	        case 'N':
	        	return;
	        default:
	        	cout << "Wrong input\n";
	        	continue;
	    }
	}
}

void Director::showUsers(){
	vector<User*> uArray = sb->getUsersArray();
	cout << setw(7) << "user_id" <<"|"
		 << setw(15) << "username" << "|"
		 << setw(30) << "fullname" << "|"
		 << setw(15) << "password" << "|"
		 << setw(1) << "type"<<"|"
		 << setw(30)<< "status"<<"|"
		 << setw(10)<< "balance"<<"|"
		 << setw(20)<< "freebets" <<endl 
		 << "-------------------------------------------------------------------------"
		 << "-----------------------------------------------------------------"<<endl;
	for(int i=0;i < uArray.size();i++)
		uArray[i]->printUserOut();
}

void Director::searchUser(){
    bool found = false;
	string tempString;
	cout << "Please give username or substring of username\n";
	cin >> tempString;
	vector<User*> uArray = sb->getUsersArray();
    cout << setw(7) << "user_id" <<"|"
         << setw(15) << "username" << "|"
         << setw(30) << "fullname" << "|"
         << setw(15) << "password" << "|"
         << setw(1) << "type"<<"|"
         << setw(30)<< "status"<<"|"
         << setw(10)<< "balance"<<"|"
         << setw(20)<< "freebets" <<endl 
         << "-------------------------------------------------------------------------"
         << "-----------------------------------------------------------------"<<endl;
	for(int i=0;i < uArray.size();i++)
		if(uArray[i]->getUsername().find(tempString) != string::npos){
            uArray[i]->printUserOut();
            found = true;
        }
    if (!found)
        cout << "Did not find any user"<<endl;    
}

void Director::lockUser(){//blocks ONLY punters
    bool found = false;
	cout << "Please enter the username of the user you want to block or unblock\n";
	string tempString;
	cin>>tempString;
	vector<User*> uArray = sb->getUsersArray();
	for(int i=0;i < uArray.size();i++)
		if(uArray[i]->getUsername() == tempString)
			if(uArray[i]->getStatus().empty() || uArray[i]->getStatus().at(0) == 'A'){
				cout << "Why do you want to block this user?\n";
				cin.ignore();
				getline(cin,tempString);
				if(tempString.empty())
					tempString = "N/A";//by default 
				uArray[i]->setStatus("L,"+tempString);
				cout << "User with username : "<<uArray[i]->getUsername()<< " has been blocked.\n";
				sb->updateUsersFile();
				return; 
			}
			else{
				cout << "Are you sure you want to unblock this user?[y/n]\n";
				cin >> tempString;
				if(tempString == "y"){
					cout << "Please add a comment ,if you want, or press enter if you don't have any\n";
					cin.ignore();
					getline(cin,tempString);
					uArray[i]->setStatus("A,"+tempString);
					sb->updateUsersFile();
					return;
				}
				else return;
			}
    /*if it reaches here, obviously username is not found*/
    cout << "Player with username - "<<tempString <<" - not found!"<<endl;
}
