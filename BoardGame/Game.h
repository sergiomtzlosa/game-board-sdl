//
//  Game.h
//  BoardGame
//
//  Created by sid on 08/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#ifndef __BoardGame__Game__
#define __BoardGame__Game__

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "SMLog.h"
#include "SoundManager.h"
#include "LoadAssets.h"

#ifdef __APPLE__

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#else

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#endif

#endif

#define kTotalRows 8

#define kTotalCols 8

#define kBaseTilePositionX 327

#define kBaseTilePositionY 100

#define kMaxTile 5

#define kOffsetPosition 44

#define kPoints 10

#define kTimeCountDown 60

typedef enum
{
    TYPE_SQUARE_NONE = -1,
    TYPE_SQUARE_RED = 0,
    TYPE_SQUARE_BLUE = 1,
    TYPE_SQUARE_GREEN = 2,
    TYPE_SQUARE_PURPLE = 3,
    TYPE_SQUARE_YELLOW = 4
}TYPE_SQUARE;

typedef struct Tile
{
    int colValue;
    int rowValue;
    int value;
    TYPE_SQUARE type;
    bool tint;
} Tile;

typedef struct CRect
{
    int x;
    int y;
    int width;
    int height;
} CRect;

using namespace std;

class Game
{
    
public:
    
    void RenderGame(SDL_Renderer *renderObject, SDL_Window *window);
	void DrawBoard();
    void SetEvent(SDL_Event *ev);
    
    ~Game();
    Game();
    
private:
    
    //tile board
    Tile board[kTotalRows][kTotalCols];
    
    //start&stop stuff
    bool startGame = false;
    int startX, startY, startWidth, startHeight;
    int stopX, stopY, stopWidth, stopHeight;
    
    void SetStartText();
    void SetStopText();
    
    //Rendering game variables
    SDL_Renderer *renderer;
    SDL_Window *win;
    SDL_Event *event;
    
    //Timer stuff
    float deltaTime = 0.0;
    int thisTime = 0;
    int lastTime = 0;
    
    //Private methods: init and private rendering
    void SetBackground(SDL_Window *win, SDL_Renderer *renderer);
    void SetFixedText();
    void SetScore(int scoreValue);
    void SetTime(int timeValue);
    void InitBoard();
    void DrawTileBoard(Tile tile);
    void FillBoardArray();
    void StartBoard();
    void TrackEvent(SDL_Event *e);
    void ResetBoard();
    void StartTimer();
    int GetNewValueForBoard(int row, int col);
    bool CanMakeMoves();
    void SetResetBoardText();
    
    //Drawing text method
    CRect DrawText(const char *text, CRect srcRect, CRect dstRect, int fontSize, const char *fontName);
    
    //reset boards without movements
    bool canMove;
    int resetBWidth, resetBHeight, resetBX, resetBY;
    
    //Swap tiles
    Tile baseTile;
    Tile swapTile;
    Tile tempSwap, tempBase;
    bool touches = false;
    vector<Tile> hCheck;
    vector<Tile> vCheck;
    
    void CheckBoard(int posX, int posY);
    bool CheckDestroy(Tile tile);
    void DestroyTiles();
    TYPE_SQUARE GetNewRandomType();
    TYPE_SQUARE CheckForDifferentType(Tile oldTile);
    void ShowInvalidSwapText();
    void SearchTileCombinationToDestroy();
    
    //game over variables
    bool gameOver;
    int retryWidth, retryHeight, retryX, retryY;
    
    void CreateGameOver();
    bool IsPointInRect(int x, int y, int rPointX, int rPointY, int rWidth, int rHeight);
    
    //Inline helper methods
    static inline string ConvertIntToString(int number)
    {
        std::ostringstream ss;
        ss << number;
        
        return ss.str(); //return a string with the contents of the stream
    }
    
    static inline bool SortingVectorCols(const Tile& key1, const Tile& key2)
    {
        return key1.colValue < key2.colValue;
    }
    
    static inline bool SortingVectorRows(const Tile& key1, const Tile& key2)
    {
        return key1.rowValue < key2.rowValue;
    }
    
    static inline SDL_Surface* SDL_CopySurface(SDL_Surface* src)
    {
        return SDL_ConvertSurface(src, src->format, SDL_SWSURFACE);
    }
};

#endif /* defined(__BoardGame__Game__) */
