#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern char turn = 'W';
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
extern bool isEmpty(char grid[8][8], int, int);
extern char getLower(char);
extern char getSide(char);
extern char nextTurn(char);
extern bool isValid(char grid[8][8], char[], char, int);
extern char getColor(int, int);

#endif // VARIABLES_H_INCLUDED
