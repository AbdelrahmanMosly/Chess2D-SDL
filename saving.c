#include <stdio.h>
#include <windows.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

extern int hasMoved[8][8];
extern char grid[8][8];
extern char turn;
extern char eatenWhite[16], eatenBlack[16];
extern int ewCtr,ebCtr;
extern char nextTurn(char);
extern int moveCounter;
extern void maintainGrid();

char choice[8];
char statesGrid[8][8][12000];
int  statesMoveCounter[12000], stateshasMoved[8][8][12000];
char statesEatenW[16][12000], statesEatenB[16][12000];
int ctr, mnCtr=1, mxCtr;

bool messageBoxAnswer(const char *title,const char *body)
{
    const SDL_MessageBoxButtonData buttons[] =
    {
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "no" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
    };
    const SDL_MessageBoxColorScheme colorScheme =
    {
        {   { 255,   0,   0 },
            {   0, 255,   0 },
            { 255, 255,   0 },
            {   0,   0, 255 },
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata =
    {
        SDL_MESSAGEBOX_INFORMATION,
        NULL,
        title,
        body,
        SDL_arraysize(buttons),
        buttons,
        &colorScheme
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0)
    {
        SDL_Log("error displaying message box");
        return false;
    }
    if (buttonid == -1)
    {
        SDL_Log("no selection");
        return false;
    }
    else
        return(buttons[buttonid].text[0]=='y'?true:false);
}

//after making sure the user want to save the file we save it in savedGame.txt file
//putting the grid and the turn
void save()
{
    if(messageBoxAnswer("Save Game","Are you sure you want to save the current game?"))
    {
        FILE* fptr = fopen("savedGame.txt", "w+");
        if(fptr==NULL)
        {
            printf("Saving failed\n");
            return;
        }
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
                fprintf(fptr, "%c ",grid[i][j]);
            fprintf(fptr, "\n");
        }
        fprintf(fptr, "%c\n", turn);
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
                fprintf(fptr, "%d ",hasMoved[i][j]);
            fprintf(fptr, "\n");
        }
        fprintf(fptr, "%d %d %d %d\n", ctr, moveCounter,ewCtr,ebCtr);
        for(int i=0;i<16;i++)
            fprintf(fptr, "%c ", eatenWhite[i]);
        fprintf(fptr,"\n");
        for(int i=0;i<16;i++)
            fprintf(fptr, "%c ", eatenBlack[i]);
        printf("Saved Successfully!\n");
        fclose(fptr);
        return;
    }
    else
    {
        printf("Saving cancelled\n");
        return;
    }

}
//after making sure the user want to load the file we load it from savedGame.txt file
//getting the grid and the turn
void load()
{
    FILE* fptr = fopen("savedGame.txt", "r+");
    if(fptr==NULL)
    {
        printf("Load failed\n");
        return;
    }

    if(messageBoxAnswer("Load Game","Are you sure you want to Load the game?"))
    {
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
                fscanf(fptr, "%c ", &grid[i][j]);
            fscanf(fptr, "\n");
        }
        fscanf(fptr, "%c\n", &turn);
        for (int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
                fscanf(fptr, "%d ",&hasMoved[i][j]);
            fscanf(fptr, "\n");
        }
        fscanf(fptr, "%d %d %d %d\n", &ctr, &moveCounter, &ewCtr, &ebCtr);
        for(int i=0;i<16;i++)
            fscanf(fptr, "%c ", eatenWhite+i);
        fscanf(fptr,"\n");
        for(int i=0;i<16;i++)
            fscanf(fptr, "%c ", eatenBlack+i);
        mnCtr=ctr--;
        maintainGrid();
        printf("Loaded Successfully!\n");
        fclose(fptr);
        return;
    }
    else
    {
        printf("Loaded Cancelled!\n");
        return;
    }

}
//after making sure the user want to undo the move we undo the move by getting the previous game state
void undo()
{
    if(ctr==mnCtr)
    {
        printf("No more moves to undo!\n");
        return;
    }
    ctr--;

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            grid[i][j]=statesGrid[i][j][ctr];

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            hasMoved[i][j]=stateshasMoved[i][j][ctr];

    for(int i=0; i<16; i++)
        eatenWhite[i]=statesEatenW[i][ctr];
    for(int i=0; i<16; i++)
        eatenBlack[i]=statesEatenB[i][ctr];
    ewCtr=strlen(eatenWhite);
    ebCtr=strlen(eatenBlack);
    moveCounter=statesMoveCounter[ctr];
    turn=nextTurn(turn);
    printf("Undone Successfully!\n");
}
//after making sure the user want to redo the move we redo the move by getting the undone game state
void redo()
{
    if(ctr==mxCtr)
    {
        printf("No more moves to redo!\n");
        return;
    }
    ctr++;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            grid[i][j]=statesGrid[i][j][ctr];

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            hasMoved[i][j]=stateshasMoved[i][j][ctr];

    for(int i=0; i<16; i++)
        eatenWhite[i]=statesEatenW[i][ctr];
    for(int i=0; i<16; i++)
        eatenBlack[i]=statesEatenB[i][ctr];
    ewCtr=strlen(eatenWhite);
    ebCtr=strlen(eatenBlack);
    moveCounter=statesMoveCounter[ctr];
    turn=nextTurn(turn);
    printf("Redone Successfully!\n");
}

//saving our game in 3d array where 3rd index indicate the turn number
void maintainGrid()
{
    ctr++;
    mxCtr=ctr;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            statesGrid[i][j][ctr]=grid[i][j];
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            stateshasMoved[i][j][ctr]=hasMoved[i][j];

    int lenW=strlen(eatenWhite), lenB=strlen(eatenBlack);
    for(int i=0; i<lenW; i++)
        statesEatenW[i][ctr]=eatenWhite[i];
    for(int i=0; i<lenB; i++)
        statesEatenB[i][ctr]=eatenBlack[i];

    statesMoveCounter[ctr]=moveCounter;
}
