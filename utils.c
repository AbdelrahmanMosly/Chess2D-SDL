#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>

#define BOARD_WIDTH 600
#define BOARD_HEIGHT 600
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define OFFSET_X 20
#define OFFSET_Y 17

extern int promotionInput();
extern void doPromotion(char[8][8],int,int,int,int,char);
extern bool messageBoxAnswer(const char*,const char*);
extern bool isCheck(char[8][8], char);
extern char statesGrid[8][8][12000];
extern int ctr;
extern SDL_Window* window;
int ewCtr, ebCtr;
char eatenWhite[16], eatenBlack[16];
int hasMoved[8][8];
bool canCastle(char grid[8][8], int row, int col, int toRow, int toCol, char turn, int mainTurn);
char getLower(char c);
int moveCounter=0;
int possibleCellCounter;
SDL_Rect possibleCell[64];


//change the turn  if W which means white we change it to B which means black and vise versa
char nextTurn(char turn)
{
    return (turn=='W'?'B':'W');
}
//get square color from its index in the array
char getColor(int row, int col)
{
    return (row+col)%2?'W':'B';
}
//check if this index in the array contain something other than - .
bool isEmpty(char grid[8][8], int row, int col)
{
    return grid[row][col]=='.'||grid[row][col]=='-';
}
//if the letter is lowercase we know its white other wise its black
char getSide(char piece)
{
    if('a'<=piece && piece<='z')
        return 'W';
    return 'B';
}
//we check if the input of the promotion is right so we can call our doPromotion function
void isValidPromotionInput(char grid[8][8], int row, int col, int toRow, int toCol)
{
    char piece=grid[row][col];
    if(getLower(piece)=='p'&&((getSide(piece)=='W'&&toRow==0)||(getSide(piece)=='B'&&toRow==7))){
        char promote=promotionInput();
        doPromotion(grid,row,col,toRow,toCol,promote);
    }

}
//we check if the isValidPromotionInput true then we can safely do our promotion
void doPromotion(char grid[8][8], int row, int col, int toRow, int toCol, char promote)
{
    char piece=grid[row][col], promotion='\0';

    if(promote=='r')
        promotion='R';
    if(promote=='n')
        promotion='N';
    if(promote=='b')
        promotion='B';
    if(promote=='q')
        promotion='Q';

    if(getSide(piece)=='W')
        promotion=getLower(promotion);

    grid[row][col]=promotion;

}

//void function take 6 arguments grid and from coordinate and to coordinate and if there is a promote piece we promote it
//and simply move our piece as we already made sure it’s a valid move
void move(char grid[8][8], int row, int col, int toRow, int toCol, char promote)
{
    isValidPromotionInput(grid,row,col,toRow,toCol);
    hasMoved[row][col]=ctr;
    hasMoved[toRow][toCol]=ctr;
    moveCounter++;
    if(getLower(grid[row][col])=='p')
    {
        moveCounter=0;
    }
    if(!isEmpty(grid, toRow, toCol))
    {
        char piece=grid[toRow][toCol];
        if(getSide(piece)=='W')
            eatenWhite[ewCtr++]=piece;
        else
            eatenBlack[ebCtr++]=piece;
        moveCounter=0;
    }
    grid[toRow][toCol] = grid[row][col];
    grid[row][col]=getColor(row, col)=='W'?'.':'-';


}
//check if the pawn hasn't moved yet
bool notMovedPawn(int row, int col, int turn)
{
    return (turn=='W'&&row==6)||(turn=='B'&&row==1);
}
//we change the input to row to be used in grid
int getRow(char r)
{
    return 8-(r-'0');
}
//we change the input to column to be used in grid
int getCol(char c)
{
    return c-'a';
}
//we get lower case if its upper case if not we get it as it's
char getLower(char c)
{
    return c>='a'&&c<='z'?c:c-'A'+'a';
}
//we check if the input is valid
//handling special case for promotion input using helper function
bool isValidInput(char grid[8][8],char in[])
{
    char piece=grid[getRow(in[1])][getCol(in[0])];
    if(strlen(in)>5||(in[0]<'a'||in[0]>'z'||in[1]<'1'||in[1]>'9')
            ||(in[2]<'a'||in[2]>'z'||in[3]<'1'||in[3]>'9')||
            ((getLower(piece)=='p')&&((getSide(piece)=='W'&&getRow(in[3])==0)||(getSide(piece)=='B'&&getRow(in[3])==7))))
        return false;
    return true;
}
//we assume a pawn from the previous state of our game if we got a real pawn then we know it has just moved
//so we can get this pawn by enpassant
bool isEnPassent(char grid[8][8], int row, int col, int toRow, int toCol, char turn, int mainTurn)
{
    int sign=(turn=='W'?-1:1);
    if(toRow!=row+1*sign||abs(toCol-col)!=1)
        return false;
    char currentPawn=grid[toRow-sign][toCol];
    bool flag = getLower(currentPawn)=='p'&&getSide(currentPawn)!=turn&&hasMoved[toRow+sign][toCol]==ctr-1;
         flag&=(turn=='W'&&toRow==2)||((turn=='B'&&toRow==5));
    if(flag&&mainTurn==2)
    {
        if(turn=='B')
            eatenWhite[ewCtr++]='p';
        else
            eatenBlack[ebCtr++]='P';
        grid[toRow-sign][toCol]=getColor(row, col)=='W'?'-':'.';
    }
    return flag;
}
//check the legal moves for the pawn
//sign if its white move up as -1 and if black move down +1
//check if the pawn want to do enpassent and check if its legal move and allowed
//check if the move want to move 1 forward move or 2 if its not moved and check if those places are empty
//using isEmpty function
//check if it want to move diagonaly left or right using (abs(predifined function in math.h)) and if its different team
bool checkPawnMove(char grid[8][8], int row, int col, int toRow, int toCol, char turn, int mainTurn)
{
    int sign=(turn=='W'?-1:1);
    return  (   isEnPassent(grid, row, col, toRow, toCol, turn, mainTurn)
                ||((toRow==row+1*sign)
                   &&((toCol==col&&isEmpty(grid, toRow, toCol))
                      ||((int)abs(toCol-col)==1&&!isEmpty(grid, toRow, toCol)&&getSide(grid[toRow][toCol])!=getSide(grid[row][col]))))
                ||(toRow==row+2*sign&&toCol==col&&notMovedPawn(row, col, turn)&&isEmpty(grid, toRow, toCol)&&isEmpty(grid, toRow-sign, toCol)));
}
//we check if it move on same colomn or same row
bool checkRookMove(char grid[8][8], int row, int col, int toRow, int toCol)
{
    if(toRow!=row&&toCol!=col)
        return false;
    int dx[4]= {1,-1,0,0};
    int dy[4]= {0,0,1,-1};
    for(int j=0; j<4; j++)
    {
        for(int i=1; i<=8; i++)
        {
            if(row+i*dx[j]==toRow&&col+i*dy[j]==toCol)
                return true;
            if(!isEmpty(grid, row+i*dx[j], col+i*dy[j]))
                break;
        }
    }
    return false;
}
//we check if it move on different row and different col with same difference between each other
bool checkBishopMove(char grid[8][8], int row, int col, int toRow, int toCol)
{
    if(abs(toRow-row)!=abs(toCol-col))
        return false;
    int dx[4]= {1,1,-1,-1};
    int dy[4]= {1,-1,1,-1};
    for(int j=0; j<4; j++)
    {
        for(int i=1; i<=8; i++)
        {
            if(row+i*dx[j]==toRow&&col+i*dy[j]==toCol)
                return true;
            if(!isEmpty(grid, row+i*dx[j], col+i*dy[j]))
                break;
        }
    }
    return false;
}
//we see if it move 2 forward or 2 backward and one right or one left
//or we see if it move 2 right or 2 left and one left or one right
bool checkKnightMove(char grid[8][8], int row, int col, int toRow, int toCol)
{
    return (abs(toRow-row)==2&&abs(toCol-col)==1)||(abs(toRow-row)==1&&abs(toCol-col)==2);
}
//we use our function for rook and bishop to check the queen movement as it is a combination of both
bool checkQueenMove(char grid[8][8], int row, int col, int toRow, int toCol)
{
    return checkBishopMove(grid, row, col, toRow, toCol)||checkRookMove(grid, row, col, toRow, toCol);
}
//we see if the king want to move any direction for one square any direction
bool checkKingMove(char grid[8][8], int row, int col, int toRow, int toCol, int mainTurn)
{
    return (abs(toRow-row)<=1&&abs(toCol-col)<=1)||canCastle(grid,row,col,toRow,toCol,getSide(grid[row][col]),mainTurn);
}
//we check if the move is valid (legal)
//if we are catching empty index then it'snot valid
//then we check the possible move for this piece using check functions
//in case of ischeck we put mainturn=0 as we dont want to do the stuff in this condition in that case
//when the user input mainturn =2 and from program will be 1 or 0
bool isValid(char grid[8][8], char in[], char turn, int mainTurn)
{
    /*if(!isValidInput(grid,in))
        return false;*/

    int   row=getRow(in[1]),   col=getCol(in[0]),
          toRow=getRow(in[3]), toCol=getCol(in[2]);
    if(isEmpty(grid, row, col))
        return false;

    char piece=grid[row][col], toCell=grid[toRow][toCol];
    if(getSide(piece)!=turn||(!isEmpty(grid, toRow, toCol)&&getSide(toCell)==turn))
        return false;

    if(mainTurn)
    {
        grid[row][col]=(getColor(row, col)=='W'?'-':'.');
        grid[toRow][toCol]=piece;
        bool flag=isCheck(grid, turn);
        grid[toRow][toCol]=toCell;
        grid[row][col]=piece;
        if(flag)
            return false;
    }

    switch(getLower(piece))
    {
    case 'p':
        return checkPawnMove(grid, row, col, toRow, toCol, turn, mainTurn);
    case 'k':
        return checkKingMove(grid, row, col, toRow, toCol, mainTurn);
    case 'n':
        return checkKnightMove(grid, row, col, toRow, toCol);
    case 'b':
        return checkBishopMove(grid, row, col, toRow, toCol);
    case 'r':
        return checkRookMove(grid, row, col, toRow, toCol);
    case 'q':
        return checkQueenMove(grid, row, col, toRow, toCol);
    default:
        return false;
    }
}
//we check if the castling conditions meet
//the side the king going to castle is empty
//no check the king will pass over it
//the king and the rook hadn't been moved
bool canCastle(char grid[8][8], int row, int col, int toRow, int toCol, char turn, int mainTurn)
{
    bool flag;
    if(turn=='W')
    {
        if(row!=7||col!=4||toRow!=7||(toCol!=2&&toCol!=6))
            return false;
        if(toCol==2)
        {
            char trial[5]= {'e','1','d','1'};
            flag = !hasMoved[7][0]&&isEmpty(grid,7,1)&&isEmpty(grid,7,2)&&isEmpty(grid,7,3)&&!hasMoved[7][4]&&isValid(grid,trial,turn,1)&&!isCheck(grid,turn);
            if(flag&&mainTurn==2)
                move(grid,7,0,7,3,0);
        }
        else
        {
            char trial[5]= {'e','1','f','1'};
            flag = !hasMoved[7][4]&&isEmpty(grid,7,5)&&isEmpty(grid,7,6)&&!hasMoved[7][7]&&isValid(grid,trial,turn,1)&&!isCheck(grid,turn);
            if(flag&&mainTurn==2)
                move(grid,7,7,7,5,0);
        }
    }
    else
    {
        if(row!=0||col!=4||toRow!=0||(toCol!=2&&toCol!=6))
            return false;
        if(toCol==2)
        {
            char trial[5]= {'e','8','d','8'};
            flag = !hasMoved[0][0]&&isEmpty(grid,0,1)&&isEmpty(grid,0,2)&&isEmpty(grid,0,3)&&!hasMoved[0][4]&&isValid(grid,trial,turn,1)&&!isCheck(grid,turn);
            if(flag&&mainTurn==2)
                move(grid,0,0,0,3,0);
        }
        else
        {
            char trial[5]= {'e','8','f','8'};
            flag = !hasMoved[0][4]&&isEmpty(grid,0,5)&&isEmpty(grid,0,6)&&!hasMoved[0][7]&&isValid(grid,trial,turn,1)&&!isCheck(grid,turn);
            if(flag&&mainTurn==2)
                move(grid,0,7,0,5,0);
        }
    }
    if(flag&&mainTurn==2)
        moveCounter--;
    return flag;
}
//check for suffecient pieces for the player to win if both doesnt have that suffecient pieces we declare a draw.
bool isDeadPosition(char grid[8][8])
{

    int countKnightW=0;//count knight for White team
    int countKnightB=0;//count knight for black team
    int countBishopW=0;//count knight for white team
    int countBishopB=0;//count knight for black team

    int blackSquareColorBishops=0;//count bishops on black color (neglecting the side of this bishop)
    int whiteSquareColorBishops=0;//count bishops on white color (neglecting the side of this bishop)

    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
            //if we found any pawn or queen or rook on any side we return false as there a chance for win to any player
            if(getLower(grid[i][j])=='p'||getLower(grid[i][j])=='q'||getLower(grid[i][j])=='r')
            {
                return false;

            }
            //we check if we found knight or bishop and count it and put it in our counter as if this >= 2 we can return false for both teams
            //we divided both conditions as we can check if we found bishop check for its square color
            //because if the bishops on different colors we can see its suffecient pieces to win
            else if(getLower(grid[i][j])=='n')
            {
                if(getSide(grid[i][j])=='W')
                {
                    countKnightW++;
                }
                else if(getSide(grid[i][j])=='B')
                {
                    countKnightB++;
                }
            }
            else if(getLower(grid[i][j])=='b')
            {
                if(getSide(grid[i][j])=='W')
                {
                    countBishopW++;
                }
                else if(getSide(grid[i][j])=='B')
                {
                    countBishopB++;
                }
                if(getColor(i,j)=='W')  //get the square color
                {
                    whiteSquareColorBishops++;
                }
                else if(getColor(i,j)=='B')
                {
                    blackSquareColorBishops++;
                }
            }
            //check for pieces enough for the win
            if(countKnightB>=1&&(countBishopB>=1 ||countBishopW>=1 ||countKnightW>=1))
                return false;
             if(countKnightW>=1&&(countBishopB>=1 ||countBishopW>=1 ||countKnightB>=1))
                return false;
            if(countBishopB>=1&&(countKnightB>=1||countKnightW>=1))
                return false;
            if(countBishopW>=1&&(countKnightB>=1||countKnightW>=1))
                return false;
            if(whiteSquareColorBishops>=1&&blackSquareColorBishops>=1)
                return false;

        }
    }
    //if we didnt found anything of the previous so we cant reach win for both teams so we declare draw
    return true;

}
//called when player enters FF so he want resign we check if he really wants or just miss click
void forfeit(char turn)
{
    if(turn=='W')
    {
        if(messageBoxAnswer("Resign","White!\nAre you sure you want to resign?"))
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Resigned","White Resigned!\nBlack wins!",window);
            exit(0);
        }
        else
        {
            return;
        }
    }
    else
    {
        if(messageBoxAnswer("Resign","Black!\nAre you sure you want to resign?"))
        {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Resigned","Black Resigned!\nWhite wins!",window);
            exit(0);

        }
        else
        {
            return;
        }
    }
    // printf("%s Resigned!\n%s wins!",  turn=='W'?"White":"Black",turn=='W'?"Black":"White");

}

//checking if he want to draw or not or entered any other thing call it self again and ask for new input
/*void acceptingDraw(){
    char choice[8];
    memset(choice, 0, sizeof(char)*8);
    fflush(stdin);
    fgets(choice,8,stdin);
    if(strlen(choice)!=2||(choice[0]!='Y'&&choice[0]!='N')){
        printf("Invalid Choice!\n");
        return acceptingDraw();
    }
    if(choice[0]=='N'){
        printf("offering Draw refused\n");
        Sleep(1000);
        return ;
    }
    printf("Draw accepted!\nThe game is tied!");
    exit(0);
}
*/
//called when one of 2 player enters D in the input
void offeringDraw(char turn)
{
    if(turn=='W')
    {
        if(messageBoxAnswer("Draw Offer","White!\nAre you sure you want to offer draw?"))
        {
            if(messageBoxAnswer("Draw Offer","Black!\nWhite is offering Draw,Do you accept it?"))
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Tie","Both players decided to draw",window);
                exit(0);
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        if(messageBoxAnswer("Draw Offer","Black!\nAre you sure you want to offer draw?"))
        {
            if(messageBoxAnswer("Draw Offer","White!\nBlack is offering Draw,Do you accept it?"))
            {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Tie","Both players decided to draw",window);
                exit(0);
            }
        }
        else
        {
            return;
        }
    }

}


int promotionInput()
{
    const SDL_MessageBoxButtonData buttons[] =
    {
        { 0, 'n', "Knight" },
        { 0, 'b', "Bishop" },
        { 0, 'r', "Rook" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 'q', "Queen" }
    };
    const SDL_MessageBoxColorScheme colorScheme =
    {
        {
            { 255,   0,   0 },
            {   0, 255,   0 },
            { 255, 255,   0 },
            {   0,   0, 255 },
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata =
    {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        "Pawn Promotion", /* .title */
        "Select a piece to promote your pawn to", /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);
    printf("%d\n",buttonid);
    return buttonid;
}
//not involved due to deadline
void getPossibleMove_Graphics(char grid[8][8],char lastCell[],char turn){

        for(char colX='a';colX<='h';colX++){
            for(int rowX=0;rowX<=7;rowX++){
                    char trial[5]={lastCell[0],lastCell[1],colX,rowX+'1'};

                    if(isValid(grid,trial,turn,1)){
                        possibleCell[possibleCellCounter].x=(OFFSET_X+(BOARD_WIDTH/8)*getCol(colX))+2;
                        possibleCell[possibleCellCounter].y=((BOARD_HEIGHT/8)*getRow(rowX+'1'))+2;
                        possibleCell[possibleCellCounter].w=(BOARD_WIDTH/8)-4;
                        possibleCell[possibleCellCounter].h=(BOARD_HEIGHT/8)-4;
                        possibleCellCounter++;
                    }
                }
            }

}

void displayPossibleMove_Graphics(SDL_Renderer *renderer){
        for(int j=possibleCellCounter-1;j>=0;j--){
                        SDL_SetRenderDrawColor(renderer, 0,125,0,0);
                        SDL_RenderFillRect(renderer, &possibleCell[j]);
            }
}
