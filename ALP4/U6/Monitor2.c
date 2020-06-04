#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct timespec max_timeout;

int round = 0;

int num_con, num_pro, num_place;

struct monitors{
	pthread_mutex_t mutex;
	pthread_cond_t is_empty;
	pthread_cond_t is_full;
	int last;
	int *buffer;
	void *threadid;
	int *record;
};

void put(struct monitors *monitor, int i){
	pthread_mutex_lock(&monitor -> mutex);
	
	while (monitor -> last == num_place){
		pthread_cond_wait(&monitor -> is_full, &monitor -> mutex);
	}
	
	/* 
		we have only one chance each time to put product into the buffer,
    	with this flag, we can determine, 
		whether we have already put things into the buffer.
    */
	int flag = 0;
	for(int index = 0; index < num_place; index ++){
		// the bigger the nummer, the longer the product has been put in the buffer
		if(monitor -> record[index] != 0){
			monitor -> record[index] ++;
		}

		// if index is equal 0, it means that this place is empty
		if(monitor -> record[index] == 0 && flag == 0){
			monitor -> record[index] = 1;
			monitor -> buffer[index] = i;
			flag = 1;
			printf("Producer puts %d at %d, round %d \n", monitor -> buffer[index], index, round);
		}
	}
	round ++;
	monitor -> last++;
	
	// release the signal
	pthread_cond_broadcast(&monitor -> is_empty);
	
	pthread_mutex_unlock(&monitor -> mutex);
}

int search_biggest_position(struct monitors *monitor){
	int temp_pos = 0;
	int temp_max = monitor -> record[0];

	for(int i = 0; i < num_place; i++){
		if(temp_max < monitor -> record[i]){
			temp_pos = i;
			temp_max = monitor -> record[i];
		}
	}
	return temp_pos;
}

int take(struct monitors *monitor){
	pthread_mutex_lock(&monitor -> mutex);

	while (monitor -> last == 0){
		if (pthread_cond_timedwait(&monitor -> is_empty, &monitor -> mutex, &max_timeout) != 0){
			printf("Consumer Timedout, round %d\n", round);
			pthread_mutex_unlock(&monitor -> mutex);
			return -1;
		}
	}

	int pos = search_biggest_position(monitor);
	monitor -> record[pos] = 0;

	printf("Consumer takes %d from %d, round %d\n", monitor -> buffer[pos], pos, round);
	
	monitor -> last--;
	round ++;

	pthread_cond_broadcast(&monitor -> is_full);
	
	pthread_mutex_unlock(&monitor -> mutex);
	return 0;
}

void *producer(struct monitors *monitor){
	for (int i = 0; i < 1000; i++){
		put(monitor, i);
	}
	pthread_exit(NULL);
}

void *consumer(struct monitors *monitor){
	while (1){
		if (take(monitor) != 0){
			break;
		}
	}
	pthread_exit(NULL);
}

int main(void){
	int rc;
	long t;

	clock_gettime(CLOCK_REALTIME, &max_timeout);
	max_timeout.tv_sec += 5;

	printf("How many consumers?\n");
	scanf("%d", &num_con);

	printf("How many producers?\n");
	scanf("%d", &num_pro);

	printf("How many places?\n");
	scanf("%d", &num_place);

	// check inputs
	if (num_con < 0 || num_pro < 0){
		printf("Input can't be negative!\n");
		exit(0);
	}

	// initialize threads 
	pthread_t consumers[num_con];
	pthread_t producers[num_pro];

	// initialize the struct
	struct monitors *monitor = malloc(sizeof(struct monitors));
    monitor -> buffer = calloc(num_place, sizeof(int));
    monitor -> record = calloc(num_place, sizeof(int));
    monitor -> last = 0;
    pthread_mutex_init(&monitor -> mutex, NULL);
    pthread_cond_init(&monitor -> is_full, NULL);
    pthread_cond_init(&monitor -> is_empty, NULL);

	// create threads
	for (t = 0; t < num_con; t ++){
		rc = pthread_create(&consumers[t], NULL, consumer, (void *)monitor);
		if (rc){
			printf("ERROR: return code from pthread_create () is %d\n", rc);
			exit(-1);
		}
	}

	for (t = 0; t < num_pro; t ++){
		rc = pthread_create(&producers[t], NULL, producer, (void *)monitor);
		if (rc){
			printf("ERROR: return code from pthread_create () is %d\n", rc);
			exit(-1);
		}
	}

	// joining consumer threads
	for (t = 0; t < num_con; t ++){
		pthread_join(consumers[t], NULL);
	}

	// joining producer threads
	for (t = 0; t < num_pro; t ++){
		pthread_join(producers[t], NULL);
	}

	// release condition variables
    pthread_cond_destroy(&monitor -> is_empty);
    pthread_cond_destroy(&monitor -> is_full);

    // release the mutex
    pthread_mutex_destroy(&monitor -> mutex);

    // free memory
    free(monitor -> buffer);
    free(monitor -> record);
    free(monitor);

	pthread_exit(NULL);
}
