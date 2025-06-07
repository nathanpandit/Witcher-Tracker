//
// Created by Casperr on 24.05.2025.
//

#include "Bestiary.h"
#include <iostream>
#include <algorithm>

#include "Alchemy.h"
#include "Inventory.h"

using namespace std;

Bestiary::Bestiary() = default; // default constructor
Bestiary::~Bestiary() = default; // default destructor


/*
 * checks if a sign or a potion is effective against a given monster.
 * we use this in learn command, if the monster is not known, we initialize it in map,
 * if its already known we update the value in map.
 */
void Bestiary::checkAndUpdateEffectiveness(const string& type, const string& itemName, const string& monsterName) {
    auto itr = bestiary.find(monsterName);
    if (itr == bestiary.end()) { // if the monster is not known, adds it to bestiary
        cout << "New bestiary entry added: " << monsterName << endl;
        if (type == "sign") {
            vector<string> signVector;
            vector<string> potionVector;
            signVector.push_back(itemName);
            pair<vector<string>,vector<string>> pair;
            pair.first = signVector;
            pair.second = potionVector;
            bestiary[monsterName] = pair; // initializes the bestiary
        }
        else if (type == "potion") {
            vector<string> signVector;
            vector<string> potionVector;
            potionVector.push_back(itemName);
            pair<vector<string>,vector<string>> pair;
            pair.first = signVector;
            pair.second = potionVector;
            bestiary[monsterName] = pair; // initializes the bestiary
        }
    }
    else { // if the monster is known.
        if (type == "sign") {
            vector<string> &signVector = bestiary[monsterName].first; // first vector holds sign type items
            auto signItr = find(signVector.begin(), signVector.end(), itemName);
            if (signItr == signVector.end()) {
                signVector.push_back(itemName);
                cout << "Bestiary entry updated: " << monsterName << endl;
            }
            else {
                cout << "Already known effectiveness" << endl;
            }
        }
        else if (type == "potion") {
            vector<string> &potionVector = bestiary[monsterName].second; // second vector holds potion type items
            auto potionItr = find(potionVector.begin(), potionVector.end(), itemName);
            if (potionItr == potionVector.end()) {
                potionVector.push_back(itemName);
                cout << "Bestiary entry updated: " << monsterName << endl;
            }
            else {
                cout << "Already known effectiveness" << endl;
            }
        }
    }
}

/*
 * when geralt encounters a monster, we firstly check if the monster is known (in bestiary)
 * if the monster isn't known, Geralt barely escapes.
 * if its known, Geralt consumes every potion he knows that is effective and uses every sign (however sign count does'nt exists)
 * if geralt knows the monster, but doesn't have enough potions or signs to fight against, he barely escapes.
 */
void Bestiary::encounterMonster(Alchemy& alchemy, string monsterName, Inventory& inventory) {
    auto itr = bestiary.find(monsterName);
    if (itr == bestiary.end()) { // mosnter is not known.
        cout << "Geralt is unprepared and barely escapes with his life" << endl;
        return;
    }
    bool canFight = false;
    if (bestiary[monsterName].first.size() > 0) { // if there is a sign against that monster
        canFight = true; // geralt can fight
    }

    if (bestiary[monsterName].second.size() > 0) { // if there is no signs against monster, we check if geralt knows any potions against that monster.
        vector<string> potionVector = bestiary[monsterName].second;
        for (string potionName : potionVector) {
            if (alchemy.consumePotion(inventory, potionName)) {
                canFight = true;
            }
        }
    }

    if (canFight) {
        inventory.addToMap(inventory.trophies, 1, monsterName);
        cout << "Geralt defeats " << monsterName << endl;
    }
    else {
        cout << "Geralt is unprepared and barely escapes with his life" << endl;
    }
}

/*
 * this function adds every item in effective signs vector and effective potions vector in a one vector
 * sorts that vector and prints the items in sorted order.
 */
void Bestiary::printSortedEffectiveItems(const string& monsterName) {
    auto itr = bestiary.find(monsterName);
    if (itr == bestiary.end()) {
        cout << "No knowledge of " << monsterName << endl;
        return;
    }
    vector<string> allEffectiveItems = itr->second.first;
    allEffectiveItems.insert(allEffectiveItems.end(), itr->second.second.begin(), itr->second.second.end());
    sort(allEffectiveItems.begin(), allEffectiveItems.end());
    bool willPlaceComma = false;
    for (string s : allEffectiveItems) {
        if (!willPlaceComma) {
            cout << s;
            willPlaceComma = true;
        }
        else {
            cout << ", " << s;
        }
    }
    cout << endl;
}