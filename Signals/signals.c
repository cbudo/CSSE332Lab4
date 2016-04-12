/* 
  Copyright Christopher Budo, Alia Robinson
  This is where you implement the core solution.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
char *username = "";
void signal_callback_handler(int signum) {
	exit(signum);
}
void ps_callback_handler(int signum) {
	char cmd[50]; 
	snprintf(cmd, 50, "ps -u %s", username);
	printf("%s\n",cmd);
	system(cmd);
}

void alarm_callback_handler(int signum) {
	//Beeping may or may not work
	printf("Beep!\n");
	fflush(stdout);
	system("echo -n \"\\007\"");
}

void kill_callback_handler(int signum) {
	// prompt the user for a process to kill
	// send the process id specified the kill command
	int pid = 0;
	char cmd[50];
	printf("What pid shall I kill for you master?\n");
	scanf("%d", &pid); 
	snprintf(cmd, 50, "kill %d", pid);
	printf("%s\n",cmd);
	system(cmd);
}

int main(int argc, char* argv[]) {
	if (argc != 2){
		fprintf(stderr, "If you didn't add a username, go f*ck yourself");
		return 0;
	}
	username = argv[1];
	signal(SIGINT, signal_callback_handler);
	signal(SIGALRM, alarm_callback_handler);
	signal(SIGQUIT, ps_callback_handler);
	signal(SIGUSR1, kill_callback_handler);
	system("ps -u budocf");
	printf("current pid: %d. \n", getpid());

	while (1) {
		alarm(5);
		sleep(5);
	}
	return 0;
}