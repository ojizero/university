/**
The YAHS/BIHS project was done by:
	** Ameer Alkam  -- 1120217
	** Ali Alnubani -- 1120130
	** Weam Mikkawi -- 1120442
Supervidsed by: Dr. Ahmed Afaneh
**/


#ifndef __IPC_HEADER__
#define __IPC_HEADER__
	#include <stdlib.h>
	#include <sys/ipc.h>
	#include <sys/shm.h>
	#include <sys/sem.h>
	#include <sys/socket.h>
	#include <netinet/in.h>

	/**
		defines a simpler interface to handle shared memory and the creation of sockets
	**/

	// a data structure to pass for shared memory functions
	typedef struct {
		int shm_id;			// shared memory ID
		void* shm_base;		// shared memory base
	} shm_t;

	/* wrapper functions */
	int _req_sharedmem(int, int);		// encapsulates shmget()
	void* _map_sharedmem(int, void*);	// encapsulates shmat()
	int _det_sharedmem(void*);			// encapsulates shmdt()


	/* usable shared memory interface */
	shm_t make_shared(int, int);		// creates a shared memory space
					/*
						int:size, int:key
							- if key is zero, it is automatically set base on current directory
					*/
	int unshare_memory(shm_t*);			// removes a shared memory space
					/*
						shm_t*:shared memory to remove
					*/

	/* wrapper and interface for creating listening server sockets */
	int create_socket(struct sockaddr_in*, int, int);
					/*
						sockaddr_in*: server address, int:q, int:nonblock_flag
							- q is the ques size for listen
							- nonblock_flag if nonzero, server socket is nonblocking
								- gives an error instead of waiting on accept
					*/
#endif
