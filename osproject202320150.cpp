#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

struct Process
{
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;

    int completionTime;
    int waitingTime;
    int turnaroundTime;
    int responseTime;
};

struct Metrics
{
    double averageWaiting;
    double averageTurnaround;
};

void inputProcesses(vector<Process>& processes)
{
    int n;

    cout << "Enter number of processes: ";
    cin >> n;

    for (int i = 0; i < n; i++)
    {
        Process p;

        p.id = i + 1;

        cout << "\nProcess " << p.id << endl;

        cout << "Arrival Time: ";
        cin >> p.arrivalTime;

        cout << "Burst Time: ";
        cin >> p.burstTime;

        cout << "Priority: ";
        cin >> p.priority;

        p.remainingTime = p.burstTime;
        p.completionTime = 0;
        p.waitingTime = 0;
        p.turnaroundTime = 0;
        p.responseTime = -1;

        processes.push_back(p);
    }
}

void printResults(vector<Process> processes, Metrics result)
{
    cout << "\nProcess\tArrival\tBurst\tPriority\tCompletion\tWaiting\tTurnaround\tResponse\n";

    for (int i = 0; i < processes.size(); i++)
    {
        cout << "P" << processes[i].id << "\t"
            << processes[i].arrivalTime << "\t"
            << processes[i].burstTime << "\t"
            << processes[i].priority << "\t\t"
            << processes[i].completionTime << "\t\t"
            << processes[i].waitingTime << "\t"
            << processes[i].turnaroundTime << "\t\t"
            << processes[i].responseTime << endl;
    }

    cout << fixed << setprecision(2);

    cout << "\nAverage Waiting Time = "
        << result.averageWaiting << endl;

    cout << "Average Turnaround Time = "
        << result.averageTurnaround << endl;
}

Metrics FCFS(vector<Process> processes, bool display)
{
    int currentTime = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    for (int i = 0; i < processes.size() - 1; i++)
    {
        for (int j = i + 1; j < processes.size(); j++)
        {
            if (processes[i].arrivalTime > processes[j].arrivalTime)
            {
                swap(processes[i], processes[j]);
            }
        }
    }

    if (display)
    {
        cout << "\n========== FCFS ==========\n";
        cout << "Gantt Chart:\n";
    }

    for (int i = 0; i < processes.size(); i++)
    {
        if (currentTime < processes[i].arrivalTime)
        {
            if (display)
            {
                cout << "Idle (" << currentTime << "-"
                    << processes[i].arrivalTime << ")\n";
            }

            currentTime = processes[i].arrivalTime;
        }

        int startTime = currentTime;

        currentTime = currentTime + processes[i].burstTime;

        processes[i].completionTime = currentTime;

        processes[i].turnaroundTime =
            processes[i].completionTime -
            processes[i].arrivalTime;

        processes[i].waitingTime =
            processes[i].turnaroundTime -
            processes[i].burstTime;

        processes[i].responseTime =
            startTime - processes[i].arrivalTime;

        totalWaiting =
            totalWaiting + processes[i].waitingTime;

        totalTurnaround =
            totalTurnaround + processes[i].turnaroundTime;

        if (display)
        {
            cout << "P" << processes[i].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

Metrics SJF(vector<Process> processes, bool display)
{
    int currentTime = 0;
    int completed = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    vector<bool> done(processes.size(), false);

    if (display)
    {
        cout << "\n========== SJF Non-Preemptive ==========\n";
        cout << "Gantt Chart:\n";
    }

    while (completed < processes.size())
    {
        int shortest = -1;

        for (int i = 0; i < processes.size(); i++)
        {
            if (!done[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                if (shortest == -1 ||
                    processes[i].burstTime <
                    processes[shortest].burstTime)
                {
                    shortest = i;
                }
            }
        }

        if (shortest == -1)
        {
            currentTime++;
            continue;
        }

        int startTime = currentTime;

        currentTime =
            currentTime + processes[shortest].burstTime;

        processes[shortest].completionTime = currentTime;

        processes[shortest].turnaroundTime =
            processes[shortest].completionTime -
            processes[shortest].arrivalTime;

        processes[shortest].waitingTime =
            processes[shortest].turnaroundTime -
            processes[shortest].burstTime;

        processes[shortest].responseTime =
            startTime - processes[shortest].arrivalTime;

        done[shortest] = true;
        completed++;

        totalWaiting =
            totalWaiting + processes[shortest].waitingTime;

        totalTurnaround =
            totalTurnaround + processes[shortest].turnaroundTime;

        if (display)
        {
            cout << "P" << processes[shortest].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

Metrics SJFPreemptive(vector<Process> processes, bool display)
{
    int currentTime = 0;
    int completed = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    vector<bool> started(processes.size(), false);

    if (display)
    {
        cout << "\n========== SJF Preemptive ==========\n";
        cout << "Gantt Chart:\n";
    }

    while (completed < processes.size())
    {
        int shortest = -1;

        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0)
            {
                if (shortest == -1 ||
                    processes[i].remainingTime <
                    processes[shortest].remainingTime)
                {
                    shortest = i;
                }
            }
        }

        if (shortest == -1)
        {
            currentTime++;
            continue;
        }

        if (!started[shortest])
        {
            processes[shortest].responseTime =
                currentTime -
                processes[shortest].arrivalTime;

            started[shortest] = true;
        }

        int startTime = currentTime;

        processes[shortest].remainingTime--;
        currentTime++;

        if (display)
        {
            cout << "P" << processes[shortest].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }

        if (processes[shortest].remainingTime == 0)
        {
            processes[shortest].completionTime =
                currentTime;

            processes[shortest].turnaroundTime =
                processes[shortest].completionTime -
                processes[shortest].arrivalTime;

            processes[shortest].waitingTime =
                processes[shortest].turnaroundTime -
                processes[shortest].burstTime;

            totalWaiting =
                totalWaiting +
                processes[shortest].waitingTime;

            totalTurnaround =
                totalTurnaround +
                processes[shortest].turnaroundTime;

            completed++;
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

Metrics PriorityNonPreemptive(vector<Process> processes, bool display)
{
    int currentTime = 0;
    int completed = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    vector<bool> done(processes.size(), false);

    if (display)
    {
        cout << "\n========== Priority Non-Preemptive ==========\n";
        cout << "Gantt Chart:\n";
    }

    while (completed < processes.size())
    {
        int highest = -1;

        for (int i = 0; i < processes.size(); i++)
        {
            if (!done[i] &&
                processes[i].arrivalTime <= currentTime)
            {
                if (highest == -1 ||
                    processes[i].priority <
                    processes[highest].priority)
                {
                    highest = i;
                }
            }
        }

        if (highest == -1)
        {
            currentTime++;
            continue;
        }

        int startTime = currentTime;

        currentTime =
            currentTime +
            processes[highest].burstTime;

        processes[highest].completionTime =
            currentTime;

        processes[highest].turnaroundTime =
            processes[highest].completionTime -
            processes[highest].arrivalTime;

        processes[highest].waitingTime =
            processes[highest].turnaroundTime -
            processes[highest].burstTime;

        processes[highest].responseTime =
            startTime -
            processes[highest].arrivalTime;

        done[highest] = true;
        completed++;

        totalWaiting =
            totalWaiting +
            processes[highest].waitingTime;

        totalTurnaround =
            totalTurnaround +
            processes[highest].turnaroundTime;

        if (display)
        {
            cout << "P" << processes[highest].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

Metrics PriorityPreemptive(vector<Process> processes, bool display)
{
    int currentTime = 0;
    int completed = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    vector<bool> started(processes.size(), false);

    if (display)
    {
        cout << "\n========== Priority Preemptive ==========\n";
        cout << "Gantt Chart:\n";
    }

    while (completed < processes.size())
    {
        int highest = -1;

        for (int i = 0; i < processes.size(); i++)
        {
            if (processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0)
            {
                if (highest == -1 ||
                    processes[i].priority <
                    processes[highest].priority)
                {
                    highest = i;
                }
            }
        }

        if (highest == -1)
        {
            currentTime++;
            continue;
        }

        if (!started[highest])
        {
            processes[highest].responseTime =
                currentTime -
                processes[highest].arrivalTime;

            started[highest] = true;
        }

        int startTime = currentTime;

        processes[highest].remainingTime--;
        currentTime++;

        if (display)
        {
            cout << "P" << processes[highest].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }

        if (processes[highest].remainingTime == 0)
        {
            processes[highest].completionTime =
                currentTime;

            processes[highest].turnaroundTime =
                processes[highest].completionTime -
                processes[highest].arrivalTime;

            processes[highest].waitingTime =
                processes[highest].turnaroundTime -
                processes[highest].burstTime;

            totalWaiting =
                totalWaiting +
                processes[highest].waitingTime;

            totalTurnaround =
                totalTurnaround +
                processes[highest].turnaroundTime;

            completed++;
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

Metrics RoundRobin(vector<Process> processes,
    int quantum,
    bool display)
{
    int currentTime = 0;
    int completed = 0;

    double totalWaiting = 0;
    double totalTurnaround = 0;

    vector<bool> added(processes.size(), false);

    vector<bool> started(processes.size(), false);

    queue<int> readyQueue;

    if (display)
    {
        cout << "\n========== Round Robin ==========\n";
        cout << "Time Quantum = " << quantum << endl;
        cout << "Gantt Chart:\n";
    }

    while (completed < processes.size())
    {
        for (int i = 0; i < processes.size(); i++)
        {
            if (!added[i] &&
                processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0)
            {
                readyQueue.push(i);
                added[i] = true;
            }
        }

        if (readyQueue.empty())
        {
            currentTime++;
            continue;
        }

        int current = readyQueue.front();
        readyQueue.pop();

        if (!started[current])
        {
            processes[current].responseTime =
                currentTime -
                processes[current].arrivalTime;

            started[current] = true;
        }

        int startTime = currentTime;

        int runTime = quantum;

        if (processes[current].remainingTime < quantum)
        {
            runTime =
                processes[current].remainingTime;
        }

        processes[current].remainingTime =
            processes[current].remainingTime - runTime;

        currentTime =
            currentTime + runTime;

        if (display)
        {
            cout << "P" << processes[current].id
                << " (" << startTime << "-"
                << currentTime << ")\n";
        }

        for (int i = 0; i < processes.size(); i++)
        {
            if (!added[i] &&
                processes[i].arrivalTime <= currentTime &&
                processes[i].remainingTime > 0)
            {
                readyQueue.push(i);
                added[i] = true;
            }
        }

        if (processes[current].remainingTime > 0)
        {
            readyQueue.push(current);
        }
        else
        {
            processes[current].completionTime =
                currentTime;

            processes[current].turnaroundTime =
                processes[current].completionTime -
                processes[current].arrivalTime;

            processes[current].waitingTime =
                processes[current].turnaroundTime -
                processes[current].burstTime;

            totalWaiting =
                totalWaiting +
                processes[current].waitingTime;

            totalTurnaround =
                totalTurnaround +
                processes[current].turnaroundTime;

            completed++;
        }
    }

    Metrics result;

    result.averageWaiting =
        totalWaiting / processes.size();

    result.averageTurnaround =
        totalTurnaround / processes.size();

    if (display)
    {
        printResults(processes, result);
    }

    return result;
}

void compareAll(vector<Process> processes)
{
    int quantum;

    cout << "\nEnter Time Quantum for Round Robin: ";
    cin >> quantum;

    Metrics fcfs =
        FCFS(processes, false);

    Metrics sjf =
        SJF(processes, false);

    Metrics sjfPreemptive =
        SJFPreemptive(processes, false);

    Metrics priority =
        PriorityNonPreemptive(processes, false);

    Metrics priorityPreemptive =
        PriorityPreemptive(processes, false);

    Metrics rr =
        RoundRobin(processes, quantum, false);

    cout << "\n";
    cout << "============================================================\n";
    cout << "                 COMPARISON OF ALGORITHMS\n";
    cout << "============================================================\n";

    cout << left
        << setw(28) << "Algorithm"
        << setw(22) << "Avg Waiting Time"
        << setw(22) << "Avg Turnaround Time"
        << endl;

    cout << "------------------------------------------------------------\n";

    cout << left
        << setw(28) << "FCFS"
        << setw(22) << fixed << setprecision(2)
        << fcfs.averageWaiting
        << setw(22)
        << fcfs.averageTurnaround
        << endl;

    cout << left
        << setw(28) << "SJF Non-Preemptive"
        << setw(22)
        << sjf.averageWaiting
        << setw(22)
        << sjf.averageTurnaround
        << endl;

    cout << left
        << setw(28) << "SJF Preemptive"
        << setw(22)
        << sjfPreemptive.averageWaiting
        << setw(22)
        << sjfPreemptive.averageTurnaround
        << endl;

    cout << left
        << setw(28) << "Priority Non-Preemptive"
        << setw(22)
        << priority.averageWaiting
        << setw(22)
        << priority.averageTurnaround
        << endl;

    cout << left
        << setw(28) << "Priority Preemptive"
        << setw(22)
        << priorityPreemptive.averageWaiting
        << setw(22)
        << priorityPreemptive.averageTurnaround
        << endl;

    cout << left
        << setw(28) << "Round Robin"
        << setw(22)
        << rr.averageWaiting
        << setw(22)
        << rr.averageTurnaround
        << endl;

    cout << "============================================================\n";
}

void showMenu()
{
    cout << "\n====================================\n";
    cout << "       CPU SCHEDULING SIMULATOR\n";
    cout << "====================================\n";
    cout << "1. FCFS\n";
    cout << "2. SJF Non-Preemptive\n";
    cout << "3. SJF Preemptive\n";
    cout << "4. Priority Non-Preemptive\n";
    cout << "5. Priority Preemptive\n";
    cout << "6. Round Robin\n";
    cout << "7. Compare All Algorithms\n";
    cout << "0. Exit\n";
    cout << "====================================\n";
}

int main()
{
    vector<Process> processes;

    inputProcesses(processes);

    int choice;

    do
    {
        showMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            FCFS(processes, true);
            break;

        case 2:
            SJF(processes, true);
            break;

        case 3:
            SJFPreemptive(processes, true);
            break;

        case 4:
            PriorityNonPreemptive(processes, true);
            break;

        case 5:
            PriorityPreemptive(processes, true);
            break;

        case 6:
        {
            int quantum;

            cout << "\nEnter Time Quantum: ";
            cin >> quantum;

            RoundRobin(processes, quantum, true);

            break;
        }

        case 7:
            compareAll(processes);
            break;

        case 0:
            cout << "\nProgram ended.\n";
            break;

        default:
            cout << "\nInvalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}