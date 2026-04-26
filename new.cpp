#include <bits/stdc++.h>
using namespace std;

struct Process {
    string pid;
    int at, bt;
    int start, ct, tat, wt;
};

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    srand(time(0));

    for (int i = 0; i < n; i++) {
        processes[i].pid = "P" + to_string(i + 1);
        processes[i].at = rand() % 11;        // 0 to 10
        processes[i].bt = rand() % 10 + 1;    // 1 to 10
    }

    // Sort by Arrival Time (FCFS)
    sort(processes.begin(), processes.end(),
         [](const Process &a, const Process &b) {
             return a.at < b.at;
         });

    int current_time = 0;
    double total_tat = 0, total_wt = 0;

    vector<pair<string, pair<int, int>>> timeline;

    for (auto &p : processes) {

        if (current_time < p.at) {
            timeline.push_back({"Idle", {current_time, p.at}});
            current_time = p.at;
        }

        p.start = current_time;
        p.ct = current_time + p.bt;
        p.tat = p.ct - p.at;
        p.wt = p.tat - p.bt;

        total_tat += p.tat;
        total_wt += p.wt;

        timeline.push_back({p.pid, {p.start, p.ct}});
        current_time = p.ct;
    }

    double avg_tat = total_tat / n;
    double avg_wt = total_wt / n;

    // Sort back by PID for display
    sort(processes.begin(), processes.end(),
         [](const Process &a, const Process &b) {
             return a.pid < b.pid;
         });

    cout << "\nFCFS Scheduler\n\n";
    cout << left << setw(6) << "PID"
         << setw(6) << "AT"
         << setw(6) << "BT"
         << setw(8) << "START"
         << setw(6) << "CT"
         << setw(6) << "TAT"
         << setw(6) << "WT" << "\n";

    for (auto &p : processes) {
        cout << left << setw(6) << p.pid
             << setw(6) << p.at
             << setw(6) << p.bt
             << setw(8) << p.start
             << setw(6) << p.ct
             << setw(6) << p.tat
             << setw(6) << p.wt << "\n";
    }

    cout << "\nAverage Waiting Time    : " << fixed << setprecision(2) << avg_wt;
    cout << "\nAverage Turnaround Time : " << fixed << setprecision(2) << avg_tat << "\n";

    // Gantt Chart (Text)
    cout << "\nGantt Chart:\n";

    for (auto &t : timeline) {
        cout << "| " << t.first << " ";
    }
    cout << "|\n";

    for (auto &t : timeline) {
        cout << t.second.first << setw(6);
    }
    cout << current_time << "\n";

    return 0;
}
