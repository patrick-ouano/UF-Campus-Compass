#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

struct Edge {
    int to;
    int time;
    bool isOpen;

    Edge(int to, int time, bool isOpen) {
        this->to = to;
        this->time = time;
        this->isOpen = isOpen;
    }
};

struct Student {
    string name;
    int id;
    int residenceLocationId;
    vector<string> classCodes;

    Student() {
        this->id = 0;
        this->residenceLocationId = 0;
    }

    Student(string name, int id, int residenceLocationId, vector<string> classCodes) {
        this->name = name;
        this->id = id;
        this->residenceLocationId = residenceLocationId;
        this->classCodes = classCodes;
    }
};

struct ClassInfo {
    int locationId;
    string startTime;
    string endTime;

    ClassInfo() {
        this->locationId = 0;
    }

    ClassInfo(int locationId, string startTime, string endTime) {
        this->locationId = locationId;
        this->startTime = startTime;
        this->endTime = endTime;
    }
};

class CampusCompass {
private:
    // represents graph: LocationID -> List of Edges
    map<int, vector<Edge>> adjList;

    // stores data
    map<string, ClassInfo> allCourseMap;
    map<int, Student> studentMap;

    // helper functions to validate items
    bool isValidId(const string& id);
    bool isValidName(const string& name);
    bool isValidClassCode(const string& code);

    // helper for Dijkstra's algorithm
    pair<map<int, int>, map<int, int>> runDijkstra(int startNode);

    // helper to convert time to minutes
    int timeToMinutes(const string& time);

public:
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);

    // required functions for command processing
    void insert(string name, string id, int residenceLocationID, vector<string> classes);
    void remove(int id);
    void dropClass(int id, string classCode);
    void replaceClass(int id, string classCode1, string classCode2);
    void removeClass(string classCode);

    void toggleEdgesClosure(vector<int> locationIDs);
    void checkEdgeStatus(int locationID1, int locationID2);
    void isConnected(int locationID1, int locationID2);

    void printShortestEdges(int id);
    void printStudentZone(int id);

    void verifySchedule(int id); // extra credit
};