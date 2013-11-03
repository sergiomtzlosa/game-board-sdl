//
//  SoundManager.h
//  BoardGame
//
//  Created by sid on 10/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#ifdef __APPLE__

#import <TargetConditionals.h>

#if TARGET_OS_IPHONE

#include "SDL_mixer.h"

#else

#include <SDL2_Mixer/SDL_mixer.h>

#endif

#endif

#include <string>

using namespace std;

class SoundManager
{
public:
    
	static SoundManager* Instance();
	~SoundManager();
	SoundManager();

    int PlaySound(const char *file);
    int PlayMusic(const char *musicFile);

    void CleanUp();
    
private:
    
	static SoundManager* singletonInstance;
    
    Mix_Chunk *wav = nullptr;
    Mix_Music *backMusic = nullptr;
};
