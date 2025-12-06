#include <iostream>
#include "CampusCompass.h"

using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("../data/edges.csv", "../data/classes.csv");

    // reads number of commands
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); // ignores newline left by cin

    // processes each command
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        // parses and executes command
        compass.ParseCommand(command);
    }

    return 0;
}
