#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <queue>
#include <set>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <regex>
#include "CampusCompass.h"

using namespace std;

// constructor - initializes empty graph
CampusCompass::CampusCompass(){

}

// loads and parses data from CSV - https://www.geeksforgeeks.org/csv-file-management-using-c/
bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath){
    // load edges
    ifstream edgesFile(edges_filepath);
    if(!edgesFile.is_open()){
        return false;
    }

    string line;
    getline(edgesFile, line);

    // reads each line
    while(getline(edgesFile, line)){
        stringstream ss(line);
        string locationID1Str, locationID2Str, name1, name2, timeStr;

        // parses each field separated by commas
        getline(ss, locationID1Str, ',');
        getline(ss, locationID2Str, ',');
        getline(ss, name1, ',');
        getline(ss, name2, ',');
        getline(ss, timeStr, ',');

        // convert strings to integers
        int locationID1 = stoi(locationID1Str);
        int locationID2 = stoi(locationID2Str);
        int time = stoi(timeStr);

        // adds edges in both directions (graph is undirected)
        this->adjList[locationID1].push_back(Edge(locationID2, time, true));
        this->adjList[locationID2].push_back(Edge(locationID1, time, true));
    }
    edgesFile.close();

    // load classes
    ifstream classesFile(classes_filepath);
    if(!classesFile.is_open()){
        return false;
    }

    getline(classesFile, line);

    while(getline(classesFile, line)){
        stringstream ss(line);
        string classCode, locationIDStr, startTime, endTime;

        getline(ss, classCode, ',');
        getline(ss, locationIDStr, ',');
        getline(ss, startTime, ',');
        getline(ss, endTime, ',');

        // converts location to integer
        int locationID = stoi(locationIDStr);

        // stores class information
        this->allCourseMap[classCode] = ClassInfo(locationID, startTime, endTime);
    }
    classesFile.close();

    return true;
}

// parsing from commands - https://www.geeksforgeeks.org/processing-strings-using-stdistringstream/
bool CampusCompass::ParseCommand(const string &command){
    istringstream in(command);
    string cmd;
    in >> cmd;

    if(cmd == "insert"){
        // reads up to first quotation mark
        string next;
        getline(in, next, '"');

        // reads name between quotation marks
        string name;
        getline(in, name, '"');

        // reads id as string first to validate length
        string idStr;
        in >> idStr;

        // reads residence location
        int residenceLocationID;
        in >> residenceLocationID;

        // reads number of classes
        int numClasses;
        in >> numClasses;

        // reads all class codes
        vector<string> classes;
        for(int i = 0; i < numClasses; i++){
            string classCode;
            in >> classCode;
            classes.push_back(classCode);
        }

        // validates correct num of classes
        if((int)classes.size() != numClasses){
            cout << "unsuccessful" << endl;
            return false;
        }

        this->insert(name, idStr, residenceLocationID, classes);
    }
    else if(cmd == "remove"){
        int id;
        in >> id;
        this->remove(id);
    }
    else if(cmd == "dropClass"){
        int id;
        string classCode;
        in >> id >> classCode;
        this->dropClass(id, classCode);
    }
    else if(cmd == "replaceClass"){
        int id;
        string classCode1, classCode2;
        in >> id >> classCode1 >> classCode2;
        this->replaceClass(id, classCode1, classCode2);
    }
    else if(cmd == "removeClass"){
        string classCode;
        in >> classCode;
        this->removeClass(classCode);
    }
    else if(cmd == "toggleEdgesClosure"){
        int n;
        in >> n;

        // reads n*2 location IDs (n edges, 2 endpoints each)
        vector<int> locationIDs;
        for(int i = 0; i < n * 2; i++){
            int locationID;
            in >> locationID;
            locationIDs.push_back(locationID);
        }

        this->toggleEdgesClosure(locationIDs);
    }
    else if(cmd == "checkEdgeStatus"){
        int locationID1, locationID2;
        in >> locationID1 >> locationID2;
        this->checkEdgeStatus(locationID1, locationID2);
    }
    else if(cmd == "isConnected"){
        int locationID1, locationID2;
        in >> locationID1 >> locationID2;
        this->isConnected(locationID1, locationID2);
    }
    else if(cmd == "printShortestEdges"){
        int id;
        in >> id;
        this->printShortestEdges(id);
    }
    else if(cmd == "printStudentZone"){
        int id;
        in >> id;
        this->printStudentZone(id);
    }
    else if(cmd == "verifySchedule"){
        int id;
        in >> id;
        this->verifySchedule(id);
    }
    else{
        // unknown command
        cout << "unsuccessful" << endl;
        return false;
    }

    return true;
}

// checks if student ID is valid (8 digits)
bool CampusCompass::isValidId(const string& id){
    if(id.length() != 8){
        return false;
    }

    // checks that every character is a digit (0-9)
    for(char c : id){
        if(!isdigit(c)){
            return false;
        }
    }

    return true;
}

// checks if name is valid with spaces and letters - https://www.onlinegdb.com/kCOQdeHfqd
bool CampusCompass::isValidName(const string& name){
    const regex validNamePattern("^[a-zA-Z ]+$");
    return regex_match(name, validNamePattern);
}

// checks if class code is valid (3 uppercase letters + 4 digits)
bool CampusCompass::isValidClassCode(const string& code){
    // checks if class code is 7 characters
    if(code.length() != 7){
        return false;
    }

    // checks if first 3 characters are uppercase letters
    for(int i = 0; i < 3; i++){
        if(!isupper(code[i])){
            return false;
        }
    }

    // checks if last characters are 4 digits
    for(int i = 3; i < 7; i++){
        if(!isdigit(code[i])){
            return false;
        }
    }

    return true;
}

// converts time string to minutes
int CampusCompass::timeToMinutes(const string& time){
    int colonPos = time.find(':');

    // get hours and minutes strings
    string hoursStr = time.substr(0, colonPos);
    string minutesStr = time.substr(colonPos + 1);

    // convert hours and minutes from string to integer
    int hours = stoi(hoursStr);
    int minutes = stoi(minutesStr);

    // calculates total minutes
    return hours * 60 + minutes;
}

// Dijkstra's algorithm helper
pair<map<int, int>, map<int, int>> CampusCompass::runDijkstra(int startNode){
    // stores shortest distance to each node
    map<int, int> distances;
    // stores the previous node in the shortest path
    map<int, int> parent;

    // stores pair of distance and nodes where smallest distance is on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // initalizes all distances to infinity - used https://en.cppreference.com/w/cpp/types/numeric_limits
    for(auto& mapPair : this->adjList){
        distances[mapPair.first] = numeric_limits<int>::max();
    }

    distances[startNode] = 0; // starting node's distance is 0
    pq.push({0, startNode});
    parent[startNode] = -1; // Start node has no parent

    // Process nodes in order of increasing distance
    while(!pq.empty()){
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        // if better path found, skip
        if(currentDistance > distances[currentNode]){
            continue;
        }

        // checks if node has any edges by checking its neighbors based off current node
        if(this->adjList.find(currentNode) != this->adjList.end()){
            for(const Edge& edge : this->adjList[currentNode]){
                if(!edge.isOpen){
                    continue;
                }

                // calculates new distance through current node
                int newDistance = currentDistance + edge.time;

                // if path is shorter than before, update shortest distance
                if(newDistance < distances[edge.to]){
                    distances[edge.to] = newDistance;
                    parent[edge.to] = currentNode;
                    pq.push({newDistance, edge.to});
                }
            }
        }
    }

    return {distances, parent};
}

// inserts a new student to the class(es)
void CampusCompass::insert(string name, string idStr, int residenceLocationID, vector<string> classes){
    // checks if ID is valid (string check)
    if(!this->isValidId(idStr)){
        cout << "unsuccessful" << endl;
        return;
    }

    int id = stoi(idStr);

    // checks if ID exists
    if(this->studentMap.find(id) != this->studentMap.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    // checks if name is valid
    if(!this->isValidName(name)){
        cout << "unsuccessful" << endl;
        return;
    }

    // checks if students have 1-6 classes
    if(classes.size() < 1 || classes.size() > 6){
        cout << "unsuccessful" << endl;
        return;
    }

    // checks the format for each class code and if the class exists
    for(const string& classCode : classes){
        if(!this->isValidClassCode(classCode)){
            cout << "unsuccessful" << endl;
            return;
        }
        if(this->allCourseMap.find(classCode) == this->allCourseMap.end()){
            cout << "unsuccessful" << endl;
            return;
        }
    }

    this->studentMap[id] = Student(name, id, residenceLocationID, classes);
    cout << "successful" << endl;
}

// removes a student from their class if student exists
void CampusCompass::remove(int id){
    if(this->studentMap.find(id) != this->studentMap.end()){
        // student exists
        this->studentMap.erase(id);
        cout << "successful" << endl;
    }
    else{
        // student doesn't exist
        cout << "unsuccessful" << endl;
    }
}

// finds and drops a specific class for a given student
void CampusCompass::dropClass(int id, string classCode){
    // checks if student exists
    if(this->studentMap.find(id) == this->studentMap.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    // checks if the class exists
    if(this->allCourseMap.find(classCode) == this->allCourseMap.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    Student& student = this->studentMap[id];

    // finds class in student's class list
    auto it = find(student.classCodes.begin(), student.classCodes.end(), classCode);

    // if class found, remove but if no classes are left then student is removed
    if(it != student.classCodes.end()){
        student.classCodes.erase(it);
        if(student.classCodes.empty()){
            this->studentMap.erase(id);
        }
        cout << "successful" << endl;
    }
    else{
        cout << "unsuccessful" << endl;
    }
}

// finds student and replaces class with another class
void CampusCompass::replaceClass(int id, string classCode1, string classCode2){
    if(this->studentMap.find(id) == this->studentMap.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    // checks if new class exists
    if(this->allCourseMap.find(classCode2) == this->allCourseMap.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    Student& student = this->studentMap[id];

    // checks if student has new class
    if(find(student.classCodes.begin(), student.classCodes.end(), classCode2) != student.classCodes.end()){
        cout << "unsuccessful" << endl;
        return;
    }

    // finds old class in student's class list
    auto it = find(student.classCodes.begin(), student.classCodes.end(), classCode1);

    // checks if replacement is successful
    if(it != student.classCodes.end()){
        *it = classCode2;
        cout << "successful" << endl;
    }
    else{
        cout << "unsuccessful" << endl;
    }
}

// removes a class from the schedule for all students
void CampusCompass::removeClass(string classCode){
    int count = 0;
    vector<int> studentsToRemove;

    for(auto& mapPair : this->studentMap){
        Student& student = mapPair.second;

        auto it = find(student.classCodes.begin(), student.classCodes.end(), classCode);

        if(it != student.classCodes.end()){
            count++;
            student.classCodes.erase(it);

            // if student has no classes, gets ready to remove
            if(student.classCodes.empty()){
                studentsToRemove.push_back(mapPair.first);
            }
        }
    }

    // removes all students who have no classes left
    for(int studentID : studentsToRemove){
        this->studentMap.erase(studentID);
    }

    // print number of students the class was dropped from
    cout << count << endl;
}

// toggles the closure (soft delete) of N different edges
void CampusCompass::toggleEdgesClosure(vector<int> locationIDs){
    // processes pairs of locations
    for(size_t i = 0; i + 1 < locationIDs.size(); i += 2){
        int locationID1 = locationIDs[i];
        int locationID2 = locationIDs[i + 1];

        // toggles edge from locationID1 to locationID2
        if(this->adjList.find(locationID1) != this->adjList.end()){
            for(Edge& edge : this->adjList[locationID1]){
                if(edge.to == locationID2){
                    edge.isOpen = !edge.isOpen;
                    break;
                }
            }
        }

        // toggles edge from locationID2 to locationID1 with undirected graph
        if(this->adjList.find(locationID2) != this->adjList.end()){
            for(Edge& edge : this->adjList[locationID2]){
                if(edge.to == locationID1){
                    edge.isOpen = !edge.isOpen;
                    break;
                }
            }
        }
    }

    cout << "successful" << endl;
}

// checks whether a given edge is open, closed or does not exist
void CampusCompass::checkEdgeStatus(int locationID1, int locationID2){
    // checks if locationID1 exists
    if(this->adjList.find(locationID1) == this->adjList.end()){
        cout << "DNE" << endl;
        return;
    }

    for(const Edge& edge : this->adjList[locationID1]){
        if(edge.to == locationID2){
            if(edge.isOpen){
                cout << "open" << endl;
            }
            else{
                cout << "closed" << endl;
            }
            return;
        }
    }

    cout << "DNE" << endl;
}

// checks if it is possible to travel between locations using any of the available edges
void CampusCompass::isConnected(int locationID1, int locationID2){
    // runs Dijkstra's algorithm from the first location
    auto result = this->runDijkstra(locationID1);
    map<int, int>& distances = result.first;

    // checks if second location is reachable
    if(distances.find(locationID2) != distances.end() &&
        distances[locationID2] != numeric_limits<int>::max()){
        cout << "successful" << endl;
    }
    else{
        cout << "unsuccessful" << endl;
    }
}

// prints the shortest walking time from a student's residence to each of their classes
void CampusCompass::printShortestEdges(int id){
    Student& student = this->studentMap[id];

    cout << "Name: " << student.name << endl;

    // runs Dijkstra's algorithm from the student's residence
    auto result = this->runDijkstra(student.residenceLocationId);
    map<int, int>& distances = result.first;

    // sort class codes alphabetically
    vector<string> sortedClasses = student.classCodes;
    sort(sortedClasses.begin(), sortedClasses.end());

    // prints distance to each class
    for(const string& classCode : sortedClasses){
        int classLocationID = this->allCourseMap[classCode].locationId;

        int distance = -1; // -1 if distance is unreachable

        // checks if reachable
        if(distances.find(classLocationID) != distances.end() &&
            distances[classLocationID] != numeric_limits<int>::max()){
            distance = distances[classLocationID];
        }

        cout << classCode << " | Total Time: " << distance << endl;
    }
}

// prints the total minimum spanning tree cost for a student's zone
void CampusCompass::printStudentZone(int id){
    Student& student = this->studentMap[id];

    // runs Dijkstra's algorithm from student's residence
    auto result = this->runDijkstra(student.residenceLocationId);
    map<int, int>& parent = result.second;

    // collects all locations that are part of shortest paths to classes
    set<int> subgraphVertices;
    subgraphVertices.insert(student.residenceLocationId);

    // trace back path and add all locations
    for(const string& classCode : student.classCodes){
        int classLocationID = this->allCourseMap[classCode].locationId;

        int currentLocation = classLocationID;
        while(currentLocation != -1){
            subgraphVertices.insert(currentLocation);

            if(parent.find(currentLocation) != parent.end()){
                currentLocation = parent[currentLocation];
            }
            else{
                break; // reached the start
            }
        }
    }

    // builds a list of edges
    vector<pair<int, pair<int, int>>> subgraphEdges;

    for(int vertex : subgraphVertices){
        if(this->adjList.find(vertex) != this->adjList.end()){
            for(const Edge& edge : this->adjList[vertex]){
                if(subgraphVertices.find(edge.to) != subgraphVertices.end() &&
                    edge.isOpen && vertex < edge.to){
                    subgraphEdges.push_back({edge.time, {vertex, edge.to}});
                }
            }
        }
    }

    // sorts edges by weight
    sort(subgraphEdges.begin(), subgraphEdges.end());

    // detects cycles
    map<int, int> parentUF;
    for(int vertex : subgraphVertices){
        parentUF[vertex] = vertex;
    }

    // finds root of a vertex's tree
    function<int(int)> findSet = [&](int x){
        if(parentUF[x] != x){
            parentUF[x] = findSet(parentUF[x]);
        }
        return parentUF[x];
    };

    // adds edges to MST if cycle isn't created
    int mstCost = 0;
    for(auto& edge : subgraphEdges){
        int weight = edge.first;
        int vertex1 = edge.second.first;
        int vertex2 = edge.second.second;

        // finds which trees the vertices are in
        int set1 = findSet(vertex1);
        int set2 = findSet(vertex2);

        // if in different trees, add edge
        if(set1 != set2){
            mstCost += weight;
            parentUF[set1] = set2;
        }
    }

    cout << "Student Zone Cost For " << student.name << ": " << mstCost << endl;
}

// checks if a student can physically get from one class to the next in the alloted time
void CampusCompass::verifySchedule(int id){
    Student& student = this->studentMap[id];

    if(student.classCodes.size() <= 1){
        cout << "unsuccessful" << endl;
        return;
    }

    // creates a list of classes with their times and locations
    vector<pair<string, pair<int, int>>> classSchedule;

    for(const string& classCode : student.classCodes){
        ClassInfo& classInfo = this->allCourseMap[classCode];

        // convert time to minutes
        int startMinutes = this->timeToMinutes(classInfo.startTime);
        int endMinutes = this->timeToMinutes(classInfo.endTime);

        classSchedule.push_back({classCode, {startMinutes, endMinutes}});
    }

    // sorts classes by earliest start time to latest
    sort(classSchedule.begin(), classSchedule.end(),
         [](const pair<string, pair<int, int>>& a, const pair<string, pair<int, int>>& b){
             return a.second.first < b.second.first;
         });

    cout << "Schedule Check for " << student.name << ":" << endl;

    // checks each consecutive pair of classes
    for(size_t i = 0; i + 1 < classSchedule.size(); i++){
        string class1Code = classSchedule[i].first;
        string class2Code = classSchedule[i + 1].first;

        int class1EndTime = classSchedule[i].second.second;
        int class2StartTime = classSchedule[i + 1].second.first;

        // calculates time gap between classes
        int timeGap = class2StartTime - class1EndTime;

        // gets locations of both classes
        int class1Location = this->allCourseMap[class1Code].locationId;
        int class2Location = this->allCourseMap[class2Code].locationId;

        // run Dijkstra's algorithm from class1 location to find shortest path to class2
        auto result = this->runDijkstra(class1Location);
        map<int, int>& distances = result.first;

        // gets the shortest path time
        int shortestPathTime = -1;
        if(distances.find(class2Location) != distances.end() &&
            distances[class2Location] != numeric_limits<int>::max()){
            shortestPathTime = distances[class2Location];
        }

        cout << class1Code << " - " << class2Code << " ";

        if(shortestPathTime == -1){
            cout << "\"Cannot make it!\"" << endl;
        }
        else if(timeGap >= shortestPathTime){
            cout << "\"Can make it!\"" << endl;
        }
        else{
            cout << "\"Cannot make it!\"" << endl;
        }
    }
}