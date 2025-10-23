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
    if (fp == NULL){
        perror("Unable to open file");
    }


    //Allocate Memory for board
    board = (int**)malloc(sizeof(int*)*ROW_SIZE);
    for(int row = 0; row < ROW_SIZE; row++){
        board[row] = (int*)malloc(sizeof(int)*COL_SIZE);
    };

    if (board == NULL){
        fclose(fp);
        return NULL;
    }

    //Assign board csv valvues to board
    for(int row = 0; row < ROW_SIZE; row++){
    for(int col = 0; col < COL_SIZE; col++){
        int num;
        int status;
        
        if (col < COL_SIZE - 1) { 
            status = fscanf(fp, "%d,", &num);
        } else { 
            status = fscanf(fp, "%d", &num);
        }

        if (status != 1){
            fprintf(stderr, "Error reading board data.\n");
            for(int r = 0; r < ROW_SIZE; r++){
                free(board[r]);
            }
            free(board);
            fclose(fp);
            return NULL;
        }

        board[row][col] = num;
    }
}
    fclose(fp);
    return board;
}


int is_board_valid(){
    pthread_t* tid;  /* the thread identifiers */
    pthread_attr_t attr;
    param_struct* parameter;




    // replace this comment with your code

    tid = (pthread_t*)malloc(sizeof(pthread_t)*NUM_OF_THREADS);
    parameter = (param_struct*)malloc(sizeof(param_struct)*NUM_OF_THREADS);

    pthread_attr_init(&attr);

    for (int i = 0; i < NUM_OF_THREADS; i++){
        parameter[i].id = i;

        if ((i / 9) == 0){
            parameter[i].starting_row = i;
            parameter[i].starting_col = 0;
            parameter[i].ending_row = i;
            parameter[i].ending_col = 8;

        }else if((i / 9) == 1){
            int col = i % 9;
            parameter[i].starting_row = 0;
            parameter[i].starting_col = col;
            parameter[i].ending_row = 8;
            parameter[i].ending_col = col;

        }else if((i / 9) == 2){
            int grid = i % 9;
            int startRow = (grid / 3) * 3;
            int startCol = (grid % 3) * 3;
            parameter[i].starting_row = startRow;
            parameter[i].starting_col = startCol;
            parameter[i].ending_row = startRow+2;
            parameter[i].ending_col = startCol+2;
        }
        pthread_create(&tid[i], &attr, runner, &parameter[i]);
    }

    for (int i = 0; i < NUM_OF_THREADS; i++){
        pthread_join(tid[i], NULL);
    }



}
void *runner(void *param){
    param_struct* params = (param_struct*)param;
    int id = params->id;
    int starting_row = params->starting_row;
    int starting_col = params->starting_col;
    int ending_row = params->ending_row;
    int ending_col = params->ending_col;
    int isThreadValid = 1;
    int check[9] = {0};
    for (int i=0; i < 9 && isThreadValid; i++){
        int row;
        int col;
        if (starting_row == ending_row){
            row = starting_row;
            col = i;
        } else if (starting_col == ending_col){
            col = starting_col;
            row = i;
        } else {
            row = starting_row + i / 3;
            col = starting_col +  i % 3;
        }
        
        int num = sudoku_board[row][col];
        if (num < 1 || num > 9){
            isThreadValid = 0;
        }
        else if (check[num-1] == 1){
            isThreadValid = 0;
        }
        else {
            check[num-1] = 1;
        }
    }
    
    if (!isThreadValid){
        worker_validation[id] = 0;
    } else {
        worker_validation[id] = 1;

    }
    pthread_exit(NULL);

}
