#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <iomanip>
#include "objects.h"
#include <random>

//#include "ODEs.h"

using namespace std;
typedef Location** ppLocation;
typedef Location* pLocation;

unsigned long long rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

CPM_module::CPM_module(World* WorldP){

	worldP = WorldP;
	
}
//-------------------------------------------------------------------------------
MedAgent::MedAgent(World* WorldP){

    worldP = WorldP;
    diffAd_replaced=NULL;

}
//-------------------------------------------------------------------------------
Hysteresis::Hysteresis(void){
    Current_Dll4_incremented_level=0;
    stabilityTimer_overall=0;
    stabilityTimer_latest=0;
     Dll4_SigRange = dll4_SIG*(MAX_dll4/100.0f); //vary Dll4_SIG to find optimal sig range
    Actin_SigRange  = 1.0f*(actinMax/100.0f); 
    
    CELL_STABLE = 200; //useing PloS model first, with 28t for delays (15 mins), but if change GRN notch delays to 4hrs period, with rearrangement model NCB2014 paper, then increase this value.
   direction = true;
   HYST_INCREMENT_MAX = 2400;
   HYST_INCREMENT = 20;
   bio_time_window = 2000;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Contact::Contact(void){

        lifeTime=0;
        finished=false;
        fil1=NULL;
        fil2=NULL;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Filopodia::Filopodia(World* WorldP){
    base=NULL;
    Cell = NULL;
    retracted=false;
    
    worldP = WorldP;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Vessel::Vessel(void){
 	
 	
 	closed=false;
        
        tip=NULL;
}

//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
Location::Location(void){
 	
 	
 	Eid=NULL;
 	type=E;
        oldType = E;
}

//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
Env::Env(World* WorldP){
	VEGF= 0.0f;
	blood=0.0f;
	worldP=WorldP;
        inside=false;
        
        
        Astro = false;
        OldAstro = false;
}
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
MemAgent::MemAgent(EC* belongsTo, World* world){

	int i;
	diffAd_replaced_cell = NULL;
	diffAd_replaced_med = NULL;
     
        MEDIUM = false;
        mediumNeighs = 0;
	labelled = false;
        labelledBlindended = false;
        labelled2=false;
	mesh3SpringsOnly=false;
	Cell = belongsTo;
        created = 0;
	FA = false;
	FIL=NONE;
	
	worldP=world;
	retracted = false;
	junction = false;
	neighs=0;
	node = true;
	filTokens =0;
	Force[0]=0;
	Force[1]=0;
	Force[2]=0;
	SumVEGF=0;
	MneighCount=0;
	deleteFlag=false;
	VRinactiveCounter=0;
	filTipTimer=0;
	plusSite=NULL;
	minusSite=NULL;
	filPos = 0;
	springJunction=false;
	filNeigh=NULL;
	FATimer=0;
	activeNotch = 0.0f;
	Dll4=0.0f;
	VEGFR = 0.0f;
	VEGFRactive=0.0f;
	
	surgeSpringConst=false;
	veilAdvancing = false;
	vonNeu=false;
        assessed=false;
        addedJunctionList=false;
        

	
	for(i=0;i<meshNeighs+NEIGHSMAX;i++){
		neigh[i]=NULL;
		SpringNeigh[i]=NULL;
		
	}
	SpringBelong=NULL;



}//-----------------------------------------------------------------------------

MemAgent::~MemAgent(void){

	EnvNeighs.clear();
}

//-----------------------------------------------------------------------------
EC::EC(World*  world){

	worldP = world;
       

        mutant = false;
        
        filCONST = FIL_VARY; //LTK link add user config value link here
     
        //wt values FLTK link
        VEGFRnorm = VEGFRNORM;
        Vsink =VsinkNorm;
        
        if(VR2_HET==true){
                VEGFRnorm = VEGFRNORM*0.5;
        }
        if(VR1_HET==true) Vsink = Vsink*0.5;
        
        //add in for Dll4 HET/KO..
        
	VEGFRtot=VEGFRnorm;
	Dll4tot=0.0f;
	Notchtot=0.0f;
	activeNotchtot=0.0f;
	stableVEGFR=VEGFRnorm;
	base_of_longest_fil= NULL;
	length_of_longest_fil = 0;
        newJunction = 0;
        
        actinUsed=0;
	
        //check this bit works when switching between setup versions
       // if(user_defined_fils_spacing == true) fixed_filopodia_spacing = 0; //FLTK link up needed here to config GUI! 
       // else{
       //     if((CELL_SETUP==1)&&((ENV_SETUP==1)||(ENV_SETUP==4))) fixed_filopodia_spacing=1;//4;
       //     else 
       //     fixed_filopodia_spacing=1;
       // }
    
	int i;
	for(i=0;i<actNot_VEGFR_delay;i++){
		NotchDelayArray.push_back(0.0f);
	}
	for(i=0;i<actNot_VEGFR_lasts;i++){
		NotchLastsArray.push_back(0.0f);
	}
	for(i=0;i<VEGFR_dll4_delay;i++){
		VEGFRDelayArray.push_back(0.0f);
	}
	for(i=0;i<VEGFR_dll4_lasts;i++){
		VEGFRlastsArray.push_back(0.0f);
	}		


      
        VonNeighs = 0;
	
}//-----------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//World::World(void){

//    Pause = 0;
//    timeStep=-1;
//    dataFile.open("output.txt");
	
	
	
//    int i,j;
//    grid=new ppLocation[xMAX];

//        for(i=0;i<xMAX;i++){
			
//            grid[i]=new pLocation[yMAX];
			
//                for(j=0;j<yMAX;j++){
				
//                    grid[i][j]=new Location[zMAX];
//                }
				
//        }
//    VEGFgradient = GRADIENT;

//    std::vector<int> myvector;
//    // set some values: TODO: this is the vector shuffle test.. shows that using own rand func for random shuffle returns same value everytime
//    //TODO: maybe replace random_shuffle as it causing issues
//    for (int i=1; i<10; ++i) myvector.push_back(i); // 1 2 3 4 5 6 7 8 9

//    // using built-in random generator:
//    //std::random_shuffle ( myvector.begin(), myvector.end() );

//    // using myrandom:
//    std::shuffle ( myvector.begin(), myvector.end(), std::default_random_engine(100));
//    //shuffle ( myvector.begin(), myvector.end());

//    // print out content:
//    std::cout << "myvector contains:";
//    for (std::vector<int>::iterator it=myvector.begin(); it!=myvector.end(); ++it)
//      std::cout << ' ' << *it;

//    std::cout << '\n';
//    #ifdef GRAPHICS
//        store_normals();
//    #endif

//   scale_ProtLevels_to_CellSize();
//}
World::World(float epsilon, float vconcst, int gradientType, /*float yBaseline,*/ float filConstNorm, float filTipMax, float tokenstrength, int filspacing, float randomFilExtend, float randFilRetract)
{
    if (TESTING == true)
        srand(100);
    else
        srand(rdtsc());

    EPSILON = epsilon;
    VconcST = vconcst;
    FIL_VARY = filConstNorm;
    FILTIPMAX = filTipMax;
    tokenStrength = tokenstrength;
    GRADIENT = gradientType;
    FIL_SPACING = filspacing;
    randFilExtend = randomFilExtend;
    RAND_FILRETRACT_CHANCE = randFilRetract;

    //this->yBaseline = yBaseline;

    cout << "world fil const: " << to_string(FIL_VARY) << endl;
    cout << "world fil tip max: " << to_string(FILTIPMAX) << endl;
    cout << "world token str: " << to_string(tokenStrength) << endl;

    if (randFilExtend >= 0 && randFilExtend <= 1)
        EPSILON = 0;

    cout << "Creating world..." << endl;

    //    char outfilename[21];
    //    char outfilename2[21];
    //    char outfilename3[21];

    //    sprintf(outfilename, "Vsink%1.3f_tipcells.txt", Vsink);
    //    sprintf(outfilename2, "Vsink%1.3f_VRoverTH.txt", Vsink);
    //    sprintf(outfilename3, "Vsink%1.3f_Avescores.txt", Vsink);
    //    RUNSfile.open(outfilename);
    //    RUNSfile2.open(outfilename2);
    //    RUNSfile3.open(outfilename3);
    Pause = 0;
    timeStep = -1;
    dataFile.open("actions.txt");
//    dataFile2.open("perceptions.txt");
//    dataFile3.open("S&P.txt");

    gridXDimensions = xMAX;
    gridYDimensions = yMAX;
    gridZDimensions = zMAX;

    cout << "xMax: " << xMAX << " yMax: " << yMAX<< " zMax: " << zMAX << endl;

    int i, j;
    grid = new ppLocation[xMAX];
    for ( int i = 0; i < xMAX; i++)
    {
        grid[i] = new pLocation[yMAX];
        for (int j = 0; j < yMAX; j++)
            grid[i][j] = new Location[zMAX];
    }



    VEGFgradient = GRADIENT;

    #if GRAPHICS
        store_normals();
    #endif

    scale_ProtLevels_to_CellSize();
    cout << "Grid size " << gridXDimensions << "x" << gridYDimensions << "x" << gridZDimensions << endl;
    cout << "World created!" << endl;
    //run simulateTimestep once to perform creation timestep
    simulateTimestep();

    cout << "Creation timestep complete." << endl;
}

World::World()
{
    cout << "Creating world..." << endl;
    if (TESTING == true)
    {
        srand(100);
    }
    else if (test_seed >= 0)
    {
        cout << "seeding with: " << test_seed << endl;
        srand(test_seed);
    }
    else
    {
        srand(rdtsc());
    }

    //    char outfilename[21];
    //    char outfilename2[21];
    //    char outfilename3[21];

    //    sprintf(outfilename, "Vsink%1.3f_tipcells.txt", Vsink);
    //    sprintf(outfilename2, "Vsink%1.3f_VRoverTH.txt", Vsink);
    //    sprintf(outfilename3, "Vsink%1.3f_Avescores.txt", Vsink);
    //    RUNSfile.open(outfilename);
    //    RUNSfile2.open(outfilename2);
    //    RUNSfile3.open(outfilename3);
    Pause = 0;
    timeStep = -1;
    dataFile.open("actions.txt");
//    dataFile2.open("perceptions.txt");
//    dataFile3.open("S&P.txt");

    gridXDimensions = xMAX;
    gridYDimensions = yMAX;
    gridZDimensions = zMAX;

    cout << "xMax: " << xMAX << " yMax: " << yMAX<< " zMax: " << zMAX << endl;

    int i, j;
    grid = new ppLocation[xMAX];
    for ( int i = 0; i < xMAX; i++)
    {
        grid[i] = new pLocation[yMAX];
        for (int j = 0; j < yMAX; j++)
            grid[i][j] = new Location[zMAX];
    }



    VEGFgradient = GRADIENT;

    #if GRAPHICS
        store_normals();
    #endif

    scale_ProtLevels_to_CellSize();
    cout << "Grid size " << gridXDimensions << "x" << gridYDimensions << "x" << gridZDimensions << endl;
    cout << "World created!" << endl;
    //run simulateTimestep once to perform creation timestep
    simulateTimestep();

    cout << "Creation timestep complete." << endl;
}
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
World::~World(void){

    destroyWorld();
    dataFile.close();

    for(int i = 0; i < xMAX; i++)
    {
        for (int j = 0; j < yMAX; j++)
        {
            delete[] grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
}
//----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
Spring::Spring(void){

	start=NULL;
	end=NULL;
	oldSize=0;
	filopodia=false;
	Junction=false;
	deletion = false;
	retracted = false;
	horizontal = false;
	veilAdvancing = false;
	right = true;
	labelled = false;
        
	
}
//-----------------------------------------------------------------------------
Coordinates::~Coordinates(void){

}
//-----------------------------------------------------------------------------
Coordinates::Coordinates(void){

	
}
//-----------------------------------------------------------------------------
Macrophage::Macrophage(World* WP){

	worldP = WP;
	
}

