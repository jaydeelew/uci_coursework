#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_JOB 5
#define MAX_ARGC 80 //need to use
#define MAX_LINE 80 //need to use

typedef struct proc_node {
	int jobID;
	pid_t pid;
	bool bg;
	char *status;
	struct proc_node *next;
	int argc;
	char *argv[80];
} proc_node_t;

typedef struct {
	int count;
	proc_node_t *head;
} job_list_t;

job_list_t* jobListInit(void);
proc_node_t* newProcess(char *cmdLine); 
void deleteProc(proc_node_t *proc);
int deleteJobPID(job_list_t *list, pid_t pid);
int deleteJobPID_NK(job_list_t *list, pid_t pid);
pid_t findPidofJob(job_list_t *list, int job);
int startJobPID(job_list_t *list, pid_t pid);
int addProcess(job_list_t *list, proc_node_t *proc);
void printJobs(job_list_t *list);
void deleteAll(job_list_t *list);
void sigttin_handler(int sig);
void sigttou_handler(int sig);
void sigint_handler(int sig);
void sigchld_handler(int sig);
void sigtstp_handler(int sig);
int newHighJob(job_list_t *list);
int isBackground(job_list_t *list, pid_t pid);
void setStopStatus(job_list_t *list, pid_t pid);
void setRunStatus(job_list_t *list, pid_t pid);
void setBgStatus(job_list_t *list, pid_t pid);
void setFgStatus(job_list_t *list, pid_t pid);
void delNoExistJobs(job_list_t *list);
void setTTYfg(pid_t tofg);

// globals
job_list_t *myJobs;
static int exitStatus;
static int hw2pid;
static int fgPid;
char *redirIn, *redirOut, *redirApd;

int main(int argc, char* argv[]) {

	hw2pid = getpid();
	proc_node_t *proc;
	int execReturned, inFileID, outFileID;
	pid_t pid;
	pid_t fg_wpid;
	pid_t waitReturned;
	pid_t killPid;
    char *token = NULL, *tokptr = NULL;
    char cwd[256] = {0};
    char cmdLine1[256] = {0};
    char cmdLine2[256] = {0};
	myJobs = jobListInit();

	if (signal(SIGTTIN, sigttin_handler) == SIG_ERR)
		fprintf(stderr, "SIGTTIN signal error\n");

	if (signal(SIGTTOU, sigttou_handler) == SIG_ERR)
		fprintf(stderr, "SIGTTOU signal error\n");

	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		fprintf(stderr, "SIGINT signal error\n");
	
	if (signal(SIGCHLD, sigchld_handler) == SIG_ERR)
		fprintf(stderr, "SIGCHLD signal error\n");

	if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR)
		fprintf(stderr, "SIGTSTP signal error\n");

	while (1) {
		redirIn = NULL, redirOut = NULL, redirApd = NULL;
        printf("prompt > ");
        fgets(cmdLine1, 256, stdin);
		strcpy(cmdLine2, cmdLine1);
		token = strtok_r(cmdLine1, " \n", &tokptr);
		// NULL
		if (token == NULL) {
			printf("a command was not entered\n");
			continue;
		}
        // pwd
		if (strcmp(token, "pwd") == 0) {
            getcwd(cwd, sizeof(cwd));
            printf("%s\n", cwd);
        }
		// cd 
		else if (strcmp(token, "cd") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
			if (token == NULL) {
				chdir(getenv("HOME"));
				getcwd(cwd, sizeof(cwd));
				printf("%s\n", cwd);
			} 
			else if (chdir(token) != 0) {
				fprintf(stderr, "Directory does not exist\n");
			}
			else {
				getcwd(cwd, sizeof(cwd));
				printf("%s\n", cwd);
			}
		}
		// jobs
		else if (strcmp(token, "jobs") == 0) {
			printJobs(myJobs);
        }
		// fg 
		else if (strcmp(token, "fg") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);
			pid_t tokpid;

			if (token == NULL) {
				printf("fg: a job or process id must be entered\n");
				continue;
			}

			if (*token != '%')
				tokpid = atoi(token);
			else
				tokpid = findPidofJob(myJobs, atoi(++token)); // ++ skips the %

			fgPid = tokpid;	

			signal(SIGTTIN, SIG_IGN);
			setTTYfg(tokpid);
			startJobPID(myJobs, tokpid);
			setFgStatus(myJobs, tokpid);
			setRunStatus(myJobs, tokpid);
			waitpid(tokpid, &exitStatus, WUNTRACED);
			setTTYfg(hw2pid);  
			signal(SIGTTIN, SIG_DFL);
        }
		// bg 
		else if (strcmp(token, "bg") == 0) {

            token = strtok_r(NULL, " \n", &tokptr);
			pid_t tokpid;

			if (token == NULL) {
				printf("a job or process id must be entered\n");
				continue;
			}

			if (*token != '%')
				tokpid = atoi(token);
			else
				tokpid = findPidofJob(myJobs, atoi(++token)); // ++ skips the %

			startJobPID(myJobs, tokpid);
			setBgStatus(myJobs, tokpid);
			setRunStatus(myJobs, tokpid);
        }
		// kill
		else if (strcmp(token, "kill") == 0) {
            token = strtok_r(NULL, " \n", &tokptr);

			if (token == NULL) {
				printf("a job or process id must be entered\n");
				continue;
			}
			
			if (*token == '%')
				killPid = findPidofJob(myJobs, atoi(++token)); // ++ skips %
			else
				killPid = atoi(token); // % already skipped 
				
			startJobPID(myJobs, killPid);
			deleteJobPID(myJobs, killPid);

			waitpid(killPid, NULL, 0);
		}
		// quit
		else if (strcmp(token, "quit") == 0) {
			deleteAll(myJobs);
		}
		// external commands
		else {
			proc = newProcess(cmdLine2);
			if(!addProcess(myJobs, proc)) {
				printf("process could not be added\n");
				deleteProc(proc);
				continue;
			}
			else if ((pid = fork()) < 0) {
				fprintf(stderr, "error forking");
				exit(EXIT_FAILURE);
			}
			else if (pid == 0) {
				// child
				setpgid(0, 0); // set pid, gpid to current pid with (0, 0)

				mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

				if (redirIn) { 
					inFileID = open(redirIn, O_RDONLY, mode);
					dup2(inFileID, STDIN_FILENO);
					close(inFileID);
				}

				if (redirOut) { 
					outFileID = open(redirOut, O_CREAT|O_WRONLY|O_TRUNC, mode);
					dup2(outFileID, STDOUT_FILENO);
					close(outFileID);
				}

				if (redirApd) { 
					outFileID = open(redirApd, O_CREAT|O_APPEND|O_WRONLY, mode);
					dup2(outFileID, STDOUT_FILENO);
					close(outFileID);
				}

				execReturned = execvp(proc->argv[0], proc->argv);
				if (execReturned < 0) {
					execReturned = execv(proc->argv[0], proc->argv);
					if (execReturned < 0) {
						// fprintf(stderr, "main: command %s not found\n", proc->argv[0]);
						fflush(stdout); // necessary?
						deleteProc(proc); // child will be killed in parent anyway
						free(myJobs); // child will be killed in the parent anyway
						exit(EXIT_SUCCESS); // this is so after waitpid returns, the job is deleted correctly
					}
				}
			}
			else if (pid > 0) {
				// parent
				proc->pid = pid;
				if (proc->bg == false) { // if child is to run in the foreground
					fgPid = pid;

					waitReturned = waitpid(pid, &exitStatus, WUNTRACED); // hw2 goes into background waiting for child to finish
					// printf("main: waitReturned with %d\n", waitReturned);
					if (WIFSTOPPED(exitStatus)) {
						proc->status = "stopped";
						proc->bg = true;
						// printf("main: the child has stopped\n");

						continue;
					}

					deleteJobPID_NK(myJobs, waitReturned); // hw2 returns and deletes child job
					if (WIFEXITED(exitStatus)) {
						int es = WEXITSTATUS(exitStatus);
						// printf("main: exit status of child was: %d\n", es);
						if (es == 1)
							deleteJobPID(myJobs, pid);
					}
				}
				else {
					printf("[%d] (%d)\n", proc->jobID, proc->pid);
				}
			}		
			else {
				printf("main: failed to fork\n");
				exit(EXIT_FAILURE);
			}
		}
	}

    return EXIT_SUCCESS ;
}

job_list_t* jobListInit(void) {
	job_list_t *new = malloc(sizeof(job_list_t));
	new->count = 0;
	new->head = NULL;
	return new;
}

proc_node_t* newProcess(char *cmdLine) {
	int len = 0;
	proc_node_t *new = malloc(sizeof(proc_node_t));
	char *token, *tokptr;
    token = strtok_r(cmdLine, " \n", &tokptr);

	int i;
	for (i = 0; token != NULL; i++) {
		len = strlen(token);

		if (*token == '<' || *token == '>') { // "<<" will be included
			while (token != NULL) {
				if (strcmp(token, "<") == 0) {
    				token = strtok_r(NULL, " \n", &tokptr);

					if (token == NULL) {
						printf("'<' must be followed by a file descriptor\n");
						return NULL;
					}
					len = strlen(token);
					redirIn = malloc(len + 1);
					while(*token) {
						*redirIn = *token;
						token++;
						redirIn++;
					}
					*redirIn = '\0';
					redirIn = redirIn - len;
				}	
				else if (strcmp(token, ">") == 0) {
    				token = strtok_r(NULL, " \n", &tokptr);
					
					if (token == NULL) {
						printf("'>' must be followed by a file descriptor\n");
						return NULL;
					}
					len = strlen(token);
					redirOut = malloc(len + 1);
					while(*token) {
						*redirOut = *token;
						token++;
						redirOut++;
					}
					*redirOut = '\0';
					redirOut = redirOut - len;
				}
				else if (strcmp(token, ">>") == 0) {
					token = strtok_r(NULL, " \n", &tokptr);
					
					if (token == NULL) {
						printf("'>>' must be followed by a file descriptor\n");
						return NULL;
					}
					len = strlen(token);
					redirApd = malloc(len + 1);
					while(*token) {
						*redirApd = *token;
						token++;
						redirApd++;
					}
					*redirApd = '\0';
					redirApd = redirApd - len;
				}

    			token = strtok_r(NULL, " \n", &tokptr);
			}

			break;
		}		
		else {
			new->argv[i] = malloc(len + 1);
			while(*token) {
				*(new->argv[i]) = *token;
				token++;
				(new->argv[i])++;
			}
			*(new->argv[i]) = '\0';
			new->argv[i] = new->argv[i] - len;
		}

    	token = strtok_r(NULL, " \n", &tokptr);
	}
	new->argv[i] = NULL;
	new->argc = i;
	if (*(new->argv[i - 1]) == '&')
		new->bg = true;
	else
		new->bg = false;

	return new;
} 

int addProcess(job_list_t *list, proc_node_t *proc) {
	if (proc == NULL) {
		printf("cannot add a null process\n");
		return 0;
	}
		
	if (list->head == NULL) {
		proc->jobID = 1;
		proc->status = "running";
		proc->next = NULL;
		list->head = proc;
		list->count = 1;
	}
	else {
		if (list->count == MAX_JOB) {
			printf("cannot exceed maximum jobs\n");
			return 0;
		}

		proc_node_t *current = list->head;

		while (current->next != NULL) {
			current = current->next;		
		}
		list->count++;
		proc->jobID = newHighJob(myJobs);
		proc->status = "running";
		proc->next = NULL;
		current->next = proc;
	}
	return 1;
}

void deleteProc(proc_node_t *proc) { // frees memory, does not kill process
	for (int i = 0; i < proc->argc; i++) {
		free(proc->argv[i]);
	}
	free(proc);
}

int deleteJobPID(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;
	proc_node_t *previous = NULL;

	while (current != NULL){
		if (current->pid == pid) {
			if (list->head == current) {
				list->head = current->next;
				kill(pid, SIGINT);
				deleteProc(current);
				list->count--;
				return 1;
			}
			else {
				previous->next = current->next;
				kill(pid, SIGINT);
				deleteProc(current);
				list->count--;
				return 1;
			}
		}
		previous = current;
		current = current->next;
	}
	// printf("deleteJobPID: cannot delete, pid %d not found\n", pid); //DEBUG
	return 0;
}

int deleteJobPID_NK(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;
	proc_node_t *previous = NULL;

	while (current != NULL){
		if (current->pid == pid) {
			if (list->head == current) {
				list->head = current->next;
				deleteProc(current);
				list->count--;
				return 1;
			}
			else {
				previous->next = current->next;
				deleteProc(current);
				list->count--;
				return 1;
			}
		}
		previous = current;
		current = current->next;
	}
	// printf("deleteJobPID_NK: cannot delete, pid %d not found\n", pid); //DEBUG
	return 0;
}

pid_t findPidofJob(job_list_t *list, int job) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->jobID == job) {
			return current->pid;
		}
		current = current->next;
	}
	// printf("findPidofJob: job %d not found\n", job); //DEBUG
	return 0;
}

int startJobPID(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			current->status = "running";
			kill(pid, SIGCONT);
			return 1;
		}
		current = current->next;
	}
	// printf("startJobPID: cannot start %d not found\n", pid); //DEBUG
	return 0;
}

void printJobs(job_list_t *list) {
	proc_node_t *current = list->head;

	while (current != NULL) {
		printf("[%d] (%d) %s %s ", current->jobID, current->pid, current->status, (current->bg == true)?"bg":"fg");
		for (int i = 0; i < current->argc; i++)
			printf("%s ", current->argv[i]);
			
		printf("\n");
		current = current->next;
	}
}

void deleteAll(job_list_t *list) {
	proc_node_t *current = list->head;
	proc_node_t *temp = NULL;
	pid_t procid;

	while (current != NULL) {
		temp = current->next;
		procid = current->pid;

		if (kill(procid, SIGINT) == 0) {
			// printf("deleteAll: pid %d killed\n", procid); //DEBUG
			deleteProc(current);
		}
		else {
			// printf("deleteAll: FAILED to kill pid %d\n", procid);
		}
		current = temp;
	}
	free(list);
	exit(0);
}

void sigttin_handler(int sig) {
	// printf("sigttin_handler: this is process %d\n", getpid());
}

void sigttou_handler(int sig) {
	// printf("sigttou_handler: this is process %d\n", getpid());
}

void sigint_handler(int sig) {
	// printf("sigint_handler: this is process %d\n", getpid());
	kill(fgPid, SIGINT);
}

void sigtstp_handler(int sig) {
	// printf("sigtstp_handler: this is process %d\n", getpid());
	kill(fgPid, SIGTSTP);
	setStopStatus(myJobs, fgPid);
	setBgStatus(myJobs, fgPid);
}

void sigchld_handler(int sig) {
	// printf("sigchld_handler: this is process %d\n", getpid());
	pid_t wpid;

	wpid = waitpid(-1, &exitStatus, WNOHANG);
	// printf("sigchld_handler: wpid is %d\n", wpid);
	if (WIFEXITED(exitStatus)) {
		deleteJobPID_NK(myJobs, wpid);
		// printf("sigchld_handler: wpid %d exited with status %d\n", wpid, WEXITSTATUS(exitStatus));
	}
	else {
		// printf("sigchld_handler: wpid %d exited abnormally\n", wpid); // happens after ctrl-c or ctrl-z
		setStopStatus(myJobs, fgPid);
		setBgStatus(myJobs, fgPid);
	}

	delNoExistJobs(myJobs);
}

int newHighJob(job_list_t *list) {
	proc_node_t *current = list->head;
	int hiJob = list->count;

	while (current != NULL) {
		if (current->jobID >= hiJob)
			hiJob = current->jobID + 1;		

		current = current->next;
	}
	return hiJob;
}

int isBackground(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			return current->bg;
		}
		current = current->next;
	}
}

void setStopStatus(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			current->status = "stopped";
		}
		current = current->next;
	}
}

void setRunStatus(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			current->status = "running";
		}
		current = current->next;
	}
}

void setBgStatus(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			current->bg = true;
		}
		current = current->next;
	}
}

void setFgStatus(job_list_t *list, pid_t pid) {
	proc_node_t *current = list->head;

	while (current != NULL){
		if (current->pid == pid) {
			current->bg = false;
		}
		current = current->next;
	}
}

void delNoExistJobs(job_list_t *list) {
	proc_node_t * current = list->head;

	while (current != NULL){
		if (kill(current->pid, 0) < 0) {
			if (errno == ESRCH)
				deleteJobPID_NK(myJobs, current->pid);
		}
		current = current->next;
	}
}

void setTTYfg(pid_t tofg) {
	pid_t before, after;
	before = tcgetpgrp(0);
	signal (SIGTTOU, SIG_IGN);
	while (1) {
		tcsetpgrp(0, tofg); // set terminal foreground process group to process group of pid 
		after = tcgetpgrp(0);
		if (after != before)
			break;
	}
	signal (SIGTTOU, SIG_DFL);
}