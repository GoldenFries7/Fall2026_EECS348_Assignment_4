// ==============================================================================
// Program Name: EECS 348 Assignment 4
// Description: An object-oriented Sudoku solver that uses recursive depth-first
//              search with backtracking to find all solutions to 9x9 puzzles.
// Inputs: Five text files (puzzle1.txt to puzzle5.txt) containing grid data.
// Outputs: The original grid and all possible solutions printed to the terminal.
// Collaborators: Gemini (GenAI baseline structure).
// Other Sources: None.
// Author: Iseoluwa Oluwapelumi Adedipe
// Creation Date: 10/15/2026
// Revision Date: 10/15/2026
// Revisions: Built the class structure, added file parsing, and wrote the recursion.
// ==============================================================================

#include <iostream> // For printing to the console
#include <fstream>  // For reading the puzzle text files
#include <string>   // For handling the file names as strings
#include <vector>   // For storing my 9x9 grid dynamically

using namespace std; // For keeping my code neat and clean so I don't have to type std:: everywhere

// -------------------------------------------------------------------------
// Class: SudokuSolver
// Description: Encapsulates the puzzle grid and the backtracking logic.
// -------------------------------------------------------------------------
class SudokuSolver { // Creating my main object-oriented class for the assignment
private: // Keeping my data hidden from the outside
    vector<vector<int>> grid; // This is my 2D array to hold the puzzle numbers
    int solutionsFound;       // A counter to track how many solutions I print

    // This helper checks if placing a specific number breaks the Sudoku rules
    bool isSafe(int row, int col, int num) { // I pass in the coordinates and the guess
        for (int x = 0; x < 9; x++) { // I need to loop 9 times to check the row and column
            if (grid[row][x] == num) { // If the number is already in this row...
                return false; // ...then it's a bad guess, return false
            } 
            if (grid[x][col] == num) { // If the number is already in this column,
                return false; //  then it's a bad guess, return false
            } 
        }

        // Now I need to check the local 3x3 block
        int startRow = row - row % 3; // Finding the top-left row index of the 3x3 box
        int startCol = col - col % 3; // Finding the top-left column index of the 3x3 box
        
        for (int i = 0; i < 3; i++) { // Looping through the 3 rows of the local box
            for (int j = 0; j < 3; j++) { // Looping through the 3 columns of the local box
                if (grid[i + startRow][j + startCol] == num) { // If my guess is in the box...
                    return false; // ...then it fails the rules, return false
                } 
            } 
        } 
        return true; // If it passed the row, column, and box checks, it's safe!
    } 

    // This helper just prints out the current state of the board
    void printBoard() { // Takes no parameters, just prints
        for (int r = 0; r < 9; r++) { // Loop through all 9 rows
            for (int c = 0; c < 9; c++) { // Loop through all 9 columns
                cout << grid[r][c] << " "; // Print the number and a space
            } 
            cout << endl; // Move to the next line after finishing a row
        } 
        cout << endl; // Add a blank line at the very end for readability
    } 

public: // Opening up my public interface
    // Constructor just sets up a blank 9x9 board and resets the counter
    SudokuSolver() { // Building the object
        grid = vector<vector<int>>(9, vector<int>(9, 0)); // Initializing the 2D vector with zeros
        solutionsFound = 0; // Starting with zero solutions
    } 

    // This method handles opening the file and parsing the characters
    bool loadPuzzle(string filename) { // Takes the file name as an argument
        ifstream file(filename); // Open a file stream
        if (!file.is_open()) { // If the file is missing or locked,
            cout << "Error: Could not open " << filename << endl; // print an error
            return false; // and bail out
        } 

        string val; // A temporary string to hold each character I read
        for (int r = 0; r < 9; r++) { // Loop through the 9 rows of the file
            for (int c = 0; c < 9; c++) { // Loop through the 9 columns of the file
                file >> val; // Grab the next chunk of text
                if (val == "_") { // If the file has an underscore,
                    grid[r][c] = 0; // I represent that as a 0 in my math logic
                } else { // Otherwise, it must be a real number
                    grid[r][c] = stoi(val); // Convert the string to an integer and save it
                } 
            } 
        } 
        file.close(); // Always close the file when done!
        solutionsFound = 0; // Reset the solution counter for this specific puzzle
        return true; // Successfully loaded
    } 

    // This is the recursive backtracking engine
    void solve(int row, int col) { // Takes the current coordinates I am looking at
        if (row == 9) { // If I dropped off the bottom of the board (row 9),
            printBoard(); // that means I completely filled it! Print it!
            solutionsFound++; // Increment my success counter
            return; // Backtrack to find more solutions
        } 

        if (col == 9) { // If I dropped off the right edge of the board,
            solve(row + 1, 0); // wrap around to the start of the very next row
            return; // Backtrack when that path finishes
        } 

        if (grid[row][col] != 0) { // If this cell already has a number from the file.
            solve(row, col + 1); // skip it and move to the next column
            return; // Backtrack when that path finishes
        } 

        // If the cell is empty, I need to guess numbers 1 through 9
        for (int num = 1; num <= 9; num++) { // Trying every possible digit
            if (isSafe(row, col, num)) { // Check if the digit breaks any Sudoku rules
                grid[row][col] = num; // If it's safe, tentatively write it on the board
                solve(row, col + 1); // Recursively dive deeper to solve the next cell
                grid[row][col] = 0;  // UNDO: If that path failed, erase the guess (backtrack)
            } 
        } 
        // If I tried 1 through 9 and nothing worked, this branch dies here.
    } 

    // A simple method to print the original board and kick off the recursion
    void run() { // Called from main
        cout << "Original Puzzle:" << endl; // Print header
        printBoard(); // Show the unsolved board first
        cout << "Solutions found:" << endl; // Print header for the answers
        solve(0, 0); // Start the recursive search at the top-left corner (0,0)
        
        if (solutionsFound == 0) { // Check if my counter never went up
            cout << "No solution found" << endl; // Inform the user the puzzle is impossible
        } else { // Otherwise.
            cout << "Total solutions: " << solutionsFound << endl; // print the total count
        } 
        cout << "-----------------------------------" << endl; // Print a divider line
    } 
}; 

// -------------------------------------------------------------------------
// Function: main
// Description: Loops through the 5 required files and executes the solver.
// -------------------------------------------------------------------------
int main() { // The entry point of my application
    // Creating a list of the exact file names required by the assignment
    vector<string> files = {"puzzle1.txt", "puzzle2.txt", "puzzle3.txt", "puzzle4.txt", "puzzle5.txt"}; 
    
    for (int i = 0; i < files.size(); i++) { // Looping through each file name in the list
        cout << "Processing " << files[i] << "..." << endl; // Telling the user which file is active
        SudokuSolver game; // Instantiate a fresh, empty Sudoku object
        
        if (game.loadPuzzle(files[i])) { // Try to load the file, and if successful,
            game.run(); // fire up the printing and backtracking logic!
        } 
    } 
    
    return 0; // Exit the program successfully
} // End of main