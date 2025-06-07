//
// Created by Casperr on 24.05.2025.
//

#include "Inventory.h"

/*
 * adds an item to a map (thi is an abstract function that can be used for all maps (potion, bestiary etc.)
 */
bool Inventory::addToMap(map<string, int>& whichMap, int quantity, const string& name) {
    if (quantity <= 0 || name.empty()) {
        return false;
    }
    whichMap[name] += quantity; // works even if key doesn't exist yet
    return true;
}

/*
 * prints the items in a map (also an abstract function that can be applied to all maps)
 * in a sorted order by key.
 */
void Inventory::printMap(const map<string, int>& whichMap) {
    for (pair<string, int> p : whichMap) {
        if (p.second == 0) {
            continue;
        }
        cout << p.first << " " << p.second << endl;
    }
}

/*
 * removes a item by wanted quantity from a map (also an abstract function that can be applied to all maps).
 * returns true if item is successfully deleted, false if it wasn't found or the amount we have was insufficient.
 * designed for using an item from inventory in a specific needed amount (for example for brew command)
 */
bool Inventory::removeFromMap(map<string, int>& whichMap, int quantity, const string& name) {
    if (quantity <= 0 || name.empty()) {
        return false;
    }
    auto itr = whichMap.find(name);
    if (itr == whichMap.end()) {
        return false;
    }
    if (itr->second < quantity) {
        return false;
    }
    if (itr->second == quantity) {
        whichMap.erase(name);
    }
    else {
        itr->second -= quantity;
    }
    return true;
}

/*
 * returns how many of an item we have in inventory now.
 * this is also an abstract function that can be applied to all maps
 */
int Inventory::totalItem(map<string, int>& whichMap, const string& name) {
    auto itr = whichMap.find(name);
    if (itr == whichMap.end()) {
        return 0;
    }
    return whichMap[name];
}

/*
 * checks if we have adequate amount of a wanted item.
 * returns true if we do, returns false if we don't
 * this is also an abstract function that can be applied to all maps
 */
bool Inventory::isEnough(map<string, int>& whichMap, int quantityNeeded, const string& name) {
    if (quantityNeeded <= 0 || name.empty()) {
        return false;
    }
    auto itr = whichMap.find(name);
    if (itr == whichMap.end()) {
        return false;
    }
    int quantityAvailable = whichMap[name];
    if (quantityAvailable >= quantityNeeded) {
        return true;
    }
    return false;
}

Inventory::Inventory() = default;
Inventory::~Inventory() = default;