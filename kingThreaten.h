SDL_Texture *threatenKing_texture;
void kingThreatenSetup(SDL_Renderer *renderer){
    threatenKing_texture=getTexture("assets/threaten King.png",renderer);

}
void kingThreatenRenderer(SDL_Renderer *renderer,char grid[8][8],char turn){

    int rowK;
    int colK;
    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++){
        if(!isEmpty(grid, i, j)
           &&getLower(grid[i][j])=='k'
           &&getSide(grid[i][j])==turn){
               rowK=i, colK=j;
               break;
        }
    }

    SDL_Rect kingCell= {OFFSET_X+(BOARD_WIDTH/8)*colK,(BOARD_HEIGHT/8)*rowK,BOARD_WIDTH/8,BOARD_HEIGHT/8};
    SDL_SetRenderDrawColor(renderer, 255,0,0,0);
    SDL_RenderFillRect(renderer, &kingCell);

}
