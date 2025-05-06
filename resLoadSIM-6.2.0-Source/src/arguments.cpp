/*---------------------------------------------------------------------------
 _____   ______  ______         _____   _____   _____   ______ _____  _____
|_____/ |______ |_____  |      |     | |_____| |     \ |_____    |   |  |  |
|    \_ |______ ______| |_____ |_____| |     | |_____/ ______| __|__ |  |  |

|.....................|  The Residential Load Simulator
|.......*..*..*.......|
|.....*.........*.....|  Authors: Christoph Troyer
|....*...........*....|
|.....*.........*.....|
|.......*..*..*.......|
|.....................|

Copyright (c) 2021 European Union

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software without
   specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.

---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "version.H"
#include "proto.H"

void parse_arguments (int argc, char **argv, int *num_households, double *days, bool *silent_mode)
{
    bool print_version = false;
    int error = 0;
    int first_pos;
    *silent_mode = false;
    
    // Check the number of arguments
    switch (argc)
    {
        case 2: // it must be the -v option
            if (argv[1][0] == '-')
            {
                if (argv[1][1] == 'v' && argv[1][2] == '\0') print_version = true;
                else if (argv[1][1] == 's' && argv[1][2] == '\0') error = 1;
                else error = 2;
            } 
            else error = 1;
            break;
        case 3: // no options, only households and days
            first_pos = 1;
            break;
        case 4: // one option plus households and days
            if (argv[1][0] == '-') // first argument needs to be an option
            {
                if (argv[1][1] == 'v' && argv[1][2] == '\0') print_version = true;
                else if (argv[1][1] == 's' && argv[1][2] == '\0') *silent_mode = true;
                else error = 2;
            }
            else error = 1;
            first_pos = 2;
            break;
        default: error = 1;
    }    

    if (error == 0 && argc > 2) // no error encountered so far
    {
        if (*num_households == 0) *num_households = atoi (argv[first_pos]);
        sscanf (argv[first_pos+1], "%lf", days);
        if (*num_households <= 0) error = 3;
        if (*days <= 0) error = 4;
    }

    switch (error)
    {
        case 0: // no error occured
            if (print_version)
            {
#ifdef DEBUG
                printf ("resLoadSIM version %s (DEBUG)\n", VERSION);
#else
                printf ("resLoadSIM version %s (OPTIMIZED)\n", VERSION);
#endif
                if (argc == 2) exit (0);    // user just wanted to know the version
            }
            break;
        case 1: // wrong number of arguments
            fprintf (stderr, "Wrong number of arguments\n");
            break;
        case 2: // unknown option
            fprintf (stderr, "Unknown option '%s'\n", argv[1]);
            break;
        case 3:
            fprintf (stderr, "Number of households must be > 0\n");
            break;
        case 4:
            fprintf (stderr, "Number of days must be > 0\n");
            break;
        default:
            fprintf (stderr, "Unknown error\n");
    }
    if (error)
    {
        fprintf (stderr, "Usage: resLoadSIM [-s|-v] households days\n");
        exit (error);
    } 
}
