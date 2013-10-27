//
//  CAnimation.h
//  BoardGame
//
//  Created by sid on 27/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#ifndef __BoardGame__CAnimation__
#define __BoardGame__CAnimation__

#include <SDL2_image/SDL_image.h>
#include <iostream>

class CAnimation
{
public:
    
    static void FadeTexture(float elapsedTime, SDL_Texture *texture);
};


#endif /* defined(__BoardGame__CAnimation__) */
