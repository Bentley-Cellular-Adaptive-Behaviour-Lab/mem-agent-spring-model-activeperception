# APSingleCodebase
Contains pybind, bistability and time to pattern analysis methods.

## Use
There are two main scripts that are used to build / run the sim: 
1. **buildSpringAgent.sh**: This builds the springAgent executable (or python .so) locally.

2. **campScript.sh**: This connects to the camp, uploads the simulations cpp/h files onto it, builds the simulation executable (or python .so), performs the specified number of runs, copies the output files off camp back onto your local machine.

## Analysis types

### Bistability
- **Number of cells**: 3
- **Max time**: 10000000

### Time to pattern
- **Number of cells**: 10
- **Max time**: 5000 (will also end automatically if/when it patterns)

### Pybind
- **Number of cells**: 2
- **Max time**: Set via the --maxtime flag. By default it is 5000.


## Command line args
**Note**: the ordering of the arguments passed does not matter when using the buildSpringAgent.sh and campScript.sh. However, the ordering does matter when using the springAgent executable.

### Build args
- Both the buildSpringAgent.sh and campScript.sh require a '--analysis' flag. The flag should be followed with either "pybind", "bistability" or "time_to_pattern". This lets the script know which analysis type the simulation should be compiled into.

- The buildSpringAgent.sh can also be passed a '--graphics' flag which will turn graphics on in the compiled sim. This does not work with the pybind version or with the camp script.

![buildscript with args](/docs/buildscript_graphics.png?raw=true)

## Runtime args
The following arguments are taken in by the springAgent executable, in the following order:
1. **Run number** - integer - this is the run number which is used for naming output files.. NOT for specifying number of runs.
2. **Epsilon** - float
3. **VconcST** - float
4. **Gradient Type** - integer - 0 = flat, 1 = exp, 2 = steady, 3 = fixed macros, 4 = astro linear, 5 = curved circles 6 = astro uniform
5. **filconstnorm** - float - this corresponds to the FIL_VARY variable in the cpp codebase.
6. **filTipMax** - float
7. **tokenStrength** - float
8. **filspacing** - int - number of memAgents between filopodia.
9. **randFilExtend** - float - If value is not between 0 and 1 then random filopodia extension will be turned off. If values IS between 0 and 1, the value represents the chance of a random fil extension, **Epsilon will be set to 0 automatically**. Essentially unlinks extension from vegf signalling.
10. **randFilRetract** - float - If value is not between 0 and 1 then random filopodia retraction will be turned off. If values IS between 0 and 1, the value represents the chance of a random fil retraction. Essentially unlinks from vegf signalling.

![spring agent executable with args](/docs/springAgent_executable_args.png?raw=true)

### Camp script args
In addition to the --analysis flag, campScript.sh can also take the following list of args:
| Flag  | Value | Description  |
| ------------- | ------------- | ------------- |
| --runs  | integer  | number of runs that will be executed (concurrently)  |
| --epsilon  | float  | sets Epsilon value in sim  |
| --vconcst  | float  | sets VconcST value in sim  |
| --filconstnorm  | float  | sets fil vary value in sim  |
| --filtipmax  | float  | sets filTipMax value in sim  |
| --tokenStrength  | integer  | sets tokenStrength value in sim  |
| --randFilExtend  | float  | sets randFilExtendValue value in sim  |
| --randFilRetract  | float  | sets randFilRetract value in sim  |
| --vary1  | param name + value list. eg: epsilon "0.9 0.8 0.7"  | defines first param to covary and list of values in quotes |
| --vary2  | param name + value list. eg: filconstnorm "2.0 3.0"  | defines second param to covary and list of values in quotes |
| --vary3  | param name + value list. eg: filspacing "2 3 4"  | defines third param to covary and list of values in quotes |
| --maxtime  | integer  | **only for pybind version.** Sets the number of timesteps the APentropyintensity.py script runs for |

Whilst analysis, vary1 and vary2 flags must be passed into the script along with values, the other flags all have the following default values if no flag and value is passed in:
- **runs**: 1
- **Epsilon**: 0.9
- **VconcST**: 0.04
- **filConstNorm**: 2.0
- **filTipMax**: 15
- **tokenStrength**: 1
- **filspacing**: 2
- **randFilExtend**: -1
- **randFilRetract**: -1

**Example command with required args:**
![camp script with required args](/docs/campscript_required_args.png?raw=true)

**Example command with extra args:**
![camp script with extra args](/docs/campscript_full_args.png?raw=true)

## Output

**NOTE:** if running the camp script and the connection between your machine and camp servers is lost, the jobs will continue to run on camp. In this scenario, ssh into the camp using the `ssh login.camp.thecrick.org` command. Then `cd` into the APSingleCodebase folder. Use `ls` to list the files in the directory, this is where all output files will be written to. Use the `sacct` command to check if all jobs have finished running (the campscript will have printed your job ID to terminal). Once all jobs have finished use the scp command `scp <your crick username>@login.camp.thecrick.org:~/APSingleCodebase/\*.{txt,npy} <local directory you want to copy files to>` to copy the .txt and .npy off camp to your local machine.

### Bistability
Output file will be named "analysis_hysteresis_filvary_#val_epsilon_#val_VconcST#val_GRADIENT#val_FILTIPMAX#val_tokenStrength#val_run_#val_.txt" #val's will be replaced with corresponding param value.

### Time to pattern
Output file will be named "time_to_pattern_filvary_#val_epsilon_#val_VconcST#val_GRADIENT#val_FILTIPMAX#val_tokenStrength#val_run_#val_.txt" #val's will be replaced with corresponding param value. This will contain the timestep at which the simulation patterned or -1 if it failed to.

### Pybind

No output files. Instead there are several analysis functions exposed via the python wrapper which can be called in between timesteps or at the end of the simulation. These are as follows:

 - **World(float, float, int, float, float, float)**: "World constructor: World(float epsilon, float vconcst, int gradientType, float filConstNorm, float filTipMax, float tokenstrength, float randFilExtension, float randFilRetract)"
 
 - **World.simulateTimestep()**: "Simulate one timestep in MemAgent-Spring Model"
 
 - **World.simulateTimestep(std::vector<std::vector<float>>)**: "Simulate one timestep in MemAgent-Spring Model with 2d array of protein value changes for each cell."
 
 - **World.timeStep**: "Read / write current timestep in sim"
 
 - **World.gridXDimensions**: "Get x dimension of grid"
 
 - **World.gridYDimensions**: "Get y dimension of grid"
 
 - **World.gridZDimensions**: "Get z dimension of grid"
 
 - **World.getGridSiteData**: "Get grid site data in 2D array. Each index in first array represents a grid site then the second array contains X, Y, Z, grid site vegf, Grid Type (0 = environment, 1 = membrane), Number of sub agents (filopodia/memagents), Cell1 vegf (the amount of vegf the cell can see in neighbour grid sites from that grid site), Cell1 vegfr, Cell1 active vegfr, cell2 vegf, cell2 vegfr, cell2 active vegfr, cell3... etc etc"
 
 - **World.getFilopodiaBaseLocationsAndTotalVegfr**: "returns a 3 dimensional vector. first dimension is cell 1 then cell 2. second dimension contains vectors for all the filopodia in the cell. 3rd dimension is the filopodia values, these are: 0. base x coordinate, 1. base y coordinate, 2. base z coordinate, 3. total active vegfr in the filopodia"
 
 - **World.getGridMapOfFilopodiaMovement**: "returns 3d vector grid reflecting grid in simulation. The inner most vector contains an array of size 2. The first value in the array represents how many filopodia extensions into that grid site have occured on the current timestep. Second value represents the amount of retractions."
