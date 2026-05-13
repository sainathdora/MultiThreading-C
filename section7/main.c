#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define N_SLAVES 5

pthread_t slaves[N_SLAVES];

// clean up functions
void memory_cleanup_handler(void *arg)
{
    printf("%s is invoked\n", __FUNCTION__);
    free(arg);
}
void close_file_cleanup_handler(void *arg)
{
    printf("%s is invoked\n", __FUNCTION__);
    free((FILE *)arg);
}

void *
write_to_file_fn(void *arg)
{

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); // deffered cancellation(99% use this)
    int *thread_id = (int *)arg;
    // add the functions(cleanup) to thread cancellation stack;
    pthread_cleanup_push(memory_cleanup_handler, arg);

    char file_name[50];
    char string_to_write[100];
    sprintf(file_name, "thread_%d.txt", *thread_id);
    FILE *fptr = fopen(file_name, "w");
    if (!fptr)
    {
        printf("%s\n", strerror(errno));
        // instead of returning use pthread_exit() instead of return
        // return NULL;
        pthread_exit(NULL);
    }

    pthread_cleanup_push(close_file_cleanup_handler, fptr);
    while (1)
    {
        int no_of_bytes = sprintf(string_to_write, "I am Thread-%d\n", *thread_id);
        fwrite(string_to_write, 1, no_of_bytes, fptr);
        fflush(fptr);
        sleep(1);
        // If cancel happens then cancel after sleep(1)
        // check if there is a cancel signal pending, if yes then cancel;
        pthread_testcancel(); // check if any cancellation signal pending then cancel if yes.
    }
    pthread_exit(NULL); // use pthread_exit() to invoke clean up functions (or) use pthread_cleanup_pop(1)
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}

int main()
{
    printf("Hello\n");
    for (int i = 0; i < N_SLAVES; i++)
    {
        int *p = malloc(sizeof(int));
        // C doesnt have reference variables like C++ does
        *p = i;
        pthread_create(&slaves[i], NULL, write_to_file_fn, (void *)p);
    }
    int choice, thread_id;
    while (1)
    {
        printf("Enter your choice(1 - exit): ");
        scanf("%d", &choice);
        if (choice == 1)
        {
            break;
        }
        printf("Enter thread_id: [0, %d]\n", N_SLAVES - 1);
        scanf("%d", &thread_id);
        printf("Entered thread_id = %d\n", thread_id);
        if (thread_id >= N_SLAVES || thread_id < 0)
        {
            printf("Error thread id\n");
            exit(0);
        }
        pthread_cancel(slaves[thread_id]);
    }
    return 0;
}