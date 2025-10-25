public class SudokuValidation{
    public static void main(String[] args){ 
        if (args.length < 1){
           System.out.println("Error: Not Enough Arguments");
           System.exit(1);
        }
        boolean debugMode = false;

        if (args.length > 1) {
            String secondArg = args[1];
            // Check for both common formats: -d or -debug
            if (secondArg.equals("-d") || secondArg.equalsIgnoreCase("-debug")) {
                debugMode = true;
                System.out.println("--- Debug Mode Activated ---");
            }
        }

        SudokuBoardReader sudokuBoard = new SudokuBoardReader(args[0], debugMode);


        if (sudokuBoard.validateBoard()){
            System.out.println("The board is valid.");
        }else {
            System.out.println("The board is not valid.");
        }
    

    }
}