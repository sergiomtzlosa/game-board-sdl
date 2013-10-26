//
//  File.cpp
//  BoardGame
//
//  Created by sid on 26/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#include "SMLog.h"

void SMLog::Log(bool allowLog, const char *fmt, ...)
{
    if (allowLog)
    {
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
}