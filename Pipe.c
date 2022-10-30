#include <stdio.h>
#include <stdlib.h> // for exit()
#include <sys/types.h>
#include <time.h>
#include <unistd.h> // for fork()

//------------const--------------
const int PIPES = 2
		,WRITING = 1
		,READING = 0
		,N = 100
		,WANTED_NUMBER = 0
		,RANGE_OF_NUMBERS = 1000
		,MAXIMUM_INT = 2147483647;

//----------prototype------------
void get_nums_from_son_n_print(int pipe_descs[]);
void open_pipe(int pipe_descs[]);
void check_if_failed(const int *status);
void do_child(int pipe_descs[]);
void do_father(int pipe_descs[]);
int get_number_of_guesses();
//------------main---------------
int main() {

	int pipe_descs[PIPES];
	pid_t status ;
	srand(time(NULL));

	open_pipe(pipe_descs);
	status = fork();
	check_if_failed(&status);

	if(status > 0)
		do_child(pipe_descs);
	else do_father(pipe_descs);

	return(EXIT_SUCCESS) ;
}
//The function closes the pipe for writing and
//reads values from the son using the pipe and print them.
//-----------do_father-------------
void do_father(int pipe_descs[]){

	close(pipe_descs[WRITING]) ; //father reads
	get_nums_from_son_n_print(pipe_descs);
}
//The function gets a number from the son and then prints him.
//-------get_numbers_from_son-----
void get_nums_from_son_n_print(int pipe_descs[]){

	int bytes_read = 1 // '1' In order to enter to the loop
		,num
		,row = 0;

	while(bytes_read > 0){
			bytes_read = read(pipe_descs[READING], &num,  sizeof(int));
			check_if_failed(&bytes_read);
			printf("%d.The number of times to get zero is : %d\n",row,num);
			row++;
		}
	close(pipe_descs[READING]);
}
//The function writes N times how much times took each lottery
//------------do_child---------------
void do_child(int pipe_descs[]){

	int guesses,
		i;

	close(pipe_descs[READING]);//child writes
	for(i = 0 ; i < N ;i++){
		int bytes;

		guesses = get_number_of_guesses();
		bytes = write(pipe_descs[WRITING], &guesses,  sizeof(int)) ;
	    check_if_failed(&bytes);
	}
	close(pipe_descs[WRITING]);
}
//The function lotteries numbers until the WANTED_NUMBER or the maximum of int,
// and then returns the number of lotteries.
//-----------get_times_to_zero-------
 int get_number_of_guesses(){

	 int counter = 1;

	 while(counter < MAXIMUM_INT){
		 int num = rand() % RANGE_OF_NUMBERS;
		 if(num == WANTED_NUMBER)
			 return counter;
		 counter++;
	 }
	 return counter; //for warning
}
//------------open_pipe--------------
void open_pipe(int pipe_descs[]){

	if (pipe(pipe_descs) == -1) {
	 perror ("cannot open pipe") ;
	 exit(EXIT_FAILURE) ;
	 }
}
//--------check_if_status_failed--------
void check_if_failed(const int * status){

	if(status < 0) {
		perror("system call failed") ;
		exit(EXIT_FAILURE) ;
	}
}
