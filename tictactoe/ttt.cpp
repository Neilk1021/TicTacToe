#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ctime> 

using namespace std;

bool gameOver = false; 

enum board {None, X, O};
board turn = X;
board playerX = X; 
board bot = O;
board TTTboard[3][3] = {
{None, None, None}, 
{None, None, None},
{None, None, None}};
int LastPlace[1];

int GetX(int a, int BSize){
    return (a - 1)%BSize; 
}

int GetY(int a, int BSize){
    int y = ((a - 1 - GetX(a, BSize))/BSize);
}

//player types in 1-9 which is converted into 0-8 which the computer turns into x y coordinates 
void PlayerChoice(){
    int a; 
    cin >> a; 
    int x = GetX(a, 3);
    int y = GetY(a, 3);

    if(TTTboard[x][y] == None && a < 10 && a > 0){
        TTTboard[x][y] = playerX; 
        LastPlace[0] = x;
        LastPlace[1] = y;
        turn = bot;
    }
    else{
        cout << "Please `make a valid choice! \n";
        PlayerChoice();
    }
}

//Bot's choice using two random values between 0-2 on the x and y axises 
void BotChoice(){
    bool Complete = false;
    srand((unsigned)time(0)); 
    while (!Complete)
    {
        int x = (rand()%3); 
        int y = (rand()%3); 
        if(TTTboard[x][y] == None)
        {
            TTTboard[x][y] = bot; 
            LastPlace[0] = x;
            LastPlace[1] = y;
            turn =  playerX; 

            Complete = true;
        }
    }
    
}


void UpdateScreen(){
    
    //switches between player and bot turns
    if(turn == playerX){
        cout << "It is your turn! \n";
        PlayerChoice();
    }
    else{
        cout << "It's the bot's turn\n";
        BotChoice();
    }
    
    //renders the board gui
    for(int j = 0;  j < 3; j++){
        for(int i = 0;  i < 3; i++){
            if(TTTboard[i][j] == None)
                cout << "[ ] ";
            else if(TTTboard[i][j] == X)
                cout << "[X] ";
            else if(TTTboard[i][j] == O) 
                cout << "[O] ";
        }
        cout << "\n";
    }

}

//checks whether the player is X or O 
void XOCheck(char d){
    switch (d){
        case 'x':
            cout << "Player is X\n";
            playerX = X;
            bot = O;
            break;
        case 'o':
            cout << "Player is O\n";
            playerX = O; 
            bot = X; 
            break; 
        default:
            cout << "Please input a valid choice \n";
            string play; 
            cin >> play;  
            char b =  play[0];
            b = char(tolower(b));
            XOCheck(b);
            break; 
    }
}

//checks if the game is complete through brute force possible win checks
/* is similar to https://stackoverflow.com/questions/1056316/algorithm-for-determining-tic-tac-toe-game-over 
although I did write this function myself I feel this citation is needed due to how similar it is, this is 
most likely due to how simple tic tac toe is there are only so many ways to solve every problem. 
*/
board CompletionChecker(){

    board PrevMove =  TTTboard[LastPlace[0]][LastPlace[1]];

    //checks the vertical axis
    for(int i = 0; i < 3; i++){
        if(TTTboard[LastPlace[0]][i] != PrevMove)
            break;  
        if(i == 2)
            return PrevMove;

    }

    //checks the horizontal axis
    for(int i = 0; i < 3; i++){
        if(TTTboard[i][LastPlace[1]] != PrevMove)
            break;  
        if(i == 2)
            return PrevMove;
    }

    //checks the diagonal y = x
    for(int i = 0; i < 3; i++){
        if(TTTboard[i][i] != PrevMove)
            break;  
        if(i == 2)
            return PrevMove;
    }

     //checks the diagonal y = -x
    for(int i = 0; i < 3; i++){
        if(TTTboard[i][2 - i] != PrevMove)
            break;  
        if(i == 2)
            return PrevMove;
    }
    return None;

}

bool DrawCheck(int BoxSize, board B[3][3]){
    for (int i = 1; i <= (BoxSize * BoxSize); i++)
    {
        int x = GetX(i, BoxSize);
        int y = GetY(i, BoxSize); 
        if(B[x][y] == None){
            return false;
        }
    } 

    return true; 
} 

//Sets up the game
void Initialization(){

    gameOver = false;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            TTTboard[i][j] = None;
        }
    }

    cout << "Tic Tac Toe but scuffed! \n";
    cout << "X or O\n";
    
    string XorO;
    cin >> XorO;  
    turn = X; 

    char d =  XorO[0];
    d = char(tolower(d));
    XOCheck(d);   

    cout << "This is the board \n";
    cout << "[1] [2] [3]\n";
    cout << "[4] [5] [6]\n";
    cout << "[7] [8] [9]\n";

    cout << "When it is your turn input the number you would like to fill \n"; 
    
}

//Allows the game to restart after someone one
void RepeatCheck(string d){

    for (int i = 0; i < d.length(); i++)
    {
       char a = d[i];
       a = char(tolower(a));

        switch (a){
            case 'y':
                Initialization();
                break;
            case 'n':
                cout << "Program ended!\n";
                break; 
            default:
                break; 
        }
        
    }
}

//Renders Gui for when the game ends and asks if the player wants to play again
void GameOver(board a){

    gameOver = true;
    switch (a)
    {
    case X:
        if(playerX == X){
            cout << "You Won!\n";
            cout << "\n";
        }else{
            cout << "Bot Won!\n";
            cout << "\n";
        }
        break;
    case O:
        if(playerX == O){
            cout << "You Won!\n";
            cout << "\n";
        }else{
            cout << "Bot Won!\n";
            cout << "\n";
        }
        break;
    default:
        break;
    }

    cout << "Play Again? (Y/N) \n";
    string yn; 
    cin >> yn;
    RepeatCheck(yn);
}

//while loop to prevent game from being one turn long
void RunGame(){
   while(!gameOver){
       UpdateScreen();
       board GameCont = CompletionChecker();
       if(GameCont != None){
           GameOver(GameCont);
       }else if(DrawCheck(3, TTTboard)){
            cout << "Draw!\n";
            cout << "\n";
            cout << "Play Again? (Y/N) \n";
            string yn; 
            cin >> yn;
            RepeatCheck(yn);
       }
   } 
}

//sets up and runs game
int main()
{
    Initialization();
    RunGame();

    return 0;
}