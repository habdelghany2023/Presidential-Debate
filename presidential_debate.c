#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

//goal: to recieve 200 callers and connect them to one of the operators so they can propose a question to the candidates. Once question is proposed, ends call and moves to
//the next caller
//param vargp: the 4th argument passed in when pthread_create is called
//return: Nothing
void* phonecall(void* vargv);

//goal: to sleep for the time inputted as the command line argument
//param vargp: the 4th arugement passed in when pthread_create is called
//return nothing
void* timer(void* vargp);

//intializing global variables
static sem_t connected_lock;
static sem_t operators;
static sem_t nextId_lock;
static int NUM_OPERATORS = 2;
static int NUM_LINES = 5;
static int connected = 0;
//used to set a callers id
static int next_id = 0;
int main(int argc, char **argv) {
  //checks if no number was passed into the command line as an argument
  if(argc == 1){
    printf("Provide a time for the debate\n");
    return 10;
  }
  //initializes semaphores
  sem_init(&operators, 0, NUM_OPERATORS);
  sem_init(&nextId_lock, 0, 1);
  sem_init(&connected_lock, 0, 1);
  //converts the string passed as a command line argument to an integer
  int sleepNum = atoi(argv[1]);
  //creates a unique thread id to be used for the timer thread function
  pthread_t tid1;
  //for loop to simulate 200 callers
  for(int i = 0; i < 200; i++){
    //creates a unique thread id to be used for the phonecall thread function
    pthread_t tid;
    //creates a new thread executing phonecall with NULL as its sole argument
    pthread_create(&tid, NULL, phonecall, NULL);
  }
  //creates a new thread executing timer with sleepNum as its sole argument
  pthread_create(&tid1, NULL, timer, (void*) &sleepNum);
  //waits for the timer thread to terminate
  pthread_join(tid1, NULL);
  //destroy's the semaphores
  sem_destroy(&operators);
  sem_destroy(&connected_lock);
  sem_destroy(&nextId_lock);
  //exits main terminating all remaining threads with it 
  exit(0);
}

//to sleep for the time inputted as the command line argument
void* timer(void* vargp){
  sleep(*(int*) vargp);
}

//to recieve 200 callers and connect them to one of the operators so they can propose a question to the candidates. Once question is proposed, ends call and moves to
//the next caller
void* phonecall(void* vargp) {
  //to have the thread reap itself
  pthread_detach(pthread_self());
  //creates a local variable to store the callers id
  int caller_id;
  sem_wait(&nextId_lock);
  //sets next_id to caller_id so every caller has a unique id and can be tracted easier
  caller_id = next_id;
  next_id++;
  sem_post(&nextId_lock);
  printf("Thread [%i] is attempting to connect ...\n", caller_id);
  sem_wait(&connected_lock);
  //a loop to check if the callers currently connected is equal to the number of lines available 
  while(connected == NUM_LINES){
    //sleeps for 1 second to simulate a wait time
    sem_post(&connected_lock);
    sleep(1);
    sem_wait(&connected_lock);
  }
  //caller connects to an available line, so the number of callers connected increases
  connected++;
  sem_post(&connected_lock);
  printf("Thread [%i] connects to an available line, call ringing ...\n", caller_id);
  sem_wait(&operators);
  printf("Thread [%i] is speaking to an operator\n", caller_id);
  //sleeps for 1 second to simulate a question being asked
  sleep(1);
  printf("Thread [%i] has propsed a question for candidates! The operator has left ...\n", caller_id);
  sem_post(&operators);
  sem_wait(&connected_lock);
  //question is proposed so the number of callers connected decreases
  connected--;
  sem_post(&connected_lock);
  printf("Thread [%i] call has ended.\n", caller_id);
}
