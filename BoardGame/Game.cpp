//
//  Game.cpp
//  BoardGame
//
//  Created by sid on 08/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "Game.h"
#include "Tint.h"
#include "CAnimation.h"

static string tiles[kMaxTile] = { "Red.png", "Blue.png", "Green.png", "Purple.png", "Yellow.png" };

static int score = 0;

static int timeout = kTimeCountDown;

Game::~Game()
{
    
}

Game::Game()
{
    InitBoard();
    StartBoard();
    gameOver = false;
}

/**
 *	Fill board with random values
 */
void Game::StartBoard()
{   
    FillBoardArray();
}

/**
 *	Start all visual games and start game
 *
 *	@param	renderObject	Object to render surfaces and all other objects
 *	@param	window	Window that holds the game
 */
void Game::RenderGame(SDL_Renderer *renderObject, SDL_Window *window)
{
    this->renderer = renderObject;
    this->win = window;
    
    SetBackground(win, renderer);
    SetFixedText();
    SetScore(0);
    SetTime(timeout);

    if (startGame)
    {
        SetStopText();
        StartTimer();
        CreateGameOver();
        
        if (!CanMakeMoves())
        {
            SMLog2("no movements left!!!, resetting board....");
            SetResetBoardText();
        }
    }
    else
    {
        SetStartText();
    }
}

/**
 *	Draw score on screen
 *
 *	@param	scoreValue	Score to draw in canvas
 */
void Game::SetScore(int scoreValue)
{
    score += scoreValue;
    
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = 120;
    dstRect.y = 110;
    
    const char *strScore = ConvertIntToString(score).c_str();
    
    DrawText(strScore, srcRect, dstRect, 32, "SourceSansPro-Regular.ttf");
}

/**
 *	Draw time on screen
 *
 *	@param	timeValue	Time value to draw on screen
 */
void Game::SetTime(int timeValue)
{
    if (timeValue < 0)
        timeValue = 0;
    
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = 120;
    dstRect.y = 195;
    
    const char *strScore = ConvertIntToString(timeValue).c_str();
    
    DrawText(strScore, srcRect, dstRect, 32, "SourceSansPro-Regular.ttf");
}

/**
 *	Method to start time countdown
 *
 */
void Game::StartTimer()
{
    thisTime = SDL_GetTicks();
    deltaTime = (float)(thisTime - lastTime) / 1000;
    lastTime = thisTime;
    
    if (deltaTime >= 0.5)
    {
        timeout -= 1;
        
        if (timeout >= 0)
        {
            SetTime(timeout);
            
            if (timeout == 0)
                gameOver = true;
        }
    }
}

/**
 *	Draw fixed strings on screen "Score" and "Time" labels
 *
 */
void Game::SetFixedText()
{
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = 100;
    dstRect.y = 70;
    
    DrawText("Score", srcRect, dstRect, 22, "SourceSansPro-Regular.ttf");

    CRect dstRect2;
    dstRect2.x = 105;
    dstRect2.y = 150;
    
    DrawText("Time", srcRect, dstRect2, 22, "SourceSansPro-Regular.ttf");
}

/**
 *	Drawing method for background image
 *
 *	@param	window	Game window
 *	@param	renderer	Drawer object for surfaces, in this case
 */
void Game::SetBackground(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *image = nullptr;
    image = LoadImage("BackGround.jpg");
    
    if (image == NULL)
    {
        SDL_Quit();
        exit(1);
    }
    
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = image->w;
    r.h = image->h;
    
    SDL_Rect m_sourceRectangle;
    SDL_Rect m_destinationRectangle;
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    SDL_QueryTexture(texture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
    
    m_destinationRectangle.x = m_sourceRectangle.x = 0;
    m_destinationRectangle.y = m_sourceRectangle.y = 0;
    m_destinationRectangle.w = m_sourceRectangle.w;
    m_destinationRectangle.h = m_sourceRectangle.h;
    
    SDL_RenderCopy(renderer, texture, &m_sourceRectangle, &m_destinationRectangle);
    SDL_DestroyTexture(texture);
}

/**
 *	Init board with default values
 *
 */
void Game::InitBoard()
{
    for (int c = 0; c < kTotalCols; c++)
    {
        for (int r = 0; r < kTotalRows; r++)
        {
            Tile defaultTile;
            defaultTile.rowValue = -1;
            defaultTile.colValue = -1;
            defaultTile.value = -1;
            defaultTile.type = (TYPE_SQUARE)-1;
            defaultTile.tint = false;
            
            board[r][c] = defaultTile;
        }
    }
}

/**
 *	Fulfill board with random values
 *
 */
void Game::FillBoardArray()
{
    for (int col = 0; col < kTotalCols; col++)
    {
        for (int row = 0; row < kTotalRows; row++)
        {
            //do random from 0 to 4 included for each tile
            int tile = 0 + rand() % (kMaxTile + 0 - 1);
            Tile item;
            item.rowValue = row;
            item.colValue = col;
            item.value = tile;
            item.type = (TYPE_SQUARE)tile;
            item.tint = false;

            board[row][col] = item;
        }
    }

    for (int col = 0; col < kTotalCols; col++)
    {
        for (int row = 0; row < kTotalRows; row++)
        {
           GetNewValueForBoard(row, col);
        }
    }
}

/**
 *	Check board to discover tiles that has same type on its right and left
 *
 *	@param	row	Row to check
 *	@param	col	Col to check
 *
 *	@return	New type returned
 */
int Game::GetNewValueForBoard(int row, int col)
{
    Tile item = board[row][col];
    
    int type = 0 + rand() % (kMaxTile + 0 - 1);
    
    int backRow = row - 1;
    int backCol = col - 1;

    if (backCol >= 0)
    {
         Tile backTile = board[row][col - 1];
        
        if (item.type == backTile.type)
        {
            item.value = type;
            item.type = (TYPE_SQUARE)type;
            
            board[row][col] = item;
            
            return GetNewValueForBoard(row, col);
        }
    }
    
    if (backRow >= 0)
    {
        Tile backTile = board[row - 1][col];
        
        if (item.type == backTile.type)
        {
            item.value = type;
            item.type = (TYPE_SQUARE)type;
            
            board[row][col] = item;
            
            return GetNewValueForBoard(row, col);
        }
    }

    return type;
}

/**
 *	Draw filled board, it loops around the bouds and draws each tile
 *
 */
void Game::DrawBoard()
{
    if (gameOver || !canMove || !startGame)
        return;
    
    for (int c = 0; c < kTotalCols; c++)
    {
        for (int r = 0; r < kTotalRows; r++)
        {
            Tile tile = board[r][c];
            DrawTileBoard(tile);
        }
    }
}

/**
 *	Drawing tile method
 *
 *	@param	tile	Tile object that contains information to draw on screen
 */
void Game::DrawTileBoard(Tile tile)
{
    string tileString = tiles[tile.value];

    if (tileString == "") return;
    
    SDL_Rect m_sourceRectangle, m_destinationRectangle;
    
    SDL_Surface *image = LoadImage(tileString);

    if (image == NULL)
    {
        SDL_Quit();
        exit(1);
    }

    if (tile.tint == true)
    {
        TintValues tint = TintValues(105, 105, 105, 1);
        Tint::TintSurface(image, tint);
    }
    
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, image);
    
    SDL_FreeSurface(image);
    SDL_QueryTexture(texture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
    
    m_sourceRectangle.x = 0;
    m_sourceRectangle.y = 0;
    
    m_destinationRectangle.x = kBaseTilePositionX + (kOffsetPosition * tile.colValue);
    m_destinationRectangle.y = kBaseTilePositionY + (kOffsetPosition * tile.rowValue);
    m_destinationRectangle.w = m_sourceRectangle.w;
    m_destinationRectangle.h = m_sourceRectangle.h;

    SDL_RenderCopy(renderer, texture, &m_sourceRectangle, &m_destinationRectangle);
    SDL_DestroyTexture(texture);
}

/**
 *	Method to manage event, we have to track mouse clicks in this class
 *
 *	@param	ev	Event to check mouse events
 */
void Game::SetEvent(SDL_Event *ev)
{
    this->event = ev;
    
    TrackEvent(event);
}

/**
 *	Helper method to check click position and seach/destroy tiles and regenerate destroyed ones
 *
 *	@param	e	Event object
 */
void Game::TrackEvent(SDL_Event *e)
{
    switch (e->type)
	{
        case SDL_MOUSEBUTTONDOWN:  // if the user clicked a mousebutton
        {
            int posX = e->button.x;
            int posY = e->button.y;
            
            if (gameOver)
            {
                if (IsPointInRect(posX, posY, retryX, retryY, retryWidth, retryWidth))
                {
                    SMLog2("resetting board for game over...");
                    gameOver = false;
                    ResetBoard();
                }
            }
            else if (!startGame)
            {
                if (IsPointInRect(posX, posY, startX, startY, startWidth, startHeight))
                {
                    startGame = true;
                }
            }
            else if (startGame)
            {
                if (IsPointInRect(posX, posY, stopX, stopY, stopWidth, stopHeight))
                {
                    startGame = false;
                }
                else if (!canMove)
                {
                    if (IsPointInRect(posX, posY, resetBX, resetBY, resetBWidth, resetBHeight))
                    {
                        timeout = kTimeCountDown;
                        SetTime(timeout);
                        
                        InitBoard();
                        FillBoardArray();
                        DrawBoard();
                        StartTimer();
                    }
                }
                else
                {
                    CheckBoard(posX, posY);
                }
            }
            
            break;
        }
        default:
            break;
	}
}

/**
 *	Reset board and restart the game agaon
 */
void Game::ResetBoard()
{
    //Reset board
    score = 0;
    SetScore(score);
    
    timeout = kTimeCountDown;
    SetTime(timeout);
    
    InitBoard();
    FillBoardArray();
    DrawBoard();
    StartTimer();
}

/**
 *	Method that gets row and col from mouse click. Then check pressed tiles and seach tiles candidates to destroy, finally it destroys tiles and regenerate them
 *
 *	@param	posX	X-Axis from mouse click
 *	@param	posY	Y-Axis from mouse click
 */
void Game::CheckBoard(int posX, int posY)
{
    int j = -1;
    int i = -1;
    
    if (posY >= kBaseTilePositionY)
    {
        i = (posY - kBaseTilePositionY) / kOffsetPosition;
        
        if (i > kTotalRows - 1)
            i = -1;
    }
    
    if (posX >= kBaseTilePositionX)
    {
        j = (posX - kBaseTilePositionX) / kOffsetPosition;
        
        if (j > kTotalCols - 1)
            j = -1;
    }
    
    if (i != -1 && j != -1)
    {
        touches++;
        
        if (touches == 1)
        {
            //first tile to swap
            tempBase = board[i][j];
            baseTile = tempBase;
            baseTile.tint = true;
            board[i][j] = baseTile;
            
            DrawTileBoard(baseTile);
        }
        else if (touches == 2)
        {
            //second tile to swap
            touches = 0;
            
            //check if the user has select a nearby tile
            if (abs(baseTile.rowValue - i) < 2 && abs(baseTile.colValue - j) < 2)
            {
                tempSwap = board[i][j];
                swapTile = tempSwap;

                swapTile.rowValue = tempBase.rowValue;
                swapTile.colValue = tempBase.colValue;
                swapTile.tint = false;
                board[tempBase.rowValue][tempBase.colValue] = swapTile;

                baseTile.rowValue = tempSwap.rowValue;
                baseTile.colValue = tempSwap.colValue;
                baseTile.tint = false;
                board[tempSwap.rowValue][tempSwap.colValue] = baseTile;

                //check tiles to destroy
           
                bool undoSwap = CheckDestroy(baseTile);
                
                //invalid movement
                if (undoSwap)
                {
                    baseTile.rowValue = tempBase.rowValue;
                    baseTile.colValue = tempBase.colValue;
                    baseTile.tint = false;
                    board[tempBase.rowValue][tempBase.colValue] = baseTile;
                    
                    swapTile.rowValue = tempSwap.rowValue;
                    swapTile.colValue = tempSwap.colValue;
                    swapTile.tint = false;
                    board[tempSwap.rowValue][tempSwap.colValue] = swapTile;
                    
                    //invalid swap text
                    ShowInvalidSwapText();
                }
                else
                {
                    swapTile.tint = false;
                    baseTile.tint = false;

                    SearchTileCombinationToDestroy();
                    
                    //redraw board
                    DrawBoard();
                    
                    SoundManager::Instance()->PlaySound("laser.wav");
                }
            }
            else
                touches = 1;
        }
    }
}

/**
 *	Method to search tiles to destroy with one tile as needle
 *
 *	@param	tile	Tile given, as needle, to seach candidates for destroying around it
 *
 *	@return	A boolean value that holds if tile's swap is valid or not (not valid -> true / valid -> false)
 */
bool Game::CheckDestroy(Tile tile)
{
    //check if type of nearby tiles is the same as the input time on N/E/W/S if not do nothing
    
    SMLog("original tile - row, col: %d, %d", tile.rowValue, tile.colValue);
    
    hCheck.clear();
    hCheck.push_back(tile);
    
    vCheck.clear();
    vCheck.push_back(tile);
    
    //check left horizontal tile
    int col = tile.colValue - 1;
    
    while (col >= 0)
    {
        Tile vLeftTile = board[tile.rowValue][col];
    
        if (vLeftTile.type == tile.type)
        {
            SMLog("left tile - row, col: %d, %d", vLeftTile.rowValue, vLeftTile.colValue);
            
            hCheck.push_back(vLeftTile);
        }
        else
            break;
        
        col--;
    }
    
    //check right horizontal tile
    for (int y = tile.colValue + 1; y < kTotalCols; y++)
    {
        Tile vRightTile = board[tile.rowValue][y];
        
        if (vRightTile.type == tile.type)
        {
            SMLog("right tile - row, col: %d, %d", vRightTile.rowValue, vRightTile.colValue);
            
            hCheck.push_back(vRightTile);
        }
        else
            break;
    }
    
    SMLog("horizontal elements : %ld", hCheck.size());
    
    //check upper vertical tile
    for (int i = tile.rowValue - 1; i >= 0; i--)
    {
        Tile hUpperTile = board[i][tile.colValue];
        
        if (hUpperTile.type == tile.type)
        {
            vCheck.push_back(hUpperTile);
        }
        else
            break;
    }
    
    //check down vertical tile
    for (int i = tile.rowValue + 1; i < kTotalCols; i++)
    {
        Tile hDownTile =  board[i][tile.colValue];
        
        if (hDownTile.type == tile.type)
        {
            vCheck.push_back(hDownTile);
        }
        else
            break;
    }
    
    SMLog("vertical elements : %ld", vCheck.size());
    
    if (vCheck.size() < 3 && hCheck.size() < 3)
    {
        SMLog2("invalid movement, switching back tiles");
        return true;
    }
    else
        return false;
}

/**
 *	Method that destroys tiles and regenerate them and update score
 *
 */
void Game::DestroyTiles()
{
    //destroy & place new tiles
    int points = 0;
    
    //ensure that we have 3 or more tiles to destroy
    if (vCheck.size() > 2)
    {
        points += vCheck.size() * kPoints;
        
        std::sort(vCheck.begin(), vCheck.end(), SortingVectorRows);

        int len = (int)vCheck.size();
        
        Tile dTile = vCheck.at(0);
        int col = (int)dTile.colValue;
        
        //move down tiles to max row destroyed and regenerate empty tiles
        for (int row = dTile.rowValue - 1; row >= -len; row--)
        {
            int nRow = row + len;
            
            Tile oldTile = board[row][col];

            oldTile.rowValue = nRow;
            
            if (row < 0)
            {
                oldTile.colValue = col;
                
                //generate different tile types
                TYPE_SQUARE type = CheckForDifferentType(oldTile);
                
                oldTile.value = (int)type;
                oldTile.type = type;
                oldTile.tint = false;
            }
                
            board[nRow][col] = oldTile;
            
            SMLog("nRow, col : %d %d", nRow , col);
        }

        vCheck.clear();
    }
    
    if (hCheck.size() > 2)
    {
        points += hCheck.size() * kPoints;
        
        //get current row from hCheck, loop over the board (in backward from current row) and increase row for all of them, when we are at first row create a new tile for row = 0
        
        std::sort(hCheck.begin(), hCheck.end(), SortingVectorCols);

        SMLog("size: %ld", hCheck.size());

        for (Tile dTile : hCheck)
        {
            int col = dTile.colValue;
            
            for (int row = dTile.rowValue; row >= 0; row--)
            {
                int srcRow = row - 1;
                
                if (srcRow == -1)
                {
                    Tile newTile;
                    newTile.rowValue = 0;
                    newTile.colValue = col;

                    TYPE_SQUARE type = CheckForDifferentType(newTile);
                    
                    newTile.value = (int)type;
                    newTile.type = type;
                    newTile.tint = false;
                    board[0][col] = newTile;
                    
                    SMLog("srcCol : %d", col);
                }
                else
                {
                    Tile tile = board[srcRow][col];
                    
                    tile.rowValue = row;
                    
                    board[row][col] = tile;
                    
                    SMLog("srcRow : %d, %d", row, col);
                }
            }
        }
        
        hCheck.clear();
    }
    
    SetScore(points);
}

/**
 *	Search for 3-tile combination (or more) after swap&move down tiles
 *
 */
void Game::SearchTileCombinationToDestroy()
{
    //important!
    //post-check of board, if we discover a new 3-tile combination (or more) after regenerate tile it must be destroyed
    //if we do not this check, it will be 3-tile combination (or more) without destroy
    for (int col = 0; col < kTotalCols; col++)
    {
        for (int row = 0; row < kTotalRows; row++)
        {
            if (!CheckDestroy(board[row][col]))
            {
                DestroyTiles();
            }
        }
    }
}

/**
 *	Get random tile type different to given one
 *
 *	@param	oldType	Given type to avoid on type generation
 *
 *	@return	New random type of tile
 */
TYPE_SQUARE Game::CheckForDifferentType(Tile oldTile)
{
    TYPE_SQUARE newType = GetNewRandomType();
    
    int r = oldTile.rowValue;
    int c = oldTile.colValue;
    
    Tile right;
    Tile left;
    
    int typeRight = -1;
    int typeLeft = -1;
    
    if (c < kTotalCols - 1)
    {
        right = board[r][c + 1];
        typeRight = right.type;
    }
    
    if (c > 0)
    {
        left = board[r][c - 1];
        typeLeft = left.type;
    }
    
    Tile down, up;
    int typeUp = -1;
    int typeDown = -1;
    
    if (r > 0)
    {
        up = board[r - 1][c];
        typeUp = up.type;
    }
    
    if (r < kTotalRows - 1)
    {
        down = board[r + 1][c];
        typeDown = down.type;
    }

    SMLog("Type up: %d, down: %d, right: %d, left: %d, newType: %d", typeUp, typeDown, typeRight, typeLeft, newType);
    
    if (typeRight != -1)
    {
         if (typeRight == newType)
         {
             return CheckForDifferentType(oldTile);
         }
    }
    
    if (typeLeft != -1)
    {
        if (typeLeft == newType)
        {
            return CheckForDifferentType(oldTile);
        }
    }
    
    if (typeUp != -1)
    {
        if (typeUp == newType)
        {
            return CheckForDifferentType(oldTile);
        }
    }
    
    if (typeDown != -1)
    {
        if (typeDown == newType)
        {
            return CheckForDifferentType(oldTile);
        }
    }

    return newType;
}

/**
 *	Simple random type generator
 *
 *	@return	New tile type
 */
TYPE_SQUARE Game::GetNewRandomType()
{
    int type = 0 + (rand() % kMaxTile);
    
    return (TYPE_SQUARE)type;
}

/**
 *	Method that shows a warning string when the user does an invalid movement
 *
 */
void Game::ShowInvalidSwapText()
{
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = kBaseTilePositionX + 60;
    dstRect.y = 480;
    
    DrawText("Invalid move!", srcRect, dstRect, 40, "SourceSansPro-Regular.ttf");
}

/**
 *	Method that shows a string to warn user when game is over
 *
 */
void Game::CreateGameOver()
{
    if (!gameOver) return;
    
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = kBaseTilePositionX + 60;;
    dstRect.y = 150;

    DrawText("Game Over", srcRect, dstRect, 40, "SourceSansPro-Regular.ttf");
    
    CRect dstRect2;
    dstRect2.x = kBaseTilePositionX + 60;
    dstRect2.y = 250;
    
    std::stringstream ss;
    ss << "Your score is: " << score;
    const char *strScore = ss.str().c_str();
    
    DrawText(strScore, srcRect, dstRect2, 30, "SourceSansPro-Regular.ttf");
    
    CRect dstRect3;
    dstRect3.x = kBaseTilePositionX + 50;
    dstRect3.y = 350;
    
    CRect final = DrawText("Click here to retry", srcRect, dstRect3, 30, "SourceSansPro-Regular.ttf");
    
    retryWidth = final.width;
    retryHeight = final.height;
    retryX = final.x;
    retryY = final.y;
}

/**
 *	Check if given coords are inside a rect
 *
 *	@param	x	X point
 *	@param	y	Y point
 *
 *	@return	Boolean value that says if point is inside "Game over rect" (true : inside / false : outside)
 */
bool Game::IsPointInRect(int x, int y, int rPointX, int rPointY, int rWidth, int rHeight)
{
    if ((x >= rPointX && x <= rPointX + rWidth) && (y >= rPointY && y <= rPointY + rHeight))
    {
        return true;
    }
    
    return false;
}

/**
 *	Method that checks if user can do valir movements, if not, reset board
 *
 */
bool Game::CanMakeMoves()
{
    bool move = true;

    for (int col = 0; col < kTotalCols; col++)
    {
        for (int row = 0; row < kTotalRows; row++)
        {
            Tile currentTile = board[row][col];

            int upDiagonal = 0;
            int downDiagonal = 0;
            int sameTiles = 0;
            
            if (row - 1 > 0 && col - 1 > 0)
            {
                Tile upLeft = board[row - 1][col - 1];
                
                if (currentTile.type == upLeft.type)
                {
                    upDiagonal++;
                    sameTiles++;
                }
            }
            
            if (row - 1 > 0 && col + 1 < kTotalCols)
            {
                Tile upRight = board[row - 1][col + 1];
                
                if (currentTile.type == upRight.type)
                {
                    downDiagonal++;
                    sameTiles++;
                }
            }
            
            if (row + 1 < kTotalRows && col - 1 > 0)
            {
                Tile downLeft = board[row + 1][col - 1];
                
                if (currentTile.type == downLeft.type)
                {
                    downDiagonal++;
                    sameTiles++;
                }
            }
            
            if (row + 1 < kTotalRows && col + 1 < kTotalCols)
            {
                Tile downRight = board[row + 1][col + 1];
                
                if (currentTile.type == downRight.type)
                {
                    upDiagonal++;
                    sameTiles++;
                }
            }
            
            if (col - 1 > 0)
            {
                Tile left = board[row][col - 1];
                
                if (currentTile.type == left.type)
                {
                    sameTiles++;
                }
            }
           
            if (col + 1 < kTotalCols)
            {
                Tile right = board[row][col + 1];
                
                if (currentTile.type == right.type)
                {
                    sameTiles++;
                }
            }
           
            if (row - 1 > 0)
            {
                Tile up = board[row - 1][col];
                
                if (currentTile.type == up.type)
                {
                    sameTiles++;
                }
            }
            
            if (row + 1 < kTotalRows)
            {
                Tile down = board[row + 1][col];
                
                if (currentTile.type == down.type)
                {
                    sameTiles++;
                }
            }
            
            SMLog("same tiles %d", sameTiles);
            
            if (sameTiles >= 2)
            {
                // if we have 2 same type tiles and they are diagonal combination, avoid them
                if (upDiagonal != sameTiles || downDiagonal != sameTiles)
                {
                    move = true;
                    break;
                }
                
            }
        }
    }
    
    canMove = move;
    
    return move;
}

/**
 *	Warn user about he/she cannot make moves, so reset board
 *
 */
void Game::SetResetBoardText()
{
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = kBaseTilePositionX + 40;
    dstRect.y = 150;
    
    DrawText("No movements left!!!", srcRect, dstRect, 30, "SourceSansPro-Regular.ttf");

    CRect dstRect2;
    dstRect2.x = kBaseTilePositionX + 20;
    dstRect2.y = 280;

    CRect finalRect = DrawText("Click here to reset board", srcRect, dstRect2, 30, "SourceSansPro-Regular.ttf");
    
    resetBX = finalRect.x;
    resetBY = finalRect.y;
    resetBWidth = finalRect.width;
    resetBHeight = finalRect.height;
}

/**
 *	Method for "start text" drawing, this text is shown the first time the user plays and when user restart game from stop action
 *
 */
void Game::SetStartText()
{
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = kBaseTilePositionX + 30;
    dstRect.y = 150;
    
    DrawText("Gems Game", srcRect, dstRect, 50, "SourceSansPro-Regular.ttf");

    CRect dstRect2;
    dstRect2.x = kBaseTilePositionX + 20;
    dstRect2.y = 280;
    
    CRect dstFinal = DrawText("Click here to start", srcRect, dstRect2, 40, "SourceSansPro-Regular.ttf");
    
    startX = dstFinal.x;
    startY = dstFinal.y;
    startWidth = dstFinal.width;
    startHeight = dstFinal.height;
}

/**
 *	This method draws stop "button", it stops game
 *
 */
void Game::SetStopText()
{
    if (gameOver) return;
    
    CRect srcRect;
    srcRect.x = 0;
    srcRect.y = 0;
    
    CRect dstRect;
    dstRect.x = 75;
    dstRect.y = 440;

    CRect dstFinal = DrawText("Stop", srcRect, dstRect, 40, "SourceSansPro-Regular.ttf");
    
    stopX = dstFinal.x;
    stopY = dstFinal.y;
    stopWidth = dstFinal.width;
    stopHeight = dstFinal.height;
}

/**
 *	Draw text strings given a origin rect, a destination rect, font size and font text
 *
 *	@param	text	Text to draw
 *	@param	srcRect	Source rect of text
 *	@param	dstRect	Destination rect of text
 *	@param	fontSize	Font size to draw
 *	@param	fontName	Font name to draw
 *
 *	@return	Return destination rect (x,y,width,height) as CRect typedef
 */
CRect Game::DrawText(const char *text, CRect srcRect, CRect dstRect, int fontSize, const char *fontName)
{
    TTF_Init();
    
    TTF_Font *font = TTF_OpenFont(fontName, fontSize);
    
    if (font == NULL)
    {
        cerr << "Load SourceSansPro-Regular.ttf Failed: " << TTF_GetError() << endl;
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
        
    SDL_Color foregroundColor = { 255, 255, 255 };
    
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "", foregroundColor);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(textSurface);
    
    textSurface = TTF_RenderText_Solid(font, text, foregroundColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(textSurface);
    
    SDL_Rect m_sourceRectangle;
    SDL_Rect m_destinationRectangle;
    
    m_sourceRectangle.x = srcRect.x;
    m_sourceRectangle.y = srcRect.y;
    
    SDL_QueryTexture(texture, NULL, NULL, &m_sourceRectangle.w, &m_sourceRectangle.h);
    
    srcRect.width = m_sourceRectangle.w;
    srcRect.height = m_sourceRectangle.h;
    
    m_destinationRectangle.x = dstRect.x;
    m_destinationRectangle.y = dstRect.y;
    m_destinationRectangle.w = srcRect.width;
    m_destinationRectangle.h = srcRect.height;
  
    SDL_RenderCopy(renderer, texture, &m_sourceRectangle, &m_destinationRectangle);
    SDL_DestroyTexture(texture);
    
    TTF_CloseFont(font);
    TTF_Quit();
    
    CRect final;
    final.x = m_destinationRectangle.x;
    final.y = m_destinationRectangle.y;
    final.width = m_destinationRectangle.w;
    final.height = m_destinationRectangle.h;
    
    return final;
}
