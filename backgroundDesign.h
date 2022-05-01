SDL_Texture *whitePiecesLabel_texture;
SDL_Texture *blackPiecesLabel_texture;
SDL_Texture *nameTag_texture;
SDL_Texture *chessQuote_texture;
SDL_Rect whitePiecesLabel_POS;
SDL_Rect blackPiecesLabel_POS;
SDL_Rect nameTag_POS;
SDL_Rect chessQuote_POS;

void BackgroundDesignSetup(SDL_Renderer *renderer)
{
        whitePiecesLabel_texture=getTexture("assets/WhitePieces.png",renderer);
        blackPiecesLabel_texture=getTexture("assets/BlackPieces.png",renderer);
        nameTag_texture=getTexture("assets/nameTag.png",renderer);
        chessQuote_texture=getTexture("assets/ChessQuote.png",renderer);

        whitePiecesLabel_POS.x=650;
        whitePiecesLabel_POS.y=10;
        whitePiecesLabel_POS.w=160;
        whitePiecesLabel_POS.h=40;

        blackPiecesLabel_POS.x=830;
        blackPiecesLabel_POS.y=10;
        blackPiecesLabel_POS.w=160;
        blackPiecesLabel_POS.h=40;

        nameTag_POS.x=625;
        nameTag_POS.y=630;
        nameTag_POS.w=350;
        nameTag_POS.h=70;

        chessQuote_POS.x=625;
        chessQuote_POS.y=550;
        chessQuote_POS.w=350;
        chessQuote_POS.h=80;



}
void BackgroundDesignRenderer(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer,whitePiecesLabel_texture,NULL,&whitePiecesLabel_POS);
    SDL_RenderCopy(renderer,blackPiecesLabel_texture,NULL,&blackPiecesLabel_POS);
    SDL_RenderCopy(renderer,nameTag_texture,NULL,&nameTag_POS);
    SDL_RenderCopy(renderer,chessQuote_texture,NULL,&chessQuote_POS);

}


