#include <sys/types.h>
#include <stdio.h>
#include "log.h"

static struct DB {
	FILE *p_writefd, *p_readfd; // production
	FILE *b_writefd, *b_readfd; // backup
} files;


/* find the most recent value of a searched for variable */
char *db_read() {
	char * output;
	return output;
}

/* add a change to end of file while maintaining atomicity */
int db_write(char *input) {
	l_write("dirty");

	fprintf(files.p_writefd, "%s", input);
	fflush(files.p_writefd); /* ensure that the data is written to file */
	
	l_write("clean");

	return 0;
}

/* fills DB struct with appropriate info 
 *
 * WARNING: expects logging to be initialized for atomicity purposes
 */
int init_db() {
	// db writefds
	files.p_writefd = fopen("../files/prod.db", "a");
	files.b_writefd = fopen("../files/back.db", "a");
	// db readfds
	files.p_readfd = fopen("../files/prod.db", "r");
	files.b_readfd = fopen("../files/back.db", "r");

	if (check_atomicity()) {
		return -1;
	}

	return 0;
}

/* close down */
int close_db() {
	// writefds
	fclose(files.p_writefd);
	fclose(files.b_writefd);
	// readfds
	fclose(files.p_readfd);
	fclose(files.b_readfd);
	return 0;
}
