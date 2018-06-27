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

void initialize_readers_writer() {
    /*
     * Initialize the shared structures, including those used for
     * synchronization.
     */
    init_resource(&data, "my_data");
}


void rw_read(char *value, int len) {
    printf("[rw_read] Value: %s, len: %d\n", value, len);
    print_stats(&data);
    read_resource(&data, value, len);
}


void rw_write(char *value, int len) {
    printf("[rw_write] Value: %s, len: %d\n", value, len);
    write_resource(&data, value, len);
}
