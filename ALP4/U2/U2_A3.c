#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int *sides;
char on_bridge;
int num_threads;

int *tickets;
char *enter;

int lock(long tid){
    int i, max = 0;
    enter[tid] = 1;
    on_bridge = 1;

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
        while ((tickets[i] != 0) && (on_bridge == 1) &&
          ((tickets[tid] > tickets[i]) ||
            ((tickets[tid] == tickets[i]) &&
              (tid > i))));
    }
}

int unlock(long tid){
    tickets[tid] = 0;
    on_bridge = 0;
    return 0;
}

void *cross_bridge(void *threadid){
    long tid = (long)threadid;
    
    for(int i = 0; i < 10000; i++){
        if(on_bridge == 1){
            printf("Unfall!\n");
        }else{
            printf("Kein Problem!\n");
        }

        lock(tid);

        // Zeit auf der Brücke simulieren   
        sleep(1);

        // Auf der anderen Seite erreichen
        if(sides[tid] == 0){
            sides[tid] = 1;
        }else{
            sides[tid] = 0;
        }

        printf("I am thread %ld and to the %d side\n", tid, sides[tid]);
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
    
    // initialize the array
    sides = (int *)malloc(sizeof(int) * num_threads);

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
    
    free(tickets);
    free(enter);

    pthread_exit(NULL); 
}
