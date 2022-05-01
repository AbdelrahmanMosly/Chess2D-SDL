#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define BOARD_WIDTH 600
#define BOARD_HEIGHT 600
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define OFFSET_X 20
#define OFFSET_Y 17

extern void renderImage(SDL_Renderer*,char,SDL_Rect);
extern char getSide(char);
extern char eatenWhite[16];
extern char eatenBlack[16];
extern char turn;

void displayEaten(SDL_Renderer* renderer, char eaten[16]){
    for(int i=0;i<16;i++){
        if(!eaten[i])
            continue;
        SDL_Rect cell = {3*OFFSET_X+BOARD_WIDTH+BOARD_WIDTH/10*(i>=8)+3*BOARD_WIDTH/10*(getSide(eaten[i])=='B')
                        ,3*OFFSET_Y+i*BOARD_HEIGHT/10-(i>=8)*BOARD_HEIGHT/10*8
                        ,BOARD_WIDTH/10
                        ,BOARD_HEIGHT/10};
        renderImage(renderer, eaten[i], cell);
    }
}
