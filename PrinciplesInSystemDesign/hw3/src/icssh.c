#include "icssh.h"
#include <signal.h>
#include <readline/readline.h>

int main(int argc, char* argv[]) {
	int exec_result;
	int exit_status;
	pid_t pid;
	pid_t wait_result;
	char* line;
	char cwd[256];

	#ifdef GS
		rl_outstream = fopen("/dev/null", "w");
	#endif

	// setup segmentation fault handler
	if (signal(SIGSEGV, sigsegv_handler) == SIG_ERR) {
		perror("Failed to set signal handler");
		exit(EXIT_FAILURE);
	}

    // print the prompt and wait for the user to enter commands string
	printf("> ");
	while ((line = readline(SHELL_PROMPT)) != NULL) {
		printf("> ");
        // command string is parsed into a job struct
        // will print out error message if command string is invalid
		job_info* job = validate_input(line);
        if (job == NULL) {  // command was empty string or invalid
			free(line);
			continue;
		}

        // prints out the job linked list struture for debugging
        #ifdef DEBUG   // if DEBUG flag removed in makefile, this will not longer print
            debug_print_job(job);
        #endif

		// example built-in: exit
		if (strcmp(job->procs->cmd, "exit") == 0) {
			// terminating the shell
			free(line);
			free_job(job);
			// calling validate_input with NULL will free the memory it has allocated
            validate_input(NULL);
            return 0;
		}
		// built-in: cd 
		else if (strcmp(job->procs->cmd, "cd") == 0) {
			if (job->procs->argv[1] == NULL) {
				chdir(getenv("HOME"));
				if (getcwd(cwd, sizeof(cwd)) == NULL)
					printf(DIR_ERR);
				else
					printf("%s\n", cwd);
			} 
			else if (chdir(job->procs->argv[1]) != 0) {
				printf(DIR_ERR);
			}
			else {
				getcwd(cwd, sizeof(cwd));
				printf("CWD:%s\n", cwd);
			}
		}
		// built-in: estatus 
		else if (strcmp(job->procs->cmd, "estatus") == 0) {
			printf("%d\n", exit_status);
		}
		// error handling
		else if ((pid = fork()) < 0) {
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) {
		    proc_info* proc = job->procs;
			exec_result = execvp(proc->cmd, proc->argv);
			printf("exec_result is: %d\n", exec_result);
			if (exec_result < 0) {  // error checking
				printf(EXEC_ERR, proc->cmd);
				
				// cleaning up to make Valgrind happy 
				// not necessary because child will exit. Resources will be reaped by parent
				free_job(job);  
				free(line);
				// calling validate_input with NULL will free the memory it has allocated
    			validate_input(NULL);

				exit(EXIT_FAILURE);
			}
		} else {
            // as the parent, wait for the foreground job to finish
			wait_result = waitpid(pid, &exit_status, 0);
			if (wait_result < 0) {
				printf(WAIT_ERR);
				exit(EXIT_FAILURE);
			}
		}

		free_job(job);  // if a foreground job, we no longer need the data
		free(line);
	}

    // calling validate_input with NULL will free the memory it has allocated
    validate_input(NULL);

#ifndef GS
	fclose(rl_outstream);
#endif
	return 0;
}
