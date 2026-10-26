// COMSC-210-5293 | Lab 28 | Yuyi Chen

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <limits>
#include "Goat.h"
using namespace std;

string searchName;
string searchColor;
int ageLimit;
const int SZ_NAMES = 200, SZ_COLORS = 25;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();
bool matchName(const Goat &goat);
bool matchColor(const Goat &goat);
bool compareAge(const Goat &a, const Goat &b);
bool sameName(const Goat &a, const Goat &b);
bool youngerThan(const Goat &goat);

int main() {
    srand(time(0));
    bool again;

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++]);
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++]);
    fin1.close();

    // create & populate a trip of Goats using std::list of random size 8-15
    int tripSize = rand() % 8 + 8;
    list<Goat> trip;
    int age;
    string name, color;
    for (int i = 0; i < tripSize; i++) {
        age = rand() % MAX_AGE;  // defined in Goat.h
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        Goat tmp(name, age, color);
        trip.push_back(tmp);
    }
    
    // Goat Manager 3001 Engine
    int sel = main_menu();
    while (sel != 12) {
        switch (sel) {
            case 1:
                cout << "Adding a goat.\n";
                add_goat(trip, names, colors);
                break;
            case 2:    
                cout << "Removing a goat.\n";
                delete_goat(trip);
                break;
            case 3:    
                cout << "Displaying goat data.\n";
                display_trip(trip);
                break;

            case 4: {
                cout << "Sorting goats by name.\n";

                // Copy goats into a temporary list
                list<Goat> copiedTrip(trip.size());
                copy(trip.begin(), trip.end(), copiedTrip.begin());

                // Sort the copied list by name
                copiedTrip.sort();

                // Copy the sorted goats back to the original list
                copy(copiedTrip.begin(), copiedTrip.end(), trip.begin());

                display_trip(trip);

                break;
            }

            case 5:
                cout << "Reversing goat order.\n";

                // Reverse the order of goats using an STL algorithm
                reverse(trip.begin(), trip.end());

                display_trip(trip);
                break;

            case 6: {
                cout << "Enter goat name: ";
                cin >> searchName;

                // Search for the first goat with the matching name
                auto it = find_if(trip.begin(), trip.end(), matchName);

                if (it != trip.end()) {
                    cout << "Goat found: "
                         << it->get_name() << " ("
                         << it->get_age() << ", "
                         << it->get_color() << ")" << endl;
                }
                else {
                    cout << "Goat not found." << endl;
                }

                break;
            }

            case 7: {
                cout << "Enter goat color: ";
                cin >> searchColor;

                // Count goats with the matching color
                int count = count_if(trip.begin(), trip.end(), matchColor);

                cout << "Number of goats with color "
                     << searchColor << ": " << count << endl;

                break;
            }

            case 8: {
                cout << "Finding the youngest goat.\n";

                if (!trip.empty()) {
                    // Find the goat with the smallest age
                    auto it = min_element(trip.begin(), trip.end(), compareAge);

                    cout << "Youngest goat: "
                         << it->get_name() << " ("
                         << it->get_age() << ", "
                         << it->get_color() << ")" << endl;
                }
                else {
                    cout << "List is empty." << endl;
                }

                break;
            }

            case 9: {
                cout << "Finding the oldest goat.\n";

                if (!trip.empty()) {
                    // Find the goat with the largest age
                    auto it = max_element(trip.begin(), trip.end(), compareAge);

                    cout << "Oldest goat: "
                         << it->get_name() << " ("
                         << it->get_age() << ", "
                         << it->get_color() << ")" << endl;
                }
                else {
                    cout << "List is empty." << endl;
                }

                break;
            }

            case 10: {
                cout << "Removing adjacent duplicate goats.\n";

                int oldSize = trip.size();

                // Remove adjacent goats with matching names
                auto newEnd = unique(trip.begin(), trip.end(), sameName);
                trip.erase(newEnd, trip.end());

                int removed = oldSize - trip.size();

                cout << "Removed " << removed << " duplicate goat(s).\n";

                display_trip(trip);

                break;
            }

            case 11: {
                cout << "Enter age limit: ";
                cin >> ageLimit;

                while (cin.fail() || ageLimit < 0 || ageLimit > MAX_AGE) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Invalid age, again --> ";
                    cin >> ageLimit;
                }

                int oldSize = trip.size();

                // Move goats that meet the age requirement to the front
                auto newEnd = remove_if(trip.begin(), trip.end(), youngerThan);

                // Erase the removed goats from the list
                trip.erase(newEnd, trip.end());

                int removed = oldSize - trip.size();

                cout << "Removed " << removed << " goat(s).\n";

                display_trip(trip);

                break;
            }

            default:
                cout << "Invalid selection.\n";
                break;
        }
        sel = main_menu();
    }
    

    return 0;
}

int main_menu() {
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] Sort goats by name\n";
    cout << "[5] Reverse goat order\n";
    cout << "[6] Find goat by name\n";
    cout << "[7] Count goats by color\n";
    cout << "[8] Find youngest goat\n";
    cout << "[9] Find oldest goat\n";
    cout << "[10] Remove adjacent duplicate goats\n";
    cout << "[11] Remove goats younger than an age\n";
    cout << "[12] Quit\n";
    cout << "Choice --> ";
    int choice;
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 12) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Invalid, again --> ";
        cin >> choice;
    }

    return choice;
}

void delete_goat(list<Goat> &trip) {
    cout << "DELETE A GOAT\n";
    int index = select_goat(trip);
    auto it = trip.begin();
    advance(it, index-1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << endl;
}

void add_goat(list<Goat> &trip, string nms[], string cls[]) {
    cout << "ADD A GOAT\n";
    int age = rand() % MAX_AGE;
    string nm = nms[rand() % SZ_NAMES];
    string cl = cls[rand() % SZ_COLORS];
    Goat tmp(nm, age, cl);
    trip.push_back(tmp);
    cout << "Goat added. New trip size: " << trip.size() << endl;
}

void display_trip(list<Goat> trp) {
    int i = 1;
    for (auto gt: trp)
        cout << "\t" 
             << "[" << i++ << "] "
             << gt.get_name() 
             << " (" << gt.get_age() 
             << ", " << gt.get_color() << ")\n";
}

int select_goat(list<Goat> trp) {
    int input;
    cout << "Make a selection:\n";
    display_trip(trp);
    cout << "Choice --> ";
    cin >> input;
    while (input < 1 or input > trp.size()) {
        cout << "Invalid choice, again --> ";
        cin >> input;
    }
    return input;
}

// matchName() checks if a goat's name matches the search name
// arguments: reference to a Goat object
// returns: true if the names match, otherwise false
bool matchName(const Goat &goat) {
    return goat.get_name() == searchName;
}

// matchColor() checks if a goat's color matches the search color
// arguments: reference to a Goat object
// returns: true if the colors match, otherwise false
bool matchColor(const Goat &goat) {
    return goat.get_color() == searchColor;
}

// compareAge() compares the ages of two goats
// arguments: references to two Goat objects
// returns: true if the first goat is younger than the second
bool compareAge(const Goat &a, const Goat &b) {
    return a.get_age() < b.get_age();
}

// sameName() checks if two goats have the same name
// arguments: references to two Goat objects
// returns: true if both goats have the same name, otherwise false
bool sameName(const Goat &a, const Goat &b) {
    return a.get_name() == b.get_name();
}

// youngerThan() checks if a goat is younger than the age limit
// arguments: reference to a Goat object
// returns: true if the goat's age is below the limit, otherwise false
bool youngerThan(const Goat &goat) {
    return goat.get_age() < ageLimit;
}