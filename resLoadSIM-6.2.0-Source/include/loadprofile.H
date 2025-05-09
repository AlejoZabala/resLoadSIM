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

#ifndef LOAD_PROFILE_H
#define LOAD_PROFILE_H

#include <filesystem>

class LoadProfile
{   
private:
    static int num_profiles;            // number of standard load profiles
    static LoadProfile *profile_list;   // pointer to the first load profile
    double profile[96][9];              // values are stored quarter-hourly, hence 96 records
                                        // the 9 values are: saturday, sunday and workday for 3 seasons (winter, summer and transitional season)

public:
    std::string name;                   // name of the profile
    LoadProfile ();
    LoadProfile (const std::filesystem::path &path);
    static void init();
    static LoadProfile *get_random_profile();
    static LoadProfile *get_profile (const std::string &profile_name);
    double get_value();
};

#ifdef MAIN_MODULE
int LoadProfile::num_profiles = 0;
class LoadProfile* LoadProfile::profile_list = NULL;
#endif

#endif
