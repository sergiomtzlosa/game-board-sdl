//
//  File.h
//  BoardGame
//
//  Created by sid on 26/10/13.
//  Copyright (c) 2013 Sergio. All rights reserved.
//

#ifndef __BoardGame__File__
#define __BoardGame__File__

#include <iostream>
#include <stdarg.h>
#include <stdio.h>

#define SMLog(fmt, ...) SMLog::Log(kDefineLog, fmt, __VA_ARGS__)

class SMLog
{
public:
    
    static void Log(bool allowLog, const char *fmt, ...);
};

#endif /* defined(__BoardGame__File__) */
