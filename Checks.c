#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <SDL2/SDL.h>

extern void displayEaten(SDL_Renderer*, char[16]);
extern bool isEmpty(char grid[8][8], int, int);
extern char getLower(char);
extern char getSide(char);
extern char nextTurn(char);
extern bool isValid(char grid[8][8], char[], char, int);
extern char getColor(int, int);

//first we search for the king place
//then we see all possible move that threaten the king
bool isCheck(char grid[8][8], char turn){
    int rowK;
    char colK;
    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
        if(!isEmpty(grid, i, j)
           &&getLower(grid[i][j])=='k'
           &&getSide(grid[i][j])==turn){
               rowK=7-i, colK='a'+j;
               break;
        }
    }
    bool flag=false;
    for(char colX='a';colX<='h';colX++){
        for(int rowX=0;rowX<=7;rowX++){
            char trial[5]={colX, '1'+rowX, colK, '1'+rowK};
            flag|=isValid(grid, trial, nextTurn(turn), 0);
        }
    }


    return flag;
}
//we see all the possible moves for all the pieces on the grid if no move are possible then we return true
//no move are there
//if commented out the lines in comment will show possible moves
bool cantMove(char grid[8][8], char turn){
    //printf("Valid Moves:\n");
    bool flag=true;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            for(int k=0;k<8;k++)
                for(int l=0;l<8;l++){
                    char trial[5]={'a'+i, '1'+j, 'a'+k, '1'+l};
                    bool currentF=isValid(grid, trial, turn, 1);
                    //if(currentF)
                    //    printf("%s\n", trial);
                    flag&=!currentF;
                }
    return flag;
}
