#include "user.h"


void User::addPocketmon(const Pocketmon& pocketmon) 
{
    pocketmons.push_back(pocketmon);
}

vector<Pocketmon> User::getPocketmons() 
{
    return pocketmons;
}


void User::printPocketmon()
{
    for (Pocketmon& p: pocketmons){
        p.printPocketInfo();
    }
}



