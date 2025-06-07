#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "Alchemy.h"
#include "Bestiary.h"
#include "Inventory.h"

using namespace std;

int isAllDigits(const string& s);
int isAllAlpha(const string& s);
bool listIsValid(int startPos, int endPos, const vector<string>& tokens);
string trim_spaces(string_view sv);
string getExactPotionName(const string& input);
bool spacesAreValid(const string& potionName);

int main() {

    string inputLine;

    cout << ">> "; // >> on stdout for grader check
    fflush(stdout);

    if (!getline(cin, inputLine)) {
        return 0;
    } // detect EOF

    // instantiate inventory, bestiary and alchemy
    Inventory inventory = Inventory();
    Bestiary bestiary = Bestiary();
    Alchemy alchemy = Alchemy();

    //our main loop, gets input lines until line is Exit
    while (inputLine != "Exit") {

        vector<string> tokens; // first version of the tokens
        vector<string> finalTokens;
        istringstream iss(inputLine); // tokenize
        string token;

        while (iss >> token) {
            tokens.push_back(token);
        } //push every token into a vector

        // separate the commas as distinct tokens and push each new token into finalTokens
        for (const string& t : tokens) {

            if (t.find(',') == string::npos) {
                finalTokens.push_back(t);
            } // if there is no comma in token, add it as is
            else {
                if (t == ",") {
                    finalTokens.push_back(t);
                    continue;
                } // if the token is comma, push it
                int pos = t.find(','); //get the position of comma

                if (pos == 0) {
                    finalTokens.emplace_back(",");
                    finalTokens.push_back(t.substr(1, t.length()));
                } // if comma is at the start, add a comma to finalTokens, then rest of the token
                else if (pos == t.length() - 1) {
                    finalTokens.push_back(t.substr(0, t.length()-1));
                    finalTokens.emplace_back(",");
                } //if comma is at the end, add the first n-1 letters of token, then a comma
                else {
                    finalTokens.push_back(t.substr(0, pos));
                    finalTokens.emplace_back(",");
                    finalTokens.emplace_back(t.substr(pos+1, t.length()));
                } // if it is in the middle, add everything before the comma, then comma, then everyting after comma
            }
        }

        // if first token is geralt (we will perform an action)
        if (finalTokens[0] == "Geralt") {
            if (finalTokens[1] == "loots") {
                //if ingredient list is valid format, add each item to ingredients map
                if (listIsValid(2, finalTokens.size()-1, finalTokens)) {
                    for (int i = 2; i < finalTokens.size(); i++) {
                        if (finalTokens[i] == ",") {
                            continue;
                        }
                        int quantity = stoi(finalTokens[i]);
                        i++;
                        string ingredientName = finalTokens[i];
                        inventory.addToMap(inventory.ingredients, quantity, ingredientName);
                    }
                    cout << "Alchemy ingredients obtained" << endl;
                }
                else {
                    cout << "INVALID\n";
                    goto again;
                }
            }
            else if (tokens[1] == "trades") {
                // look for the word "trophy".
                int trophyPos = -1;
                for (int i = 0; i < (int)finalTokens.size(); i++) {
                    if (finalTokens[i] == "trophy") {
                        trophyPos = i;
                        break;
                    }
                }

                // invalid if trophy keyword not found
                if (trophyPos == -1) {
                    cout << "INVALID\n";
                    goto again;
                }

                // invalid if trophy word is at the end
                if (trophyPos == (int)finalTokens.size()-1) {
                    cout << "INVALID\n";
                    goto again;
                }

                // invalid if for doesn't come right after trophy
                if (finalTokens[trophyPos+1] != "for") {
                    cout << "INVALID\n";
                    goto again;
                }

                // checks trophy list
                if (listIsValid(2, trophyPos-1, finalTokens)) {
                    // checks ingredient list
                    if (listIsValid(trophyPos+2, (int)finalTokens.size()-1, finalTokens)) {
                        bool trophiesSufficient = true;
                        for (int i = 2; i < trophyPos; i++) {
                            // checks if we have enough of that trophy
                            if (finalTokens[i] == ",") {
                                continue;
                            }
                            int quantity = stoi(finalTokens[i]);
                            i++;
                            string trophyName = finalTokens[i];
                            if (!inventory.isEnough(inventory.trophies, quantity, trophyName)) {
                                trophiesSufficient = false;
                                break;
                            }
                        }

                        // if we have enough trophies, remove trophies and add ingredients
                        if (trophiesSufficient) {
                            for (int i = 2; i < trophyPos; i++) {
                                if (finalTokens[i] == ",") {
                                    continue;
                                }
                                int quantity = stoi(finalTokens[i]);
                                i++;
                                string trophyName = finalTokens[i];
                                inventory.removeFromMap(inventory.trophies, quantity, trophyName);
                            }
                            for (int i = trophyPos+2 ; i < (int)finalTokens.size(); i++) {
                                if (finalTokens[i] == ",") {
                                    continue;
                                }
                                int quantity = stoi(finalTokens[i]);
                                i++;
                                string ingredientName = finalTokens[i];
                                inventory.addToMap(inventory.ingredients, quantity, ingredientName);
                            }
                            cout << "Trade successful" << endl;
                        }
                        else {
                            cout << "Not enough trophies\n";
                            goto again;
                        }

                    }
                    else {
                        cout << "INVALID\n";
                        goto again;
                    }
                }
                else {
                    cout << "INVALID\n";
                    goto again;
                }

            }
            else if (tokens[1] == "brews") {

                if (finalTokens.size() <= 2) { // check for missing potion name
                    cout << "INVALID\n";
                    goto again;
                }

                string potionName = getExactPotionName(inputLine);

                // if potion name has multiple spaces, invalid
                if (spacesAreValid(potionName)) {
                    if (isAllAlpha(potionName) == 1) {
                        alchemy.brewPotion(inventory, potionName);

                    }
                    else {
                        cout << "INVALID\n";
                    }

                }
                else {
                    cout << "INVALID\n";
                }
            }
            else if (tokens[1] == "learns") {

                // if geralt is learning a sign, the sentence has a definite format, check for that format
                if (finalTokens[3] == "sign") {
                    if (finalTokens[4] != "is" || finalTokens[5] != "effective" || finalTokens[6] != "against" || finalTokens.size() != 8) {
                        cout << "INVALID\n";
                    }
                    else {
                        string signName = finalTokens[2];
                        string monsterName = finalTokens[7];
                        if (!isAllAlpha(signName) ||!isAllAlpha(monsterName)) {
                            cout << "INVALID\n";
                        }
                        else {
                            bestiary.checkAndUpdateEffectiveness("sign", signName, monsterName);
                        }
                    }
                }
                // if not a sign, we look for the placement of the "potion" word.
                else {
                    int potionIndex = -1;
                    for (int i = 0; i < (int)finalTokens.size(); i++) {
                        if (finalTokens[i] == "potion") {
                            potionIndex = i;
                            break;
                        }
                    }
                    // invalid if potion name is not found
                    if (potionIndex == -1) {
                        cout << "INVALID\n";
                        goto again;
                    }
                    string potionName = getExactPotionName(inputLine);
                    if (!isAllAlpha(potionName) || !spacesAreValid(potionName)) {
                        cout << "INVALID\n";
                        goto again;
                    }
                    if (potionIndex + 3 >= finalTokens.size()) {
                        cout << "INVALID\n";
                        goto again;
                    }
                    // potion recipe case:
                    if (finalTokens[potionIndex+1] == "consists" && finalTokens[potionIndex+2] == "of") {
                        if (listIsValid(potionIndex+3, (int)finalTokens.size()-1, finalTokens)) {
                            vector<pair<string, int>> recipe;
                            for (int i = potionIndex+3 ; i < (int)finalTokens.size(); i++) {
                                if (finalTokens[i] == ",") {
                                    continue;
                                }
                                int quantity = stoi(finalTokens[i]);
                                i++;
                                string ingredientName = finalTokens[i];
                                pair<string,int> newPair;
                                newPair.first = ingredientName;
                                newPair.second = quantity;
                                recipe.push_back(newPair);
                            }
                            alchemy.learnNewRecipe(potionName, recipe);
                            goto again;
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }

                    // potion effectiveness case:
                    else if (finalTokens[potionIndex+1] == "is" && finalTokens[potionIndex+2] == "effective" && finalTokens[potionIndex+3] == "against") {
                        if (potionIndex+4 != finalTokens.size()-1) {
                            cout << "INVALID\n";
                            goto again;
                        }
                        string monsterName = finalTokens[potionIndex+4];
                        if (!isAllAlpha(monsterName)) {
                            cout << "INVALID\n";
                            goto again;
                        }
                        bestiary.checkAndUpdateEffectiveness("potion", potionName, monsterName);
                    }

                    else {
                        cout << "INVALID\n";
                    }

                }
            }
            else if (tokens[1] == "encounters") {
                // sentence has to consist exactly 4 words: "Geralt encounters a ...".
                if (finalTokens.size() != 4) {
                    cout << "INVALID\n";
                    goto again;
                }
                if (finalTokens[2] != "a") {
                    cout << "INVALID\n";
                    goto again;
                }
                string monsterName = finalTokens[3];
                if (!isAllAlpha(monsterName)) {
                    cout <<"INVALID\n";
                    goto again;
                }
                bestiary.encounterMonster(alchemy, monsterName, inventory);
            }
            else {
                cout << "INVALID\n";
            }
        }

        // total queries
        else if (finalTokens[0] == "Total") {
            /*
             * each case does the same thing for different inventories
             * total queries sorts the inventory in alphabetical order and prints it
             */
            // ingredient queries
            if (finalTokens[1] == "ingredient") {
                if (finalTokens[2] == "?" && finalTokens.size() == 3) {
                    if (inventory.ingredients.empty()) {
                        cout << "None" << endl;
                        goto again;
                    }
                    bool willPlaceComma = false;
                    for (pair<string, int> item : inventory.ingredients) {
                        if (!willPlaceComma) {
                            cout << item.second << " " << item.first;
                            willPlaceComma = true;
                            continue;
                        }
                        cout << ", ";
                        cout << item.second << " " << item.first;
                    }
                    cout << endl;
                }
                else {
                    if (finalTokens.size() == 3) {
                        if (finalTokens[2][finalTokens[2].length()-1] == '?') {
                            finalTokens[2].pop_back();
                            if (isAllAlpha(finalTokens[2])) {
                                cout << inventory.totalItem(inventory.ingredients, finalTokens[2]) << endl;
                            }
                            else {
                                cout << "INVALID\n";
                            }
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                    else if (finalTokens.size() == 4) {
                        if (isAllAlpha(finalTokens[2]) && finalTokens[3] == "?") {
                            cout << inventory.totalItem(inventory.ingredients, finalTokens[2]) << endl;

                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                    else {
                        cout << "INVALID\n";
                    }
                }
            }
            else if (finalTokens[1] == "ingredient?" && finalTokens.size() == 2) {
                if (inventory.ingredients.empty()) {
                    cout << "None" << endl;
                    goto again;
                }
                bool willPlaceComma = false;
                for (pair<string, int> item : inventory.ingredients) {
                    if (!willPlaceComma) {
                        cout << item.second << " " << item.first;
                        willPlaceComma = true;
                        continue;
                    }
                    cout << ", ";
                    cout << item.second << " " << item.first;
                }
                cout << endl;
            }

            // trophy queries
            else if (finalTokens[1] == "trophy") {
                if (finalTokens[2] == "?" && finalTokens.size() == 3) {
                    if (inventory.trophies.empty()) {
                        cout << "None" << endl;
                        goto again;
                    }
                    bool willPlaceComma = false;
                    for (pair<string, int> item : inventory.trophies) {
                        if (!willPlaceComma) {
                            cout << item.second << " " << item.first;
                            willPlaceComma = true;
                            continue;
                        }
                        cout << ", ";
                        cout << item.second << " " << item.first;
                    }
                    cout << endl;
                }
                else {
                    if (finalTokens.size() == 3) {
                        if (finalTokens[2][finalTokens[2].length()-1] == '?') {
                            finalTokens[2].pop_back();
                            if (isAllAlpha(finalTokens[2])) {
                                cout << inventory.totalItem(inventory.trophies, finalTokens[2]) << endl;
                            }
                            else {
                                cout << "INVALID\n";
                            }
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                    else if (finalTokens.size() == 4) {
                        if (isAllAlpha(finalTokens[2]) && finalTokens[3] == "?") {
                            cout << inventory.totalItem(inventory.trophies, finalTokens[2]) << endl;
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                    else {
                        cout << "INVALID\n";
                    }
                }
            }
            else if (finalTokens[1] == "trophy?" && finalTokens.size() == 2) {
                if (inventory.trophies.empty()) {
                    cout << "None" << endl;
                    goto again;
                }
                bool willPlaceComma = false;
                for (pair<string, int> item : inventory.trophies) {
                    if (!willPlaceComma) {
                        cout << item.second << " " << item.first;
                        willPlaceComma = true;
                        continue;
                    }
                    cout << ", ";
                    cout << item.second << " " << item.first;
                }
                cout << endl;
            }

            // potion queries
            else if (finalTokens[1] == "potion") {
                if (finalTokens.size() == 2) {
                    cout << "INVALID\n";
                    goto again;
                }
                if (finalTokens[2] == "?" && finalTokens.size() == 3) {
                    if (inventory.potions.empty()) {
                        cout << "None" << endl;
                        goto again;
                    }
                    bool willPlaceComma = false;
                    for (pair<string, int> item : inventory.potions) {
                        if (!willPlaceComma) {
                            cout << item.second << " " << item.first;
                            willPlaceComma = true;
                            continue;
                        }
                        cout << ", ";
                        cout << item.second << " " << item.first;
                    }
                    cout << endl;
                }
                else {
                    if (!spacesAreValid(getExactPotionName(inputLine))) {
                        cout << "INVALID\n";
                        goto again;
                    }
                    else {
                        if (finalTokens[finalTokens.size()-1][finalTokens[finalTokens.size()-1].length()-1] == '?') {
                            finalTokens[finalTokens.size()-1].pop_back();
                            string potionName = "";
                            for (int i = 2; i < finalTokens.size(); i++) {
                                if (!isAllAlpha(finalTokens[i])) {
                                    cout << "INVALID\n";
                                    goto again;
                                }
                                if (!finalTokens[i].empty()) {
                                    potionName += finalTokens[i];
                                    potionName += " ";
                                }
                            }
                            potionName.pop_back();
                            cout << inventory.totalItem(inventory.potions, potionName) << endl;
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                }
            }
            else if (finalTokens[1] == "potion?" && finalTokens.size() == 2) {
                if (inventory.potions.empty()) {
                    cout << "None" << endl;
                    goto again;
                }
                bool willPlaceComma = false;
                for (pair<string, int> item : inventory.potions) {
                    if (!willPlaceComma) {
                        cout << item.second << " " << item.first;
                        willPlaceComma = true;
                        continue;
                    }
                    cout << ", ";
                    cout << item.second << " " << item.first;
                }
                cout << endl;
            }

            else {
                cout << "INVALID\n";
            }

        }

        // what queries
        else if (finalTokens[0] == "What" && finalTokens[1] == "is") {

            // potion recipe query
            if (finalTokens[2] == "in") {

                if (finalTokens[3] == "\0") { // if potion name is mssing
                    cout << "INVALID\n";
                    goto again;
                }

                if (!spacesAreValid(getExactPotionName(inputLine))) {
                    cout << "INVALID\n";
                    goto again;
                }
                else {
                    // assert "?" grammar
                    if (finalTokens[finalTokens.size()-1][finalTokens[finalTokens.size()-1].length()-1] == '?') {
                        // missing potion name
                        if (finalTokens[finalTokens.size()-1] == "?" && finalTokens.size() == 4) {
                            cout << "INVALID\n";
                            goto again;
                        }
                        finalTokens[finalTokens.size()-1].pop_back(); // delete question mark
                        string potionName = "";
                        for (int i = 3; i < finalTokens.size(); i++) {
                            if (!isAllAlpha(finalTokens[i])) {
                                cout << "INVALID\n";
                                goto again;
                            }
                            if (!finalTokens[i].empty()) {
                                potionName += finalTokens[i];
                                potionName += " ";
                            }
                        }
                        potionName.pop_back(); // delete the extra space added at the end
                        alchemy.printSortedIngredients(potionName);
                    }
                    else {
                        cout << "INVALID\n";
                    }

                }
            }

            // monster effectiveness query
            else if (finalTokens[2] == "effective") {
                if (finalTokens[3] == "against") {
                    if (finalTokens.size() == 6) {
                        if (finalTokens[5] == "?" && isAllAlpha(finalTokens[4])) {
                            bestiary.printSortedEffectiveItems(finalTokens[4]);
                        }
                        else {
                            cout << "INVALID\n";
                        }
                    }
                    else if (finalTokens.size() == 5) {
                        if (finalTokens[4][finalTokens[4].length()-1] != '?') {
                            cout << "INVALID\n";
                        }
                        else {
                            if (finalTokens[4] == "?") {
                                cout << "INVALID\n";
                                goto again;
                            }
                            finalTokens[4].pop_back();
                            if (isAllAlpha(finalTokens[4])) {
                                bestiary.printSortedEffectiveItems(finalTokens[4]);
                            }
                            else {
                                cout << "INVALID\n";
                            }
                        }
                    }

                    else {
                        cout << "INVALID\n";
                    }
                }
                else {
                    cout << "INVALID\n";
                }
            }

            else {
                cout << "INVALID\n";
            }

        }

        // invalid input
        else {
            cout << "INVALID\n";
        }


        again:
        fflush(stdout);
 // DO SOMETHING LIKE MEMSET HERE
        printf(">> ");
        fflush(stdout);
        if (!getline(cin, inputLine)) {
            break;
        } // Detect EOF
    }




    return 0;
}

/*
 * checks if ingredient list is of valid format
 * valid format is "<quantity> <name>, <quantity> <name> . . .
 */
bool listIsValid(int startPos, int endPos, const vector<string>& tokens) {

    if (startPos > endPos) {
        return false;
    }

    int i = startPos;

    while (i <= endPos) {
        // must start with a number
        if (!isAllDigits(tokens[i]) || stoi(tokens[i]) <= 0) {
            return false;
        }
        i++;

        // must be followed by a valid ingredient
        if (i > endPos || !isAllAlpha(tokens[i])) {
            return false;
        }
        i++;

        // if end of list, break (valid)
        if (i > endPos) {
            return true;
        }

        // expect a comma
        if (tokens[i] != ",") {
            return false;
        }
        i++;

        // must be followed by another quantity
        if (i > endPos) {
            return false;
        }
    }

    return true;
}

/*
 * trims the space at the start and at the end and returns the trimmed string
 */
string trim_spaces(string_view sv) {

    size_t first = sv.find_first_not_of(' ');

    // all spaces or empty
    if (first == string_view::npos) {
        return "";
    }

    size_t last = sv.find_last_not_of(' ');

    return string(sv.substr(first, last - first + 1));
}


/*
 * returns the string with normal spacing
 */
string normalizeSpaces(const string& s) {
    istringstream iss(s);

    string word, result;

    while (iss >> word) {
        result += word + " ";
    }

    // remove trailing space
    if (!result.empty()) {
        result.pop_back();
    }
    return result;
}


/*
 * returns the part of the string that consists the potion name
 * used for spacing checks
 */
string getExactPotionName(const string& input) {

    string_view input_sv = input;

    // brews case
    if (size_t pos = input_sv.find("brews"); pos != string_view::npos) {
        auto potion_part = input_sv.substr(pos + 6);
        return trim_spaces(potion_part);
    }
    // what query case
    else if (size_t posWhat = input_sv.find("What"); posWhat != string_view::npos) {
        if (size_t posIs = input_sv.find("is"); posIs != string_view::npos) {
            if (size_t posIn = input_sv.find("in"); posIn != string_view::npos) {
                auto potion_part = input_sv.substr(posIn + 2);
                return trim_spaces(potion_part);
            }
            else {
                return {};
            }
        }
        else {
            return {};
        }
    }
    // total query case
    else if (size_t pos = input_sv.find("Total"); pos != string_view::npos) {
        size_t potion_pos = input_sv.find("potion", pos);
        if (potion_pos != string_view::npos) {
            auto potion_part = input_sv.substr(potion_pos + 7);
            return trim_spaces(potion_part);
        }
    }
    // learns case
    else if (size_t potion_pos = input_sv.find("potion"); potion_pos != string_view::npos) {
        if (size_t learns_pos = input_sv.find("learns"); learns_pos != string_view::npos) {
            size_t start = learns_pos + 7;
            size_t end = potion_pos;
            if (start < end) {
                auto potion_sv = input_sv.substr(start, end - start);
                return trim_spaces(potion_sv);
            }
        }
    }
    return {};
}

/*
 * returns false if there are two consecutive spaces in the string
 * returns true otherwise
 */
bool spacesAreValid(const string& potionName) {
    if (potionName.empty())
        return false;

    bool inWord = true;  // are we currently in a word?
    int spaceCount = 0;

    for (size_t i = 0; i < potionName.size(); ++i) {
        char c = potionName[i];
        if (c == ' ') {
            if (inWord) {
                // transition from word to space
                inWord = false;
                spaceCount = 1;
            } else {
                // already in space, count
                spaceCount++;
                // only consider multiple spaces invalid if we later find another word after it
            }
        } else {
            // found a non-space character
            if (!inWord) {
                // transition from space to word
                if (spaceCount > 1) {
                    if (c == '?') { // spaces before final ? are valid
                        return true;
                    }
                    return false;
                }
                inWord = true;
                spaceCount = 0;
            }
        }
    }
    return true; // format is valid
}

/*
 * returns 1 if the string is all digits
 * returns 0 if the string consists a non-digit character
 */
int isAllDigits(const string& s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            return 0; // not a digit, fail
        }
    }
    return 1; // all digits
}


/*
 * returns 1 if the string only consists alpha charactres and spaces
 * returns 0 if the string consists a non-alpha character
 */
int isAllAlpha(const string& s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isalpha(s[i]) && s[i] != ' ' && s[i] != '\0') {
            return 0; // not alpha or space, fail
        }
    }
    return 1; // all alpha or space
}