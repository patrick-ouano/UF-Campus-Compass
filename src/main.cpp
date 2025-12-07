#include <iostream>
#include "CampusCompass.h"
#include <limits>

using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("data/edges.csv", "data/classes.csv");

    // reads number of commands
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignores newline left by cin - https://en.cppreference.com/w/cpp/types/numeric_limits

    // processes each command
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        // parses and executes command
        compass.ParseCommand(command);
    }

    return 0;
}