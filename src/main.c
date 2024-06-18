#include <stdio.h>
#include <string.h>
#include <time.h>
#include "log.h"
#include "manip.h"

#define BUFSIZE 1024

/*
 * Creating log database
 * 
 * This database will require to keep a copy of all the data to ensure that
 * rolling back after a failed operation is possible.
 *
 * Three files are necessary as such: prod.db (actual database), back.db
 * (the backup database), and log.txt (to log operations and ensure atomicity).
 */
int main () {
	char input[BUFSIZE];

	clock_t begin = clock();

	init_log();
	init_db();

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Init time (%f secs)\n", time_spent);

	while (1) {
		printf("> ");
		fgets(input, BUFSIZE, stdin);

		if (!strncmp(input, "quit", strlen("quit"))) {
			break;
		}

		clock_t begin = clock();

		db_write(input);

		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("Completed: (%f secs)\n", time_spent);
	}

	close_db();
	close_log();
	
	return 0;
}

