#include "Alchemy.h"

#include <algorithm>
#include "Inventory.h"

using namespace std;

Alchemy::Alchemy() = default; // default constructor
Alchemy::~Alchemy() = default; // default destructor

/*
 * adds the potion name and its recipe to recipeBook object
 * if the potion name is already in the book, returns "Already known formula"
 */
void Alchemy::learnNewRecipe(string potionName, vector<pair<string,int>> ingredientList) {
    auto itr = recipeBook.find(potionName);
    if (itr != recipeBook.end()) {
        cout << "Already known formula" << endl;
    }
    else {
        recipeBook[potionName] = ingredientList;
        cout << "New alchemy formula obtained: " << potionName << endl;
    }
}

/*
 * makes potion using the required ingredients from inventory,
 * if there isn't sufficient amount ingredients or formula is unknown can't brew the function, returns false
 * if brews successfully returns true.
 */
bool Alchemy::brewPotion(Inventory& inventory, string potionName) {
    auto itr = recipeBook.find(potionName);
    if (itr == recipeBook.end()) {
        cout << "No formula for " << potionName << endl;
        return false;
    }

    vector<pair<string, int>> recipe = recipeBook[potionName];

    for (pair<string, int> ingredient : recipe) {
        if (!inventory.isEnough(inventory.ingredients, ingredient.second, ingredient.first)) {
            cout << "Not enough ingredients" << endl;
            return false;
        }
    }

    for (pair<string, int> ingredient : recipe) {
        inventory.removeFromMap(inventory.ingredients, ingredient.second, ingredient.first);
    }
    inventory.addToMap(inventory.potions, 1, potionName);
    cout << "Alchemy item created: " << potionName << endl;
    return true;

}

/*
 * helper function for encounter action,
 * if the potion exists in inventory and we know that its effective against encountered monster,
 * we consume 1 unit of that function.
 */
bool Alchemy::consumePotion(Inventory &inventory, string potionName) {
    bool isEnough = inventory.isEnough(inventory.potions, 1, potionName);
    if (!isEnough) {
        return false;
    }
    inventory.removeFromMap(inventory.potions, 1, potionName);
    return true;
}

/*
 * prints the ingredient list for the given potion,
 * if the formula is not known prints "No formula for..."
 */
void Alchemy::printSortedIngredients(string potionName) {
    auto itr = recipeBook.find(potionName);
    if (itr == recipeBook.end()) {
        cout << "No formula for " << potionName << endl;
        return;
    }

    vector<pair<string, int>> recipe = recipeBook[potionName];
    sort(recipe.begin(), recipe.end(),  [](const pair<string,int>& a, const pair<string,int>& b) {
        if (a.second == b.second) {
            return a.first < b.first;
        }
         return a.second > b.second;
     });
    bool willPlaceComma = false;
    for (pair<string, int> ingredient : recipe) {
        if (!willPlaceComma) {
            cout << ingredient.second << " " << ingredient.first;
            willPlaceComma = true;
        }
        else {
            cout << ", ";
            cout << ingredient.second << " " << ingredient.first;
        }
    }
    cout << endl;
}