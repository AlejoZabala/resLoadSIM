# Household Energy Demand Simulation Using resLoadSIM

This repository provides the activation code required to run simulations using resLoadSIM, enabling users to model heat and electricity demand for residential buildings. Specifically, it contains input files and parameterizations tailored to simulate current and future energy consumption scenarios for households located in Oldenburg and Karlsruhe (Germany), and Auckland and Christchurch (New Zealand).

These resources facilitate reproducible analyses and support further studies on residential energy demand forecasting under varying conditions.

# About resLoadSIM
**resLoadSIM** is a stochastic bottom-up simulation tool for predicting electric residential load profiles of individual households with a high time resolution. For each household some individual characteristics are defined, e.g. number of residents, the size of the house or apartment and the devices used according to some statistical information. Each device is characterized by its energy class. In the simulation devices are used according to the estimated habits of the inhabitants. 

Weekly and seasonal variations are also captured. Beside using the tool for load prediction it is also possible to study the potential for load shifting in private households by implementing different control mechanisms based e.g. on variable pricing.

# How to obtain resLoadSIM
resLoadSIM was developed by the Joint Research Centre (JRC). The JRC works as the scientific advisory body of the European Commission, employing researchers to conduct studies and deliver impartial scientific guidance and expertise to inform and support European Union (EU) policies. 

You can find more infomration about the JRC Modelling Suit here: https://ses.jrc.ec.europa.eu/our-modelling-suite 

If you want to obtain resLoadSIM, you have to address directly to the JRC authors. Their contact information might be obtained directly with the JRC.

## Project structure
.
├── countries
│   ├── DE
│   │   └── locations
│   │       ├── Karlsruhe
│   │       └── Oldenburg
│   └── NZ
│       └── locations
│           ├── Auckland
│           └── Christchurch
├── README.md
└── structure.txt

9 directories, 2 files

# Acknowledgment and License Information
resLoadSIM, is provided by under an MIT License. This project cites resLoadSIM to acknowledge its intellectual property rights and licensing terms.


