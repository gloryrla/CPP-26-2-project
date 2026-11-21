#include <string>
#include <vector>
#include "pocketmon.h"
using namespace std;


class User {
private:
    string name;
    vector<Pocketmon> pocketmons;

public:
    void addPocketmon(const Pocketmon& pocketmon);
    vector<Pocketmon> getPocketmons();
    void printPocketmon();

};



