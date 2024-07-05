#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED

#include <iostream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <sstream>

using namespace std;


string operation;
int last_instant, no_of_process;
vector<pair<char, int>> schedules;
vector<tuple<string, int, int>> tasks;
vector<vector<char>> timeline; 
unordered_map<string, int> tasktoindex;

// Results
vector<int> endtime;
vector<int> turnaroundtime;
vector<float> normtime;


void input_schedules(const string& schedule_list) {
    stringstream stream(schedule_list);
    string t;

    while (getline(stream, t, ',')) {
        char id = t[0];
        int quantum = -1;
        if (t.size() > 2 && t[1] == '-') { 
            quantum = stoi(t.substr(2));
        }
        schedules.emplace_back(id, quantum);
    }
}


void input_tasks() {
    string input, name;
    int process_arrival_time, process_service_time;
    for (int i = 0; i < no_of_process; i++) {
        cin >> input;

        stringstream stream(input);
        string t;
        getline(stream, t, ',');
        name = t;
        getline(stream, t, ',');
        process_arrival_time = stoi(t);
        getline(stream, t, ',');
        process_service_time = stoi(t);

        tasks.emplace_back(name, process_arrival_time, process_service_time);
        tasktoindex[name] = i;
    }
}


void input() {
    string schedule;
    cin >> operation >> schedule >> last_instant >> no_of_process;
    input_schedules(schedule);
    input_tasks();
    endtime.resize(no_of_process);
    turnaroundtime.resize(no_of_process);
    normtime.resize(no_of_process);
    timeline.resize(last_instant, vector<char>(no_of_process, ' ')); // Resize and initialize timeline
}

#endif 
