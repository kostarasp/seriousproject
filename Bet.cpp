#include "Bet.h"
#include <iostream>
using namespace std;

Bet::Bet(int b_id,int u_id,string n_id,double stk,char res):
	bet_id(b_id),user_id(u_id),node_id(n_id),stake(stk),result(res)
{
}

Bet::~Bet()
{
}

void Bet::printBet(ofstream& output){
	output << bet_id << "|"
		   << user_id << "|"
		   << node_id << "|"
		   << stake << "|"
		   << result << endl;
}

void Bet::printBetOut(){
	cout << bet_id << "|"
		   << user_id << "|"
		   << node_id << "|"
		   << stake << "|"
		   << result << endl;
}
