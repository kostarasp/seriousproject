#include "Punter.h"
#include "Bet.h"
#include "User.h"
#include "Selection.h"
#include "Sportsbook.h"
#include <iomanip>
#include <cstdlib>//for atof at place bet menu
#include <unistd.h>//for change password at Account

ofstream olog("audit.log",ios::app);

Punter::Punter(int id,string un,string fn,string pw,int tp,string st,
            double balance,vector<double> fbs):
	User(id,un,fn,pw,tp,st,balance,fbs)
{
}

Punter::~Punter()
{
}

void Punter::printInfoToFile(ofstream& output){
	int i;
	output   << getID() <<"|"
			 << getUsername() << "|"
			 << getFullname() << "|"
			 << getPassword() << "|"
			 << getType()<<"|"
			 << getStatus()<<"|"
			 << getBalance()<<"|";

			 if (getFreebets().size() != 0){
				for(i = 0; i < getFreebets().size()-1; i++)
			 		output << getFreebets()[i] << ",";
			 	output << getFreebets()[i]<<endl;//print last freebet without comma
			 }
			 else
			 	output <<endl;
}

void Punter::printUserOut(){
	int i;
	cout << setw(7) << getID() <<"|"
		 << setw(15) << getUsername() << "|"
		 << setw(30) << getFullname() << "|"
		 << setw(15) << getPassword() << "|"
		 << setw(4) << getType()<<"|"
		 << setw(30) << getStatus()<<"|"
		 << setw(10) << getBalance()<<"|"
		 << setw(20);
		 
		 if (getFreebets().size() != 0){
				for(i = 0; i < getFreebets().size()-1; i++)
			 		cout << getFreebets()[i] << ",";
			 	cout << getFreebets()[i]<<endl;//print last freebet without comma
			 }
			 else
			 	cout <<endl;

}


void Punter::printOptions(Sportsbook* sportsbookPointer){
	cout << "You have logged in as a punter!\n";
	olog << this->getUsername() << " logged in"<<endl;
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
             << "A (Account), to manage your account\n";
        if(curNode->isMarketLevel())
        	cout << "P (Place), to place your bet.\n";
        cout << "X (Exit), to exit the system\n\n";
        curNode->printAvailableOptions();
        cin.clear();
        cin >> choice;
        switch(choice.at(0)){//first check if choice is letter
            case 'H':
                curNode = homeNode;
                location = "/" + sysName;
                olog << getUsername() << " went back to home (H)"<<endl;
                continue;
            case 'T':
            	Selection::toggleMode();
            	olog << getUsername() << " toogled yield mode (T)"<<endl;
                continue;
            case 'X':
                cout << "Exiting "<<sysName<<endl;
                cout << "Thanks for preferring us "<< getFullname() <<"!\n";
            	olog << getUsername() << " logged out (X)"<<endl;
            	olog.close();
                return;//exits function
            case 'A':
            	olog << getUsername() << " accessed his account (A)"<<endl;
            	printAccountMenu();
            	continue;
            case 'P':
            	if(curNode->isMarketLevel()==false)//you can place bet only at last level
            		break;
            	printPlaceBetMenu(curNode);
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
            olog << this->getUsername() << " went to "<< curNode->getNodesRef()[intChoice-1]->getFullID()<<
            " == " << curNode->getNodesRef()[intChoice-1]->getName() << endl;
            curNode = curNode->getNodesRef()[intChoice-1];//adjust curNode to the node which user chose
        }
    }
}


void Punter::printAccountMenu(){
	char choice;
	while(1){
		cout << "********************************"<<endl;
		cout << "Player : "<< getFullname()<<endl
		 << "Account's balance : " << getBalance()<<endl
		 << "Available coupons : "; 
		printFreebets();
		cout << "\nPress :"<<endl
		 << "R (Rename), to change your username\n"
		 << "P (Password), to change your password\n"
		 << "B (Bets), to view your bet history\n"
		 << "D (Deposit), to add money to your account's balance\n"
		 << "N (returN), to return to viewing the hierarchy of available bets\n\n";//exrta
		 cin.clear();
		cin >> choice;
		switch(choice){
			case 'R':
				olog << this->getUsername() << " changed his username to : ";
				changeUsername();
				olog << this->getUsername() << " (R)" <<endl;
				continue;
			case 'P':
				changePassword();
				olog << this->getUsername() << " changed his password (P)"<<endl;
				continue;
			case 'B':
				showPunterBets();
				olog << this->getUsername() << " viewed his last bets (B)"<<endl;
				continue;
			case 'D':
				increaseBalance();
				continue;
			case 'N':
				olog << this->getUsername() << " returned to viewing hierarchy (N)"<<endl;
				return;//exit this function ;return to viewing heriarchy
		}
	}
}

void Punter::changeUsername(){ 
	string newUsername;
	cout << "Please enter your new username.\n";
	cin >> newUsername;
	if(newUsername == getUsername()){
		cout << "You should enter a different username from the one you have now!\n";
		return;
	}
	if(sb->usernameFound(newUsername)){//if new username already exists
		cout << "This username is already in use.\n";
		return;
	}
	else{
		this->setUsername(newUsername);
		cout <<"You username has been changed successfully to "<<newUsername<<endl;
		sb->updateUsersFile();//updates database
	}
}

void Punter::changePassword(){
	string newPassword1,newPassword2;
	newPassword1 = getpass("Please enter your new password.\n");
	if(newPassword1  == getPassword()){
		cout << "You should enter a different password from the one you have now!\n";
		return;
	}
	newPassword2 = getpass("Please re-enter your new password.\n");
	if(newPassword1 == newPassword2){
		setPassword(newPassword1);
		cout << "You password has been changed successfully!\n";
		sb->updateUsersFile();//updates database
	}
	else
		cout<<"Passwords do not match.\n";
}

void Punter::increaseBalance(){
	double moneyToAdd;
	cout << "Please enter the amount of money you want to add to your account.\n";
	cin >> moneyToAdd;
	if(moneyToAdd <= 0){
		cout << "Obviously you have to add an amount > 0 !\n";
		return;
	}
	increaseBalanceBy(moneyToAdd);
	cout << moneyToAdd<<" added successfully to your balance!\n";
	olog << this->getUsername() << " increased his balance (D) by "<< moneyToAdd <<endl;
	sb->updateUsersFile();//updates database
}

void Punter::printPlaceBetMenu(Node* marketNode){
	if(marketNode->getIsSettled()){
		cout << "This market is already settled, thus you can't place a bet in this market.\n";
		return;
	}
	vector<Node*>* selectionArray = marketNode->getNodes();
	char choice;
	int intChoice;
	string betMoney;//coupon letter or amount of money
	double doubleBetMoney;//convert to int
	cout << "Current balance : "<<this->getBalance()<<endl;
	cout << "Which option do you want to bet on? If you want to cancel the bet press 0\n";
	while(1){
		cin >> intChoice;
		if(intChoice  == 0){
			cout << "Bet cancelled.\n";
			return;}
		else if(intChoice <= 0 || intChoice > selectionArray->size() || selectionArray->at(intChoice-1)->getHidden() == true)
			cout << intChoice<< " is not a valid option. Please try giving ID of another selection\n";
		else if(selectionArray->at(intChoice-1)->getIsVoided())
			cout << "You can't place your bet on a voided selection!\nPlease try giving ID of another selection\n";
		else{
			cout <<"Coupons : ";
			printFreebets();
			cout <<"Please enter the amount of money you want to place on your choice\n"
				 <<"or the letter of the coupon you want to use(if you have one)\n";
			break;
		}
	}
	while(1){
		cin >> betMoney;
		char coupon = betMoney.at(0);
		if(coupon >= 'a' && coupon <= getFreebets().size() + 'a'){
			cout << "Are you sure you want to use this coupon?[y/n]\n";
			cin >> choice;
			if(choice == 'Y' || choice == 'y'){
				int bet_id1 = sb->getBetsArray().size()+1;//starts from 1 and increases...
				sb->getBetsArray().push_back(new Bet(bet_id1,this->getID(),selectionArray->at(intChoice-1)->getFullID(),getFreebets()[coupon - 97]));
				sb->addLastBetToFile();
				int couponPos = coupon -'a';
				getFreebets().erase(getFreebets().begin() + couponPos);
				cout <<"You have placed this bet successfully!\n";
				olog << this->getUsername() << " used a coupon of "<< getFreebets()[coupon - 'a'] <<" money on bet "<< selectionArray->at(intChoice-1)->getFullID()<<" (P)"<<endl;
				sb->updateUsersFile();
				return;
			}
			else//if input is not y or Y
				return;
		}
		/*converting string to double*/
		double doubleBetMoney = atof(betMoney.c_str());
		if(doubleBetMoney <=0.0){
			cout << "Invalid amount. Please give a valid amount(>0)\n";
			continue;
		}
		cout << "Are you sure you want to place this bet?[y/n]\n";
		cin >> choice;
		if(choice == 'Y' || choice == 'y'){
			if(this->getBalance() == 0){
				cout << "Your current balance is 0. Please fill your wallet with money\n"
					 << "if you want to bet. You can do this at your Account's Opions.\n";
				return;
			}
			else if(this->getBalance() < doubleBetMoney)
				cout << "Please bet an amount equal to or less than your balance ("<< this->getBalance()<<")"<<endl;
			else{//amount is legit
				sb->increaseProfit(doubleBetMoney);
				this->decreaseBalanceBy(doubleBetMoney);
				int bet_id = sb->getBetsArray().size()+1;//starts from 1 and increases...
				sb->getBetsArray().push_back(new Bet(bet_id,this->getID(),selectionArray->at(intChoice-1)->getFullID(),doubleBetMoney));
				sb->addLastBetToFile();
				cout <<"You have placed this bet successfully!\n";
				olog << this->getUsername() << " placed "<< doubleBetMoney <<" money on bet "<< selectionArray->at(intChoice-1)->getFullID()<<" (P)"<<endl;
				sb->updateUsersFile();
				return;
			}
		}
		else{
			cout<<"Bet cancelled.\n";
			return;
		}
	}
}

void Punter::showPunterBets(){
	cout << "bet_id|user_id|node_id|stake|result\n";
	for(int i = 0; i < sb->getBetsArray().size(); i++)
		if(sb->getBetsArray().at(i)->getUserID() == this->getID())
			sb->getBetsArray().at(i)->printBetOut();
}

void Punter::printFreebets(){
	char c = 97;
	for(int i = 0; i < getFreebets().size(); i++){
		c = 97 + i;
		cout << c << ") "<< getFreebets()[i] << " ";
	}
	cout <<endl;
}













