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

#ifndef BUSINESS_H
#define BUSINESS_H

#include "types.H"
#include "loadprofile.H"

class Business
{
private:
    static Business *business_list;  // pointer to the first business
    static int local_count;          // local number of businesses (per process in a parallel calculation)
    static int first_number;         // number of first local business
    int number;                      // business ID
    LoadProfile *load_profile;       // pointer to a standard load profile used to simulate 
                                     // the energy consumption of this business
    double scale_factor;             // the values in the load profile are normalized and
                                     // need to be scaled
    double power_factor;             // used to calculate the reactive power
    double consumption;              // electrical power integral over time [kWh]
    class CommercialFridge *fridge;  // some businesses may have fridges to simulate;

    void simulate_1st_pass();
    
public:
    static int global_count;            // total number of businesses
    static double real_power_total;     // real power of all businesses [kW]
    static double reactive_power_total; // reactive power of all businesses [kVAR]
    static double apparent_power_total; // apparent power of all businesses [kVA]
    Power power;                        // current power drawn [kW]
    
    Business();
    static void allocate_memory (int num_businesses);
    static void deallocate_memory();
    static void reset_integrals();
    static void simulate();
    void increase_power (double real, double reactive);
};

#ifdef MAIN_MODULE
class Business* Business::business_list = NULL;
int Business::local_count;
int Business::global_count;
int Business::first_number = 1;
double Business::real_power_total;
double Business::reactive_power_total;
double Business::apparent_power_total;
#endif

#endif
