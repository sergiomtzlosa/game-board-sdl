//
//  Tint.cpp
//
//  SoundManager.cpp
//  BoardGame
//
//  Created by sid on 10/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "SoundManager.h"
#include "SMLog.h"

using namespace std;

SoundManager* SoundManager::singletonInstance = NULL;

SoundManager* SoundManager::Instance()
{
	if (singletonInstance == NULL)
    {
		singletonInstance = new SoundManager;
	}
    
	return singletonInstance;
}

SoundManager::SoundManager()
{	
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
        SMLog2("audio error");
    }
}

SoundManager::~SoundManager()
{

}

int SoundManager::PlaySound(const char *file)
{
   wav = Mix_LoadWAV(file);
    
    int channel = Mix_PlayChannel(-1, wav, 0);
    
    if (channel == -1)
    {
        fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());
    }
    
    return channel;
}

int SoundManager::PlayMusic(const char *musicFile)
{
    backMusic = Mix_LoadMUS(musicFile);
    
    //If there is no music playing
	if (Mix_PlayingMusic() == 0)
	{
		//Play the music
		if (Mix_PlayMusic(backMusic, -1) == -1)
		{
			return 1;
		}
	}
	
	return 0;
}

void SoundManager::CleanUp()
{
    if (wav != NULL)
        Mix_FreeChunk(wav);
    
    Mix_FreeMusic(backMusic);
    
    //Quit SDL_mixer
    Mix_CloseAudio();
}
