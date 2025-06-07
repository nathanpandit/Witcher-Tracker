#ifndef INVENTORY_H
#define INVENTORY_H
#include <iostream>
#include <string>
#include <map>
#include <ostream>

using namespace std;

class Inventory {
public:
    map<string, int> ingredients;
    map<string, int> potions;
    map<string, int> trophies;

    Inventory();
    ~Inventory();

    bool addToMap(map<string, int>& whichMap, int quantity, const string& name);
    void printMap(const map<string, int>& whichMap);
    bool removeFromMap(map<string, int>& whichMap, int quantity, const string& name);
    int totalItem(map<string, int>& whichMap, const string& name);
    bool isEnough(map<string, int>& whichMap, int quantityNeeded, const string& name);

};



#endif //INVENTORY_H