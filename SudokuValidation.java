public class SudokuValidation{
    public static void main(String[] args){ 
        if (args.length < 1){
           System.out.println("Error: Not Enough Arguments");
           System.exit(1);
        }
        
        SudokuBoardReader sudokuBoard = new SudokuBoardReader(args[0]);

        if (sudokuBoard.validateBoard()){
            System.out.println("The board is valid.");
        }else {
            System.out.println("The board is not valid.");
        }
    

    }
}