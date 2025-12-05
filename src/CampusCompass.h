#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

struct Edge {
    int to;
    int time;
    bool isOpen;
};

struct Student {
    string name;
    int id;
    int residenceLocationId;
    vector<string> classCodes;
};

struct ClassInfo {
    int locationId;
    string startTime;
    string endTime;
};

class CampusCompass {
private:
    // Graph representation: LocationID -> List of Edges
    unordered_map<int, vector<Edge>> adjList;

    // Data storage
    unordered_map<string, ClassInfo> allCourseMap;
    unordered_map<int, Student> studentMap;

    // Helpers
    bool isValidId(int id);
    bool isValidName(const string& name);
    bool isValidClassCode(const string& code);
    pair<unordered_map<int, int>, unordered_map<int, int>> runDijkstra(int startNode);

public:
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

    // Required functions
    void insertStudent(string name, int id, int resId, vector<string> classes);
    void removeStudent(int id);
    void dropClass(int id, string classCode);
    void replaceClass(int id, string oldCode, string newCode);
    void removeClassFromAll(string classCode);

    void toggleEdgesClosure(vector<int> locations);
    void checkEdgeStatus(int from, int to);
    void isConnected(int from, int to);

    void printShortestEdges(int id);
    void printStudentZone(int id);

};
