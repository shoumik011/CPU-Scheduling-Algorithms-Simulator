# CPU Scheduling Algorithms Simulator (C)

A modular **Operating Systems CPU scheduling simulator** written in **C**.  
Given a workload file of tasks (**task name, priority, CPU burst**), the program simulates multiple classic CPU scheduling policies and reports scheduling results + average timing metrics.

## Keywords (ATS-friendly)
Operating Systems, CPU Scheduling, FCFS, SJF, Priority Scheduling, Round Robin, Time Quantum, Virtual CPU, C, Linked List, Modular Design, GCC, Makefile

---

## Scheduling Algorithms Implemented
This repo includes separate implementations for:
- **FCFS** (First Come, First Served)
- **SJF** (Shortest Job First)
- **Priority Scheduling**
- **Round Robin (RR)** — time quantum = **10 ms**
- **Priority + Round Robin** — RR within same priority

### Tie-breaking rule
If tasks are tied (same burst for SJF or same priority for Priority / Priority+RR), tasks run **in the same order they appear in the input file**.

---

## Input Format
The workload file is plain text, one task per line:

```text
T1, 4, 20
T2, 2, 25
T3, 3, 25
T4, 3, 15
T5, 10, 10
