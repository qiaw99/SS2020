#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct timespec max_timeout;

int last = 0;
int *buffer;
int num_con, num_pro, num_place, num_to_take;

// to store the sequece of products in which products are put into buffer
int *record;

// default initialize mutex and conditional varibales
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t is_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t is_full = PTHREAD_COND_INITIALIZER;

void put(void *threadid, int i){
	pthread_mutex_lock(&mutex);
	
	while (last == num_place){
		pthread_cond_wait(&is_full, &mutex);
	}
	
	/* 
		we have only one chance each time to put product into the buffer,
    	with this flag, we can determine, 
		whether we have already put things into the buffer.
    */
	int flag = 0;
	for(int index = 0; index < num_place; index ++){
		// the bigger the nummer, the longer the product has been put in the buffer
		if(record[index] != 0){
			record[index] ++;
		}

		// if index is equal 0, it means that this place is empty
		if(record[index] == 0 && flag == 0){
			record[index] = 1;
			buffer[index] = i;
			flag = 1;
			printf("Producer %ld puts %d at %d \n", (long)threadid, buffer[index], index);
		}
	}

	last++;
	
	// release the signal
	pthread_cond_broadcast(&is_empty);
	
	pthread_mutex_unlock(&mutex);
}

int search_biggest_position(){
	int temp_pos = 0;
	int temp_max = record[0];

	for(int i = 0; i < num_place; i++){
		if(temp_max <= record[i]){
			temp_pos = i;
			temp_max = record[i];
		}
	}
	return temp_pos;
}

int take(void *threadid){
	int pos = 0;
	pthread_mutex_lock(&mutex);
	
	while (last < num_to_take){
		if (pthread_cond_timedwait(&is_empty, &mutex, &max_timeout) != 0){
			printf("Consumer %ld Timedout\n", (long)threadid);
			pthread_mutex_unlock(&mutex);
			return -1;
		}
	}

	for(int i = 0; i < num_to_take; i++){	
		pos = search_biggest_position();
		record[pos] = 0;

		printf("Consumer %ld takes %d from %d\n", (long)threadid, buffer[pos], pos);
		last--;

		pthread_cond_broadcast(&is_full);
	}
	pthread_mutex_unlock(&mutex);
	return 0;
}

void *producer(void *threadid){
	for (int i = 0; i < 1000; i++){
		put(threadid, i);
	}
	pthread_exit(NULL);
}

void *consumer(void *threadid){
	while (1){
		if (take(threadid) != 0){
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

	printf("How many products to take each time?\n");
	scanf("%d", &num_to_take);

	// check inputs
	if (num_con < 0 || num_pro < 0 || num_to_take < 0){
		printf("Input can't be negative!\n");
		exit(-1);
	} else if(num_to_take > num_place) {
		printf("Can't take so much products!\n");
		exit(-1);
	}

	// allocate the buffer
	buffer = (int *)malloc(sizeof(int) * num_place);
	record = (int *)malloc(sizeof(int) * num_place);

	// initialize threads 
	pthread_t consumers[num_con];
	pthread_t producers[num_pro];

	// initialize the buffer with 0 
	for (t = 0; t < num_place; t++){
		buffer[t] = 0;
		record[t] = 0;
	}

	last = 0;

	// create threads
	for (t = 0; t < num_con; t ++){
		rc = pthread_create(&consumers[t], NULL, consumer, (void *)t);
		if (rc){
			printf("ERROR: return code from pthread_create () is %d\n", rc);
			exit(-1);
		}
	}

	for (t = 0; t < num_pro; t ++){
		rc = pthread_create(&producers[t], NULL, producer, (void *)t);
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
	pthread_cond_destroy(&is_empty);
	pthread_cond_destroy(&is_full);

	// release the mutex
	pthread_mutex_destroy(&mutex);

	pthread_exit(NULL);
}
