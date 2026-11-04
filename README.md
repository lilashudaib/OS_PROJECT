# OS_PROJEC
# CPU Scheduling Simulator

A C++ simulator for CPU scheduling algorithms, built as part of the Operating Systems course.

**Author:** Lilas Hudaib

---

## 📌 Project Description

This project simulates different CPU scheduling algorithms and calculates the scheduling performance for each process.

## ⚙️ Algorithms Implemented

| # | Algorithm |
|---|-----------|
| 1 | FCFS (First Come First Served) |
| 2 | SJF Non-Preemptive |
| 3 | SJF Preemptive |
| 4 | Priority Non-Preemptive |
| 5 | Priority Preemptive |
| 6 | Round Robin |

## 📥 Input

The user enters the number of processes, then for each process:

- Arrival Time
- Burst Time
- Priority

For **Round Robin**, the user also enters the **Time Quantum**.

## 📤 Output

The program displays:

- Gantt Chart / Execution Timeline
- Completion Time
- Waiting Time
- Turnaround Time
- Response Time
- Average Waiting Time
- Average Turnaround Time

It also provides a **comparison between all algorithms** using Average Waiting Time and Average Turnaround Time.

## ▶️ How to Run

1. Compile the C++ file:
   ```bash
   g++ osproject202320150.cpp -o scheduler
   ```
2. Run the program:
   ```bash
   ./scheduler
   ```
3. Enter the process information.
4. Choose an algorithm from the menu.
5. For Round Robin, enter the Time Quantum.
6. Choose **option 7** to compare all algorithms.

> On Windows, run it with `scheduler.exe` instead of `./scheduler`.

## 📝 Notes

- `std::vector` is used to store the processes.
- `std::queue` is used for the Ready Queue in Round Robin.
- In Priority Scheduling, a **smaller priority number means higher priority**.

## 📂 Files

- `osproject202320150.cpp`: source code
- `Operating Systems Project (1).pdf`: project description document
- `Project OS.txt`: project notes
