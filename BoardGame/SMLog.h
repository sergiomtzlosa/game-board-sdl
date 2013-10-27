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

#define SMLog2(fmt) SMLog::Log(kDefineLog, fmt, NULL)

using namespace std;

class SMLog
{
public:
    
    static void Log(bool allowLog, const char *fmt, ...);
    
private:
    
    static inline string CurrentDateTime()
    {
        time_t now = time(0);
        struct tm  tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
        
        return buf;
    }
};

#endif /* defined(__BoardGame__File__) */
