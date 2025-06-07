
#ifndef BESTIARY_H
#define BESTIARY_H
#include <map>
#include <string>
#include <vector>

#include "Alchemy.h"

class Inventory;
using namespace std;

class Bestiary {
public:
    map<string, pair<vector<string>, vector<string>>> bestiary;
    // keys are monster names.
    // values are vector pairs.
    // first vector stores sign names, second vector stores potion names.
    Bestiary();
    ~Bestiary();
    void checkAndUpdateEffectiveness(const string& type, const string& itemName, const string& monsterName);
    void encounterMonster(Alchemy& alchemy, string monsterName, Inventory& inventory);
    void printSortedEffectiveItems(const string& monsterName);
};



#endif //BESTIARY_H