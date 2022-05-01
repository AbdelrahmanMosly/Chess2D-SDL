#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Buttons.h"
#include "Coordinates.h"
#include "DisplayTurns.h"
#include "backgroundDesign.h"

extern void displayEaten(SDL_Renderer*,char[16]);
extern char eatenWhite[16];
extern char eatenBlack[16];
extern int ctr;
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
extern bool isEmpty(char[8][8], int,int);
extern bool isCheck(char[8][8], char);
extern char nextTurn(char);
extern bool cantMove(char[8][8], char);
extern int moveCounter;
extern bool isDeadPosition(char [8][8]);
extern void forfeit(char turn);
extern void offeringDraw(char turn);
extern void coordinateSetup(SDL_Renderer*);
extern void coordinateRenderer(SDL_Renderer*);
extern char grid[8][8];
extern char getSide(char);
extern char turn;
extern int moveCounter;
extern int possibleCellCounter;

char* lastCell;
char lastPiece;
SDL_Renderer* renderer;

SDL_Window* window;

#include "kingThreaten.h"


char* getCell(int x,int y)
{
    x-=OFFSET_X;
    int row=7-y/(BOARD_HEIGHT/8),
        col=x/(BOARD_WIDTH/8);
    char* pos=malloc(4);
    pos[0]='a'+col;
    pos[1]='1'+row;
    pos[2]=pos[3]='\0';
    return pos;
}

void renderGrid(SDL_Renderer *renderer)
{
    SDL_RenderClear(renderer);
    SDL_Rect cell= {0,0,BOARD_WIDTH/8,BOARD_HEIGHT/8};
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            cell.x=OFFSET_X+BOARD_WIDTH/8*i;
            cell.y=BOARD_HEIGHT/8*j;
            if((i+j)%2)
                SDL_SetRenderDrawColor(renderer, 32,32,32,255);
            else
                SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            SDL_RenderFillRect(renderer, &cell);
        }
    }
    return;
}

char pieces[]="RNBQKPrnbqkp";
char* filename;
SDL_Texture* imgs[16];

int indexOf(char p)
{
    for(int i=0; i<12; i++)
        if(pieces[i]==p)
            return i;
    exit(0);
}

void getFileName(char* filename, char piece)
{
    strcpy(filename,"assets\\");
    if(getSide(piece)=='W')
        strcat(filename,"w_");
    else
        strcat(filename,"b_");
    switch(tolower(piece))
    {
    case 'p':
        strcat(filename,"pawn");
        break;
    case 'r':
        strcat(filename,"rook");
        break;
    case 'n':
        strcat(filename,"knight");
        break;
    case 'b':
        strcat(filename,"bishop");
        break;
    case 'q':
        strcat(filename,"queen");
        break;
    case 'k':
        strcat(filename,"king");
        break;
    }
    strcat(filename,"_1x.png");
    return;
}

SDL_Texture* loadImage(SDL_Renderer *renderer, char piece)
{
    getFileName(filename, piece);
    SDL_Surface* screen = IMG_Load(filename);
    SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, screen);
    return image;
}

void renderImage(SDL_Renderer *renderer, char piece, SDL_Rect cell)
{
    SDL_RenderCopy(renderer, imgs[indexOf(piece)], NULL, &cell);
}


void GUI()
{
    lastCell=malloc(4);
    IMG_Init(IMG_INIT_PNG);
    SDL_Init(SDL_INIT_EVERYTHING);
    window=SDL_CreateWindow("Chess project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer=SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    buttonsSetup(renderer);
    coordinateSetup(renderer);
    kingThreatenSetup(renderer);
    displayTurnsSetup(renderer);
    BackgroundDesignSetup(renderer);

    filename = malloc(100);

    for(int i=0; i<12; i++)
        imgs[i]=loadImage(renderer, pieces[i]);
    bool gameRunning=true, holding=false;
    int renderedEnd=0;
    while(gameRunning&&renderedEnd<2)
    {
        int mousePosX=0, mousePosY=0;
        //background
        SDL_SetRenderDrawColor(renderer,192,192,192,255);
        SDL_RenderClear(renderer);
        renderGrid(renderer);

        BackgroundDesignRenderer(renderer);
        buttonsRenderer(renderer);
        coordinateRenderer(renderer);
        displayTurnRenderer(renderer,turn);
        displayEaten(renderer,eatenWhite);
        displayEaten(renderer,eatenBlack);
        displayPossibleMove_Graphics(renderer);

        if(cantMove(grid, turn))
        {
            renderedEnd++;
            if(renderedEnd==2&&isCheck(grid, turn))
            {
                if(turn=='W')
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Checkmate!","Checkmate!\nBlack wins!",window);
                else
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Checkmate!","Checkmate!\nWhite wins!",window);
            }
            else if(renderedEnd==2&&!isCheck(grid, turn))
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Stalemate!","Stalemate!\nThe game is tied!",window);
        }
        if(moveCounter>=100)
        {
            if(renderedEnd++)
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Tie!","Fifty-move rule!\nThe game is tied!",window);
        }
        //check if both teams cant win due to insuffecient pieces to win then draw
        if(isDeadPosition(grid))
        {
            if(renderedEnd++)
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Tie!","Dead position!\nThe game is tied!",window);
        }
        if(isCheck(grid,turn)){
            kingThreatenRenderer(renderer,grid,turn);
        }



        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                char piece=grid[i][j];
                if(!isalpha(piece)||(getRow(lastCell[1])==i&&getCol(lastCell[0])==j&&holding))
                    continue;
                SDL_Rect cell= {OFFSET_X+BOARD_WIDTH/8*j,BOARD_HEIGHT/8*i,BOARD_WIDTH/8,BOARD_HEIGHT/8};
                renderImage(renderer, piece, cell);
            }
        }
        if(holding)
        {
            SDL_GetMouseState(&mousePosX,&mousePosY);
            SDL_Rect cell= {mousePosX-BOARD_WIDTH/16,mousePosY-BOARD_HEIGHT/16,BOARD_WIDTH/8,BOARD_HEIGHT/8};
            renderImage(renderer, lastPiece, cell);
        }
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                gameRunning=false;
                break;
            case SDL_MOUSEMOTION:
                mousePosX=event.motion.x;
                mousePosY=event.motion.y;
                if(!(mousePosX>OFFSET_X&&mousePosX<OFFSET_X+BOARD_WIDTH&& mousePosY>0 && mousePosY<BOARD_HEIGHT)){
                    holding=false;
                    possibleCellCounter=0;
                }

                if(mousePosX>saveButtonImage_POS.x && mousePosX<saveButtonImage_POS.x+saveButtonImage_POS.w && mousePosY>saveButtonImage_POS.y && mousePosY<saveButtonImage_POS.y+saveButtonImage_POS.h)
                    isChoosingSaveButton=true;
                else
                    isChoosingSaveButton=false;
                if(mousePosX>loadButtonImage_POS.x && mousePosX<loadButtonImage_POS.x+loadButtonImage_POS.w && mousePosY>loadButtonImage_POS.y && mousePosY<loadButtonImage_POS.y+loadButtonImage_POS.h)
                    isChoosingLoadButton=true;
                else
                    isChoosingLoadButton=false;
                if(mousePosX>undoButtonImage_POS.x && mousePosX<undoButtonImage_POS.x+undoButtonImage_POS.w && mousePosY>undoButtonImage_POS.y && mousePosY<undoButtonImage_POS.y+undoButtonImage_POS.h)
                    isChoosingUndoButton=true;
                else
                    isChoosingUndoButton=false;
                if(mousePosX>redoButtonImage_POS.x && mousePosX<redoButtonImage_POS.x+redoButtonImage_POS.w && mousePosY>redoButtonImage_POS.y && mousePosY<redoButtonImage_POS.y+redoButtonImage_POS.h)
                    isChoosingRedoButton=true;
                else
                    isChoosingRedoButton=false;
                if(mousePosX>drawButtonImage_POS.x && mousePosX<drawButtonImage_POS.x+drawButtonImage_POS.w && mousePosY>drawButtonImage_POS.y && mousePosY<drawButtonImage_POS.y+drawButtonImage_POS.h)
                    isChoosingDrawButton=true;
                else
                    isChoosingDrawButton=false;
                if(mousePosX>resignButtonImage_POS.x && mousePosX<resignButtonImage_POS.x+resignButtonImage_POS.w && mousePosY>resignButtonImage_POS.y && mousePosY<resignButtonImage_POS.y+resignButtonImage_POS.h)
                    isChoosingResignButton=true;
                else
                    isChoosingResignButton=false;

                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mousePosX, &mousePosY);
                char* cell=malloc(4);
                strcpy(cell,getCell(mousePosX,mousePosY));
                int row=-1,col=-1;
                if(mousePosX>OFFSET_X&&mousePosX<OFFSET_X+BOARD_WIDTH&& mousePosY>0 && mousePosY<BOARD_HEIGHT){
                    row=getRow(cell[1]), col=getCol(cell[0]);
                }
                else{
                    holding=false;
                }
                if(mousePosX>saveButtonImage_POS.x && mousePosX<saveButtonImage_POS.x+saveButtonImage_POS.w && mousePosY>saveButtonImage_POS.y && mousePosY<saveButtonImage_POS.y+saveButtonImage_POS.h)
                {
                    save();
                    break;
                }
                if(mousePosX>loadButtonImage_POS.x && mousePosX<loadButtonImage_POS.x+loadButtonImage_POS.w && mousePosY>loadButtonImage_POS.y && mousePosY<loadButtonImage_POS.y+loadButtonImage_POS.h)
                {
                    load();
                    break;
                }
                if(mousePosX>undoButtonImage_POS.x && mousePosX<undoButtonImage_POS.x+undoButtonImage_POS.w && mousePosY>undoButtonImage_POS.y && mousePosY<undoButtonImage_POS.y+undoButtonImage_POS.h)
                {
                    undo();
                    break;
                }
                if(mousePosX>redoButtonImage_POS.x && mousePosX<redoButtonImage_POS.x+redoButtonImage_POS.w && mousePosY>redoButtonImage_POS.y && mousePosY<redoButtonImage_POS.y+redoButtonImage_POS.h)
                {
                    redo();
                    break;
                }
                if(mousePosX>drawButtonImage_POS.x && mousePosX<drawButtonImage_POS.x+drawButtonImage_POS.w && mousePosY>drawButtonImage_POS.y && mousePosY<drawButtonImage_POS.y+drawButtonImage_POS.h)
                {
                    offeringDraw(turn);
                    break;
                }
                if(mousePosX>resignButtonImage_POS.x && mousePosX<resignButtonImage_POS.x+resignButtonImage_POS.w && mousePosY>resignButtonImage_POS.y && mousePosY<resignButtonImage_POS.y+resignButtonImage_POS.h)
                {
                    forfeit(turn);
                    break;
                }
                if(row!=-1 && col!=-1){
                    if(!isEmpty(grid,row,col)&&getSide(grid[row][col])==turn&&!holding)
                    {
                        holding=true;
                        lastPiece=grid[row][col];
                        strcpy(lastCell,cell);
                        getPossibleMove_Graphics(grid,lastCell,turn);
                    }
                    else if(holding)
                    {
                        char trial[5]= {lastCell[0],lastCell[1],cell[0],cell[1]};
                        possibleCellCounter=0;
                        if(isValid(grid,trial,turn,2))
                        {
                            printf("Move: %s\n",trial);
                            move(grid,getRow(lastCell[1]),getCol(lastCell[0]),row,col,0);
                            maintainGrid();
                            turn=nextTurn(turn);
                        }
                        else
                            grid[getRow(lastCell[1])][getCol(lastCell[0])]=lastPiece;
                        holding=false;
                    }
                }
                break;
            }
        }

        SDL_RenderPresent(renderer);
        //printGrid();
        SDL_Delay(1000/60);
        //system("cls");
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
