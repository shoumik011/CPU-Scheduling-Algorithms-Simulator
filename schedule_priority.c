
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

struct node *head = NULL;

static Task *create_task(char *name, int priority, int burst)
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
    insert(&head, create_task(name, priority, burst));
}

void schedule(void)
{
    int n = 0;
    for (struct node *c = head; c; c = c->next)
        n++;
    Task **array1 = (Task **)malloc(sizeof(Task *) * n);
    int i = 0;
    for (struct node *c = head; c; c = c->next)
        array1[i++] = c->task;

    // restore original input order
    for (int l = 0, r = n - 1; l < r; l++, r--)
    {
        Task *temp = array1[l];
        array1[l] = array1[r];
        array1[r] = temp;
    }

    int *done = (int *)calloc(n, sizeof(int));
    int *completion_time = (int *)malloc(sizeof(int) * n);
    int time = 0, finished = 0;

    while (finished < n)
    {
        int num = -1;
        for (int k = 0; k < n; k++)
            if (!done[k])
            {
                if (num == -1 || array1[k]->priority > array1[num]->priority)
                    num = k;
            }
        Task *t = array1[num];
        run(t, t->burst);
        time += t->burst;
        completion_time[num] = time;
        done[num] = 1;
        delete(&head, t);
        finished++;
    }

    long long sw = 0, st = 0, sr = 0;
    for (int k = 0; k < n; k++)
    {
        int wait = completion_time[k] - array1[k]->burst;
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
        free(array1[k]->name);
        free(array1[k]);
    }
    free(array1);
    free(done);
    free(completion_time);
}
