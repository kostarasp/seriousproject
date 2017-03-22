#include "Trader.h"
#include "Node.h"
#include "Bet.h"
#include "Selection.h"
#include "Sportsbook.h"

Trader::Trader(int id,string un,string fn,string pw,int type):
	User(id,un,fn,pw,type)
{
}

Trader::~Trader()
{
}

void Trader::printInfoToFile(ofstream& output){
	output 	 << getID() <<"|"
			 << getUsername() << "|"
			 << getFullname() << "|"
			 << getPassword() << "|"
			 << getType()<<"|||\n";
}

void Trader::printUserOut(){
	cout << setw(7) << getID() <<"|"
		 << setw(15) << getUsername() << "|"
		 << setw(30) << getFullname() << "|"
		 << setw(15) << getPassword() << "|"
		 << setw(4) << getType() <<"|"
		 << setw(31)<<"|"
		 << setw(11)<<"|"<<endl;
}

void Trader::printOptions(Sportsbook* sportsbookPointer){
	cout << "You have logged in as a trader!\n";
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
        if(curNode->isMarketLevel()){//traders/directors exclusive
        	cout << "V (Void), to void a selection.\n";
    		cout << "S (Settle), to settle a market.\n";
   	 	}
    	cout << "X (Exit), to exit the system\n\n";
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
            	cout << "Exiting "<< sysName << endl;
                return;//exits function
            case 'B':
            	sb->showLastBets(20);
            	continue;
            case 'F':
            	giftFreebet();
            	continue;	
            case 'V':
            	if(curNode->isMarketLevel()==false)//you must be on marketlevel to void a bet
            		break;
            	printVoidBetMenu(curNode);
            	continue;
            case 'S':
            	if(curNode->isMarketLevel()==false)//you must be on marketlevel to settle a bet
            		break;
            	printSettleBetMenu(curNode);
            	continue;
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


void Trader::printVoidBetMenu(Node* marketNode){//we pass the event node which contains available markets
	if(marketNode->getIsSettled()){
		cout << "This market is settled. No point in Voiding any selection.\n";
		return;
	}
	cout << "Available selections :\n";
	marketNode->printAvailableOptions();
	cout << "Choose the ID of the selection you want to void\n";
	vector<Node*>* selArray = marketNode->getNodes();
	int choice;
	cin >> choice;
	if(choice <= 0 || choice > selArray->size() || selArray->at(choice-1)->getHidden() == true) {
		cout << "There is no selection with id "<< choice<<endl;
		return;
	}
	if(selArray->at(choice-1)->getIsVoided()){
		cout << "This selection is already voided!\n";
		return;
	}
	string idOfVoidedBet = selArray->at(choice-1)->getFullID();
	cout << "Bets with node_id " << idOfVoidedBet <<" will be voided!\n";
	//now we have to return money to users who betted on that option
	vector<Bet*>& betsArray = sb->getBetsArray();
	vector<User*>& usersArray = sb->getUsersArray();
	for(int i=0;i<betsArray.size();i++)
		if(betsArray[i]->getNodeID() == idOfVoidedBet){
			sb->refund(betsArray[i]->getStake(),usersArray[betsArray[i]->getUserID()-1]);
			betsArray[i]->setResult('V');
		}
	selArray->at(choice -1)->setSelectionVoided();
	sb->updateHierarchyFile();
	sb->updateUsersFile();
	sb->updateBetsFile();
}

void Trader::printSettleBetMenu(Node* marketNode){
	if(marketNode->getIsSettled()){
		cout << "This market is already settled!\n";
		return;
	}
	cout << "Available selections :\n";
	marketNode->printAvailableOptions();
	cout << "Choose the ID of the selection you want to settle (as Won)\n"
		 << "Note that the rest of the selections of this market will be set to Lost.\n";
	vector<Node*>* selArray = marketNode->getNodes();
	int choice;
	cin >> choice;
	if(choice <= 0 || choice > selArray->size() || selArray->at(choice-1)->getHidden() == true){
		cout << "There is no selection with id "<< choice<<endl;
		return;
	}
	if(selArray->at(choice-1)->getIsVoided()){
		cout << "This selection is voided. You can't settle a voided selection.\n";
		return;
	}
	string idOfSettledBet = selArray->at(choice-1)->getFullID();
	vector<Bet*>& betsArray = sb->getBetsArray();
	vector<User*>& usersArray = sb->getUsersArray();
	string first7LettersOfIdOfSettledBet = idOfSettledBet.substr(0,7);
	string lastIDOfIdOfSettledBet = idOfSettledBet.substr(8,lastIDOfIdOfSettledBet.size());//get the last ID
	for(int i=0;i<betsArray.size();i++)
		if(betsArray[i]->getNodeID().substr(0,7) == first7LettersOfIdOfSettledBet){
			if(betsArray[i]->getNodeID().substr(8,betsArray[i]->getNodeID().size()) == lastIDOfIdOfSettledBet){
				betsArray[i]->setResult('W');//first settle bet
				double prizeWon = betsArray[i]->getStake() * selArray->at(choice-1)->getPrice();
				sb->userWins(prizeWon,usersArray[betsArray[i]->getUserID()-1]);
				cout << "User "  << usersArray[betsArray[i]->getUserID()-1]->getUsername()
					 << "has won--> " << prizeWon <<"\n";
			}
			else//if last letter is different it means the best is lost
				betsArray[i]->setResult('L');
		}
	/*before choice-1 are lost*/
	for(int i=0;i<choice-1;i++)
		if(selArray->at(i)->getIsVoided() == false)
			selArray->at(i)->setResult("L");
	/*at(choice-1) is won*/
	selArray->at(choice-1)->setResult("^WIN^");//this is Selection::setResult, not the same as the above
	/*after choice-1 are lost*/
	for(int i=choice-1+1;i<selArray->size();i++)
		if(selArray->at(i)->getIsVoided() == false)
			selArray->at(i)->setResult("L");
	marketNode->setIsSettledTrue();
	sb->updateHierarchyFile();
	sb->updateUsersFile();
	sb->updateBetsFile();
}

void Trader::giftFreebet(){
	string giftuser;
	double gift;
	cout << "Enter the username of the player you want to gift a coupon"<<endl;
	cin >> giftuser;
	vector<User*> uArray = sb->getUsersArray();
	for(int i=0;i < uArray.size();i++)
		if(uArray[i]->getUsername() == giftuser){
			cout << "Enter the price of the gift"<<endl;
			cin >> gift;
			uArray[i]->getFreebets().push_back(gift);
			cout << "You gifted a coupon of "<<gift << " to player " << giftuser<<endl;
			sb->updateUsersFile();
			return;
		}
	/*if it reaches here, obviously username is not found*/
    cout << "Player with username - "<<giftuser <<" - not found!"<<endl;
}