#define BOARD_WIDTH 600
#define BOARD_HEIGHT 600
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define OFFSET_X 20
#define OFFSET_Y 17

SDL_Texture * saveButton_texture;
SDL_Rect saveButtonImage_POS;
bool isChoosingSaveButton;

SDL_Texture * loadButton_texture;
SDL_Rect loadButtonImage_POS;
bool isChoosingLoadButton;

SDL_Texture * undoButton_texture;
SDL_Rect undoButtonImage_POS;
bool isChoosingUndoButton;

SDL_Texture * redoButton_texture;
SDL_Rect redoButtonImage_POS;
bool isChoosingRedoButton;

SDL_Texture * drawButton_texture;
SDL_Rect drawButtonImage_POS;
bool isChoosingDrawButton;

SDL_Texture * resignButton_texture;
SDL_Rect resignButtonImage_POS;
bool isChoosingResignButton;

SDL_Texture *getTexture(const char* file_name,SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}

void buttonsSetup(SDL_Renderer *renderer){
    saveButton_texture = getTexture("assets/Buttons/Save.png",renderer);
    loadButton_texture = getTexture("assets/Buttons/Load.png",renderer);
    undoButton_texture = getTexture("assets/Buttons/Undo.png",renderer);
    redoButton_texture = getTexture("assets/Buttons/Redo.png",renderer);
    drawButton_texture = getTexture("assets/Buttons/Draw.png",renderer);
    resignButton_texture = getTexture("assets/Buttons/Resign.png",renderer);
    int Buttons_width, Buttons_height;
    SDL_QueryTexture(saveButton_texture, NULL, NULL, &Buttons_width, &Buttons_height);
    saveButtonImage_POS.x = 30;
    saveButtonImage_POS.y = 620;
    saveButtonImage_POS.w = Buttons_width;
    saveButtonImage_POS.h = Buttons_height;

    loadButtonImage_POS.x = 160;
    loadButtonImage_POS.y = 620;
    loadButtonImage_POS.w = Buttons_width;
    loadButtonImage_POS.h = Buttons_height;

    undoButtonImage_POS.x = 290;
    undoButtonImage_POS.y = 620;
    undoButtonImage_POS.w = Buttons_width;
    undoButtonImage_POS.h = Buttons_height;

    redoButtonImage_POS.x = 420;
    redoButtonImage_POS.y = 620;
    redoButtonImage_POS.w = Buttons_width;
    redoButtonImage_POS.h = Buttons_height;

    drawButtonImage_POS.x = 30;
    drawButtonImage_POS.y = 650;
    drawButtonImage_POS.w = Buttons_width;
    drawButtonImage_POS.h = Buttons_height;

    resignButtonImage_POS.x = 420;
    resignButtonImage_POS.y = 650;
    resignButtonImage_POS.w = Buttons_width;
    resignButtonImage_POS.h = Buttons_height;
}

void buttonsRenderer(SDL_Renderer *renderer){
    SDL_RenderCopy(renderer,saveButton_texture,NULL,&saveButtonImage_POS);
      if(isChoosingSaveButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&saveButtonImage_POS);
    }
    SDL_RenderCopy(renderer,loadButton_texture,NULL,&loadButtonImage_POS);
      if(isChoosingLoadButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&loadButtonImage_POS);
    }
    SDL_RenderCopy(renderer,undoButton_texture,NULL,&undoButtonImage_POS);
      if(isChoosingUndoButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&undoButtonImage_POS);
    }

    SDL_RenderCopy(renderer,redoButton_texture,NULL,&redoButtonImage_POS);
     if(isChoosingRedoButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&redoButtonImage_POS);
    }
    SDL_RenderCopy(renderer,drawButton_texture,NULL,&drawButtonImage_POS);
      if(isChoosingDrawButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&drawButtonImage_POS);
    }
    SDL_RenderCopy(renderer,resignButton_texture,NULL,&resignButtonImage_POS);
      if(isChoosingResignButton){
        SDL_SetRenderDrawColor(renderer,255,255,0,255);
        SDL_RenderDrawRect(renderer,&resignButtonImage_POS);
    }
}
