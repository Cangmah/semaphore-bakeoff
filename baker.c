#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

//global variables for all the semaphores 
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
//create a struct to store 5 recipes for baker() to use
//each recipes points to a pointer for array of ingredients
typedef struct {
    char *name; //name of recipe
    sem_t **ingredients; //array of pointers to the semaphores/ingredients
    int *inFridge; //pointer to location of each ingredients, 1 = fridge, 0=pantry
    char **ingredientNames; //pointer to pointer of strings of ingredients names
    int numIngredients; 
} Recipe;

//create struct instances of the 5 recipes and their ingredients using array pointer for ingredients
//create int array to identify the location of each ingredients, 0 = pantry, 1 = fridge
//create char array pointer to store names of ingredients 
sem_t *cookieIngredients[] = {&flour_sem, &sugar_sem, &milk_sem, &butter_sem};
int cookieLocation[] = {0, 0, 1, 1}; //index relative to ingredients' list
char *cookieNames[] = {"Flour", "Sugar", "Milk", "Butter"}; //index relative to ingredient list
Recipe cookies = {"Cookies", cookieIngredients, cookieLocation, cookieNames, 4}; //Recipe struct instance 

sem_t *pancakeIngredients[] = {&flour_sem, &sugar_sem, &soda_sem, &salt_sem, &egg_sem, &milk_sem, &butter_sem};
int pancakeLocation[] = {0, 0, 0, 0, 1, 1, 1};
char *pancakeNames[]= {"Flour", "Sugar", "Baking Soda", "Salt", "Egg", "Milk", "Butter"};
Recipe pancakes = {"Pancakes", pancakeIngredients, pancakeLocation, pancakeNames, 7};

sem_t *pizzaIngredients[] = {&yeast_sem, &sugar_sem, &salt_sem};
int pizzaLocation[] = {0, 0, 0};
char *pizzaNames[] = {"Yeast", "Sugar", "Salt"};
Recipe pizza = {"Pizza Dough", pizzaIngredients, pizzaLocation, pizzaNames, 3};

sem_t *pretzelIngredients[] = {&flour_sem, &sugar_sem, &salt_sem, &yeast_sem, &soda_sem, &egg_sem};
int pretzelLocation[] = {0, 0, 0, 0, 0, 1};
char *pretzelNames[]= {"Flour", "Sugar", "Salt", "Yeast", "Baking Soda", "Egg"};
Recipe pretzel = {"Soft Pretzels", pretzelIngredients, pretzelLocation, pretzelNames, 6};

sem_t *cinnRollIngredients[] = {&flour_sem, &sugar_sem, &salt_sem, &butter_sem, &egg_sem, &cinn_sem};
int cinnLocation[] = {0, 0, 0, 1, 1, 0};
char *cinnNames[] = {"Flour", "Sugar", "Salt", "Butter", "Egg", "Cinnamon"};
Recipe cinnRoll = {"Cinnamon Rolls", cinnRollIngredients, cinnLocation, cinnNames, 6};

    //function for each baker as thread
void *baker(void *arg) {
    //cast and dereference int pointer argument to get the value of id
    int id = *(int *)arg; 
    free(arg); 

    //declare the array of the 5 recipes
    Recipe recipes[] = {cookies, pancakes, pizza, pretzel, cinnRoll};

    //loop through each recipes to bake 
    for (int i = 0; i < 5; i++) {
        restart: //restart this current recipe if Ramsied
        printf("Baker %d is making %s\n", id, recipes[i].name);
        //loop through each ingredient in a recipe, get the ingredient and release 
        for (int j = 0; j < recipes[i].numIngredients; j++)
        {
            //check if ingredient is in fridge or pantry, store the value in *room and wait()
            sem_t *room = (recipes[i].inFridge[j] == 1) ? &fridge_sem : &pantry_sem;
            sem_wait(room);
            //then grab/wait() the ingredient from the struct Recipe struct pointer to ingredients
            sem_wait(recipes[i].ingredients[j]);
            //print progress
            printf("Baker %d is grabbing %s\n", id, recipes[i].ingredientNames[j]);
            //release the ingredient and its location
            sem_post(recipes[i].ingredients[j]);
            sem_post(room); //release the room
        }

        //get the bowl, spoon, and mixer
        sem_wait(&bowl_sem);
        printf("Baker %d grabs the bowl\n", id);
        sem_wait(&spoon_sem);
        printf("Baker %d grabs the spoon\n", id);
        sem_wait(&mixer_sem);
        printf("Baker %d grabs the mixer\n", id);
        printf("Baker %d is now mixing %s\n", id, recipes[i].name); 

        //check if baker 0 will get Ramsied or not (1 or 0)
        //if Ramsied(1), then reset recipe without going to oven
        if(id == 0 && rand() %2 ==1) {
            printf("Baker 0 got Ramsied! Restarting %s recipe now!\n", recipes[i].name);
            sem_post(&mixer_sem);
            sem_post(&spoon_sem);
            sem_post(&bowl_sem);
            goto restart;
            
        }
        //release the bowl, spoon and mixer 
        sem_post(&mixer_sem);
        sem_post(&spoon_sem);
        sem_post(&bowl_sem);

        //oven thruuuuu
        sem_wait(&oven_sem);
        printf("Baker %d throws %s in the oven!\n", id, recipes[i].name);
        sem_post(&oven_sem); //release oven

        //this recipe finished
        printf("Baker %d has finished baking %s!\n", id, recipes[i].name);
    }

    //finished all recipes by baker
    printf("Baker %d has completed all recipes!\n", id);

    return NULL;
}
int main() {
    //get user input for number of bakers 
    int numBakers;
    printf("Enter the number of bakers: ");
    scanf("%d", &numBakers);

    //generate randomness for a baker to be Ramsied using time
    srand(time(NULL)); 

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
        //create unique id for each thread to identify by baker()
        int *id = malloc(sizeof(int)); //allocate memory for each id
        *id = i; //store current baker value to id 
        //create thread for each baker from list, and passed baker's id pointer to baker() func
        pthread_create(&threads[i], NULL, baker, id );
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