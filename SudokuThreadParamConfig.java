import java.util.Arrays;

public class SudokuThreadParamConfig {
    private int id;
    private int starting_row;
    private int starting_col;
    private int ending_row;
    private int ending_col;
    private int[][] board;
    private int[] worker_validation;

    public SudokuThreadParamConfig(int id, int starting_row, int starting_col, int ending_row, int ending_col, int[][] board, int[] worker_validation) {
        this.id = id;
        this.starting_row = starting_row;
        this.starting_col = starting_col;
        this.ending_row = ending_row;
        this.ending_col = ending_col;
        this.board = board;
        this.worker_validation = worker_validation;
    }

    public int getId() {
        return id;
    }

    public int getStartingRow() {
        return starting_row;
    }

    public int getStartingCol() {
        return starting_col;
    }

    public int getEndingRow() {
        return ending_row;
    }

    public int getEndingCol() {
        return ending_col;
    }

    public int[][] getBoard() {
        return board;
    }

    public int[] getWorkerValidation() {
        return worker_validation;
    }
     @Override
    public String toString() {
        return 
               "id=" + id +
               ", area=[" + (starting_row+1) + ":" + (ending_row+1) + ", " + (starting_col+1) + ":" + (ending_col+1) + "]";
    }
}
