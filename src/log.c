#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

static FILE *logfd;

/* for logging purposes */
int l_write(char *text) {
	fseek(logfd, 0L, SEEK_SET);
	fprintf(logfd, "%s", text);
	fflush(logfd);
	return 0;
}

/* at startup check to ensure there are no partially done operations 
 *
 * returns: 0 (success), -1 (previous operation not completed)
 */
int check_atomicity() {
	char text[100];

	fgets(text, 100, logfd);
	if (!strncmp(text, "dirty", strlen("dirty"))) {
		return -1;
	}
	
	return 0;
}

int init_log() {
	logfd = fopen("../files/log", "r+");
	return 0;
}

int close_log() {
	fclose(logfd);
	return 0;
}
