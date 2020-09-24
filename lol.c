
#include <stdio.h>
#include <assert.h>
#include <pthread.h>


void mythread( void *arg){
	printf(" %s ", (char *) arg);
}

int main(int argc, char *argv[]){
//initialising the threads
	pthread_t t1, t2;

	printf("BEGIN\n");
	int m;


//creating threads
	m = pthread_create(&t1, NULL, mythread, "A");
	assert(m==0);

	m = pthread_create(&t2, NULL, mythread, "B");
	assert(m==0);


//waiting
	m = pthread_join(t1, NULL);
	assert(m==0);

	m = pthread_join(t2, NULL);
	assert(m==0);


	printf("END\n");

	return 0;
}