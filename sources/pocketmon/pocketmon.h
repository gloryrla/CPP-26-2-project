#pragma once
#include <iostream>
#include <string>
using namespace std;

class Pocketmon {
private:
    int id;
    int maxHp;
    int crrHp;
    string name;
    string art;
    void printHpBar();
public:
    Pocketmon();
    Pocketmon(int id);
    void printPocketInfo();
};
