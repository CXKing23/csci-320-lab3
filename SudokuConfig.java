public final class SudokuConfig {
    public static final int ROW_SIZE = 9;
    public static final int COL_SIZE = ROW_SIZE;
    public static final int NUM_OF_SUBGRIDS = ROW_SIZE;
    public static final int NUM_OF_THREADS = (ROW_SIZE + COL_SIZE + NUM_OF_SUBGRIDS);

    private SudokuConfig() {}
}
