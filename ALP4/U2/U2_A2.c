#include<stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

int sides[NUM_THREADS];
int favoured = 1;
char on_bridge;
char _lock[2];
int accident = 0;

int lock(long tid){
	_lock[tid] = 1;
	while(_lock[NUM_THREADS - tid - 1]){
		if(favoured != tid){
			_lock[tid] = 0;
			while(favoured != tid);
			_lock[tid] = 1;
		}
	}
	return 0;
}

int unlock(long tid){
	favoured = NUM_THREADS - 1 - tid;
	_lock[tid] = 0;
	return 0;
}

void *cross_bridge(void *threadid){
	long tid = (long)threadid;

	for(int i = 0; i < 10000; i++){
		lock(tid);

		// kontrollieren, ob es auf der Brücke Auto gibt.
		if(on_bridge == 1){
			printf("Unfall!\n");
      accident++;
		}else{
			printf("Kein Problem!\n");
		}

		// Fahren auf der Brücke
		on_bridge = 1;

		// Zeit auf der Brücke simulieren	
		//sleep(1);

		// Auf der anderen Seite erreichen
		sides[tid] = NUM_THREADS - tid - 1;
    
		on_bridge = 0;
    //printf("I am thread %ld and to the %d side\n", tid, sides[tid]);
		
		unlock(tid);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv){
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	// initialize the array
	sides[0] = 0;
	sides[1] = 1;
	_lock[0] = 0;
	_lock[1] = 0;

	// 0 bedeutet, dass kein Auto auf der Brücke ist.
	on_bridge = 0;

	//creating threads
	for(t = 0; t < NUM_THREADS; t++){
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, cross_bridge, (void*)t);
		if(rc){
			printf("Error; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// joining threads
	for(t = 0; t < NUM_THREADS; t++){
		pthread_join(threads[t], NULL);
	}
  printf("accident: %d \n", accident);
	pthread_exit(NULL);
}
