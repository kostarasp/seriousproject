#pragma once

#include <fstream>
#include "Node.h"


class Selection : public Node
{
	int fract1;
	int fract2;
	double price;
	static string mode;
	bool isVoided;
	
	public:
		Selection(string,int,int,int,int,int,int,int);
		Selection( Selection& cp):Node(cp),fract1(cp.getFract1()),fract2(cp.getFract2()),price(cp.getPrice()),isVoided(cp.getIsVoided()){}
		~Selection();
		
		int getFract1(){return fract1;}
		int getFract2(){return fract2;}
		double getPrice(){return price;}

		static void toggleMode();
		static string getMode(){return mode;}

		void setSelectionVoided();

		void printNodeOption(bool=false);
		void reduceIDby1();
		void increaseIDby1();
		bool getIsVoided(){return isVoided;}
		void setResult(string);
		void printWholeNode(ofstream&);
		string getFullID();
		virtual Selection* makeCopy(){return new Selection(*this);}
};


