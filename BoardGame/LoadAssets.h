//
//  LoadAssets.h
//  HybridGems
//
//  Created by sid on 16/11/13.
//
//

#ifndef __HybridGems__LoadAssets__
#define __HybridGems__LoadAssets__

#include <iostream>

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

class LoadAssets
{
public:
    static LoadAssets* Instance();
    
    SDL_Surface *tilesTextures[5];
    SDL_Surface *redTile;
    SDL_Surface *blueTile;
    SDL_Surface *greenTile;
    SDL_Surface *purpleTile;
    SDL_Surface *yellowTile;
    SDL_Surface *background;
    
protected:
    LoadAssets();
    LoadAssets(const LoadAssets &) ;
    LoadAssets &operator= (const LoadAssets &) ;
    
private:
    static LoadAssets* pinstance;
    
    static inline SDL_Surface* LoadImage(std::string fileName)
    {
        SDL_Surface *image = IMG_Load(fileName.c_str());
        
        if(image == NULL)
            return NULL;
        
        SDL_Surface *optimizedImage = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_BGRA8888, SDL_TEXTUREACCESS_STATIC);
        SDL_FreeSurface(image);
        
        return optimizedImage;
    }
};

#endif /* defined(__HybridGems__LoadAssets__) */
