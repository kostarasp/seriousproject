#include "User.h"
#include <fstream>
#include <iostream>
using namespace std;
#include "Node.h"
#include "Sportsbook.h"
#include "Selection.h"

User::User(int id,string un,string fn,string pw,int tp,string st,
            double bc,vector<double> fbs):
    userID(id),username(un),fullname(fn),password(pw),type(tp),
    status(st),balance(bc),freebets(fbs)
{
    //cout << "User with id "<< id << " has been created\n";
}

User::~User()
{
}

void User::printInfoToFile(ofstream& output){//prints user formatted to output
	output 	 << userID <<"|"
			 << username << "|"
			 << fullname << "|"
			 << password << "|"
			 << type<< "|||\n";
}

void User::printUserOut(){//print user to console (setw used for format) (neeeded for director)
	cout << setw(7) << userID <<"|"
		 << setw(15) << username << "|"
		 << setw(30) << fullname << "|"
		 << setw(15) << password << "|"
		 << setw(4) << type<<"|"
		 << setw(31)<<"|"
		 << setw(11)<<"|"<<endl;
}

void User::printOptions(Sportsbook* sb){//simple user is guest
	cout << "You have logged in as a guest!\n";
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
        	 << "X (Exit), to exit the system\n\n";
        curNode->printAvailableOptions();
       	cin.clear();//buffer flush because it may cause infinite loop
        cin >> choice;
        switch(choice.at(0)){//first check if choice is letter
            case 'H':
                curNode = homeNode;
                location = "/" + sysName;
                continue;
            case 'T':
            	Selection::toggleMode();
                continue;
            case 'X':
                cout << "Exiting "<<sysName<<endl;
                cout << "If you want to start betting register to our system. Thank you!\n";
                return;//exits function
            default:
                break;
        }
        intChoice = atoi(choice.c_str());//convert input from string to int
        if(intChoice <= 0 || intChoice > curNode->getNodesRef().size() ||//intChoice must be in boundaries
           curNode->getNodesRef()[intChoice-1]->getHidden() == true){//prevent access to hidden nodes
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
