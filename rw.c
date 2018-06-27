/*Required Headers*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include "rw.h"
#include "resource.h"

/*
 * Declarations for reader-writer shared variables -- plus concurrency-control
 * variables -- must START here.
 */

static resource_t data;
static pthread_mutex_t mutex;
static pthread_cond_t cond;
static int reading, writing;

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    init_resource(&data, "my_data");
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    reading = 0;
    writing = 0;
}


void rw_read(char *value, int len) {
    printf("[rw_read] Value: %s, len: %d\n", value, len);
    print_stats(&data);

    pthread_mutex_lock(&mutex);
    while (writing) {
        pthread_cond_wait(&cond, &mutex);
    }
    reading++;
    pthread_mutex_unlock(&mutex);

    read_resource(&data, value, len);

    pthread_mutex_lock(&mutex);
    reading--;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}


void rw_write(char *value, int len) {
    printf("[rw_write] Value: %s, len: %d\n", value, len);
    print_stats(&data);

    pthread_mutex_lock(&mutex);
    while (reading) {
        pthread_cond_wait(&cond, &mutex);
    }
    writing++;
    pthread_mutex_unlock(&mutex);

    write_resource(&data, value, len);

    pthread_mutex_lock(&mutex);
    writing--;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}
