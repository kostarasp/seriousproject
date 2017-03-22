#include "Sportsbook.h"
#include <iostream>
#include <fstream>
#include "Category.h"
#include "Director.h"
#include "Trader.h"
#include "Punter.h"
#include "Bet.h"
#include "User.h"
#include "Event.h"
#include "Market.h"
#include "Selection.h"
#include "Category.h"
#include "Subcategory.h"
#include <unistd.h>
using namespace std;

Sportsbook::Sportsbook(): name("Better"),posostoPromithias(0.05),user(NULL),hierarchy(new Node("home"))
{
	cout << "                   Welcome to                  \n\n";
    cout << "\x1B[32m";
    cout <<"OOOOOO  OOOOOOO OOOOOOO OOOOOOO OOOOOOO OOOOO\n"
         <<"O     O O          O       O    O       O    O\n"
         <<"O     O O          O       O    O       O     O\n"
         <<"O     O O          O       O    O       O     O\n"
         <<"O     O O          O       O    O       O    O\n"
         <<"OOOOOO  OOOOOOO    O       O    OOOOOOO OOOOO\n"
         <<"O     O O          O       O    O       O    O\n"
         <<"O     O O          O       O    O       O     O\n"
         <<"O     O O          O       O    O       O      O\n"
         <<"OOOOOO  OOOOOOO    O       O    OOOOOOO O       O\n"
         <<"\033[0m" <<endl;
	initializeProfits();
	loadUsersToProgram();
	loadHierarchyToProgram();
	loadBetsToProgram();
}

Sportsbook::~Sportsbook()
{
	
}
/**************************************************************************************************************************************/
void Sportsbook::loadUsersToProgram(){
	ifstream input("users.csv");
	int user_id,type=0;
	double balance;
	string username,fullname,password,status;
	input.ignore(256,'\n');//ignore 1st line
	while(1){
		/*read data formatted from users.csv*/
		input >> user_id;
		if(input.eof()) break; //sinthiki gia na teliwsei to loop
		input.ignore();
		getline(input,username,'|');
		getline(input,fullname,'|');
		getline(input,password,'|');
		input >> type;
		/*we proceed depending on type*/

		vector<double> freebets;
		double coupon;
		char c;
		
		switch(type){
			case 1://punter
				input.ignore();
				getline(input,status,'|');
				input >>balance;
				input.ignore();
				while((c = input.peek()) != '\n'){//if no freebets do nothing
					if (c == ',')//if next char comma ignore
						input.ignore();
					input >> coupon;
					freebets.push_back(coupon);
				}

				usersArray.push_back(new Punter(user_id,username,fullname,password,type,status,balance,freebets));
				break;
			case 2://trader
				usersArray.push_back(new Trader(user_id,username,fullname,password));
				input.ignore(256,'\n');//ignore rest of the line
				break;
			case 3://director
				usersArray.push_back(new Director(user_id,username,fullname,password));
				input.ignore(256,'\n');//ignore rest of the line
				break;
			default://guest
				usersArray.push_back(new User(user_id,username,"guest",password));
				input.ignore(256,'\n');
				break;
		}
	}
	//cout << "Users loaded to program successfully. Total users : " << usersArray.size() <<endl;
	input.close();
}

void Sportsbook::loadHierarchyToProgram(){
	ifstream inHierarchy("hierarchy.dat");
	char cc;
	int dotnum = 0;//dot counter to determine type of node
	string tempname;//needed for names of nodes
	int idArray[5] = {0,0,0,0,0};//positions : 0->categoryID,1->subcategoryID,2->eventID,3->marketID,4->selectionID
	int selprice1,selprice2;//needed for yield
	while(inHierarchy){
		inHierarchy >> idArray[0];	//reads first id
		if(!inHierarchy.eof()){	//eof can be encountered anywhere
			cc = inHierarchy.get();	
			if (cc == '.') dotnum++;
		}
		else break;
		while (cc != ' '){	//After ' ' it's the node name	
			inHierarchy >> idArray[dotnum];
			cc = inHierarchy.get();
			if(cc == '.') dotnum++;		//count dots to recognize node type and save last num for node id 	
		}
		
		if(dotnum != 4){
			getline(inHierarchy,tempname,'\n');
		}
		else{	//if it is selection read differently
			getline(inHierarchy,tempname,'#');	// # X / Y
			inHierarchy >> selprice1;
			inHierarchy.get();
			inHierarchy >> selprice2;
		} 
		switch(dotnum){
			//category
		  	case 0 : hierarchy->getNodesRef()
			.push_back(new Category(tempname,idArray[0])); break;
			//subcategory
			case 1 : hierarchy->getNodesRef()[idArray[0]-1]->getNodesRef()
			.push_back(new Subcategory(tempname,idArray[0],idArray[1])); break;
			//event
			case 2 : hierarchy->getNodesRef()[idArray[0]-1]->getNodesRef()[idArray[1]-1]->getNodesRef()
			.push_back(new Event(tempname,idArray[0],idArray[1],idArray[2])); break;
			//market
			case 3 : hierarchy->getNodesRef()[idArray[0]-1]->getNodesRef()[idArray[1]-1]->getNodesRef()[idArray[2]-1]->getNodesRef()
			.push_back(new Market(tempname,idArray[0],idArray[1],idArray[2],idArray[3])); break;
			//selection
			case 4 : hierarchy->getNodesRef()[idArray[0]-1]->getNodesRef()[idArray[1]-1]->getNodesRef()[idArray[2]-1]->getNodesRef()[idArray[3]-1]->getNodesRef()
			.push_back(new Selection(tempname,idArray[0],idArray[1],idArray[2],idArray[3],idArray[4],selprice1,selprice2)); break;
			
		}
		dotnum = 0;
	}
	inHierarchy.close();
	//cout<<"Hierarchy loaded to program successfully!"<<endl;
}

void Sportsbook::loadBetsToProgram(){
	ifstream input("bets.csv");
	input.ignore(256,'\n');//ignore 1st line
	int bet_id,user_id;
	string node_id;
	double stake;
	char result;
	while(1){
		input >> bet_id;
		if(input.eof()) break;
		input.ignore();
		input >> user_id;
		input.ignore();
		getline(input,node_id,'|');
		input >> stake;
		input.ignore();
		input >> result;
		input.ignore(); //ignores \n
		betsArray.push_back(new Bet(bet_id,user_id,node_id,stake,result));
	}
	input.close();
	//cout << "Bets loaded to program successfully\n";
}
/**************************************************************************************************************************************/
int Sportsbook::createAccount(){
	string usernameGiven,
		   passwordGiven,
		   passwordGiven2,
		   fullname;
	cout << "Please give username for your new account.\n";
	getline(cin,usernameGiven,'\n');
	for(int i = 0;i<usersArray.size();i++)//checks if username exists
			if(usersArray[i]->getUsername() == usernameGiven){//if username exists return 0
				cout << "This username already exists. Programm will be terminated.\n";
				return 0;
			}
	while(1){//break when passwords match
		//cout << "Password : ";
		//getline(cin,passwordGiven,'\n');
		passwordGiven = getpass("Password : ");
		//cout << "Please re-type your password.\n";
		//getline(cin,passwordGiven2,'\n');
		passwordGiven2 = getpass("Please re-type your password : ");
		if(passwordGiven == passwordGiven2)//if passwords match
			break;
		else
			cout << "Passwords do not match. Please try again.\n";
	}
	cout << "Please give your full name.\n";
	getline(cin,fullname,'\n');
	int ID = usersArray.size() + 1 ;
	usersArray.push_back(new Punter(ID,usernameGiven,fullname,passwordGiven,1,"A",0.0));
	cout << "Your account has been created successfully!\n";
	updateUsersFile();
	user = usersArray[ID-1];//assign last user of array(the one we just added)
	user->printOptions(this);
	return 1;
}

int Sportsbook::login(){
	string usernameGiven,passwordGiven;
	
	cout <<"Please give username and password to login.\n"
	 	 <<"If you don't have an account, press enter\n"
		 <<"or guest:guest to login as visitor.\n";
	while(1){
		cout << "Username : ";
		getline(cin,usernameGiven,'\n');
		if(usernameGiven.empty()){//if enter is pressed
			user = usersArray[3];//assign guest to user
			user->printOptions(this);
			return 1;
		}
		for(int i=0;i<usersArray.size();i++)//checks if username exists
			if(usernameGiven == usersArray[i]->getUsername()){
				this->user = usersArray[i];//assign to the temp. user of program
				break;
			}
		//cout << "Password : ";
		//getline(cin,passwordGiven,'\n');
		passwordGiven = getpass("Password : ");
		if(user != NULL && passwordGiven == user->getPassword()){//check if password matches temp user's password
			if(user->getType()==1){
				if(user->getStatus().at(0) == 'L'){
					cout << "This account is blocked. Program will be terminated.\n";
					return 0;
				}
				else {
					this->user->printOptions(this);
					return 1;
				}
			}
			else{//we can't block traders
				this->user->printOptions(this);
				return 1;
			}
		}
		cout << "Your credentials are wrong. Try again.\n";
	}
}	
/**************************************************************************************************************************************/
void Sportsbook::updateUsersFile(){
	ofstream output("users.csv");
	output << "user_id|username|fullname|password|type|status|balance|freeBets\n";
	for(int i=0;i<usersArray.size();i++)
		usersArray[i]->printInfoToFile(output);
	cout << "~users.csv updated.\n";
	output.close();
}

void Sportsbook::updateHierarchyFile(){	//access hierarchy and print to file --selections have different print method
	//for each category,for each subcategory etc.
	ofstream outHierarchy("hierarchy.dat");
	for(int i = 0; i < hierarchy->getNodesRef().size(); i++){
		hierarchy->getNodesRef()[i]->printWholeNode(outHierarchy);
		for (int j = 0; j < hierarchy->getNodesRef()[i]->getNodesRef().size(); j++){
			hierarchy->getNodesRef()[i]->getNodesRef()[j]->printWholeNode(outHierarchy);
			for(int k = 0; k <  hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef().size(); k++){
				hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->printWholeNode(outHierarchy);
				for(int m = 0; m < hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef().size(); m++){
					hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->printWholeNode(outHierarchy);
					for(int n = 0; n < hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef().size(); n++){
						outHierarchy <<hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->getFullID()<<" "
									 <<hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->getName();
						outHierarchy <<"#"<<hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->getFract1();
						outHierarchy <<"/"<<hierarchy->getNodesRef()[i]->getNodesRef()[j]->getNodesRef()[k]->getNodesRef()[m]->getNodesRef()[n]->getFract2()<<endl;
					}
				}
			}
		}
	}
	outHierarchy.close();
	cout << "~hierarchy.dat updated\n";
}

void Sportsbook::updateBetsFile(){
	ofstream betFile("bets.csv");
	betFile << "bet_id|user_id|node_id|stake|result\n";
	for(int i=0; i< betsArray.size();i++)
		betsArray[i]->printBet(betFile);
	betFile.close();
	cout << "~bets.csv updated\n";
}

void Sportsbook::addLastBetToFile(){
	ofstream betFile("bets.csv",ios::app);
	betsArray[betsArray.size()-1]->printBet(betFile);//print last bet to betFile
	cout << "~Bet added to bets.csv successfully!\n";
	betFile.close();
}
/**************************************************************************************************************************************/
bool Sportsbook::usernameFound(string unForSearch){
	for(int i=0;i<usersArray.size();i++)
		if(usersArray[i]->getUsername() == unForSearch)
			return true;//return true if username found
	return false;//return false if username not found
}

void Sportsbook::showLastBets(int n){/*prints last n bets*/
	cout << "bet_id|user_id|node_id|stake|result\n";
	if(n >= betsArray.size())//print last 20
		for(int i = betsArray.size()-1;i>=0;i--)
			betsArray[i]->printBetOut();
	else//if n < betsArray.size() 
		for(int i=betsArray.size()-1;i>betsArray.size()-n-1;i--)
			betsArray[i]->printBetOut();
}

void Sportsbook::refund(double stake,User* us){
	profit -= stake;
	us->increaseBalanceBy(stake);
	cout << "REFUND : System returned "<< stake <<" to user "<< us->getUsername()<<endl;
}

void Sportsbook::userWins(double amount,User* us){
	us->increaseBalanceBy((1-posostoPromithias)*amount);
	profit += amount*posostoPromithias;
}

void Sportsbook::initializeProfits(){
	ifstream infile("profit.txt");
	infile >> profit;
	infile.close();
	//cout << name << "'s profit : "<< profit<<endl;
}

































