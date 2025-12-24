# CPU Scheduling Algorithms Simulator (C)

A C-based simulator for classic **Operating Systems CPU scheduling algorithms**.  
The program reads a workload file of tasks (task name, priority, CPU burst) and simulates how different scheduling policies choose which task runs next. It also reports the **average turnaround time, waiting time, and response time** for each algorithm.

---

## Algorithms Implemented

- **FCFS** (First-Come, First-Served)
- **SJF** (Shortest Job First)
- **Priority Scheduling**
- **Round Robin (RR)** — time quantum = **10 ms**
- **Priority + Round Robin** — schedules by priority, and uses RR among tasks with the same priority

### Tie-breaking rule
If there are ties in **burst time** (SJF) or **priority** (Priority / Priority+RR), tasks execute **in the same order they appear in the input file**.

---

## Input Format

Provide a text file (example: `schedule.txt`) with one task per line:

```text
T1, 4, 20
T2, 2, 25
T3, 3, 25
T4, 3, 15
T5, 10, 10
