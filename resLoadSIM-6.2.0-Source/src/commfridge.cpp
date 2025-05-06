/*---------------------------------------------------------------------------
 _____   ______  ______         _____   _____   _____   ______ _____  _____
|_____/ |______ |_____  |      |     | |_____| |     \ |_____    |   |  |  |
|    \_ |______ ______| |_____ |_____| |     | |_____/ ______| __|__ |  |  |

|.....................|  The Residential Load Simulator
|.......*..*..*.......|
|.....*.........*.....|  Authors: Christoph Troyer
|....*...........*....|           Heinz Wilkening
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

#include <stdlib.h>

#include "appliance.H"
#include "business.H"
#include "proto.H"
#include "globals.H"


CommercialFridge::CommercialFridge (Business *b)
{
    business = b;

    delta_t_rise = config->fridge.delta_t_rise_factor
                   * normal_distributed_random (config->fridge.delta_t_rise_mean, config->fridge.delta_t_rise_sigma)
                   * config->timestep_size / 60.;
    delta_t_drop = config->fridge.delta_t_drop_factor
                   * normal_distributed_random (config->fridge.delta_t_drop_mean, config->fridge.delta_t_drop_sigma)
                   * config->timestep_size / 60.;

    target_temperature = get_random_number (config->fridge.min_temperature+1,
                                            config->fridge.max_temperature-1);
    temperature = get_random_number (target_temperature-1.0-0.5*delta_t_drop,
                                     target_temperature+1.0+0.5*delta_t_rise);

    if (get_random_number (1, 100) <= 100./(1.+ delta_t_drop/delta_t_rise))
    {
        status = ON;
    }
    else
    {
        status = OFF;
    }
    num_energy_classes = config->fridge.num_energy_classes;
    energy_class = random_energy_class (config->fridge.energy_classes);

    if (config->energy_classes_2021)
    {
        /* The energy class dependent figures below and the formula to
        calculate the power are from the following source:
        COMMISSION DELEGATED REGULATION (EU) 2019/2016 of 11 March 2019
        It can be downloaded here:
        https://eur-lex.europa.eu/legal-content/EN/TXT/?uri=CELEX:32019R2016
        */
        double EEI[] = {31, 41, 51, 64, 80, 100, 125, 155};
        double index = get_random_number (EEI[energy_class], EEI[energy_class+1]);
        // only 1 compartment with volume Vc assumed
        double Vc = normal_distributed_random_with_limits (config->fridge.Vc_mean[5],
                                                           config->fridge.Vc_sigma[5],
                                                           config->fridge.Vc_low[5],
                                                           config->fridge.Vc_high[5]);
        Vc *= 100;   // ### just for the test version
        double C = 1.0;
        double D = 1.0;
        double Ac = 1.0;
        double Bc = (1.0 + 1.02) / 2.0;
        double Nc = 138.0;
        double rc = 1.0;
        double Mc = 0.12;
        double SAE = C * D * Ac * Bc * (Nc + Vc * rc * Mc);
        power.real = config->fridge.factor_2 * index/100. * SAE/(365.*24.);
    }
    else
    {
        /* The energy class dependent figures below and the formula to
        calculate the power are from the following source:
        COMMISSION DELEGATED REGULATION (EU) No 1060/2010 of 28 September 2010
        It can be downloaded here:
        http://eur-lex.europa.eu/legal-content/EN/TXT/PDF/?uri=CELEX:32010R1060&from=EN
        */
        double EEI[] = {0.175, 0.275, 0.375, 0.485, 0.650, 0.850, 1.025, 1.175, 1.375, 1.575};
        double Vc = normal_distributed_random_with_limits (config->fridge.Vc_mean[5],
                                                           config->fridge.Vc_sigma[5],
                                                           config->fridge.Vc_low[5],
                                                           config->fridge.Vc_high[5]);
        Vc *= 100;   // ### just for the test version
        double Tc = config->fridge.Tc;
        double FF = 1.;
        double CC = 1.;
        double BI = 1.;
        double Veq = Vc * (25.-Tc) * 0.05 * FF * CC * BI;
        double M = 0.233;
        double N = 245;
        double CH = 50.;
        double SAEc = Veq * M + N + CH;
        power.real = config->fridge.factor_2 * EEI[energy_class] * SAEc / (365.*24.);  // runs approx. 6 minutes per hour
    }
    power.reactive = sqrt ((power.real/config->fridge.power_factor)*(power.real/config->fridge.power_factor)
                           - power.real*power.real);
}


void CommercialFridge::simulate()
{
    if (status == OFF) temperature += delta_t_rise;
    else temperature -= delta_t_drop;

    if (temperature > target_temperature + 1. && status == OFF)
    {
        status = ON;
        delta_t_drop = config->fridge.delta_t_drop_factor
                        * normal_distributed_random (config->fridge.delta_t_drop_mean, config->fridge.delta_t_drop_sigma)
                        * config->timestep_size / 60.;
    }
    else if (temperature < target_temperature - 1. && status == ON)
    {
        status = OFF;
        delta_t_rise = config->fridge.delta_t_rise_factor
                        * normal_distributed_random (config->fridge.delta_t_rise_mean, config->fridge.delta_t_rise_sigma)
                        * config->timestep_size / 60.;
    }
    if (status == ON)
    {
        business->increase_power (power.real, power.reactive);
    }
}