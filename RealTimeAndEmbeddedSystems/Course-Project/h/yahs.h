/**
The YAHS/BIHS project was done by:
	** Ameer Alkam  -- 1120217
	** Ali Alnubani -- 1120130
	** Weam Mikkawi -- 1120442
Supervidsed by: Dr. Ahmed Afaneh
**/


#ifndef __YAHS__
#define __YAHS__
	#include <stdio.h>
	#include <fcntl.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <signal.h>
	#include <string.h>
	#include <sys/types.h>

	/**
		defines the functions and structures for the workersa and the main server
	**/


	#include "ipc_wrapper.h"

	// global flag controlled by system signals
	/*
		0 : no terminate status
		1 : server terminating
		2 : server restarting
	*/
	int terminated = 0;

	/***
		Main server structures and functions
	***/

	// a data structure to keep information regarding the workers
	typedef struct {
		int id;		// it's ID
		shm_t shm;	// SHM assigned to it
	} worker_info_t;

	// server default configurations
	#define DEFAULT_PORT_NUMBER 80
	#define DEFAULT_NUMBER_WORKERS 4
	#define DEFAULT_CONN_PER_WORKER 512
	#define DEFAULT_BUFFER_SIZE 1024
	#define config_lines 1024
	#define DEFAULT_MAX_REQUEST 2048
	#define DEFAULT_SRV_HEADER "Server: YAHS/BIHS\nContent-Type: text/html; charset=UTF-8\nConnection: close\n"
	#define DEFAULT_SERVER_PATH "./server_root\0"

	#define MAX_PATH 1000


	// the configuration file path
	char config_file[MAX_PATH];

	// server active configuration
	int server_port = DEFAULT_PORT_NUMBER;
	int number_of_workers = DEFAULT_NUMBER_WORKERS;
	int conn_per_worker = DEFAULT_CONN_PER_WORKER;
	int buffer_size = DEFAULT_BUFFER_SIZE;
	char files_path[MAX_PATH] = DEFAULT_SERVER_PATH;
	int max_req = DEFAULT_MAX_REQUEST;
	char *srv_header = DEFAULT_SRV_HEADER;
	int srv_header_len = 75;


	int srv_socket;					// maine server socket
	struct sockaddr_in srv_adrs;	// main server address
	worker_info_t *workers;			// array of workers info

	// Main server functions
	int config(const char*);	// read, parse and apply configuration file
	void init_sem();			// configures server, creates socket and creates workers
	void init_socket();
	void init_workers();
	void init_signlas();
	void cli();					// simple command line interface for the main server

	// workers manipulations/communications
	void create_workers(int, worker_info_t*);	// creates a number of workers specified by input integer
	void create_new_worker(worker_info_t*);		// creates a worker and builds a shared memory between it and parent

	// string manipulation function
	int find_char(char const*, const char);		// returns the index of a character if found in a string

	// system signals overides
	void *sig_reconfig(int);
	void *sig_terminate(int);
	void *sig_child_term(int);


	/* --------------------------------------------- */
	/* --------------------------------------------- */
	/* --------------------------------------------- */


	/***
		Worker data structures and functions
	***/

	// data structures to manage the connections at the worker side

	// to manage the connections with the clients
	typedef enum {
		SLT_FREE, SLT_SEND, SLT_RECV, SLT_HNDL
	} stats_e;			// the stats a connection may be at

	// connection 'slots' used to organized communications
	typedef struct {
		unsigned int socket_d;
		struct sockaddr_in* cli_adrs;
		stats_e status;

		char *request; //[max_req];
		char verb;
		char *file;
		char *head;

		int fd;

		struct connection_t* next;
	} connection_t;		// information regarding the connections

	// lists for connections used and free
	connection_t *free_slots, *used_slots;

	void start_worker();		// workers main loop

	int parse_request(char*, char*, char**, char**);	// takes a request, returns the verb in it, the file requested, and the header sent -> returns negative number on errors
	int get_file(char const*, int);						// if integer input is 0 -> calls _get_file()
	int _get_file(char const*);							// primitive file getter -> if file exists try open it -> returns negative for errors or file descriptor



	// TODO if time permits -- simple files cache
	/* --------------- */
	/* --------------- */
	typedef struct {
		int fd;
		long last_used;
		short used;
	} filed_t;
	filed_t files[2014/*TODO set it to config*/];
	/* --------------- */
	/* --------------- */
#endif
