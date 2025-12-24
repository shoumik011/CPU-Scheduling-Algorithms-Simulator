
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *head = NULL;

static Task *make_task(const char *name, int priority, int burst)
{
    Task *t = (Task *)malloc(sizeof(*t));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst; // will be decremented during RR
    t->tid = 0;
    return t;
}

void add(char *name, int priority, int burst)
{
    insert(&head, make_task(name, priority, burst)); // prepends (reverses)
}

void schedule(void)
{
    // 1) array, then reverse to get input order
    int n = 0;
    for (struct node *c = head; c; c = c->next)
        n++;
    Task **arr = (Task **)malloc(sizeof(Task *) * n);
    int i = 0;
    for (struct node *c = head; c; c = c->next)
        arr[i++] = c->task;
    for (int l = 0, r = n - 1; l < r; l++, r--)
    {
        Task *tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
    }

    // Save original bursts for metrics; we'll mutate t->burst for printing
    int *orig = (int *)malloc(sizeof(int) * n);
    for (int k = 0; k < n; k++)
        orig[k] = arr[k]->burst;

    // 2) state
    int *first = (int *)malloc(sizeof(int) * n); // first time task runs
    int *comp = (int *)malloc(sizeof(int) * n);  // completion times
    for (int k = 0; k < n; k++)
    {
        first[k] = -1;
        comp[k] = -1;
    }

    // simple queue of indices in input order
    int cap = n * 100;
    int *q = (int *)malloc(sizeof(int) * cap);
    int qh = 0, qt = 0;
    for (int k = 0; k < n; k++)
        q[qt++] = k;

    int time = 0;

    while (qh < qt)
    {
        int idx = q[qh++];

        // If this task already finished (burst==0), skip
        if (arr[idx]->burst <= 0)
            continue;

        int slice = arr[idx]->burst < QUANTUM ? arr[idx]->burst : QUANTUM;

        if (first[idx] == -1)
            first[idx] = time;

        run(arr[idx], slice);
        time += slice;
        arr[idx]->burst -= slice; // mutate remaining burst (affects subsequent print lines)

        if (arr[idx]->burst == 0)
        {
            comp[idx] = time;
            delete(&head, arr[idx]);
        }
        else
        {
            q[qt++] = idx; // goes to the back of queue
        }
    }

    long long sw = 0, st = 0, sr = 0;
    for (int k = 0; k < n; k++)
    {
        int turn = comp[k];
        int wait = turn - orig[k];
        int resp = first[k];
        st += turn;
        sw += wait;
        sr += resp;
    }
    printf("Average waiting time = %.2f\n", (double)sw / n);
    printf("Average turnaround time = %.2f\n", (double)st / n);
    printf("Average response time = %.2f\n", (double)sr / n);

    free(arr);
    free(orig);
    free(first);
    free(comp);
    free(q);
}
