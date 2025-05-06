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

#include "business.H"
#include "appliance.H"
#include "proto.H"
#include "globals.H"
#ifdef PARALLEL
#   include <mpi.h>
#endif


Business::Business()
{
    static int counter = 0;
    static bool first = true;

    if (first)
    {
        first = false;
        real_power_total = 0.;
        reactive_power_total = 0.;
        apparent_power_total = 0.;
    }
    number = first_number + counter;
    counter++;
    // Figure out the business type (bt)
    int sum = config->business.types[0].count;
    int bt = 0;
    while (number > sum)
    {
        bt++;
        sum += config->business.types[bt].count;
    }
    scale_factor = get_random_number (config->business.types[bt].scale_factor.min, config->business.types[bt].scale_factor.max);
    power_factor = get_random_number (config->business.types[bt].power_factor.min, config->business.types[bt].power_factor.max);
    load_profile = LoadProfile::get_profile (config->business.types[bt].load_profile_name);

    fridge = NULL;
    if (config->business.types[bt].has_fridge)
    {
        try
        {
            fridge = new CommercialFridge (this);
        }
        catch (...)
        {
            fprintf (stderr, "Unable to allocate memory for appliance.\n");
            exit (1);
        }
    }
}


void Business::simulate()
{
    for (int i=0; i<local_count; i++) business_list[i].simulate_1st_pass();
}


void Business::simulate_1st_pass()
{
    static const double factor = config->timestep_size/3600.;

    power.real = load_profile->get_value() * scale_factor;
    real_power_total += power.real;

    power.reactive = sqrt ((power.real/power_factor)*(power.real/power_factor) - power.real*power.real);
    reactive_power_total += power.reactive;

    apparent_power_total = sqrt (real_power_total*real_power_total + reactive_power_total*reactive_power_total);
    
    if (fridge) fridge->simulate();

    consumption += power.real * factor;
}


void Business::allocate_memory (int num_businesses)
{
    global_count = num_businesses;
    local_count = num_businesses/num_processes + (p_rank < num_businesses % num_processes);
#ifdef PARALLEL
    MPI_Status status;
    int next_first_number;
    if (p_rank > 0) MPI_Recv (&first_number, 1, MPI_INT, p_rank-1, 1, MPI_COMM_WORLD, &status);
    next_first_number = first_number + local_count;
    if (p_rank < num_processes-1) MPI_Send (&next_first_number, 1, MPI_INT, p_rank+1, 1, MPI_COMM_WORLD);
#endif
    if (local_count) alloc_memory (&business_list, local_count, "Business::allocate_memory");
}


void Business::deallocate_memory()
{
    if (local_count) delete [] business_list;
}


void Business::reset_integrals()
{
    for (int i=0; i<local_count; i++)
    {
        business_list[i].consumption = 0.;
    }
}


void Business::increase_power (double real, double reactive)
{
    power.real += real;
    real_power_total += real;

    power.reactive += reactive;
    reactive_power_total += reactive;
    
    apparent_power_total = sqrt (real_power_total*real_power_total + reactive_power_total*reactive_power_total);
}
