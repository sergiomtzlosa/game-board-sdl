//
//  Tint.cpp
//  BoardGame
//
//  Created by sid on 10/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "Tint.h"
#include <assert.h>

#define TINTER(v) \
        if((int)v + tint.v > 255) { \
            v = 255; \
        }else if((int)v + tint.v < 0) { \
            v = 0; \
        }else{ \
            v += tint.v; \
        }

/**
 *	Tint method for a SDL_Surface
 *
 *	@param	surface	Surface to tint
 *	@param	tint	Tint colour value
 */
void Tint::TintSurface(SDL_Surface *surface, const TintValues &tint)
{
	assert(SDL_LockSurface(surface) == 0);
	
	for(int x = 0; x < surface->w; x++)
    {
		for(int y = 0; y < surface->h; y++)
        {
			Uint32 pixel = GetPixel(surface, x, y);
			
			Uint8 r, g, b, a;
			SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
			
			TINTER(r);
			TINTER(g);
			TINTER(b);
			TINTER(a);
			
			pixel = SDL_MapRGBA(surface->format, r, g, b, a);
			
			SetPixel(surface, x, y, pixel);
		}
	}
	
	SDL_UnlockSurface(surface);
}

/**
 *	Get pixel from surfacde to tint
 *
 *	@param	surface	Surface to place pixel
 *	@param	x	X position to place pixel
 *	@param	y	Y position to place pixel
 *	@param	pixel	Tinted pixel
 */
void Tint::SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	switch (bpp)
    {
		case 1:
			*p = pixel;
			break;
			
		case 2:
			*(Uint16 *)p = pixel;
			break;
			
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
            else
            {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
            
			break;
			
		case 4:
			*(Uint32 *)p = pixel;
			break;
			
		default:
			break;
	}
}

/**
 *	Get pixel to tint from surface
 *
 *	@param	surface	Surface to get the pixel
 *	@param	x	X coordinate from pixel
 *	@param	y	Y coordinate from pixel
 *
 *	@return	Pixel to tint
 */
Uint32 Tint::GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	switch (bpp)
    {
		case 1:
			return *p;
			
		case 2:
			return *(Uint16 *)p;
			
		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
				return p[0] << 16 | p[1] << 8 | p[2];
			}
            else
            {
				return p[0] | p[1] << 8 | p[2] << 16;
			}
			
		case 4:
			return *(Uint32 *)p;
			
		default:
			return 0;
	}
}
