#include <iostream>
using namespace std;
#include "Sportsbook.h"

int main(int argc, char** argv) {
    Sportsbook sportsbook;
    if(argc==2 && *(++argv[1]) == 'R')
        sportsbook.createAccount();//createAccount logs user into better after registration
    else
        sportsbook.login();//if no args
    return 0;
}

