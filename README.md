# Presidential-Debate
For this project, I put the program in a single .c file. It is titled presidential_debate.c—this is where the simulation of callers asking candidates questions is. It
starts with the initialization of a few global variables including next_id, which is used to set the callers id. After the initialization of the global variables, there is 
an “if” statement in main to check if the user has entered a time limit for the simulation. The next statement converts the string passed into the command line as an argument to 
an integer to be used later for the length of the simulation. After, a for loop is created to simulate 200 calls being made. Within this for loop, 200 “callers”—or threads—are 
created and executed in the phonecall function. This function filters all the callers attempting to connect, connects them to the operators, allowing the callers to propose a 
question to the candidates, and finally ends the call. Outside of the for loop another thread is created and executed in the timer function, which sleeps for the length of time 
that the user inputted. Its argument is the number the user inputted as a command line argument for how long they want the simulation to run for. After that, the program waits 
for the timer thread to terminate. Once this happens, the program destroys all the semaphores initialized and exits out of main to clean up any remaining threads that may not 
have terminated when the timer thread finished. 



Youtube Demonstration Link:

https://youtu.be/DIFLhy2ryQM
