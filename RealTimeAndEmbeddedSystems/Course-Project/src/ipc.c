/**
The YAHS/BIHS project was done by:
	** Ameer Alkam  -- 1120217
	** Ali Alnubani -- 1120130
	** Weam Mikkawi -- 1120442
Supervidsed by: Dr. Ahmed Afaneh
**/


#include "ipc_wrapper.h"

/**
	implementation of the shared memory and socket creation interfaces and wrappers
**/


int _get_sharedmem(int size, int key) {
	if (!key) {
		// if no key is given get one using ftok() on current directory
		key = ftok("./", 0);
	}

	int shmid;
	if ((shmid = shmget(key, size, 0666|IPC_CREAT)) < 0) {
		perror("Existing with error code from shmget() ...\n");
		exit(shmid);
	}

	return shmid;
}

void* _map_sharedmem(int shmid, void* base) {
	/*
		if base input is NULL then system decides what value to use for base address
	*/

	void* _base;

	if ((_base = shmat(shmid, base, 0)) < 0) {
		perror("Existing with error code from shmat() ...\n");
		exit(_base);
	}

	return _base;
}

int _det_sharedmem(void* adrs){
	int s;

	if ((s = shmdt(adrs)) < 0) {
		perror("Existing with error code from shmdt() ...\n");
		exit(s);
	}

	return shmdt(adrs);
}


shm_t make_shared(int size, int key) {
	shm_t ret;

	ret.shm_id = _get_sharedmem(size, key);
	ret.shm_base = _map_sharedmem(ret.shm_id, NULL);

	return ret;
}

int unshare_memory(shm_t* shm) {
	int ret = _det_sharedmem(shm->shm_base);

	shm->shm_id = -1;
	shm->shm_base = NULL;

	return ret;
}

int create_socket(struct sockaddr_in *adrs, int q, int nonblock) {
	int sd;

	// if nonblock is nonzero then set f to SOCK_NONBLOCK else set it to zero
	int f = (nonblock)?(SOCK_NONBLOCK):(0);
	// ORing it with SOCK_STREAM sets the server socket flag
	if ((sd = socket(AF_INET, SOCK_STREAM | f, 0)) < 0) {
		perror("Existing with error code from socket() ...\n");
		exit(-154);
	}

	// if it either bind() or listen() give errors return zerom, else return socket
	return ((!bind(sd, adrs, sizeof((*adrs)))) && (!listen(sd, q)))?(sd):(0);
}
