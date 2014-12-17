//
//  pyInterpreter.h
//  Bash_Defender_V2
//
//  Created by ben on 16/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//

#ifndef __Bash_Defender_V2__pyInterpreter__
#define __Bash_Defender_V2__pyInterpreter__

#include <stdio.h>

void sendStringToPython(char * userInput);
int quitPython();
int initPython(int argc, char ** argv);

#endif /* defined(__Bash_Defender_V2__pyInterpreter__) */
