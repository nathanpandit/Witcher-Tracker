//
// Created by Casperr on 27.05.2025.
//

#ifndef ALCHEMY_H
#define ALCHEMY_H
#include <string>
#include <vector>
#include <map>
#include <iostream>

class Inventory;
using namespace std;

class Alchemy {

public:

    map<string, vector<pair<string,int>>> recipeBook;
    // recipe book has potion name as key
    // and ingredients in a vector as pairs <ingredientName, neededQuantity> as value

    Alchemy();
    ~Alchemy();
    void learnNewRecipe(string potionName, vector<pair<string,int>> ingredientList);
    bool brewPotion(Inventory& inventory, string potionName);
    bool consumePotion(Inventory &inventory, string potionName);
    void printSortedIngredients(string potionName);
};



#endif //ALCHEMY_H