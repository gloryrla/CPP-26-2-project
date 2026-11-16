#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

const string NAMES[151] = {
    "bulbasaur","ivysaur","venusaur","charmander","charmeleon","charizard","squirtle","wartortle","blastoise","caterpie",
    "metapod","butterfree","weedle","kakuna","beedrill","pidgey","pidgeotto","pidgeot","rattata","raticate",
    "spearow","fearow","ekans","arbok","pikachu","raichu","sandshrew","sandslash","nidoran_f","nidorina",
    "nidoqueen","nidoran_m","nidorino","nidoking","clefairy","clefable","vulpix","ninetales","jigglypuff","wigglytuff",
    "zubat","golbat","oddish","gloom","vileplume","paras","parasect","venonat","venomoth","diglett",
    "dugtrio","meowth","persian","psyduck","golduck","mankey","primeape","growlithe","arcanine","poliwag",
    "poliwhirl","poliwrath","abra","kadabra","alakazam","machop","machoke","machamp","bellsprout","weepinbell",
    "victreebel","tentacool","tentacruel","geodude","graveler","golem","ponyta","rapidash","slowpoke","slowbro",
    "magnemite","magneton","farfetchd","doduo","dodrio","seel","dewgong","grimer","muk","shellder",
    "cloyster","gastly","haunter","gengar","onix","drowzee","hypno","krabby","kingler","voltorb",
    "electrode","exeggcute","exeggutor","cubone","marowak","hitmonlee","hitmonchan","lickitung","koffing","weezing",
    "rhyhorn","rhydon","chansey","tangela","kangaskhan","horsea","seadra","goldeen","seaking","staryu",
    "starmie","mr_mime","scyther","jynx","electabuzz","magmar","pinsir","tauros","magikarp","gyarados",
    "lapras","ditto","eevee","vaporeon","jolteon","flareon","porygon","omanyte","omastar","kabuto",
    "kabutops","aerodactyl","snorlax","articuno","zapdos","moltres","dratini","dragonair","dragonite","mewtwo",
    "mew"
};

class Pocketmon {
private:
    int id;
    string name;
    string art;

public:
    Pocketmon() {
        id = rand() % 151 + 1;
        name = NAMES[id - 1];   // 배열은 0부터라서 -1

        ifstream fin("../../pocketmonImages/" + to_string(id) + "_" + name + ".txt");
        string line;
        while (getline(fin, line)) art += line + "\n";
    }

    void print() {
        cout << "No." << id << " " << name << endl << art;
    }
};

int main() {
    srand(time(0));
    Pocketmon p;
    p.print();
}