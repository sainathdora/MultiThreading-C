#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //for working with posix threads
#include <unistd.h>  //pause the thread

static void *thread_fn_callback(void *arg)
{
    // (void *)input_data
    // this input data is avaiable to use as void *arg
    char *data = (char *)arg;
    while (1)
    {
        printf("%s\n", data);
        sleep(1);
    }
}

void thread1_create()
{
    pthread_t pthread1; // inbuilt DS that defined, dont worry about internal defintion
    // of pthread_t
    // pthread_t = thread handle/thead name
    static char *input_data = "I am thread1";
    int rc = pthread_create(&pthread1, NULL, thread_fn_callback, (void *)input_data); // this is a fork point
    // (void *)input_data this must be on either be a heap or static(data segment), it must not be on stack
    // reseaon being by the time the pthread1 is created the thead1_create() would have been completed and all
    // local variables are deleted and we are referencing something that doesn't exist
    // rc == 0 (on success) else rc<0
    // thread_fn_callback() is the function that the new thread is gonna execute(these functions must return void*, arg is also
    // void *)
}

int main(int argc, char **argv)
{
    thread1_create();
    printf("Main thread paused\n");
    static char *t = "Hello world";
    printf("%p\n", (void *)t);
    pause(); // pause the main thread to prevent deletion of child threads we created
}

// The -g flag is the "Debug" argument.
// When you include it in your gcc or g++ command, you are telling the compiler to store
// Debugging Information inside the resulting executable file.

// Without -g, the computer only sees machine code (binary).
//  With -g, the computer "remembers" how that binary relates back to your original C source code.

// gcc -g -c HelloWorld.c -lpthread -o main