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
    int level;
    int exp;
    string name;
    string art;
    vector<Skill> skills;

public:
    void printHpBar();
    // hide면 그림 대신 같은 줄 수만큼 빈 줄 (화면 배치가 안 흔들리게)
    void printArt(bool hide = false) {
        if (!hide) { cout << art; return; }
        for (char c : art) if (c == '\n') cout << c;
    }
    Pocketmon();
    Pocketmon(int id);
    void printPocketInfo();
    void printSkills();
    vector<Skill> getSkills();
    int getHp();
    bool DemagePocketmon(Skill skill, bool flag);
    string getName();
    bool tryCatch();
    bool gainExp(int amount);
    int getLevel();
    void setLevel(int amount);
};
