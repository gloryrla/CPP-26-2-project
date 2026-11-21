#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Skill { string name; int power; };

class Pocketmon {
private:
    int id;
    int maxHp;
    int crrHp;
    string name;
    string art;
    vector<Skill> skills;
    void printHpBar();
public:
    Pocketmon();
    Pocketmon(int id);
    void printPocketInfo();
    void printSkills();
    vector<Skill> getSkills();
};
