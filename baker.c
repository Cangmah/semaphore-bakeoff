#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

//resources and ingredients declarations 
sem_t mixer_sem; 
sem_t pantry_sem;
sem_t fridge_sem;
sem_t bowl_sem; 
sem_t spoon_sem; 
sem_t oven_sem; 

//ingredients in pantry
sem_t sugar_sem;
sem_t flour_sem;
sem_t yeast_sem; 
sem_t soda_sem; 
sem_t salt_sem; 
sem_t cinn_sem; 

//ingredients in fridge
sem_t egg_sem; 
sem_t milk_sem; 
sem_t butter_sem;

    //main()
    // 1. get number of bakers from user
    // 2. initialize ALL semaphores 
    // 3. spawn baker threads
    // 4. wait for threads to finish
    // 5. destroy semaphores

    //function for each baker as thread
void *baker(void *arg) {

    return NULL;
}
int main() {
    //get user input for number of bakers 
    int numBakers;
    printf("Enter the number of bakers: ");
    scanf("%d", &numBakers);

    //create list of bakers' threadIDs
    pthread_t threads[numBakers]; 
    //init all semaphores thats been declared using their address
    sem_init(&mixer_sem,  0, 2);
    sem_init(&pantry_sem, 0, 1);
    sem_init(&fridge_sem, 0, 2);
    sem_init(&bowl_sem,   0, 3);
    sem_init(&spoon_sem,  0, 5);
    sem_init(&oven_sem,   0, 1);

    sem_init(&sugar_sem, 0, 1);
    sem_init(&flour_sem, 0, 1);
    sem_init(&yeast_sem, 0, 1);
    sem_init(&soda_sem,  0, 1);
    sem_init(&salt_sem,  0, 1);
    sem_init(&cinn_sem,  0, 1);

    sem_init(&egg_sem,    0, 1);
    sem_init(&milk_sem,   0, 1);
    sem_init(&butter_sem, 0, 1);

    //creating the threads for each baker 
    for(int i = 0; i < numBakers; i++) {
        pthread_create(&threads[i], NULL, baker, NULL );

    }

    //wait for all threads/bakers to complete via join()
    for (int i = 0; i< numBakers; i++) {
        pthread_join(threads[i], NULL);
    }

    //destroy all the semaphores after threads are complete
    sem_destroy(&mixer_sem);
    sem_destroy(&pantry_sem);
    sem_destroy(&fridge_sem);
    sem_destroy(&bowl_sem);
    sem_destroy(&spoon_sem);
    sem_destroy(&oven_sem);

    sem_destroy(&sugar_sem);
    sem_destroy(&flour_sem);
    sem_destroy(&yeast_sem);
    sem_destroy(&soda_sem);
    sem_destroy(&salt_sem);
    sem_destroy(&cinn_sem);

    sem_destroy(&egg_sem);
    sem_destroy(&milk_sem);
    sem_destroy(&butter_sem);
}