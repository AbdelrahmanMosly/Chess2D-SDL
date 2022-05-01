#define BOARD_WIDTH 600
#define BOARD_HEIGHT 600
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define OFFSET_X 20
#define OFFSET_Y 17

SDL_Texture* lettersT[8];
SDL_Texture* numbersT[8];
SDL_Rect lettersTPOS[8];
SDL_Rect numbersTPOS[8];

void coordinateSetup(SDL_Renderer *renderer)
{
    char filename[100];
    for(int i=0;i<8;i++){
        snprintf(filename,25,"assets/Coordinates/%c.png", (char)'A'+i);
        lettersT[i]=getTexture(filename,renderer);
        snprintf(filename,25,"assets/Coordinates/%d.png", i+1);
        numbersT[i]=getTexture(filename,renderer);
    }
    int coordiantes_width, coordiantes_height;
    SDL_QueryTexture(lettersT[0], NULL, NULL, &coordiantes_width, &coordiantes_height);
    //position numbers
    for(int i=0;i<8;i++){
        numbersTPOS[i].x=0;
        numbersTPOS[i].y=(BOARD_HEIGHT/8)*(8-i)-2*coordiantes_height;
        numbersTPOS[i].w=coordiantes_width;
        numbersTPOS[i].h=coordiantes_height;
    }
    //position the letters
    for(int i=0;i<8;i++){
        lettersTPOS[i].x=(BOARD_WIDTH/8)*i+2*coordiantes_width;
        lettersTPOS[i].y=OFFSET_Y+BOARD_HEIGHT-coordiantes_height;
        lettersTPOS[i].w=coordiantes_width;
        lettersTPOS[i].h=coordiantes_height;
    }
}

void coordinateRenderer(SDL_Renderer *renderer){
    for(int i=0;i<8;i++)
        SDL_RenderCopy(renderer,numbersT[i],NULL,&numbersTPOS[i]);
    for(int i=0;i<8;i++)
        SDL_RenderCopy(renderer,lettersT[i],NULL,&lettersTPOS[i]);
}
