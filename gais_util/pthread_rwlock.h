#ifndef H_PTHREAD_RWLOCK
#define H_PTHREAD_RWLOCK

typedef struct sPThreadRWLock {
	int		iReader;
	pthread_mutex_t mutex;
	pthread_cond_t lock_free;
} SPThreadRWLock;

#endif