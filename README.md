# APSingleCodebase
Contains pybind, bistability and time to pattern analysis methods.

## Use
**buildSpringAgent.sh**: This builds the springAgent executable (or python .so) locally.

**springAgent**: executable which needs to be run with [runtime arguments](#Runtime-args).

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

## Output

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
