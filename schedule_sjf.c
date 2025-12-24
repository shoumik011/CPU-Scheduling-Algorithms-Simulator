
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *head = NULL;

static Task *make_task(char *name, int priority, int burst)
{
    Task *t = (Task *)malloc(sizeof(Task));
    t->name = strdup(name);
    t->priority = priority;
    t->burst = burst;
    t->tid = 0;
    return t;
}

void add(char *name, int priority, int burst)
{
    insert(&head, make_task(name, priority, burst)); // storage been reversed
}

void schedule(void)
{
    int n = 0;
    for (struct node *c = head; c; c = c->next)
        n++;
    Task **arr = (Task **)malloc(sizeof(Task *) * n);
    int i = 0;
    for (struct node *c = head; c; c = c->next)
        arr[i++] = c->task;

    // restore original input order
    for (int l = 0, r = n - 1; l < r; l++, r--)
    {
        Task *tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
    }

    int *done = (int *)calloc(n, sizeof(int));
    int *completion = (int *)malloc(sizeof(int) * n);
    int time = 0, finished = 0;

    while (finished < n)
    {
        int pick = -1;
        for (int k = 0; k < n; k++)
            if (!done[k])
            {
                if (pick == -1 || arr[k]->burst < arr[pick]->burst)
                    pick = k;
            }
        Task *t = arr[pick];
        run(t, t->burst);
        time += t->burst;
        completion[pick] = time;
        done[pick] = 1;
        delete(&head, t);
        finished++;
    }

    long long sw = 0, st = 0, sr = 0;
    for (int k = 0; k < n; k++)
    {
        int wait = completion[k] - arr[k]->burst;
        int turn = completion[k];
        int resp = wait; // non preemptive
        sw += wait;
        st += turn;
        sr += resp;
    }
    printf("Average waiting time = %.2f\n", (double)sw / n);
    printf("Average turnaround time = %.2f\n", (double)st / n);
    printf("Average response time = %.2f\n", (double)sr / n);

    for (int k = 0; k < n; k++)
    {
        free(arr[k]->name);
        free(arr[k]);
    }
    free(arr);
    free(done);
    free(completion);
}
