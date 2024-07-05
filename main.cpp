
#include "process.h"
using namespace std;


int main()
{
    input();
    for (int idx = 0; idx < (int)schedules.size(); idx++)
    {
        clear_timeline();
        execute_algorithm(schedules[idx].first, schedules[idx].second,operation);
        if (operation == FLOW)
            print_flow(idx);
        else if (operation == STATISTICS)
            print_stats(idx);
        cout << "\n";
    }
    return 0;
}