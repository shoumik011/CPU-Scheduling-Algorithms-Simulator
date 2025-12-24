# CPU Scheduling Algorithms Simulator (C)

A modular **Operating Systems CPU scheduling simulator** written in **C**, implementing classic scheduling algorithms via separate scheduler modules. Build and run different policies (FCFS/SJF/Priority/RR/Priority+RR) as separate executables.

## Keywords (ATS-friendly)
Operating Systems, CPU Scheduling, FCFS, SJF, Priority Scheduling, Round Robin, Time Quantum, Preemptive Scheduling, C, Modular Design, Linked List, Systems Programming, GCC, Makefile

---

## Scheduling Algorithms Implemented
This project provides separate implementations for:
- **FCFS** (First-Come, First-Served)
- **SJF** (Shortest Job First)
- **Priority Scheduling**
- **Round Robin (RR)**
- **Priority + Round Robin (Priority RR)**

---

## Build (matches the provided Makefile)

> There is no default `all` target — build the scheduler you want:

```bash
make fcfs
make sjf
make priority
make rr
make priority_rr

