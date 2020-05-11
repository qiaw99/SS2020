#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

char on_bridge;
int num_threads;
int accident = 0;

int *tickets;
char *enter;

// lock aus der VL
int lock(long tid){
    int i, max = 0;
    enter[tid] = 1;

    for (i = 0; i < num_threads; i++) {
        if (max < tickets[i])
            max = tickets[i];
    }
    tickets[tid] = max + 1;
    enter[tid] = 0;
    for (i = 0; i < num_threads; i++) {
        if (i != tid) {
            while (enter[i]);
        }
        while ((tickets[i] != 0) && 
          ((tickets[tid] > tickets[i]) ||
            ((tickets[tid] == tickets[i]) &&
              (tid > i))));
    }
	return 0;
}

int unlock(long tid){
    tickets[tid] = 0;  
    return 0;
}

void *cross_bridge(void *threadid){
    long tid = (long)threadid;
    
    for(int i = 0; i < 10000; i++){
        lock(tid);

        if(on_bridge == 1){
            printf("Unfall!\n");
            accident++;
        }else{
            printf("Kein Problem!\n");
        }

    	on_bridge = 1;
        
        // Zeit auf der Brücke simulieren   
        sleep(0.0000000000000000000000000001);

        on_bridge = 0;
        unlock(tid);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    int rc;
    long t;
    
    // Anfrage zur Anzahl von Threads
    printf("Anzahl von Threads?\n");
    scanf ("%d",&num_threads);

    pthread_t threads[num_threads];
    
    // memory reallocation
    tickets = realloc(tickets, num_threads * sizeof(int));
    enter = realloc(enter, num_threads * sizeof(char));

    // 0 bedeutet, dass kein Auto auf der Brücke ist.
    on_bridge = 0;

    //creating threads
    for(t = 0; t < num_threads; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, cross_bridge, (void*)t);
        if(rc){
            printf("Error; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // joining threads
    for(t = 0; t < num_threads; t++){
        pthread_join(threads[t], NULL);
    }
    printf("accident %d\n", accident);
    
    free(tickets);
    free(enter);

    pthread_exit(NULL); 
}
