SDL_Texture *WhitesTurn_texture;
SDL_Texture *BlacksTurn_texture;
SDL_Rect turnDisplay_POS;

void displayTurnsSetup(SDL_Renderer *renderer){
      WhitesTurn_texture=getTexture("assets/Turns display/White'sTurn.png",renderer);
      BlacksTurn_texture=getTexture("assets/Turns display/Black'sTurn.png",renderer);

      turnDisplay_POS.x=190;
      turnDisplay_POS.y=650;
      turnDisplay_POS.w=200;
      turnDisplay_POS.h=40;
}
void displayTurnRenderer(SDL_Renderer *renderer,char turn){
    if(turn=='W'){
        SDL_RenderCopy(renderer,WhitesTurn_texture,NULL,&turnDisplay_POS);
    }
    else{
        SDL_RenderCopy(renderer,BlacksTurn_texture,NULL,&turnDisplay_POS);
    }
}
