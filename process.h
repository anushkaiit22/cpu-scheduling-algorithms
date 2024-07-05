#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <bits/stdc++.h>
#include "input.h"

#define all(v)  v.begin(),v.end()
using namespace std;

const string FLOW = "flow";
const string STATISTICS = "stats";
const string ALGORITHMS[9] = {"","FCFS","RR-","SPN","SRT","HHRN","FB-1","FB-2i","AGING"};


bool sort_by_at(const tuple<string, int, int> &a, const tuple<string, int, int> &b) {
    return get<1>(a) < get<1>(b);
}


bool sort_by_st(const tuple<string, int, int> &a, const tuple<string, int, int> &b) {
    return get<2>(a) < get<2>(b);
}


void clear_timeline() {
    for (auto &row : timeline) {
        fill(row.begin(), row.end(), ' ');
    }
}


string get_name(const tuple<string, int, int> &a) {
    return get<0>(a);
}


int get_at(const tuple<string, int, int> &a) {
    return get<1>(a);
}


int get_st(const tuple<string, int, int> &a) {
    return get<2>(a);
}


int get_priority(const tuple<string, int, int> &a) {
    return get<2>(a);
}


double response_ratio(int wait_t, int service_t) {
    return (wait_t + service_t) * 1.0 / service_t;
}


void fill_wait_t() {
    for (int i = 0; i < no_of_process; i++) {
        int arrival_time = get_at(tasks[i]);
        for (int j = arrival_time; j < endtime[i]; j++) {
            if (timeline[j][i] != '*') {
                timeline[j][i] = '.';
            }
        }
    }
}


bool by_response_ratio(const tuple<string, double, int> &a, const tuple<string, double, int> &b) {
    return get<1>(a) > get<1>(b);
}


bool by_priority(const tuple<int, int, int> &a, const tuple<int, int, int> &b) {
    return get<0>(a) > get<0>(b);
}

//Non-Preemptive Scheduling

void first_come_first_serve(){
    int time = get_at(tasks[0]);
    for(int i=0;i<no_of_process;i++){
        int at = get_at(tasks[i]);
        int st = get_at(tasks[i]);
        
        endtime[i] = (time + st);
        turnaroundtime[i] = (endtime[i] - at);
        normtime[i] = (turnaroundtime[i]*1.0 / st);

        for(int j = time; j< endtime[i];j++){
            timeline[j][i] = '*';
        }
        for(int j = at ; j< time ;j++){
            timeline[j][i] = '.';
        }
        time += st;
    }
}

void shortest_job_first(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    int i = 0;
    for(int j =0; j<last_instant;j++){
        while(i<no_of_process && get_at(tasks[i]) <=j){
            pq.push(make_pair(get_st(tasks[i]),i));
            i++;
        }
        if(!pq.empty()){
            int no = pq.top().second;
            int at = get_at(tasks[no]);
            int st = get_st(tasks[no]);
            pq.pop();

            for (int k = at;k< j;k++){
               timeline[k][no] = '.';
            }
            for(int k = j; k<j+st;k++){
               timeline[k][no] = '*';
            }

            endtime[no] = (j + st);
            turnaroundtime[no] = (endtime[no] - at);
            normtime[no] = (turnaroundtime[no]*1.0 /st);
            j += st-1;
        }
    }
}

void round_robin(int quantum){
    queue<pair<int,int>>store;
    int j =0;
    if(get_at(tasks[j])==0){
        store.push(make_pair(j,get_st(tasks[j])));
        j++;
    }
    int currentq = quantum;
    for(int t =0; t<last_instant;t++){
        if(!store.empty()){
            int no = store.front().first;
            store.front().second = store.front().second -1;
            
            int remaining = store.front().second;
            int at = get_at(tasks[no]);
            int st = get_st(tasks[no]);

            currentq--;
            timeline[t][no] = '*';

            while(j<no_of_process && get_at(tasks[j])==t+1){
                store.push(make_pair(j,get_st(tasks[j])));
                j++;
            }

            if(currentq==0 && remaining==0){
                endtime[no] = t +1;
                turnaroundtime[no] = (turnaroundtime[no]-at);
                normtime[no] = (turnaroundtime[no]*1.0/st);
                currentq = quantum;
                store.pop();
            }
            else if(currentq == 0 && remaining != 0){
                store.pop();
                store.push(make_pair(no,remaining));
                currentq = quantum;
            }
            else if(currentq!=0 && remaining==0){
                endtime[no] = t +1;
                turnaroundtime[no] = (turnaroundtime[no]-at);
                normtime[no] = (turnaroundtime[no]*1.0/st);
                currentq = quantum;
                store.pop();
            }
        }
        while(j<no_of_process && get_at(tasks[j])== t +1){
            store.push(make_pair(j,get_st(tasks[j])));
            j++;
        }
    }
    fill_wait_t();

}

void shortest_remaining_time(){
   priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int j =0;
    for(int i=0 ; i<last_instant;i++){
        while(j<no_of_process && get_at(tasks[j])==i){
           pq.push(make_pair(get_st(tasks[j]),j));
           j++;
        }
        if(!pq.empty()){
            int no = pq.top().second;
            int remaining = pq.top().first;
            pq.pop();
            
            int at = get_at(tasks[no]);
            int st = get_st(tasks[no]);

            timeline[i][no] = '*';

            if(remaining ==1){
                endtime[no] = i+1;
                turnaroundtime[no] = (endtime[no] - at);
                normtime[no] = (turnaroundtime[no] *1.0 / st);
            }
            else{
                pq.push(make_pair(remaining-1,no));
            }
        }
    }
    fill_wait_t();
}





void print(int index){
    int id = schedules[index].first - '0';
    if(id == 2)
        cout << ALGORITHMS[id] << schedules[index].second<<endl;
    else
        cout<<  ALGORITHMS[id]<<endl;   
}

void print_processes()
{
    cout << "Process    ";
    for (int i = 0; i < no_of_process; i++)
        cout << "|  " << get_name(tasks[i]) << "  ";
    cout << "|\n";
}

void print_at()
{
    cout << "Arrival    ";
    for (int i = 0; i < no_of_process; i++)
        printf("|%3d  ",get_at(tasks[i]));
    cout<<"|\n";
}

void print_st()
{
    cout << "Service    |";
    for (int i = 0; i < no_of_process; i++)
        printf("%3d  |",get_st(tasks[i]));
    cout << " Mean|\n";
}

void print_endtime()
{
    cout << "Finish     ";
    for (int i = 0; i <no_of_process; i++)
        printf("|%3d  ",endtime[i]);
    cout << "|-----|\n";
}

void print_turnaroundtime()
{
    cout << "Turnaround |";
    int sum = 0;
    for (int i = 0; i < no_of_process; i++)
    {
        printf("%3d  |",turnaroundtime[i]);
        sum += turnaroundtime[i];
    }
    if((1.0 * sum / turnaroundtime.size())>=10)
		printf("%2.2f|\n",(1.0 * sum / turnaroundtime.size()));
    else
	 	printf(" %2.2f|\n",(1.0 * sum / turnaroundtime.size()));
}

void print_normturn()
{
    cout << "NormTurn   |";
    float sum = 0;
    for (int i = 0; i < no_of_process; i++)
    {
        if( normtime[i]>=10 )
            printf("%2.2f|",normtime[i]);
        else
            printf(" %2.2f|",normtime[i]);
        sum += normtime[i];
    }

    if( (1.0 * sum / normtime.size()) >=10 )
        printf("%2.2f|\n",(1.0 * sum / normtime.size()));
	else
        printf(" %2.2f|\n",(1.0 * sum / normtime.size()));
}

void print_stats(int index)
{
    print(index);
    print_processes();
    print_at();
    print_st();
    print_endtime();
    print_turnaroundtime();
    print_normturn();
}


void print_flow(int index)
{
    for (int i = 0; i <= last_instant; i++)
        cout << i % 10<<" ";
    cout <<"\n";
    cout << "------------------------------------------------\n";
    for (int i = 0; i < no_of_process; i++)
    {
        cout << get_name(tasks[i]) << "     |";
        for (int j = 0; j < last_instant; j++)
        {
            cout << timeline[j][i]<<"|";
        }
        cout << " \n";
    }
    cout << "------------------------------------------------\n";
}


void execute_algorithm(char algorithm_id, int quantum,string operation)
{
    switch (algorithm_id)
    {
    case '1':
        if(operation==FLOW)cout<<"FCFS  ";
        first_come_first_serve();
        break;
    case '2':
        if(operation==FLOW)cout<<"RR-"<<quantum<<"  ";
        round_robin(quantum);
        break;
    case '3':
        if(operation==FLOW)cout<<"SPN   ";
        shortest_job_first();
        break;
    case '4':
        if(operation==FLOW)cout<<"SRT   ";
        shortest_remaining_time();
        break;
    // case '5':
    //     if(operation==TRACE)cout<<"HRRN  ";
    //     highestResponseRatioNext();
    //     break;
    // case '6':
    //     if(operation==TRACE)cout<<"FB-1  ";
    //     feedbackQ1();
    //     break;
    // case '7':
    //     if(operation==TRACE)cout<<"FB-2i ";
    //     feedbackQ2i();
    //     break;
    // case '8':
    //     if(operation==TRACE)cout<<"Aging ";
    //     aging(quantum);
    //     break;
    default:
        break;
    }
}


#endif 