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

#include "loadprofile.H"
#include "proto.H"
#include "globals.H"


LoadProfile::LoadProfile()
{}

LoadProfile::LoadProfile (const std::filesystem::path &path)
{
    FILE *fp = NULL;
    double *ptr = (double *)profile;

    open_file (&fp, path.string().c_str(), "r");
    for (int row=1; row<=96; row++)
    {
        fscanf (fp, "%*s");
        for (int col=1; col<=9; col++)
        {
            fscanf (fp, "%lf", ptr++);
        }
    }
    fclose (fp);
    name = path.filename().string();  
    name.erase (name.end()-4, name.end());  // remove the .csv ending 
}


double LoadProfile::get_value()
{
    int col_1, col_2, row_1, row_2, season, offset;
 
    if (   sim_clock->month >= 11
        || sim_clock->month <= 2
        || (sim_clock->month == 3 && sim_clock->day <= 20))      season = 0;    // winter
    else if (   (sim_clock->month >= 6 && sim_clock->month <= 8) 
             || (sim_clock->month == 5 && sim_clock->day >= 15)
             || (sim_clock->month == 9 && sim_clock->day <= 14)) season = 1;    // summer
    else season = 2;    // transitional season
    if (sim_clock->weekday == 6) offset = 0;        // saturday
    else if (sim_clock->weekday == 0) offset = 1;   // sunday
    else offset = 2;                                // weekday
    col_1 = col_2 = season * 3 + offset;
    row_2 = sim_clock->daytime / 900;
    row_1 = (row_2 + 95) % 96;
    if (row_2 == 95)  // check whether we are at the season boundaries
    {
        if (sim_clock->last_day_of_season[0]) col_2 += 6;
        if (sim_clock->last_day_of_season[1]) col_2 += 3;
        if (sim_clock->last_day_of_season[2]) col_2 -= 3;
        if (sim_clock->last_day_of_season[3]) col_2 -= 6;
    }
    return profile[row_1][col_1] + (int)sim_clock->daytime % 900 / 900. * (profile[row_2][col_2] - profile[row_1][col_1]);
}


void LoadProfile::init()
{
    std::string dir_name  = "countries/" + location->country + "/standard_load_profiles";
    num_profiles = 0;
    for (const auto & entry : std::filesystem::directory_iterator(dir_name))
    {
        if (entry.path().extension() == ".csv") num_profiles++;
    }
    profile_list = new LoadProfile[num_profiles];
    int i = 0;
    for (const auto & entry : std::filesystem::directory_iterator (dir_name))
    {
        if (entry.path().extension() == ".csv") profile_list[i++] = LoadProfile (entry.path());
    }
}


LoadProfile* LoadProfile::get_random_profile()
{
    return &profile_list[get_random_number (0, num_profiles-1)];
}


LoadProfile* LoadProfile::get_profile (const std::string &profile_name)
{
    for (int i=0; i<num_profiles; i++)
    {
        if (profile_name == profile_list[i].name) return &profile_list[i];
    }
    fprintf (stderr, "LoadProfile::get_profile: Could not find load profile named '%s'\n", profile_name.c_str());
    exit (1);
}