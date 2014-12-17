//
//  pyInterpreter.c
//  Bash_Defender_V2
//
//  Created by ben on 16/12/2014.
//  Copyright (c) 2014 Michael. All rights reserved.
//


#include "Python.h"
#include "../includes/parser.h"
#include "pyInterpreter.h"

int initPython(int argc, char ** argv)
{
    Py_SetProgramName(argv[0]);  /* optional but recommended */
    Py_Initialize();
    //Py_Main(argc, argv);
    return 1;
}

int quitPython ()
{
    Py_Finalize();
    return 1;
}

void sendStringToPython(char * userInput)
{
    if(!parse(userInput)) {
        printf("crashed?\n");
        PyRun_SimpleString(userInput);
        
    }
}