#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "lab2.h"

int** sudoku_board;
int* worker_validation;

void *runner(void *param);

int** read_board_from_file(char* filename){
    FILE *fp = NULL;
    int** board = NULL;
    //Open The File
    fp = fopen(filename, "r");
    //If File cant be open, send error and return a Null Board
    if (fp == NULL){
        perror("Unable to open file");
        return NULL;
    }


    //Allocate Memory for board
    board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
        board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    };

    //If memory allocation fails return a Null Board
    if (board == NULL){
        fclose(fp);
        return NULL;
    }

    //Assign board csv valvues to board
    for(int row = 0; row < ROW_SIZE; row++){
    for(int col = 0; col < COL_SIZE; col++){
        // number from the board at the row and collum 
        int num;
        //Status to see if read from file correctly
        int status;
        
        if (col < COL_SIZE - 1) { 
            status = fscanf(fp, "%d,", &num);
        } else { 
            status = fscanf(fp, "%d", &num);
        }

        if (status != 1){
            //If any erros from fscanf, throw error, free board memory, close file and reutnr null board
            fprintf(stderr, "Error reading board data.\n");
            for(int r = 0; r < ROW_SIZE; r++){
                free(board[r]);
            }
            free(board);
            fclose(fp);
            return NULL;
        }
        //assign board at the cow and collum from the fiel
        board[row][col] = num;
    }
}   
    //close file after finished reaidng
    fclose(fp);
    //return the completed baord
    return board;
}

//checks if the board if valid and creates worker threads
int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;

    // allocates memory for global vaiarble
    worker_validation = (int*)malloc(sizeof(int)*NUM_OF_THREADS);

    //allocates memory for tid array
    tid = (pthread_t*)malloc(sizeof(pthread_t)*NUM_OF_THREADS);
    
    //allocates mmoery for paramet array
    parameter = (param_struct*)malloc(sizeof(param_struct)*NUM_OF_THREADS);

    //init thread attributes
    pthread_attr_init(&attr);

    //Creates 27 threads
    for (int i = 0; i < NUM_OF_THREADS; i++){
        //Sets thread id
        parameter[i].id = i;

        //Threads for handling Rows
        if ((i / 9) == 0){
            parameter[i].starting_row = i;
            parameter[i].starting_col = 0;
            parameter[i].ending_row = i;
            parameter[i].ending_col = 8;

        }
        //Threads for handling coolums
        else if((i / 9) == 1){
            int col = i % 9;
            parameter[i].starting_row = 0;
            parameter[i].starting_col = col;
            parameter[i].ending_row = 8;
            parameter[i].ending_col = col;

        }
        //Threads for handling subgrids
        else if((i / 9) == 2){
            int grid = i % 9;
            int startRow = (grid / 3) * 3;
            int startCol = (grid % 3) * 3;
            parameter[i].starting_row = startRow;
            parameter[i].starting_col = startCol;
            parameter[i].ending_row = startRow+2;
            parameter[i].ending_col = startCol+2;
        }
        //creating threads
        pthread_create(&tid[i], &attr, runner, &parameter[i]);
    }
    //Join thread back to process
    for (int i = 0; i < NUM_OF_THREADS; i++){
        pthread_join(tid[i], NULL);
    }
    //Checks if any threads wrote errors to their index 
    for (int i = 0; i < NUM_OF_THREADS; i++){
        if (worker_validation[i] == 0)
        //return false if ANY threads found errors
            return 0;
    }
    //return true if no threads found erroer
    return 1;


}
//Thread running method
void *runner(void *param){
    //unpack params
    param_struct* params = (param_struct*)param;
    //derefernce params and assinged them to lcate vrabiles in the thread's stack
    int id = params->id;
    int starting_row = params->starting_row;
    int starting_col = params->starting_col;
    int ending_row = params->ending_row;
    int ending_col = params->ending_col;
    //initaly assume this thrids section of the baord is valid
    int isThreadValid = 1;
    //Create Array to both see if any duiplcates, and see if the section has 1-9
    int check[9] = {0};
    //Iterate through all 9 values, if any are invalid exit 
    for (int i=0; i < 9 && isThreadValid; i++){
        int row;
        int col;
        //Thread is checking a Row
        if (starting_row == ending_row){
            row = starting_row;
            col = i;
        } 
        //Tjread is checkign a Collum
        else if (starting_col == ending_col){
            col = starting_col;
            row = i;
        } 
        // Thread is checking a subgrind
        else {
            row = starting_row + i / 3;
            col = starting_col +  i % 3;
        }
        //gets Number from board
        int num = sudoku_board[row][col];
        // if Num is below 1 or above section is invalid
        if (num < 1 || num > 9){
            isThreadValid = 0;
        }
        //If theres duicplates section is invalid
        else if (check[num-1] == 1){
            isThreadValid = 0;
        }
        //Set the numbsers index to 1 (True)
        else {
            check[num-1] = 1;
        }
    }
    //if the thread's section of the board is invlaid, set 0 to worker validation index
    if (!isThreadValid){
        worker_validation[id] = 0;
    } 
    //if there are no duiplcated, and no numbers below 1 and above 9 after 9 iterations, it is implied that the section is valid

    else {
        worker_validation[id] = 1;

    }
    //Exit the thread
    pthread_exit(NULL);

}
