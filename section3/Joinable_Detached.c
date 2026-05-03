#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> //pause + sleep

void *thread_fn_callback(void *arg)
{
    int input_data = *((int *)arg);
    // since *arg is a dynamically allocated, so we delete it we have extracted it's contents;
    free(arg);
    int rc = 0;
    while (rc != input_data)
    {
        printf("Thread - %d doing some work\n", input_data);
        sleep(1);
        rc++;
    }
    // now square the input data and store it in heap, as thread deletion will delete it's stack
    int result = input_data * input_data;
    int *result_ptr = (int *)malloc(sizeof(int));
    *result_ptr = result;
    return (void *)result_ptr;
}

void thread_create(pthread_t *thread_add, int threadid)
{
    pthread_attr_t attr;
    pthread_attr_init(&attr); // initialize attr object with default posix values/implementation
    int *input_data = calloc(1, sizeof(int));
    *input_data = threadid;
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    // pthread_attr_setdetachstate => api provided by POSIX to determine
    // if the thread we gonna create a joinable or detached
    // PTHREAD_CREATE_DETACHED is the name you choose if you want to create detached value
    int rc = pthread_create(thread_add, &attr, thread_fn_callback, (void *)input_data);
    if (rc == 0)
    {
        printf("Successfully created thread - %d\n", threadid);
    }
}

pthread_t pthread2,
    pthread3;

int main(int argc, char **argv)
{
    printf("Hello\n");
    thread_create(&pthread2, 2);
    thread_create(&pthread3, 5);
    void *thread_result_2;
    void *thread_result_3;
    pthread_join(pthread2, &thread_result_2);
    printf("Thread-2 has finished it's execution\nResult = %d\n", *(int *)thread_result_2);
    pthread_join(pthread3, &thread_result_3);
    printf("Thread-3 has finished it's execution\nResult = %d\n", *(int *)thread_result_3);
    return 0;
}