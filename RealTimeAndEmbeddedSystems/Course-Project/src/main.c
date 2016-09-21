/**
The YAHS/BIHS project was done by:
	** Ameer Alkam  -- 1120217
	** Ali Alnubani -- 1120130
	** Weam Mikkawi -- 1120442
Supervidsed by: Dr. Ahmed Afaneh
**/


#include "yahs.h"

/**
	implements the functions and structures for the workersa and the main server
**/


int main(int argc, char const *argv[]) {
	/* let input be the config file */
	printf("Configuring server ...\n");
	if (argc > 1) {
		strncpy(config_file, argv[2], 100);
		config_file[strlen(argv[2])-1] = '\0';
	} else {
		strncpy(config_file, "./config/config\0", 16);
	}

	/*********************************/
	/* configure server */
	/*********************************/
	if (config(config_file)) {
		perror("Error loading config file ...\n");
		exit(-1);
	}
	srv_header_len = strlen(srv_header);
	printf("Server configured ...\n");

	/*********************************/
	/* start server */
	/*********************************/
	init_sem();
	init_socket();
	init_workers();
	init_signlas();
	cli();
}

int config(const char *file_name) {
	FILE* conf_f;
	if (!(conf_f = fopen(file_name, "r"))) {
		/* file couldn't be openned */
		return -1;
	}

	char buf[config_lines];
	int i, value;
	while (fgets(buf, config_lines, conf_f)) {
		if (buf[0] == '#' || buf[0] == '\n') {
			/* if the whole line is a comment or empty */
			continue;
		} else if ((i = find_char(buf, '#')) > 0) {
			/* if there's an inline comment */
			buf[i] = '\0';
		}

		if ((i = find_char(buf, ':')) < 0) {
			/* error in format */
			return -2;
		} else {
			buf[i] = ' ';
		}

		if (buf[0] == 'S' || buf[0] == 's') {
			/* if it is the server path */
			if (strncasecmp("SERVER_PATH", buf, 11)) {
				// if they're equal don't enter if
				goto bad_form;
			}
			sscanf(buf, "%*s %s", files_path);
			continue;
		}

		sscanf(buf, "%*s %d", &value);
		if (!strncasecmp("PORT", buf, 4)) {
			server_port = value;
		} else if (!strncasecmp("MAX_NUM_WORKERS", buf, 15)) {
			number_of_workers = value;
		} else if (!strncasecmp("MAX_CONN_PERWORKER", buf, 18)) {
			conn_per_worker = value;
		} else if (!strncasecmp("MAX_DATA_BUFF", buf, 13)) {
			buffer_size = value;
		} else if (!strncasecmp("MAX_REQ", buf, 7)) {
			max_req = value;
		} else {
			bad_form:
				return -3;
		}
	}

	return 0;
}

void init_sem() {
	// create semaphore
	// all workers refer to the same semaphore
	// the common semaphore is identified by the main server ID
	int sem_id = semget(getpid(), 1, IPC_CREAT | 0666);

	if (sem_id < 0) {
		/* couldn't create semaphore */
		perror("Could not build the semaphore ... exiting\n");
		exit(-1);
	}

	// initialize the common sempahore
	if (semctl(sem_id, 0, SETVAL, 1) < 0) {
		perror("Could not initialize the semaphore ... main server exiting\n");
		exit(-1);
	}
}

void init_socket() {
	/*********************************/
	/* setup main socket */
	/*********************************/
	printf("Setting up server socket ...\n");

	// 1. create address structure
	srv_adrs.sin_family			= AF_INET;
	srv_adrs.sin_port			= htons(server_port);
	srv_adrs.sin_addr.s_addr	= htonl(INADDR_ANY);

	// 2. create socket, bind, listen
	if (!(srv_socket = create_socket(&srv_adrs, number_of_workers*conn_per_worker, 1))) {
		perror("Failed to create socket ...");
		exit(-1);
	}
	printf("Server socket set up -- PORT:%d...\n", server_port);
}

void init_workers() {
	printf("Creating workers ...\n");
	// create workers
	workers = (worker_info_t*)malloc(sizeof(worker_info_t)*number_of_workers);
	create_workers(number_of_workers, workers);
}

void init_signlas() {
	// 3. handling system signals
	printf("Setting signals handlers ...\n");
	signal(SIGINT, sig_terminate);
	signal(SIGUSR2, sig_reconfig);
	signal(SIGTSTP, sig_terminate);
	signal(SIGCHLD, sig_child_term);
}

void cli() {
	char buf[50];
	printf("Simple CLI to control main ...\nType help for help\n\n\n>> ");
	while (/*!terminated && */fgets(buf, 50, stdin)/*read(stdin, buf, 50)*/) {
		if (!(strncasecmp("help", buf, 4) && strncasecmp("h", buf, 1))) {
			printf(
				"Allowed commands are:\n\
				\r\t- Reconfig	: to reconfigure the server\n\
				\r\t- Term		: to terminate the server\n\
				\r\t- Restart	: to restart the server\n\
				\r\t- Dir		: to print the server root\n\
				\r\t- Ls		: to list the files in the directory\n\
				\r\t- Ld		: to display the current load on the server\n\n\
				\r\t=====\n\n"
			);
			/* - Stat		: show some statistics regarding the server stat\n\n\*/
		} else if (!(strncasecmp("reconfig", buf, 8))) {
			sig_reconfig(0);
		} else if (!(strncasecmp("term", buf, 4))) {
			sig_terminate(SIGTSTP);
		} else if (!(strncasecmp("restart", buf, 7))) {
			sig_terminate(SIGINT);
		} else if (!(strncasecmp("dir", buf, 3))) {
			printf("Server root is::\n\t%s\n", files_path);
		} else if (!(strncasecmp("ls", buf, 2))) {
			char t[8+strlen(files_path)];
			strcpy(t, "ls -al \0");
			strcat(t, files_path);
			system(t);
		} else if (!(strncasecmp("ld", buf, 2))) {
			int t = 0, k;
			for (size_t i = 0; i < number_of_workers; i++) {
				k = (conn_per_worker - (*((int*)(workers[i].shm.shm_base))));
				t += k;
				printf("Worker %d has %d active connections\n", i, k);
			}
			printf("\n\t**Total number of active connections is %d\n", t);
		} else if (!(strncasecmp("clear", buf, 5))) {
			system("clear");
		} else {
			printf("\tInvalid command.\nYou can use `help` command to display available commands\n\n");
		}

		printf(">> ");
	}

	// if CTRL+D (EOF) was given as input
	// exit server instead of leaving it floating
	sig_terminate(SIGTSTP);
}


// FIXME
// Signals not properly handled :/
//

/* SIGINT handler to restart server */
/* SIGTSTP handler to terminate main */
/* SIGUSR1 handler to terminate workers */
void *sig_terminate(int sig) {
	// send terminate signal to all workers and terminate this process
	terminated = (sig == SIGINT)?(2):(1);
	if (sig == SIGUSR1) {
		// worker side of terminate
		// simply return after setting terminated flag to 1
		return;
	}

	printf("\nInitiaiting terminate process ...\n");
	for (size_t i = 0; i < number_of_workers; i++) {
		kill(workers[i].id, SIGUSR1);
		unshare_memory(&(workers[i].shm));
	}
	printf("Sent terminate signals to workers ...\n");
	free(workers);

	printf("Killing server socket ...\n");
	close(srv_socket);

	printf("Waiting for workers to finsih and terminate ...\n");

	if (terminated == 2) {
		if ((sig = fork()) >= 0) {
			if (!sig) {
				printf("Server restarting ... holding to wait on resources\n");
				// TODO another method is to use execl to restart the process in the same as previous space
				terminated = 0;
				// wait to make sure any resources (specifically the server socket) are freed
				sleep(200);

				// initialize and begin server
				terminated = server_port;
				config(config_file);
				if (server_port != terminated) {
					// if socket didn't change no need to close and reopen socket
					// close(srv_socket);
					init_socket();
				}
				terminated = 0;
				init_sem();
				init_workers();

				// cli();
				// command line wouldn't work with new forks
				// since console control goes back to user :(
				while (1) {
					sleep(100);
				}
			} else {
				printf("Previous instance waiting to children terminate ...\n");
				// wait function
				while (1) {
					sleep(100);
				}
			}
		} else {
			perror("Error restarting ... exiting now\n");
			exit(-7);
		}
	}
}

void *sig_reconfig(int sig) {
	// this HACK would work if number of workers is greater than 2
	// use terminated as a temporary variable
	terminated = number_of_workers;
	config(config_file);

	printf("\nTerminating active old workers ...\n");
	for (size_t i = 0; i < terminated; i++) {
		kill(workers[i].id, SIGUSR1);
	}
	printf("Sent terminate signals to old workers ...\n");

	if (server_port == srv_adrs.sin_port) {
		/* same previous socket */
		printf("Creating new workers now ...\n");
		create_workers(number_of_workers, workers);
	} else {
		close(srv_socket);
		init_sem();
	}
}

void *sig_child_term(int sig) {
	if (terminated) {
		if ((!(terminated > 2)) && !(--number_of_workers)){
			printf("All workers closed ... ");
			if (terminated == 1) {
				printf("Server terminating\n");
			}
				exit(0);
			// } else if (terminated == 2) {
			// 	printf("Server restarting is 200 seconds ...\n");
			// 	sleep(200); // 8 minutes or so
			// 				// because qrefet mn kammn marra ma rde y3ml free lal socket el OS -.-
			// 	if (execl("./", "server_exec", NULL) < 0) {
			// 		perror("Error restarting ... exiting now\n");
			// 		exit(-7);
			// 	}
			// }
		}
	} else {
		perror("A worker crashed ... restarting it\n");

		for (size_t i = 0; i < number_of_workers; i++) {
			if (waitpid(workers[i].id, NULL, WNOHANG)) {
				/* this worker is the one terminated */
				// unshare the memory with it
				unshare_memory(&workers[i].shm);
				// create a new worker to replace it
				create_new_worker(&workers[i]);
			}
		}
	}
}


void create_workers(int count, worker_info_t *w) {
	do {
		create_new_worker(&w[count-1]);
	} while(--count > 0);
}

void create_new_worker(worker_info_t *w) {
	int pid;
	if ((pid = fork()) >= 0) {
		if (!pid) {
			/* child */
			// implement signals for worker here
			signal(SIGUSR1, sig_terminate);
			start_worker();
		} else {
			/* parent */
			// assign worker ID and assign it SHM
			w->id	= pid;
			w->shm	= make_shared(sizeof(int)*4, pid);
		}
	} else {
		perror("Can't fork new worker ...\n");
		perror("Terminating server ...\n");

		sig_terminate(SIGTSTP);
		exit(1);
	}
}

int find_char(char const* str, const char c) {
	char *cp;
	// strchr return a pointer to the character requested
	if ((cp = strchr(str, c)) == NULL) {
		// character not found
		return -1;
	} else {
		// return the difference between the base and the character's pointer
		// casted into int -> the index of the character
		return (int)(cp - str);
	}
}


/*********************************/
/*********************************/
/* worker code begins here */


void start_worker() {
	// 1. create semaphore
	// all workers use the same key, the original parent PID
	int sem_id = semget(getppid(), 1, IPC_CREAT | 0666);

	if (sem_id < 0) {
		/* couldn't create semaphore */
		perror("Could not build the semaphore ... exiting\n");
		exit(-1);
	}

	// the semaphore buffer object usd to manipulate the common sempahore
	// its flag is set to no wait, t avoid blocking
	struct sembuf acquire, release;

	acquire.sem_num	= 0;
	acquire.sem_op	= -1;
	acquire.sem_flg	= IPC_NOWAIT | SEM_UNDO;

	release.sem_num	= 0;
	release.sem_op	= +1;
	release.sem_flg	= IPC_NOWAIT | SEM_UNDO;

	// for tmporary manipulations if needed
	connection_t *tmp_slot, *prev;

	if (!(free_slots = (connection_t*)malloc(sizeof(connection_t)))){
		perror("Memory error ...\n");
		exit(-1);
	}
	memset(free_slots, 0, sizeof(connection_t));

	if (!(used_slots = (connection_t*)malloc(sizeof(connection_t)))){
		perror("Memory error ...\n");
		exit(-1);
	}
	memset(used_slots, 0, sizeof(connection_t));

	// reserve slots for all possible connections
	for (size_t i = 0; i < conn_per_worker; i++) {
		tmp_slot = (connection_t*)malloc(sizeof(connection_t));
		memset(tmp_slot, 0 , sizeof(connection_t));
		tmp_slot->request = (char*)malloc(max_req * sizeof(char));
		tmp_slot->verb = 0;
		tmp_slot->file = (char*)malloc(((int)(max_req/2)) * sizeof(char));
		tmp_slot->head = (char*)malloc(((int)(max_req/2)) * sizeof(char));
		tmp_slot->next = free_slots->next;
		free_slots->next = tmp_slot;
	}

	shm_t shm = make_shared(sizeof(int)*4, getpid());
	int *num_of_places;
	num_of_places = (int*)(shm.shm_base);
	(num_of_places[0]) = conn_per_worker;


	int tmp;
	struct sockaddr_in* tmp_adrs;
	char buf[buffer_size];
	// 2. start worker functionality
	while (!terminated || used_slots->next) {
		// first thing check for new connection request
		if (!terminated && free_slots->next && !(semop(sem_id, &acquire, 1))) {
			// if more connections can be handeld AND sempahore acquire was successful
			// Tested -> correct method for nonblocking accept()
			tmp = accept(srv_socket, tmp_adrs, sizeof(struct sockaddr_in));
			if (tmp > 0) {
				// if a connection was made
				// get a free slot
				tmp_slot = free_slots->next;
				free_slots->next = tmp_slot->next;

				// put new connection into it's slot
				tmp_slot->socket_d	= tmp;
				tmp_slot->cli_adrs	= tmp_adrs;
				tmp_slot->status	= SLT_RECV;
				tmp_slot->next		= used_slots->next;

				// place the slot into used_slots list
				used_slots->next = tmp_slot;

				// make socket a nonblocking open file
				int flags = fcntl(tmp_slot->socket_d, F_GETFL, 0);
				if (flags < 0) {
					tmp_slot->status = SLT_FREE;
				} else {
					flags = flags | O_NONBLOCK;
					if (fcntl(tmp_slot->socket_d, F_SETFL, flags) != 0) {
						// if error happened close socket, end connection
						write(tmp_slot->socket_d, "HTTP/1.0 500 Internal Server Error\n\r\n\r\n", 39);
						tmp_slot->status = SLT_FREE;
					}
				}

				// decrement the number of connections the worker can handle
				--(num_of_places[0]);
			}
		}
		// release semaphore regardles of whether it made a connection or not
		semop(sem_id, &release, 1);
		if (used_slots->next) {
			// reset prev
			prev = used_slots;
			for (tmp_slot = used_slots->next; tmp_slot != NULL; tmp_slot = tmp_slot->next) {
				switch (tmp_slot->status) {
					case SLT_RECV:		/* recieving data from connection */
						if (read(tmp_slot->socket_d, tmp_slot->request, max_req) < 0) {
							; // nothing yet TODO add a counter if a lot of delay happens then close
						} else {
							tmp_slot->status = SLT_HNDL;
						}
						break;
					case SLT_HNDL:		/* parse request, open requested file */
						if ((tmp = parse_request(tmp_slot->request, &(tmp_slot->verb), &(tmp_slot->file), &(tmp_slot->head))) < 0) {
							if (tmp == -3) {
								write(tmp_slot->socket_d, "HTTP/1.0 500 Internal Server Error\n\r\n\r\n", 39);
							} else {
								write(tmp_slot->socket_d, "HTTP/1.0 400 Bad Request\n\r\n\r\n", 29);
							}
						} else {
							if (tmp_slot->verb == 'g') {
								if ((tmp_slot->fd = get_file(tmp_slot->file, 0)) < 0) {
									write(tmp_slot->socket_d, "HTTP/1.0 404 Page Not Found\n\r\n\r\n", 32);
									tmp_slot->status = SLT_FREE;
								} else {
									write(tmp_slot->socket_d, "HTTP/1.0 200 OK\n", 16);
									// next stages send rest of data
									tmp_slot->status = SLT_SEND;
									// beging by starting to send the header first
									goto send_header;
								}
							}
						}
						break;
					case SLT_SEND:		/* send a chunk of the file */
						if (tmp_slot->verb == 'h') {
							send_header:
								write(tmp_slot->socket_d, srv_header, srv_header_len);
							if (tmp_slot->verb == 'h') {
								// this if statement is necessary
								// because we can reach this place of code from a GET command
								// at it's beginning and so the request isn't done handling yet
								goto done_send;
							}
						} else if (tmp_slot->verb == 'g') {
							tmp = read(tmp_slot->fd, buf, buffer_size);
							if (tmp > 0) {
								write(tmp_slot->socket_d, buf, tmp);
							} else if (!tmp) {
								done_send:
									write(tmp_slot->socket_d, "\n\r\n\r\n", 5);
									close(tmp_slot->fd);
									tmp_slot->status = SLT_FREE;
							} else {
								// error
								write(tmp_slot->socket_d, "HTTP/1.0 500 Internal Server Error\n\r\n\r\n", 39);
								goto done_send;
							}
						}
						break;
					case SLT_FREE:		/* free it */
						// close connection
						close(tmp_slot->socket_d);

						// move slot to free_slots
						prev->next = tmp_slot->next;
						tmp_slot->next = free_slots->next;
						free_slots->next = tmp_slot;
						tmp_slot = prev;

						// if (!terminated) {
							// indicate one extra connection slot is available
							++(num_of_places[0]);
						// }
						break;
				}
				prev = tmp_slot;
			}
		}
	}

	// if it exited the loop free all slots reserved
	for (tmp_slot = free_slots->next; tmp_slot != NULL; tmp_slot = tmp_slot->next) {
		free_slots->next = tmp_slot->next;
		free(tmp_slot);
	}
	free(free_slots);
	free(used_slots);

	unshare_memory(&shm);

	printf("Exiting %d\n", getpid());
	exit(0);
}


int parse_request (char* req, char *verb, char **file, char **head) {
	int	i=0,	/* counting index */	\
		s=0,	/* start of header */	\
		fe=0,	/* file end */			\
		len=0;	/* length */

	char	*h,	/* temporary header */	\
			*f;	/* temporary file */

	if ((strncasecmp("GET ", req, 4) == 0)){
		(*verb) = 'g';
		req += 4;
	} else if(strncasecmp("HEADER ", req, 7) == 0) {
		(*verb) = 'h';
		return 0;
	} else {
		return -1;		// invalid verb
	}

	len = strlen(req);

	for (i=0; i<len; i++) {
		if (isblank(req[i])) {
			// check if it's space or tab
			if (!fe) {
				// if file end isn't assigned yet assign it to current place
				fe = i;
			}
		} else if (req[i] == '\n') {
			// end of first line from request
			// HTTP version is ignored for now
			if (!s) {
				s = i+1;
			}
		} else {
			if (i == len-4) {
				// check for validity of the end of the request
				if (!(/*req[i-1] == '\n' &&*/ req[i] == '\r' && req[i+1] == '\n' && req[i+2] == '\r' && req[i+3] == '\n')) {
					return -2;		// invalid end of request
				}
				break;
			}
		}
	}

	f = (char*)malloc(fe*sizeof(char));
	h = (char*)malloc((len-4-s)*sizeof(char));

	if (!(f && h)) {
		return -3;		// memory error
	}

	// more properly it should cast pointers for the size
	// but characters are byte sized so no casting is required
	memcpy(f, (void*)(req), fe);			// copy file name
	memcpy(h, (void*)(req+s), len-4-s);	// copy header

	*file = f;
	*head = h;

	return 0;
}

int get_file(char const* fname, int f) {
	char *fpath;
	char file_path[1024]; //TODO max file name

	if ((fpath = (char*)malloc(strlen(fname)+strlen(files_path)+1)) < 0) {
		return -1;
	} else {
		strcpy(fpath, files_path);
		strcat(fpath, fname);
	}

	if (f){
		// loop on open files and decide which of them
		// if none replace one (or open new) with _get_file()
		int oldest = 0;
		for (size_t i = 0; i < 2014; i++) {
			// if (fcntl(files[i].fd, F_GETPATH, file_path) != -1) { // this works on OSX
			char sympath[1024];
			sprintf(sympath, "/proc/self/fd/%d\0", files[i].fd);

			if (readlink(sympath, file_path, 1024) > 0) {
				if (!(strcmp(fpath, file_path))) {
					files[i].last_used = time(0);
					return dup(files[i].fd);
				} else {
					if (files[i].last_used < files[oldest].last_used) {
						oldest = i;
					}
				}
			} else {
				return -1;
			}
		}

		int tfd;
		if ((tfd = _get_file(fpath)) < 0) {
			return -2;
		} else {
			files[oldest].fd = tfd;
			files[oldest].last_used = time(0);
			return dup(tfd);
		}
	} else {
		return _get_file(fpath);
	}
}

int _get_file(char const* fpath) {
	if (access(fpath, F_OK) != -1) {
    	// file exists
		int fd;
		if ((fd = open(fpath, O_RDONLY)) > 0) {
			return fd;
		} else {
			return -2;
		}
	} else {
	    // file doesn't exist
		return -3;
	}
}
