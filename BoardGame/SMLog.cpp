//
//  File.cpp
//  BoardGame
//
//  Created by sid on 26/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "SMLog.h"
#include <SDL2/SDL.h>

#define kAppName "Gems "

void SMLog::Log(bool allowLog, const char *fmt, ...)
{
    if (allowLog)
    {
        va_list args;
        va_start(args, fmt);
        
        string fmtStr;
        fmtStr.append(fmt);
        
        string str = "[GEMS GAME] - " + currentDateTime() + " - " + fmtStr + "\n";
        
        vfprintf(stderr, str.c_str(), args);
        
        va_end(args);
    }
}
