import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class SudokuBoardReader {
    public int[][] sudoku_board;
    public int[] worker_validation;

    public SudokuBoardReader(String fileName){
        this.sudoku_board = readBoardFromFile(fileName);
        this.worker_validation = new int[SudokuConfig.NUM_OF_THREADS];
    }

    private int[][] readBoardFromFile(String fileName){
        File boardCSV = new File(fileName);
        int[][] board = new int[SudokuConfig.ROW_SIZE][SudokuConfig.COL_SIZE];    

        try(Scanner fileReader = new Scanner(boardCSV)){
            int row = 0;
            while (fileReader.hasNextLine()){
                String rowData = fileReader.nextLine();
                for (int i = 0; i < rowData.length(); i=i+2){
                    char c = rowData.charAt(i);
                    board[row][i/2] = Character.getNumericValue(c);
            
                }
                row +=1;
            }
        } 
        catch (FileNotFoundException e){
            System.out.printf("Error: File %s not found%n", fileName);
            e.printStackTrace();
        }
        
        return board;
    }

    public boolean validateBoard(){
        Thread[] threads = new Thread[SudokuConfig.NUM_OF_THREADS];

        for (int i = 0; i < SudokuConfig.NUM_OF_THREADS; i++){
            int id = i;
            int starting_row = 0;
            int starting_col = 0;
            int ending_row = 0;
            int ending_col= 0;

             switch (i / 9) {
                case 0: // Row thread
                    starting_row = i;
                    starting_col = 0;
                    ending_row = i;
                    ending_col = 8;
                    break;

                case 1: // Column thread
                    int col = i % 9;
                    starting_row = 0;
                    starting_col = col;
                    ending_row = 8;
                    ending_col = col;
                    break;

                case 2: // Subgrid thread
                    int grid = i % 9;
                    int startRow = (grid / 3) * 3;
                    int startCol = (grid % 3) * 3;
                    starting_row = startRow;
                    starting_col = startCol;
                    ending_row = startRow + 2;
                    ending_col = startCol + 2;
                    break;
                }
            SudokuThreadParamConfig params = new SudokuThreadParamConfig(
                id, 
                starting_row, 
                starting_col, 
                ending_row, 
                ending_col, 
                sudoku_board, 
                worker_validation
            );
            SudokuRegionValidatorTask task = new SudokuRegionValidatorTask(params);
            
            Thread thread = new Thread(task);

            thread.start();

            threads[i] = thread;
        }

        for (int i = 0; i < SudokuConfig.NUM_OF_THREADS; i++){
            Thread thread = threads[i];
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            
        }

        for (int i = 0; i < SudokuConfig.NUM_OF_THREADS; i++){
            if (worker_validation[i] == 0)
            //return false if ANY threads found errors
                    return false;
        }
    //return true if no threads found error
    return true;
    }

}