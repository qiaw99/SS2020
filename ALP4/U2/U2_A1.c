#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2

char on_bridge;
int accident = 0;

void *cross_bridge(void *threadid){
	long tid = (long)threadid;

	for(int i = 0; i < 10000; i++){
		if(on_bridge == 1){
			printf("Unfall!\n");
			accident++;
		}else{
			printf("Kein Problem!\n");
		}

		// Fahren auf der Brücke
		on_bridge = 1;

		// Zeit auf der Brücke simulieren	
		sleep(0.00000000000001);

		on_bridge = 0;

	}

	pthread_exit(NULL);
}

int main(int argc, char *argv){
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

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

	printf("Accident: %d\n", accident);

	pthread_exit(NULL);
}
