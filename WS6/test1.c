#include "apue.h"

int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* parent */
		fprintf(stdout, "Process id: %d\n", getpid());
		sleep(2);
		exit(2);				/* terminate with exit status 2 */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* first child */
		fprintf(stdout, "Process id: %d\n", getpid());
		sleep(4);
		abort();				/* terminate with core dump */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* second child */
		fprintf(stdout, "Process id: %d\n", getpid());
		execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
		exit(7);				/* shouldn't get here */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* third child */
		fprintf(stdout, "Process id: %d\n", getpid());
		sleep(8);
		exit(0);				/* normal exit */
	}


	fprintf(stdout, "Process id: %d\n", getpid());
	sleep(6);					/* fourth child */
	kill(getpid(), SIGKILL);	/* terminate w/signal, no core dump */
	exit(6);					/* shouldn't get here */
}
