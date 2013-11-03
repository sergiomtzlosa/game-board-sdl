//
//  Tint.h
//  BoardGame
//
//  Created by sid on 10/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#ifndef __BoardGame__Tint__
#define __BoardGame__Tint__

#ifdef __APPLE__

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE

#include "SDL.h"
#include "SDL_image.h"

#else

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#endif

#endif

struct TintValues
{
    int r, g, b, a;
    
    TintValues() : r(0), g(0), b(0), a(0) {}
    TintValues(int ar, int ag, int ab, int aa = 0) : r(ar), g(ag), b(ab), a(aa) {}
    
    void Tint(int ar, int ag, int ab, int aa = 0)
    {
        r += ar;
        g += ag;
        b += ab;
        a += aa;
    }
    
    void Tint(TintValues &t)
    {
        Tint(t.r, t.g, t.b, t.a);
    }
    
    bool DoTint(void) const
    {
        return (r || g || b || a);
    }
};

class Tint
{
public:
    static void TintSurface(SDL_Surface *surface, const TintValues &tint);
    
private:
    static void SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
    static Uint32 GetPixel(SDL_Surface *surface, int x, int y);
};

#endif /* defined(__BoardGame__Tint__) */
