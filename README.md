# Household Energy Demand Simulation Using resLoadSIM

This repository provides the activation code required to run simulations with resLoadSIM, enabling users to model heat and electricity demand for residential buildings. Specifically, it contains input files and parameterisations tailored to simulate current and future energy consumption scenarios for households in Oldenburg and Karlsruhe (Germany) and Auckland and Christchurch (New Zealand). These resources facilitate reproducible analyses and support further studies on forecasting residential energy demand under different conditions.

The repository also contains detailed information about resLoadSIM version 6.2.0, together with instructions on how to install and configure it for your own simulations.

# About resLoadSIM
**resLoadSIM** is a stochastic bottom-up simulation tool for predicting electric residential load profiles of individual households with a high time resolution. For each household some individual characteristics are defined, e.g. number of residents, the size of the house or apartment and the devices used according to some statistical information. Each device is characterized by its energy class. In the simulation devices are used according to the estimated habits of the inhabitants. 

Weekly and seasonal variations are also captured. Beside using the tool for load prediction it is also possible to study the potential for load shifting in private households by implementing different control mechanisms based e.g. on variable pricing.

# How to obtain resLoadSIM
resLoadSIM was developed by the Joint Research Centre (JRC). The JRC works as the scientific advisory body of the European Commission, employing researchers to conduct studies and deliver impartial scientific guidance and expertise to inform and support European Union (EU) policies. 

You can find more information about the JRC Modelling Suit here: https://ses.jrc.ec.europa.eu/our-modelling-suite 

If you want to obtain resLoadSIM, the current version resLoadSIM-6.2.0-Source can be downloaded from this repository, but for updated versions you need to contact the JRC authors directly. Their contact information is embedded in the resloadSIM folder, but you can also contact the JRC directly.

## Project structure

    .
    ├── countries
    │   ├── DE
    │   │   └── locations
    │   │       ├── Karlsruhe
    │   │       └── Oldenburg
    │   └── NZ
    │       └── locations
    │           ├── Auckland
    │           └── Christchurch
    ├── README.md
    ├── resLoadSIM-6.2.0-Source
    │   ├── AUTHORS
    │   ├── build
    │   ├── CHANGELOG
    │   ├── CMakeLists.txt
    │   ├── doc
    │   ├── example
    │   ├── include
    │   ├── INSTALL
    │   ├── LICENSE
    │   └── src
    └── structure.txt

>>>>>>> Update README and add structure.txt and source folder

 ## 1. Acknowledgment and License Information
 
**resLoadSIM** is provided under an MIT License. This project cites resLoadSIM to acknowledge its intellectual property rights and licensing terms. You will find the MIT License within the resLoadSIM version 6.2.0 folder.

 The resLoadSIM activation code developed wihtin the [HINT](https://www.dlr.de/en/ve/research-and-transfer/projects/project-hint) project is licensed under the **BSD 3-Clause License**.  
 
 See the [LICENSE](LICENSE) file for details.
 
 [![License]([https://img.shields.io/badge/license-BSD%203--Clause-blue.svg)](https://github.com/AlejoZabala/resLoadSIM/blob/main/LICENSE](https://github.com/AlejoZabala/resLoadSIM/blob/main/License_activation_code.txt))
 
 ## 2. Installation 
 
 The following instructions are excerpted from the resLoadSIM manual provided under an MIT License:
 
 ### 2.1. System requirements
 
 ### Compiler
 The most basic requirement is a C++ compiler. On a Linux box it is very likely that the GNU Compiler Collection (GCC) is already installed. If not, get it from there: http://gcc.gnu.org Under Windows ResLoadSIM has been successfully compiled with Visual Studio, which you can get here for free: https://visualstudio.microsoft.com.
 Users of macOS get a free development environment called Xcode. It can be downloaded from Apple's App Store.
 
 ### CMake (optional)
 CMake is a set of tools that help to manage the build process of a software package. It isn't a requirement for building ResLoadSIM, but it makes things a lot easier. If you like to setup your makefiles by hand for example, or use an IDE like Xcode, which uses its own build system, then you can bypass CMake, otherwise get it from here: http://www.cmake.org. Visual Studio ships with an integrated CMake, so you don‘t need to worry about it.
 
 ### MPI (optional)
 To compile and execute the parallel version of ResLoadSIM you need to install an implementation of the Message Passing Interface (MPI). One possibility is Open MPI, which can be found here: http://www.open-mpi.org.
 
 ### PETSc (optional)
 If ResLoadSIM is intended to be used with grid voltage control enabled, the PETSc package needs to be installed. It can be obtained via git:
 
     git clone https://bitbucket.org/petsc/petsc.git
 
 PETSc contains a power flow solver named pflow. The pflow code has been modified by Shrirang Abhyankar to make it work together with ResLoadSIM. This updated version of pflow is available at: https://github.com/abhyshr/pflow.git.
 
 ### libcurl (optional)
 
 This library allows ResLoadSIM to download data from the PVGIS site automatically.
 
 ### 2.2. Compiling and Linking
 
 The ResLoadSIM source package is distributed as a compressed archive (*.tar.gz). The first thing you have to do is to uncompress the archive and extract the source code by executing the following command on the command-line:
 
     tar xvfz resLoadSIM-x.y.z-Source.tar.gz
 
 **Unix/Linux/macOS**
 Change into the directory of the decompressed package cd resLoadSIM-x.y.z-Source and create a build directory there, like this:
     
     mkdir build 
 
 You can name the directory as you wish and you can have more than one build directory for different builds (e.g. debug, parallel). Change into your newly created build directory and start the build process using CMake:
 
     cd build
     cmake ..
 
 CMake checks whether everything needed for ResLoadSIM is in place. If everything is ok, then CMake generates the makefiles needed for compiling the package. Start the compilation using make:
 
     make
 
 To install ResLoadSIM type:
 
     make install
 
 If no errors occur, then at this point ResLoadSIM is compiled, linked and ready to be used. 
 
 CMake will by default create a makefile for the optimized version of ResLoadSIM. If you need the debug version you have to call CMake with the following option:
     
     cmake -DCMAKE_BUILD_TYPE=DEBUG ..
 
 To get the parallel version of ResLoadSIM call:
 
     cmake -DPARALLEL:BOOL=ON ..
 
 **Windows**
 Under Windows you have several possibilities to build ResLoadSIM, but we assume that you use Visual Studio. It is the only option we have tried so far and it works pretty easy. Just fire it up and select the open local directory option in the start window. Navigate to the previously decompressed source directory (resLoadSIM-x.y.z-Source) and select it. Visual Studio will automatically invoke CMake. After that just press F7 and watch how ResLoadSIM is being compiled and linked.
 
 ## 3. Getting started
 
 Most of the parameters are read from configuration files, but two of them have to be passed to ResLoadSIM on the command-line. These are the number of households and the number of days. A simulation of 10.000 households over a period of three days would be initiated as follows:
 
     resLoadSIM 10000 3
 
 The parallel version is initiated by using the mpirun command, which is part of the MPI installation, e.g.:
 
     mpirun -np 4 resLoadSIM 10000 3
 
 In the above eample ResLoadSIM would be executed on 4 cpus (or cores). Using the -v option prints the current version (number, debug/optimized/parallel) of ResLoadSIM:
 
 resLoadSIM -v
 
 If you dont‘t want to see any status output on screen while simulating then run ResLoadSIM in silent mode by using the -s option (warnings and error messages will still be displayed):
 
     resLoadSIM -s 1000 365
     
 ResLoadSIM depends on a number of input files which are covered in the following chapter. It also depends on a certain directory structure being in place such that it can find those files. Have a look at the example directory, which demonstrates the minimum input setup for a simulation.
 
 ## 4. Configuration files
 
 ### 4.1 resLoadSIM.json
 
 This is the main configuration file. It contains parameters which control the behavior of the simulation and is basically a list of keyword-value pairs, so called settings, which are organized in groups enclosed by curly braces. The settings can be given in any order, and not all of them need to be specified. It is actually possible to run ResLoadSIM without resLoadSIM.json at all. In that case missing settings are substituted by hardcoded default values. Information about what values have been used is written to a log file named resLoadSIM.json.log. The format of this log file is the same as the format of the configuration file itself. Therefore it is possible to use the log file as the configuration for a subsequent simulation just by renaming it to resLoadSIM.json. This is especially handy when you start from scratch without your own configuration. Just run a short dummy simulation, edit the settings in the resulting logfile (all defaults) according to your needs, and rename the file to resLoadSIM.json.
 
 ### 4.2 location.json
 This file contains location dependent information and resides in the locations directory. There is a subdirectory for each location like Hannover, Rome, and so on. The file contains the following settings:
 
 - **country** The ISO code of the country
 - **type** can be ‚urban‘ or ‚rural‘; influences the simulation of e-vehicles
 - **latitude**
 - **longitude** the location‘s coordinates
 - **utc_offset** difference in hours from Universal Coordinated Time
 
 The location.json file is accompanied by a data file that contains temperature and irradiation data for this specific location. This file is usually downloaded from the PVGIS site [1] and its name can be specified in resLoadSIM.json (pv_data_file_name). If no name is provided ResLoadSIM assumes a filename according to the naming convention used by PVGIS. If no file with that name can be found in the locations directory, ResLoadSIM will try to download the data from the PVGIS site (libcurl has to be installed).
 
 ### 4.3 households.json
 The parameters, which influence the behaviour of the households, are stored in the file households.json. 
 
 They are:size_distribution, retired_1, retired_2, min_area, max_area, set_temperature_heating_day, set_temperature_heating_night, set_temperature_cooling, heating_period_start_day, heating_period_start_month, heating_period_end_day, heating_period_end_month, min_init_laundry,max_init_laundry, min_delta_laundry, max_delta_laundry, second_fridge, second_tv, third_tv, second_computer, min_vacuum_interval, max_vacuum_interval, light_factor, rnd_wakeup, rnd_wakeup_weekend, rnd_wakeup_retired, rnd_bedtime, rnd_bedtime_weekend, rnd_bedtime_retired, at_home_param, energy_class, rnd_heat_source, min_temperature_DHW,max_temperature_DHW, min_volume_handwash, max_volume_handwash, min_volume_shower, max_volume_shower, min_volume_bath, max_volume_bath, urban_car_percentage, rural_car_percentage, 
 prevalence. 
 
 ### 4.4 tech.json
 The core of the simulation are the appliances’ simulate functions. They model the appliances’ behaviour depending on several, partially random, factors. 
 Each appliance can be in one of two states: ON or OFF. All the simulate function does, is to determine the points of time, when an appliance has to change its state. This is usually done in advance at the start of each day (daytime == 0). During the simulation the current daytime is then compared to those saved points of time. In some cases though a timer is used to keep track of an appliance‘s state. For example when a washing machine is turned on we know the cycle time and
 set a timer accordingly. 
 
 All appliances, which are currently turned on, contribute to the total power consumption of a household. This is taken care of at the very end of each simulate function. In the following a description of all application related configuration parameters is presented for all appliances. All parameters are defined in the country dependent technical configuration tech.json.
 
 ## 5. Output Files
 
 The simulation generates detailed output files, which are saved annually and can be visualized using tools like Gnuplot. Key output files include:
 
 - **appliances.[year].[residents]:** Yearly electricity consumption per appliance type, categorized by household size and appliance energy classes.
 - **battery.[year]:** Annual statistics on battery charge, charging/discharging power, and related energy losses, relevant only for households with PV and battery systems.
 - **consumption.[year]:** Annual summary of electricity consumption statistics per household category, including min, max, average, median, and standard deviation.
 - **costs.[year]:** Mean electricity consumption and cost comparison for households with and without photovoltaic (PV) installations.
 - **dist.[year].[residents]:** Consumption distribution per household category, showing the frequency distribution of annual consumption.
 - **gridbalance.[year]:** Hourly grid interaction data, including net power flow to/from the grid and battery charging from the grid.
 - **households.[year].[residents]:** Detailed household-level information, including electricity consumption, PV production, battery metrics, and financial details (energy costs/income).
 - **households.stats.[residents]:** Daily consumption statistics per household.
 - **max.[year].[residents] / max_sol.[year].[residents]:** Records of peak household power demands and associated solar or grid power.
 - **power.[year].[appliance]:** Hourly power consumption profiles for each appliance type and household category.
 - **summary.[year]:** Annual overview of total energy consumption, solar production/utilization, heat generated by solar thermal collectors, and breakdown of heating/domestic hot water energy use.
 
 Additional output files related to power flow simulations (trafo.[nr], pfin/, and pfout/) are generated if configured.
 
 ## 6. Smart Grid (Load Shifting)
 
 resLoadSIM supports smart-grid control modes:
 
 - None: Smart-grid control disabled.
 - Peak Shaving: Limits peak energy consumption to a defined threshold by controlling smart appliances.
 - Profile: Adjusts household consumption to follow a specified consumption profile.
 - Price: Decentralized control influencing load distribution based on electricity tariffs.
 
 ## 7. Grid Voltage Control
 
 resLoadSIM interfaces with a power-flow solver (e.g., MATPOWER) to simulate voltage-driven grid interactions. It implements:
 
  - Undervoltage control: Reduces consumption if grid voltage falls below predefined limits.
  - Overvoltage control: Increases consumption if grid voltage exceeds set limits.
  - These mechanisms use hysteresis and partial signaling to prevent oscillations.
 
  ## 8. Space Heating and Cooling
 
 Heating/cooling demand simulation follows simplified energy balance equations derived from EN ISO 52016-1. It calculates internal temperatures and heat demands hourly, considering building characteristics (simplified to detached single-zone houses), but excludes thermal bridges, humidity, and solar heat input.
 
 Heating systems supported include:
 - Oil, Gas, District Heating
 - Heat Pumps with auxiliary electric heating
 - Solar Thermal Collectors combined with Heat Pumps
 
 ## 9. Variable Appliance Load
 
 resLoadSIM supports appliances with varying loads (e.g., washing machines). Configuration requires defining load profiles per appliance and energy efficiency class in varload.json.
 
 ## 10. Electric Vehicles
 
 Simulates EV charging based on residence location, household types, weekdays, battery specifications, user behavior, and PV availability. EVs are modeled distinctly (idle, driving, charging states) and can participate in smart-grid control schemes.
 Available battery strategies:
 - **Strategy 0:** Maximize PV self-consumption by immediate battery charging.
 - **Strategy 2:** Prevent grid overvoltage by intelligently timing battery charging using energy production and consumption forecasts.
 - **Strategy 3:** Combines strategies 0 and 2, adding morning pre-charging.
 
 Forecast methods include actual solar data, previous day's data, or external forecasts (GEFS). Optionally, surplus energy can be shared across household batteries.
