#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define BUFSIZE 1024

static struct DB {
	FILE *p_writefd, *p_readfd; // production
	FILE *b_writefd, *b_readfd; // backup
	FILE *logfd; // logging
} files;

int check_atomicity();
char *db_read();
int db_write(char *input);
int init_db();
int close_db();
int l_write(char *text);

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

		db_write(input);
	}

	
	close_db();
	
	return 0;
}

/* at startup check to ensure there are no partially done operations 
 *
 * returns: 0 (success), -1 (previous operation not completed)
 */
int check_atomicity() {
	char text[100];

	fgets(text, 100, files.logfd);
	if (!strncmp(text, "dirty", strlen("dirty"))) {
		return -1;
	}
	
	return 0;
}

/* find the most recent value of a searched for variable */
char *db_read() {
	char * output;
	return output;
}

int l_write(char *text) {
	fseek(files.logfd, 0L, SEEK_SET);
	fprintf(files.logfd, "%s", text);
	fflush(files.logfd);
	return 0;
}

/* add a change to end of file while maintaining atomicity */
int db_write(char *input) {
	clock_t begin = clock();

	l_write("dirty");
	fprintf(files.p_writefd, "%s", input);
	l_write("clean");

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Completed: (%f secs)\n", time_spent);

	return 0;
}

/* fills DB struct with appropriate info */
int init_db() {
	// db writefds
	files.p_writefd = fopen("prod.db", "a");
	files.b_writefd = fopen("back.db", "a");
	// db readfds
	files.p_readfd = fopen("prod.db", "r");
	files.b_readfd = fopen("back.db", "r");
	// logging
	files.logfd = fopen("log.db", "r+"); // allow overwriting

	if (check_atomicity()) {
		return -1;
	}

	return 0;
}

int close_db() {
	// writefds
	fclose(files.p_writefd);
	fclose(files.b_writefd);
	// readfds
	fclose(files.p_readfd);
	fclose(files.b_readfd);
	// logging
	fclose(files.logfd);
	return 0;
}
