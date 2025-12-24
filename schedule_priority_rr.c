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
    t->burst = burst; // mutating remaining bursts
    t->tid = 0;
    return t;
}

void add(char *name, int priority, int burst)
{
    insert(&head, make_task(name, priority, burst));
}

void schedule(void)
{
    // snapshot to array, then reverse so that we can restore input order
    int n = 0;
    for (struct node *c = head; c; c = c->next)
        n++;
    Task **array = (Task **)malloc(sizeof(Task *) * n);
    int i = 0;
    for (struct node *c = head; c; c = c->next)
        array[i++] = c->task;
    for (int l = 0, r = n - 1; l < r; l++, r--)
    {
        Task *tmp = array[l];
        array[l] = array[r];
        array[r] = tmp;
    }

    // keeping originals for metrics; we'll mutate t->burst
    int *original = (int *)malloc(sizeof(int) * n);
    int *first = (int *)malloc(sizeof(int) * n);
    int *comp = (int *)malloc(sizeof(int) * n);
    for (int k = 0; k < n; k++)
    {
        original[k] = array[k]->burst;
        first[k] = -1;
        comp[k] = -1;
    }

    // 10 priority queues (1..10), store indices into array[]
    int cap = n * 100; // we have enough room
    int *pq[11];
    int qh[11] = {0}, qt[11] = {0};
    for (int p = 1; p <= 10; p++)
        pq[p] = (int *)malloc(sizeof(int) * cap);

    // seeding queues in input order so RR within a bucket is stable
    for (int k = 0; k < n; k++)
        pq[array[k]->priority][qt[array[k]->priority]++] = k;

    int time = 0, finished = 0;

    while (finished < n)
    {
        // finding highest non-empty priority (10..1)
        int p = 0;
        for (int cand = 10; cand >= 1; cand--)
            if (qh[cand] < qt[cand])
            {
                p = cand;
                break;
            }
        if (!p)
            break; 

        int idx = pq[p][qh[p]++]; // popping front
        if (array[idx]->burst <= 0)
            continue; 

        int slice;
        if (qh[p] >= qt[p])
        {
            slice = array[idx]->burst; // lastinbucket => drain fully
        }
        else
        {
            slice = (array[idx]->burst < QUANTUM) ? array[idx]->burst : QUANTUM;
        }

        if (first[idx] == -1)
            first[idx] = time;

        run(array[idx], slice); // CPU.c prints name, prio, current burst, slice
        time += slice;
        array[idx]->burst -= slice; // mutate remaining so next print shows updated burst

        if (array[idx]->burst == 0)
        {
            comp[idx] = time;
            delete(&head, array[idx]);
            finished++;
        }
        else
        {
            pq[p][qt[p]++] = idx; // RR within same priority
        }
    }

    long long sw = 0, st = 0, sr = 0;
    for (int k = 0; k < n; k++)
    {
        int turn = comp[k];
        int wait = turn - original[k];
        int resp = first[k];
        st += turn;
        sw += wait;
        sr += resp;
    }
    printf("Average waiting time = %.2f\n", (double)sw / n);
    printf("Average turnaround time = %.2f\n", (double)st / n);
    printf("Average response time = %.2f\n", (double)sr / n);

    for (int p = 1; p <= 10; p++)
        free(pq[p]);
    free(array);
    free(original);
    free(first);
    free(comp);
}
