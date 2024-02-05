#include <iostream>
#include "stack.h"
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib> // to be able to use rand function in random generating part
using namespace std;


struct MazeCell {
    int x, y; //coordinates
    bool up, down, left, right; //walls
    bool visitedCell = false;
    MazeCell(int x, int y) : x(x), y(y), up(true), down(true), left(true), right(true) {} //constructor
    
    // the equality operator
     bool operator==(const MazeCell& other) const {
         return x == other.x && y == other.y;
     }
};

//Function Declarations

char randomPathGenerator(int x, int y, int numRows, int numColumns, vector<MazeCell>& maze);
void knockDownWalls(char wallToKnockDown, MazeCell*currentCell);
void mazeGenerator(int numRows, int numColumns, vector<MazeCell>& maze);
void mazePrinter(vector<MazeCell> maze,int numberOfMaze,int numRows,int numColumns);
bool validMove(int x, int y, char moveChar, int xLimit, int yLimit, vector<MazeCell>& maze);
char randomMoveGenerator(int x, int y, int numRows, int numColumns, MazeCell* currentCell);
void pathPrinter( int mazeID,int entryX,int entryY,int exitX, int exitY, int numRows, int numColumns,Stack<MazeCell*>&pathStack);
bool pathValidMove(int x, int y, char moveChar, int xLimit, int yLimit, vector<MazeCell>& maze);
void pathGenerator(int startX, int startY, int endX, int endY,int numRows,int numColumns,vector<MazeCell>& maze,Stack<MazeCell*>&pathStack);
// The Wall that we are going to knock down  1 -> 0
void knockDownWalls(char wallToKnockDown, MazeCell* currentCell){
    
    if(wallToKnockDown == 'r'){
        //knock down the right wall
        currentCell->right = false;
    }
    else if(wallToKnockDown == 'l'){
        //knock down the left wall
        currentCell->left = false;
    }
    else if(wallToKnockDown == 'u'){
        //knock down the upper wall
        currentCell->up = false;
    }
    else if(wallToKnockDown == 'd'){
        //knock down the down wall
        currentCell->down = false;
    }
}

bool validMove(int x, int y, char moveChar, int xLimit, int yLimit, vector<MazeCell>& maze) {
    // if the coordinates below or above the limits return false

    MazeCell* currentCell = &maze[0];
    // move right
    if (moveChar == 'r') {
        if (x + 1 < xLimit) {
            currentCell = &maze[xLimit * y + x + 1];
        }
    }
    // more left
    if (moveChar == 'l') {
        if (x - 1 >= 0) {
            currentCell = &maze[xLimit * y + x - 1];
        }
    }
    // move up
    if (moveChar == 'u') {
        if (y + 1 < yLimit) {
            currentCell = &maze[xLimit * (y + 1) + x];
        }
    }
    // more down
    if (moveChar == 'd') {
        if (y - 1 >= 0) {
            currentCell = &maze[xLimit * (y - 1) + x];
        }
    }

    // checking if the move is into a visited cell
    if (currentCell->visitedCell) {
        return false;
    }
    return true;
}




//generates random moves
char randomMoveGenerator(int x, int y, int numRows, int numColumns, vector<MazeCell>& maze) {
    vector<char> arrayOfMoves;
  
    // move right
    if (validMove(x, y, 'r', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('r');
    }
    // more left
    if (validMove(x, y, 'l', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('l');
    }
    // move up
    if (validMove(x, y, 'u', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('u');
    }
    // more down
    if (validMove(x, y, 'd', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('d');
    }
        
    if(arrayOfMoves.size() != 0){
        int ranGen = rand() % arrayOfMoves.size();
        return arrayOfMoves[ranGen];
    }
    else{
        return 'n';
    }
}


void mazeGenerator(int numRows, int numColumns, vector<MazeCell>& maze) {
    Stack<MazeCell*> mazeStack; //creating the stack
    MazeCell* currentCell = &maze[0]; //initializing the first cell
    mazeStack.push(currentCell); //pushing it to the stack
    currentCell->visitedCell = true; //&setting it as visited
    

    int totalCells = numRows * numColumns; // we are ought to break M*N-1 walls so count
    int count = 1;
    
    while (count < totalCells) {
        currentCell = mazeStack.getTopData();
        int currentX = currentCell->x;
        int currentY = currentCell->y;

        // Get a random move, prioritizing unvisited neighbors
        char move = randomMoveGenerator(currentX, currentY, numRows, numColumns, maze);
        
        //if the move is valid
        if (move != 'n') {
            count++; // we are ought to break M*N-1 walls so count
            knockDownWalls(move, currentCell); //knock down the selected wall of our current cell
            
            
            int newX = currentX, newY = currentY;
            
            //we should also knock down the same wall for the neighbor cell
            if (move == 'r') {
                newX = currentX + 1;
                move = 'l';
            } else if (move == 'l') {
                newX = currentX - 1;
                move = 'r';
            } else if (move == 'u') {
                newY = currentY + 1;
                move = 'd';
            } else if (move == 'd') {
                newY = currentY - 1;
                move = 'u';
            }
            //update the current cell & push it to our stack
            currentCell = &maze[newY * numColumns + newX];
            mazeStack.push(currentCell);
            
            knockDownWalls(move, currentCell);
            currentCell->visitedCell = true; // check it as a visited cell
        }
        //nowhere else to move so backtrack
        else {
            mazeStack.pop();
        }
    }
    for(int i = 0; i < maze.size();i++){
        maze.at(i).visitedCell = false;
    }
}
//checks validity of the moves for path generator
bool pathValidMove(int x, int y, char moveChar, int xLimit, int yLimit, vector<MazeCell>& maze){
   
    MazeCell* currentCell = &maze[0]; //initialize

    // move right
    if (moveChar == 'r') {
        if (x + 1 < xLimit) {
            currentCell = &maze[xLimit * y + x + 1];
        }
    }

    // more left
    if (moveChar == 'l') {
        if (x - 1 >= 0) {
            currentCell = &maze[xLimit * y + x - 1];
        }
    }

    // move up
    if (moveChar == 'u') {
        if (y + 1 < yLimit) {
            currentCell = &maze[xLimit * (y + 1) + x];
        }
    }

    // more down
    if (moveChar == 'd') {
        if (y - 1 >= 0) {
            currentCell = &maze[xLimit * (y - 1) + x];
        }
    }

    //checking whether we have visited the cell or not
    if(currentCell->visitedCell == true){
        return false;
    }
    //checking the condition of the walls
    if(moveChar=='u'){
        return(!currentCell->down);
    }
    else if(moveChar == 'd'){
        return (!currentCell->up);
    }
    else if(moveChar == 'r'){
        return(!currentCell->left);
    }
    else if(moveChar == 'l'){
        return(!currentCell->right);
    }
        
    return true; //if valid return true
}
// generates the move for the path generator
char  randomPathGenerator(int x, int y, int numRows, int numColumns, vector<MazeCell>&maze) {
    vector<char> arrayOfMoves;
  
    // move right
    if (pathValidMove(x, y, 'r', numColumns, numRows,maze)) {
        arrayOfMoves.push_back('r');
    }
    // more left
    if (pathValidMove(x, y, 'l', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('l');
    }
    // move up
    if (pathValidMove(x, y, 'u', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('u');
    }
    // more down
    if (pathValidMove(x, y, 'd', numColumns, numRows, maze)) {
        arrayOfMoves.push_back('d');
    }
        
    if(arrayOfMoves.size() != 0){
        int ranGen = rand() % arrayOfMoves.size();
        return arrayOfMoves[ranGen];
    }
    else{
        return 'n';
    }
}

void pathGenerator(int startX, int startY, int endX, int endY,int numRows,int numColumns,vector<MazeCell>& maze,Stack<MazeCell*>&pathStack){
   
    MazeCell* finalCell = &maze[endY * numColumns + endX]; //our exit coordinates
    
    MazeCell* currentCell = &maze[startY * numColumns + startX] ; //setting current cell as the starting point
    pathStack.push(currentCell); //pushing it to the stack
    currentCell->visitedCell = true; //setting the cell as visited
   
    int currentX = startX; int currentY = startY;
    
    while (currentCell != finalCell) { //when we haven't reached the exit coordinates do
        // Get a random move, prioritizing unvisited neighbors
        char move = randomPathGenerator(currentX, currentY, numRows, numColumns, maze);
       
        //if the move is valid
        if (move != 'n') {
            
            int newX = currentX, newY = currentY;

            if (move == 'r') {
                newX = currentX + 1;
            } else if (move == 'l') {
                newX = currentX - 1;
            } else if (move == 'u') {
                newY = currentY + 1;
            } else if (move == 'd') {
                newY = currentY - 1;
            }
            //updating the cell
            currentCell = &maze[newY * numColumns + newX];
            currentCell->visitedCell = true;
            pathStack.push(currentCell);
        }
        //if nowhere else to go backtrack
        else {
            pathStack.pop();
        }
        //updating the coordinates & the cell
        currentCell = pathStack.getTopData();
        currentX = currentCell->x;
        currentY = currentCell->y;
    }
    
}

//prints maze to a txt file
void mazePrinter(vector<MazeCell> maze, int numberOfMaze, int numRows, int numColumns) {
    ofstream output; //creating the file
    string filename = "maze_" + to_string(numberOfMaze) + ".txt"; //naming it
    output.open(filename.c_str()); //open it so we can write in it
    output << to_string(numRows) + " " + to_string(numColumns) << endl;
    //writing part
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numColumns; c++) {
            int index = r * numColumns + c;  // calculating the index in the 1D vector
            output << "x=" << c << " y=" << r << " l=" << maze[index].left << " r=" << maze[index].right
                   << " u=" << maze[index].up << " d=" << maze[index].down << endl;
        }
    }
    output.close(); //writing is finish so close
}
//prints path to a txt file
void pathPrinter( int mazeID,int entryX,int entryY,int exitX, int exitY, int numRows, int numColumns,Stack<MazeCell*>&pathStack){
    ofstream output; //creating the file
    //naming it
    string filename = "maze_"+ to_string(mazeID) +"_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) +"_" + to_string(exitY) + ".txt";
    output.open(filename.c_str());  //open it so we can write in it
    Stack<MazeCell*>tempPathStack; //creating another stack so we can print it in the wanted format
    while(!pathStack.isEmpty()){
        MazeCell* currentCell = pathStack.getTopData();
        tempPathStack.push(currentCell); //popping from the old stack and pushing to the new stack
        pathStack.pop();
    }
    while(!tempPathStack.isEmpty()){ // writing the path to the .txt file
        MazeCell* currentCell = tempPathStack.getTopData();
        output << currentCell->x << " " << currentCell->y << endl;
        tempPathStack.pop();
    }
    output.close(); //writing is finish so close
   
}

int main() {
    int M, N, K, mazeID, entryX, entryY, exitX, exitY;

    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N;

    vector<vector<MazeCell>> mazeVector; //created a vector that will store other Mazecell vectors as the 1D vectors are mazes -> 2nd vector stores mazes
    //creating mazes
    for(int i = 0; i < K; i++) {
        vector<MazeCell> maze;
        for(int j = 0; j < M; j++) {
            for(int k = 0; k < N; k++) {
                MazeCell currentCell(k, j);
                maze.push_back(currentCell); //pushing cells to the maze
            }
        }
        mazeGenerator(M, N, maze); //generating mazes
        mazeVector.push_back(maze); //pushing mazes to the vector that stores mazes
    }
    cout << "All mazes are generated." << endl << endl;
    

    cout << "Enter a maze ID between 1 to " << to_string(K) << " inclusive to find a path: ";
    cin >> mazeID;
   

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row):";
    cin >> entryX >> entryY;

    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row):";
    cin >> exitX >> exitY;
    
  
    Stack<MazeCell*> pathStack; //created a stack for path algorithm
    pathGenerator(entryX,entryY, exitX, exitY,M,N,mazeVector.at(mazeID-1),pathStack); //generates path
    mazePrinter(mazeVector.at(mazeID - 1), mazeID, M, N); //writes maze to the .txt file
    pathPrinter(mazeID, entryX, entryY, exitX, exitY, M, N, pathStack); //writes path to the .txt file
    

    
    return 0;
}


