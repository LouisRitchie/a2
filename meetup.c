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

static sem_t semaphore;
// sem_post - increment
// sem_wait - decrement, if 0 then block until > 0.
// sem_init(&semaphore, 1, 0);

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
    sem_init(&semaphore, 1, 0);
    init_resource(&codeword, "codeword");
}


void join_meetup(char *value, int len) {
    printf("[join_meetup] Value: %s, len: %d\n", value, len);
    print_stats(&codeword);

    write_resource(&codeword, value, len);
}
