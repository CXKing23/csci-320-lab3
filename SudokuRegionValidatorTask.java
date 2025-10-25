public class SudokuRegionValidatorTask implements Runnable {
    private int id;
    private int starting_row;
    private int starting_col;
    private int ending_row;
    private int ending_col;
    private int[][] board;
    private int[] worker_validation; 

    public SudokuRegionValidatorTask(SudokuThreadParamConfig params) {
        this.id = params.getId();
        this.starting_row = params.getStartingRow();
        this.starting_col = params.getStartingCol();
        this.ending_row = params.getEndingRow();
        this.ending_col = params.getEndingCol();
        this.board = params.getBoard();
        this.worker_validation = params.getWorkerValidation();
    }

    public void run() {

        boolean isThreadValid = true;

        int[] check = new int[9];

        for (int i=0; i < 9 && isThreadValid; i++){
            int row;
            int col;
            //Thread is checking a Row
            if (starting_row == ending_row){
                    row = starting_row;
                    col = i;
            }   
            //Thread is checking a Column
            else if (starting_col == ending_col){
                    col = starting_col;
                    row = i;
            }   
            // Thread is checking a subgrid
    else {
                    row = starting_row + i / 3;
                    col = starting_col +    i % 3;
            }
            //gets Number from board
            int num = board[row][col];
            // if Num is below 1 or above section is invalid
            if (num < 1 || num > 9){
                    isThreadValid = false;
            }
            //If there are duplicates section is invalid
            else if (check[num-1] == 1){
                    isThreadValid = false;
            }
            //Set the number's index to 1 (True)
            else {
                    check[num-1] = 1;
            }
    }
    if (!isThreadValid){
            worker_validation[id] = 0;
    }   
    //if there are no duplicates, and no numbers below 1 and above 9 after 9 iterations, it is implied that the section is valid

    else {
            worker_validation[id] = 1;

    }
    
    }
}
