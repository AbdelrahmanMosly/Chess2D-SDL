#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

char turn = 'W';
char in[8];
char grid[8][8]=    {{'R','N','B','Q','K','B','N','R'},
                     {'P','P','P','P','P','P','P','P'},
                     {'-','.','-','.','-','.','-','.'},
                     {'.','-','.','-','.','-','.','-'},
                     {'-','.','-','.','-','.','-','.'},
                     {'.','-','.','-','.','-','.','-'},
                     {'p','p','p','p','p','p','p','p'},
                     {'r','n','b','q','k','b','n','r'}};


extern void GUI();
extern void maintainGrid();
extern void save();
extern void load();
extern void undo();
extern void redo();
extern char getLower(char);
extern int getRow(char);
extern int getCol(char);
extern bool isValid(char[8][8], char[], char, int);
extern bool isValidInput(char[8][8],char[]);
extern void move(char[8][8], int, int, int, int, char);
extern void chooseMode();
extern bool isCheck(char[8][8], char);
extern char nextTurn(char);
extern bool cantMove(char[8][8], char);
extern int moveCounter;
extern bool isDeadPosition(char [8][8]);
extern void forfeit(char turn);
extern void offeringDraw(char turn);

extern char eatenWhite[16], eatenBlack[16];
extern int ewCtr,ebCtr;

//print chess board function
void printGrid(){
    for (int i=0;i<8;i++){
        printf("%d ",8-i);
        for(int j=0;j<8;j++){
            printf("%c ",grid[i][j]);
        }
        printf("\n");
    }
    printf("  ");
    for (int i=0 ; i<8;i++){
        printf("%c ",'A'+i);
    }
    printf("\n\n");
    printf("Eaten white pieces:");
    for(int i=0;i<=ewCtr;i++)
        printf("%c",eatenWhite[i]);

    printf("\nEaten black pieces:");
    for(int i=0;i<=ebCtr;i++)
        printf("%c",eatenBlack[i]);

}

//read movements
void readMovements(){
    //we check if there is any valid move if not( we  see if its on check if not on check then stalemate
    if(cantMove(grid, turn)){
        if(isCheck(grid, turn))
            printf("Checkmate!\n%s wins!", turn=='W'?"Black":"White");
        else
            printf("Stalemate!\nThe game is tied!");
        exit(0);
    }
    //counting moves for 50-move rule
    printf("\n%d\n",moveCounter);
    if(moveCounter>=100){
        printf("Fifty-move rule!\nThe game is tied!");
        exit(0);
    }
    //check if both teams cant win due to insuffecient pieces to win then draw
    if(isDeadPosition(grid)){
        printf("Dead position!\nThe game is tied!");
        exit(0);
    }
    //displayTurn
    printf("%s turn\n", turn=='W'?"White's":"Black's");
    //check if the king on check
    if(isCheck(grid, 'W'))
        printf("White's in check!\n");
    if(isCheck(grid, 'B'))
        printf("Black's in check!\n");
    while(1){
            //after checking on the previous now we can check for the input from the user
        memset(in, 0, sizeof(char)*8);
        printf("\nEnter your move: ");
        fflush(stdin);
        fgets(in,8,stdin);
        //check if the player want to save or load or undo or redo or offerdraw or resign
        if(in[0]=='S'&&strlen(in)==2)
            return save();
        if(in[0]=='L'&&strlen(in)==2)
            return load();
        if(in[0]=='U'&&strlen(in)==2)
            return undo();
        if(in[0]=='R'&&strlen(in)==2)
            return redo();
        if(in[0]=='D'&&strlen(in)==2)
            return offeringDraw(turn);
        if (in[0]=='F' && in[1]=='F'&&strlen(in)==3)
            return forfeit(turn);
        //ourcode run on lowercase inputs so if we got upper case we change it to lower case
        in[0]=getLower(in[0]);
        in[2]=getLower(in[2]);
        in[4]=getLower(in[4]);
        //check if the input valid
        if(!isValidInput(grid,in)){
            printf("\nIncorrect format!\nPlease enter input in the form X#X#\nWhere X indicates the column letter and # indicates the row number\n");
            printf("In case of promotion enter the initial of the piece to promote as the last argument\n");
            printf("for 'save'/'load' please enter 'S'/'L' for 'undo'/'redo' please enter 'U'/'R'\nfor 'offering draw'/'resign' please enter 'D'/'FF'Note:uppercase for special options\n");
            continue;//we need to check from the beginning for new input
        }
        //check if its legal move
        if(!isValid(grid, in, turn, 2))
            printf("Invalid Move!\n");
        else//passed all this cases now we can break our loop
            break;
    }
    //if it passed all those cases now safely we can move our pieces
    move(grid, getRow(in[1]), getCol(in[0]), getRow(in[3]), getCol(in[2]), in[4]);
    //put grid in 3d array to make undo redo and save later
    maintainGrid();
    //change turn
    turn=nextTurn(turn);
}

int main(int argc, char** argv)
{   //put first game in the 3d array to make undo ,redo and save
    maintainGrid();
    GUI();
    return 0;
}
