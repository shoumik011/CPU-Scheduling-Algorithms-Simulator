
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
    insert(&head, make_task(name, priority, burst)); // inserts at head (reverses input)
}

void schedule(void)
{
    // snapshot current list to array, then reverse to have the original input order
    int n = 0;
    for (struct node *c = head; c; c = c->next)
        n++;
    Task **array2 = (Task **)malloc(sizeof(Task *) * n);
    int i = 0;
    for (struct node *c = head; c; c = c->next)
        array2[i++] = c->task;
    for (int l = 0, r = n - 1; l < r; l++, r--)
    {
        Task *tmp = array2[l];
        array2[l] = array2[r];
        array2[r] = tmp;
    }

    int *completion_time = (int *)malloc(sizeof(int) * n);
    int time = 0;

    for (int k = 0; k < n; k++)
    {
        Task *t = array2[k];
        run(t, t->burst);
        time += t->burst;
        completion_time[k] = time;
        delete(&head, t);
    }

    long long sw = 0, st = 0, sr = 0;
    for (int k = 0; k < n; k++)
    {
        int wait = completion_time[k] - array2[k]->burst;
        int turn = completion_time[k];
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
        free(array2[k]->name);
        free(array2[k]);
    }
    free(array2);
    free(completion_time);
}
