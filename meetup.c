/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "meetup.h"
#include "resource.h"

/*
 * Declarations for barrier shared variables -- plus concurrency-control
 * variables -- must START here.
 */

static resource_t codeword;
static sem_t waiting_for_group, writing;
static int count, meet_first, group_size;

void initialize_meetup(int n, int mf) {
    char label[100];
    int i;

    if (n < 1) {
        fprintf(stderr, "Who are you kidding?\n");
        fprintf(stderr, "A meetup size of %d??\n", n);
        exit(1);
    }

    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    sem_init(&waiting_for_group, 1, 0);
    sem_init(&writing, 1, 0);
    count = 0;
    init_resource(&codeword, "codeword");
    meet_first = mf;
    group_size = n;
}


void join_meetup(char *value, int len) {
    printf("[join_meetup] Value: %s, len: %d\n", value, len);
    print_stats(&codeword);
    int my_count = ++count;

    if (my_count == 1) {
        sem_destroy(&writing);
        sem_init(&writing, 1, 0);
    }

    if ((my_count == 1 && meet_first) || (my_count == group_size && !meet_first)) {
        write_resource(&codeword, value, len);
        sem_post(&writing);
    } else {
        sem_wait(&writing);
        sem_post(&writing);
    }

    // if we have enough threads, reset count and proceed;
    // else, increment count and wait.
    if (my_count == group_size) {
        count = 0;
    } else {
        sem_wait(&waiting_for_group);
    }

    sem_post(&waiting_for_group);
    read_resource(&codeword, value, len);
}
