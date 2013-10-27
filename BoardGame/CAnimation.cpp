//
//  CAnimation.cpp
//  BoardGame
//
//  Created by sid on 27/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "CAnimation.h"

#define FADE_SPEED 0.7f

void CAnimation::FadeTexture(float elapsedTime, SDL_Texture *texture)
{
//    Uint8 alpha;
//    float alphaCalc = 0.0;
//    SDL_GetTextureAlphaMod(texture, &alpha);
//    
//    // Check there is a texture
//    if (texture)
//    {
//        // Set the alpha of the texture
//        SDL_SetTextureAlphaMod(texture, alpha);
//    }
//    
//    // Update the alpha value
//    if (alpha < SDL_ALPHA_OPAQUE)
//    {
//        alphaCalc += FADE_SPEED * elapsedTime;
//        alpha = alphaCalc;
//    }
//    
//    // if alpha is above 255 clamp it
//    if (alpha >= SDL_ALPHA_OPAQUE)
//    {
//        alpha = SDL_ALPHA_OPAQUE;
//        alphaCalc = (float)SDL_ALPHA_OPAQUE;
//    }
}