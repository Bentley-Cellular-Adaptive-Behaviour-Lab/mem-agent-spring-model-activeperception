#ifndef OBJECTS_H_
#define OBJECTS_H_

/**
 * \file objects.h
 *
 * \author Katie Bentley
 * \date 2006
 *
 * Development ongoing
 */

#if BAHTI_ANALYSIS
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
namespace py = pybind11;
#endif

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>


///------------------------------------
using namespace std;

class Hysteresis;
class MemAgent;
class EC;
class World;
class Spring;
class Coordinates;
class Macrophage;
class Contact;
class Filopodia;
class CPM_module;

//#define GRAPHICS true
///main switches and settings
//#ifdef GRAPHICS
//#define graphics true
//#else
//#define graphics false
//#endif

/// below defines are set through makefile
//#define ANALYSIS_HYSTERESIS true
//#define ANALYSIS_TIME_TO_PATTERN true
//#define BAHTI_ANALYSIS //TODO: add bahti stuff back in
//#define GRAPHICS false
/*#define ECpack 4
#define MAXtime 10000000*///2000 ///480 for fil lifetime

extern ofstream RUNSfile;
///analysis/quantification
#define SigRange 15.0f*(VEGFRnorm/100.0f) ///percentage of total VEGFR poss, within this range we say the cell is stable.
#define TIP_VEGFR 50*(VEGFRnorm/100.0f)///set as over 50% - its the lower limit for no of VEGFR needed to qualify as a tip cell.
#define TIP_MEMS 1.2///lower limit on no. of Magents needed to qualify as a tip cell, X times the initial value TIP_MEMS is X
#define ANALYSIS_SHUFFLING false
#define ANALYSIS_COMS false
#define ANALYSIS_JTB_SP_PATTERN false ///stability score pattern score tip cell number
#define ANALYSIS_TOTALVEGF_TOTAL_MEMBRANE true
#define ANALYSIS_contactsTest false ///only use if quantifying filopdia contacts for anastomosis (as in Benltey PLoS comp Biol 2009)
#define ANALYSIS_PROTLEVELS true
//#define ANALYSIS_HYSTERESIS true
//#define ANALYSIS_TIME_TO_PATTERN false

#define TESTING  false //if testing the behaviour against a deterministic version (random numbers always generated the same throughout for stochastic elements, seeded with 100)
#define on_the_fly_surface_agents false ///faster as doesnt do voxellisatoin but cant use for full runs as not correct
#define oldVersion false ///old VEGFR-2 activatoin function from JTB 2008

#define TOROIDAL_X true//cell_setup2: false ///will want if vessel (JTB or PLos) or some monolayers
#define TOROIDAL_X_env true //this is true for the NCB and rearrnagement setups even though strictly the vessel cannotgrow toroidally (only env lookup toroidal) TOROIDAL_X will be false in this case ..
#define TOROIDAL_Y false ///for 2D monolayers

///WORLD_SETUP
#define gap 4 //cell_setup_2: gap = 4
#define Ygap 40//cell_setup_2: Ygap=40
#define Zgap 1
#define xMAXplus 40
#define xMAX vesselLength// cell_setup2: vesselLength+xMAXplus
#define yMAX ((vesselRadius*2)+gap+1000)//cell_setup2: ((vesselRadius*2)+(Ygap*2))
#define zMAX (vesselRadius*2)+(gap*2)//cell_setup2: ((vesselRadius*2)+(2*Zgap))

///CELL_SETUP
#define CELL_SETUP 1 ///1 = vessel toroidal bottom of y axis (PLoS), 2 = blind ended sprout (NCB, rearrangement papers centre y axis, non toroidal), 3 = monolayer, 4 = single cells in a dish, 5 = zebrafish
#define BLINDENDED_SPROUT false
#define ECELLS (ECcross*ECpack)
#define ECcross 1///cell_setup2: 2 //no. of ECELLs to a cross section of vessel

#define ECwidth 20///cell_setup2: 10 //width along vessel of ECELL = width of one ECpack
#define vesselCentreY (vesselRadius+gap+0.5)//cell_setup2: (vesselRadius+Ygap)
#define vesselCentreZ (vesselRadius+gap+0.5)//cell_setup2:(vesselRadius+Zgap)
#define vesselLength (ECpack*ECwidth)
#define bloodRadius 4 //cell_setup2: 2
#define vesselRadius (bloodRadius+2) //cell_setup2: (bloodRadius+1)

///monolayer specific
#define MONOLAYERcells 5

///ENVIRONMENT SETUP
#define ENV_SETUP 1//6 /////Environment types:

//(vessel setup)

//Environment 1 (with CELL_SETUP=1): 3D VEGF gradient (increasing in y axis) (as in Bentley et al JTB 2008), Environment 2 with CELL_SETUP=1): 2D VEGF gradient adhered to astrocyte grid (as in PLoS Comp Biol), Environment 3 with CELL_SETUP=1): 2D uniform gradient , Environment 4 with CELL_SETUP=1): 3D uniform gradient (as in Bentley et al JTB 2008), Environment 5 with CELL_SETUP=2):  VEGF radiating around horizontal/vertical sprout (Jakobsson et al 2010), Environment 6 with CELL_SETUP=2): VEGF increasing in x axis (Bentley et al rearrangement submitted), Environment 7 with CELL_SETUP=3): VEGF above monolayer (Ubezio et al 2013); Environment 8: RETINA ( VIllefranc 2012)
#define VEGFbase 0//2.1 ///normal 2.1
extern float horV;
extern float perpV;
extern float HorCutOff;
#define RETINA 45
#define SQUARES 0
#define ZEBRAFISH 3
#define DIAG 1
#define UNIFORM 2
#define ASTRO NONE//UNIFORM//ENV 6: UNIFORM///RETINA///NONESQUARES///SQUARES///
#define astroWidth 8///8 ///3microns
#define astroSpacer 20///11///11
#define Vspacer 30
#define FLAT 0
#define EXP 1
#define STEADY 2
#define ASTRO_LINEAR 4
#define ASTRO_UNIFORM 6
#define FIXED_MACROS 3
#define CURVED_CIRCLES 5
#define astroCurveDepth 15
#define BACKGROUND_VEGF NONE //STEADY
#define MACROS 0///2///2
extern int GRADIENT;//FIXED_MACROS////ASTRO_UNIFORM///FIXED_MACROS///ASTRO_LINEAR///specifies the current VEGF gradient type
extern float VEGFconc; ///conc of VEGF in flat gradient
extern float VconcST;
extern float VconcSTMACRO;
extern long long test_seed;

///cytoskeleton
#define FILOPODIA true
#define VEIL_ADVANCE false
#define ANASTOMOSIS false
extern float actinMax;///128(for 0.25 scale cells (rearrangement model)///512(in JTB 2008 and PLoS CB 2009)///max filagests extra, calcs on the fly with retraction = 1 and extension  =1 rather than i actual memagents as they are updated synchronously in this version, in calc force...and remove/grids spring agents
#define RAND_VEIL_ADVANCE_CHANCE 0.000
extern float RAND_FILRETRACT_CHANCE;
#define NewNodeLength 4
#define TokDisRate 5
extern int FIL_SPACING; //determines how close in grid sites radius around it, a new filopoidum can extend from preexisting ones. Set to 4 if 3D vegf in envrionment, or 1 if using a more 2D env eg vegf from astrocytes.. used in tryTokenPassRadius from VEGFresponse()
extern float FILTIPMAX; /// max time a memagent can remain at filopodia tip before it begins to collapse.
extern float tokenStrength; ///no. of tokens need to extend filopodia by 0.5 microns. increasing param value acts like a drug reducing the actin repsonse or signal to actin.
extern float randFilExtend;
///GRN signalling pathways
extern float NotchNorm;
extern float VEGFRNORM; /// (46000.0f/100.0f)*48.6f ///total of receptors it will maintain if all else is equal - divides out to M agents
extern float VEGFRmin; /// (1000.0f/100.0f)*48.6f///min level total VEGFR is allowed to drop to (Holger said wont go to zero..)
extern float sigma; ///no. of VEGFR recs lost by one active notch receptor.
extern float MAX_dll4; ///max amount of VEGF that will induce the same amount of notch/dll4 - after this it will induce only amount specified in this param - from Liu03 paper
extern float delta;
#define actNot_VEGFR_delay 28//CEll_setup 2: 120///28 ///no. of timesteps before active notch affects VEGFR expression
#define actNot_VEGFR_lasts 1 ///no of timesteps activ notch effect on VEGFR expression lasts - after which VEGFR levels will return to normal (given no other active notch delays run out)
#define VEGFR_dll4_delay 28//120CEll_setup 2: 120////28  ///no. of timesteps it takes for an active VEGFR receptor to cause dll4 expression. goes on the dll4array stack
#define VEGFR_dll4_lasts 1///no of time steps the active effect of VEGFR has on dll4 up reg - before it goes back to normal.

///Rearrangement
#define REARRANGEMENT false
#define TIME_DIFFAD_STARTS 100000
#define ideal_Cell_area_init 90
#define biased_mig_diffAd true
extern int MCS;
#define BIAS_DIFFAD_CHANCE 0.75
#define MAJORITY_LEAD_PERCENTAGE 75
extern float M1_neta;
extern float M2_lambda;
#define mechanism_variant 1
#define Medium_width 4

///mutant switches
#define VR2_HET false
#define VR1_HET false
#define MUTANTS false
#define VsinkNorm 9.0f

///SPRING_SETUP
#define springConstant 0.05f
#define FAspringConstant 0.05f
#define filSpringConstant 0.95f
#define filBaseConstant 0.7f 
#define junctionConstant 0.4f
#define filSpringLength 0.1f
#define springLength 1.0f
#define JunctionSpringLength 0.5f


/////analysis/quantification
//#define SigRange 15.0f*(VEGFRnorm/100.0f) ///percentage of total VEGFR poss, within this range we say the cell is stable.
//#define TIP_VEGFR 50*(VEGFRnorm/100.0f)///set as over 50% - its the lower limit for no of VEGFR needed to qualify as a tip cell.
//#define TIP_MEMS 1.2///lower limit on no. of Magents needed to qualify as a tip cell, X times the initial value TIP_MEMS is X

//#define ANALYSIS_SHUFFLING false
//#define ANALYSIS_COMS false
//#define ANALYSIS_JTB_SP_PATTERN false ///stability score pattern score tip cell number
//#define ANALYSIS_TOTALVEGF_TOTAL_MEMBRANE true
//#define ANALYSIS_contactsTest false ///only use if quantifying filopdia contacts for anastomosis (as in Benltey PLoS comp Biol 2009)
//#define ANALYSIS_PROTLEVELS true

///defining memAgent attributes
///states
#define NONE -1
#define STALK 3
#define TIP 1
#define BASE 2
///initial number of memAgents , calculated at t=0 (used to calculate growth later)
extern int memINIT;
///defining memAgent mesh
#define meshNeighs 4 ///how many connections does each node agent have


///junctional arrangement offset settings
///Bentley et al PLos Comp Biol 2009
#define EQUAL_OFFSET 1
#define UNEQUAL_NEIGHS 2
#define OPTIMAL_UNEQUAL 3
extern float CellPosOffset;


///declared constants
#define Pi 3.1415927
#define NEIGHSMAX 25
#define NEIGH 26
#define E 0
#define M 1
#define BLOOD 4
#define Mac 2
#define MED 5

///GUI related
#define user_defined_fils_spacing false


extern bool MEM_LEAK_OCCURRING;

enum Gradient {
    flat,
    exp_,
    steady,
    fixedMacros,
    astroLinear,
    curvedCircles,
    astroUniform,
};

extern Gradient gradient;

extern float EPSILON;

//hysteresis
extern float dll4_SIG;
extern float FIL_VARY;
///---------------------------------------------------
class Hysteresis{
//each cell has its own hysteresis object within it storing its current stable levels..
public:
    EC* Cell;
    float Current_Dll4_incremented_level; //for fixed end cells

    //constants
    //constants
    float Dll4_SigRange;
    float Actin_SigRange;
    int CELL_STABLE; //no. of timesteps required to say cells current levels are stable
    int HYST_INCREMENT_MAX;
    int HYST_INCREMENT;
    int bio_time_window;

    //variables
    float stableDll4; //set this to initial value and start stabilising timer. Once current level outside of sigRanges from this stable level, reset to current level, reset curent timer, continue wth overall timer.
    float stableActin;
    int stabilityTimer_overall;
    int stabilityTimer_latest;
    bool direction; //increasing (true) or decreasing (false) dll4 increment

    vector <float> storeDll4;
    vector <float> storeTimes;

   Hysteresis(void);
   bool evaluate_hysteresis(ofstream& fileTo);

};

class Contact {
    ///to quantify contacts between filopodia before anastomosis occurs
    ///used in Bentley et al PLoS Comp Biol 2009
public:
    Filopodia* fil1;
    Filopodia* fil2;
    int startTime;
    int lifeTime;
    bool finished;
    bool updated;
    Contact(void);
};
///----------------------------------------
///----------------------------------------

class Filopodia {
    ///for quantification of filopodia dynamics (used in Bentley et al PLoS Comp Biol 2009)
public:
    World* worldP;
    MemAgent* base;
    int time_created;
    int time_retract_complete;

    EC* Cell;
    bool retracted;
    vector <Contact*> contactList;

    vector <Filopodia*> checkForContact(void);
    Filopodia* findFil(MemAgent* mp);

    Filopodia(World* WorldP);
};
///----------------------------------------

///----------------------------------------
class MedAgent{

public:

    World* worldP;
    float Mx;
    float My;
    float Mz;

    MemAgent* diffAd_replaced;
    int mediumNeighs;
    void checkNeighs();
    vector <MemAgent*> DiffAd_neighs;

    MedAgent(World* WorldP);

};
///----------------------------------------
///----------------------------------------
class CPM_module{

    public:
    World* worldP;
    float Hamiltonian;
    float J_sum;
    float area_sum;
    float new_Hamiltonian;

        std::vector <MedAgent*> mediumAgents;

    ///initial size of heaxagonal cell...
    #define ideal_Area  ideal_Cell_area_init///64 at start
        #define ideal_Perimeter 18 ///starts as 15


    #define J_TT 14///21
    #define J_TS  11///11
    #define J_SS  4///8
        #define J_MC  16///21


    int Temp;

        bool check_gradient(MemAgent* replaced_mem, MemAgent* replacer_mem);
    CPM_module(World* WorldP);
    void run_CPM(void);
    void calc_J_sum(void);
    void calc_area_sum(void);
    float Hamiltonian_change(void);
    bool calcProb(float change);
    void replace_agent(MemAgent* replacer_mem, MemAgent* replaced_mem, MedAgent* replacer_med, MedAgent* replaced_med, int pos);
        float calc_local_change(MemAgent* replacer_mem, MemAgent* replaced_mem, MedAgent* replacer_med, MedAgent* replaced_med);
        float calcLocal_Jsum(MemAgent * replaced_mem, MedAgent * replaced_med);
        float calc_Jsum_individual(MemAgent * individual, MedAgent* medInd);
        void calc_Cell_areas(void);
        void createMedium(void);
    void replaceSprings(MemAgent* replaced_mem, MemAgent* replacer_mem, MedAgent* replacer_med, MedAgent* replaced_med);
    int connectivity(MemAgent* replaced, MemAgent* replacer);
        void update_displaced_surfaceAgents(MemAgent* replaced_mem, MemAgent* replacer_mem);
        void clearUpSmallSeparatedBitsOfCells(void);
        bool move_fil_base(MemAgent* mp);
};
///----------------------------------------


///----------------------------------------

class Coordinates {
    ///storage class for continuous space positions
public:
    float x;
    float y;
    float z;
    Coordinates(void);
    ~Coordinates(void);
};

///----------------------------------------


///---------------------------------------------------------
class Vessel{

public:
    Coordinates start;
    Coordinates end;
    EC* tip;
    bool closed;

    ///check_if_vessel_close(void);

    Vessel(void);

};
///---------------------------------------------------------
///---------------------------------------------------

class Env {
public:
    ///pointer to the world class
    World* worldP;
    ///coordinate position of Env agent
    int Ex, Ey, Ez;
    ///env agent VEGF level
    float VEGF;
    ///no flow, just
    float blood;
    ///is it inside the vessel
    bool inside;
    ///does it contain astrocytes
    bool Astro;
    ///did it contain astrocytes before cell membrane occupied it
    bool OldAstro;


    int checkForAstro(void);
    int checkForEnv(void);
    Env(World* WorldP);
    ///check if the env agent is inside the vessel
    void calcInside(void);


};
///------------------------------------------------------------------------------------
///General storage classes
///------------------------------------------------------------------------------------

class Location {
    ///the grid is made up of location objects which then house the types of agents located there
public:
    ///cell body memAgent identifiers
    std::vector <MemAgent *> Mids;
    ///filopodia memAgent identifiers
    std::vector <MemAgent *> Fids;
    ///environment agent identifier
    Env * Eid;

    ///type is either M (membrane), E (Environment but can contain filopodia) or Blood (inside vessel label)
    int type;
    ///buffer type
    int oldType;

    ///testing CPM with medium around the cells
    MedAgent* med;

    Location(void);
};
///----------------------------------------
///----------------------------------------

class EC {
public:
    World* worldP;
    Hysteresis hyst;
    ///general
    double En_VEGF;
    int VonNeighs; ///how many memAgents with von neumann neighbour Env objects does it have
    float red, blue, green; ///individual cell colour allocation

    int calcVonNeighs(void); ///classifies memAgents with von neumann neighbour Env objects, so that receptors arent allocated to ruffled membrane which dont have surface access to the environment

    ///cytoskeleton
    float filCONST;
    int filCounter;
    float actinUsed;
    void calcCurrentActinUsed(void);
    int newJunction;
    int fixed_filopodia_spacing;

    ///Analysis and quantification
    vector <int> junctionSizes; ///used in characterize notch boundaries for PLoS CB uneven junctions study
    MemAgent* base_of_longest_fil;
    int length_of_longest_fil;
    float stableVEGFR; ///relates to calcStability();
    std::vector<Coordinates*> COMstore;
    std::vector <int> stableArray;

    void store_cell_COM(void); ///rearrangement model analysis
    void characterizeActNotchBoundaries(int which, int other); ///PLoS CB analysis of uneven junctions
    bool tipCellTest(void); ///JTB_analysis method
    void calcStability(void); ///JTB_analysis method
    Coordinates calcCOM_toroidal(void);

    ///GRN signalling pathways
    float Vsink; ///VEGFR-1 parameter
    float VEGFRnorm; //VEGFR2 level
    float actNotCurrent; ///active Notch after time delay (able to affect Gene expression)
    float actVEGFRcurrent; ///active VEGFR-2 after time delay (able to affect Gene expression)
    float VEGFRtot; ///VEGFR-2 receptor expression level
    float Dll4tot; ///Dll4 ligand expression level
    float Notchtot; ///Number of Notch receptors
    float activeNotchtot; ///activeNotch level in timestep
    float activeVEGFRtot; ///active VEGFR-2 level in timestep
    std::vector <float> NotchDelayArray;
    std::vector <float> NotchLastsArray;
    std::vector <float> VEGFRDelayArray;
    std::vector <float> VEGFRlastsArray;

    void allocateProts(void);
    void updateProteinTotals(void);
    void updateMembraneProteins(void);
    void GRN(void);
    void NotchDelay(void);
    void VEGFRDelay(void);
    void calcActNotCurrent(void);
    void updateActVEGFRArray(void);
    void updateVEGFRLastsArray(void);

    ///rearrangement
    int ideal_Cell_area;

    ///mutants
    bool mutant;
    float VEGFR2_mutant; //scalar to alter normal value by -set to 0.5 if het, set to 0 if KO, set to 1 for wt
    float VEGFR1_mutant; //scalar to alter normal value by - set to 0.5 is het.  set to 1 for wt

    ///memAgents and springs belonging to this cell
    std::vector <MemAgent *> nodeAgents;
    std::vector <MemAgent *> springAgents;
    std::vector <MemAgent *> surfaceAgents;
    std::vector <Spring*> Springs;

    ///ECmesh functions
    std::vector<MemAgent*> doubled_up_surface_agents_list;///for fast on-the-fly voxelsiation (do not use in main simulatoins, not complete voxelisation, only fast for debugging)

    bool testSpringLength(float P[3], float N[3], Spring* stp);
    void gridSpringAgents(float P[3], float N[3], bool toroidal, Spring* stp);
    void createSpringAgent(int x, int y, int z, Spring* stp);
    void removeSpringAgents(void);
    bool toroidalTest(float P[3], float N[3]);
    bool test_Pos_change(MemAgent* first, MemAgent* second);
    void createSpringTokenObject(MemAgent* start, MemAgent* end, int neigh); ///create spring
    void newNodes(void);
    void removeStalkNode(Spring *STP);
    void recruitNodes(void);
    void remove_DoubledUp_SurfaceAgents(void);
    void gridAgents(void); ///snap continuous space positions of meAgents to the gridded lattice including surface agent creation via voxelisation


    EC(World* world);

    std::vector<std::array<int, 3>> filopodiaExtensions;
    std::vector<std::array<int, 3>> filopodiaRetractions;
};
///----------------------------------------

///----------------------------------------

class MemAgent {
public:

    ///general
    float Mx, My, Mz; ///continuous space coordinates
    EC* Cell; ///pointer to the cell agent it belongs to
    World* worldP; ///pointer to the world object
    unsigned short neighs; ///how many neighbours
    unsigned short MneighCount; ///from checkNeighs, how many membrane neighbours
    std::vector <Env *>EnvNeighs; ///from checkNeighs for local grid look up
    bool addedJunctionList; ///keeping track of junction agents each timestep
    bool deleteFlag; ///keeping track of memAgents scheduled for deletion at the end of the timestep so they dont further update or affect things
    bool vonNeu; ///if it has environment in its von neumann neighbours
    unsigned short circlePos; ///setting up agents around cylinder for vessel setup CELL_SETUP=1
    bool labelled;
    bool labelledBlindended;
    bool labelled2;
    bool assessed;
    bool mesh3SpringsOnly; ///relates to a rearrangement extension (functions removed) could remove

    void JunctionTest(bool StoreInJunctionList);
    void checkNeighs(bool called_from__CPM_module);
    void calcForce(void);
    void moveAgent(float newX, float newY, float newZ, bool FAset);

    ///states
    bool junction; ///is agent on a junction with another cell true/false
    bool springJunction; ///is a node with a spring on a junction coming from it
    bool node; ///true/false, if not a node it is a spring or surface agent which lies on top of the mesh, if it is a node it has four springs coming from it

    ///cytoskeleton and filopodia
    unsigned short filTipTimer;
    short FIL; ///state of the agent in terms of filopodia (BASE = base of a fil, NONE = not in a fil, STALK = middle of a fil, TIP = tip of a filopodia)
    bool FA; ///focal adhesion, keeps it stuck in current position for good in current version if true.
    unsigned short filPos; ///involved in inserting a new node in filopodia
    bool surgeSpringConst;
    bool veilAdvancing;
    int VRinactiveCounter; ///if inactive for longer than FILTIPTIMER it wont extend filopodia any mmore and start to retract
    int filTokens;
    int FATimer;
    MemAgent* minusSite; ///if in a filopodia this points to memAgent backwards in fil (not always perfect allocation of storage)
    MemAgent* plusSite; ///if in a filopodia this points to memAgent ahead in fil
    MemAgent* filNeigh; ///points to the nodeAgent at the other end of a spring backwards into filopdia (more reliable)
    float FilLength(int type); ///used ot determine current actinUsed
    Env * findHighestConc(void); ///guides filopodia direction in VEGF gradient direction
    void TokenTrading(void); //old version of token trtading used in Bahti APsig version.

    ///vessel fusion
    void anastomosis(MemAgent* other); ///if cell body from two cells meet, checks rules for deciding wether to create a new juncton spring
    bool form_filopodia_contact(void); ///veil advance only possible if one filopodia makes contact with another cells filopodia

    ///analysis and quantification
    bool retracted; ///used in checking filopodia dynamics and contacts between filopodia
    Filopodia* base_fil_belong; ///involved in quantification of filopidia dynamics
    void tryActinPassRadiusN(int x, int y, int z, int N); ///pass actin to nearest neighbours with filopodia
    void veilAdvance(void); ///control release of filopida FAs and movement of lamella
    bool extendFil(void); ///extend filopodia
    bool filRetract(void); ///retract filopodia
    void ActinFlow(void); ///actin dynamics, simple and high level at present.. degradation after 5 timesteps
    void calcRetractDist(void); ///NEEDED TO CALC CURRENT ACTIN USAEAGE for limit on fil extension
    void triggeredAnastSurge(MemAgent* junctionedAgent); ///pulls junction together

    ///mesh
    int created; ///stores timstep when created as a surface agent (could be a debugging element to remove)
    float Force[3]; ///force vector to sum forces from mesh springs to determine memAgent movement
    Spring* SpringBelong; ///if it is a spring agent which spring does it belong to
    Coordinates previous; ///previous position before last move, useful for determining if surface triangle has changed
    std::vector <MemAgent*> triangle;
    std::vector <MemAgent*> surfaceAgentsUP;
    std::vector <MemAgent*> surfaceAgentsDOWN;
    std::vector <MemAgent*> surfaceAgents3mesher;
    vector <MemAgent*> getTriangle(bool up);
    vector <MemAgent*> three_erTriangleFix(void); ///remove possibly, for a removed function
    MemAgent* findConnectedNode(bool Hor1, bool Right1, bool Hor2, bool Right2, int distance);
    Spring* SpringNeigh[meshNeighs + NEIGHSMAX];
    MemAgent* neigh[meshNeighs + NEIGHSMAX];
    MemAgent* find_nearest_triangle_node(void);
    void gridSurfaceAgents(void);
    void store_previous_triangle_pos(void);
    bool VoxTriag_sizeTest(vector <Coordinates> nodes);
    void connectJunctions(bool alsoNormalSprings); ///used to create monolayer connected mesh
    bool meshConnected(MemAgent* tocheck); ///used to check if two neghbours in space are actually neighbours in the mesh (for rearrangement so that overlapping folder membrane couldnt try to make copyflips via springs that dotn exist)

    ///GRN and signalling pathways
    float VEGFR;
    float Notch1;
    float Dll4;
    float activeNotch;
    float VEGFRactive;
    float SumVEGF;

    bool checkNeighsVonForEnv(void); ///check if memAgent has env in von neuman neighbours, as only allocates receptors to memAgents with full surface exposed, to avoid ruffled surfaces
    void VEGFRresponse(void); ///activate VEGFR receptors and also trigger filopoida extension
    void NotchResponse(void); ///activate Notch receptors with Dll4

    ///rearrangement
    bool MEDIUM;
    std::vector <MemAgent*> DiffAd_neighs;
    EC* diffAd_replaced_cell;
    int mediumNeighs;
    MedAgent* diffAd_replaced_med;

    MemAgent(EC* belongsTo, World* world); ///constructor
    ~MemAgent(void); ///destructor
};
///----------------------------------------

class Spring {
public:
    MemAgent* start;
    MemAgent* end;

    ///states
    bool filopodia;
    bool Junction;
    bool retracted;
    bool horizontal;
    bool right;
    bool veilAdvancing;
    bool labelled;
    unsigned short oldSize; ///to determine if the spring has changed and needs re-voxelising on the new timestep
    bool deletion;

    std::vector <unsigned short> filTokPos; ///stores spring agents on this springs actin token levels and positoins incase they need ot be redistributed to new spring agents when revoxelised

    std::vector <MemAgent *> agents; ///list of this springs spring agents covering it


    Spring(void);


};

///----------------------------------------
///----------------------------------------
///----------------------------------------

class World {
public:

    ///general
    std::vector <EC *> ECagents;
    int Pause;
    int timeStep;

    ///grid
    Location*** grid;
    Location neigh[NEIGH];
    ofstream dataFile; ///runtime file to write to while developing
    bool insideWorld(int i, int j, int k); ///checks if a given location is inside the world, incase tryingot move an agent outside grid dimensions
    void setFilLocation(int x, int y, int z, MemAgent * ident); ///add filopodia memAGent to the grid reference at a given location
    void deleteOldGridRef(MemAgent*, bool fil); ///moving a filopodia agent requires deleting its identifier from a location
    bool toroidalTest(float P[3], float N[3]);

    ///environment
    int VEGFgradient;
    void check_if_InsideVessel(void);

    ///cells
    std::vector <MemAgent *> ALLmemAgents;
    std::vector <MemAgent *> JunctionAgents;
    std::vector <Macrophage *> macrophages;
    std::vector <EC*> fusedCells;
    std::vector <int> fusedCells_times;
    std::vector <Contact*> contacts;
    std::vector <Filopodia*> filopodia;
    std::vector <vector <EC*> > cellNeighbourhoods;


    ///exposed to python via pybind
    //World(void);
    //TODO: does ybaseline and 'bahti gradient' need to be put in to this version?
    //TODO: vconst is 0.04 if env setup is 1 or 0.008 if env setup is 6... sent katie message about this
    //TODO: add yMax in..
    //TODO: put ybaseline back in
    //World(float epsilon = 0.9, float vconcst = 0.04, int gradientType = 2, /*float yBaseline,*/ float filConstNorm = 2.0f, float filTipMax = 10, float tokenstrength = 1);
    World(float epsilon, float vconcst, int gradientType, /*float yBaseline,*/ float filConstNorm, float filTipMax, float tokenstrength, int filspacing, float randomFilExtend, float randFilRetract);
    void runSimulation();
    void creationTimestep(int movie);
    void simulateTimestep();
    int gridXDimensions;
    int gridYDimensions;
    int gridZDimensions;
    void simulateTimestep(std::vector< std::vector<float> > cellIncrements);
    std::vector< std::vector<float> > getGridSiteData();
    std::vector< std::vector< std::vector<float> > > getFilopodiaBaseLocationsAndTotalVegfr();
    std::vector< std::vector< std::vector < std::array<int,2> > > > getGridMapOfFilopodiaMovement();

    ///analysis
    void hysteresisAnalysis();

    ///simulation update functions
    void updateECagents(void);
    void updateMemAgents(void);
    void updateEnvironment(void);
    void calculateSpringAdjustments(void);
    void movieMaking(int movie);
    bool delete_if_spring_agent_on_a_retracted_fil(MemAgent* memp); ///within memAgent update, doesnt reassess a spring agent if is scheduled for deletion, given asynchronous agent updates this must be checked first


    ///initialisation functions
    World();
    void createAgents(void);
    void createECagents(int Junc_arrang);
    void createEnvironment(void);
    void setMLocation(int x, int y, int z, MemAgent * ident);
    void drawCell(int i, int j, EC* ecp);
    void createBlood(void);
    void createMesh(void);
    void createJunctionMesh(void);
    void drawMeshFirst(int i, int j, EC* ecp, int JunctArrangement);
    void connectMesh(void);
    void calcEnvAgentVEGF(Env * ep);
    void createNewEnvAgent(int x, int y, int z);
    void createAstrocytes(void);
    void createTestCase(void);
    int checkForAstro(int i, int j, int k);
    bool replaceAstrocytes(int i, int j, bool checkOnly);
    void createHaptoTest(void);
    void runHapto();
    void replaceBlood(int i, int j, int k);
    void curvedAstrocytes(Coordinates One, Coordinates Two, Coordinates Anchor);
    void initialise_macrophage_VEGF(void);
    void createMacrophages(void);
    void create_2D_square_cell(int cell, int centreX, int centreY);
    void create_3D_round_cell(void);
    void create_astro_retina_section(void);
    void label_env_exposed_von_neu_agents(void);
    void scale_ProtLevels_to_CellSize(void);
    void createMonolayer(void);
    void createHexagonCell(int cell, int centreX, int centreY);
    void connectMonolayer(void);
    void chooseMutants();
    int AlreadyPicked(int a, int* pastNo, int chose);

    ///geometry, maths and voxelisation functions
    float getDist(float x1, float y1, float z1, float x2, float y2, float z2);
    bool distTest2D(int x1, int y1, int x2, int y2, int dist);
    bool distTest2Dexact(float x1, float y1, float x2, float y2, float dist);
    float equation_of_line_through_two_points_given_Coord(float x1, float y1, float x2, float y2, float A, int which);
    float calcCurvature(Coordinates One, Coordinates Two, Coordinates Three);
    void calcCurves(void);
    vector <Coordinates> initialisePoints(void);
    void gridTriangle(Coordinates one, Coordinates two, Coordinates checkPoint);
    void trianglePlaneTest(void);
    float* equation_of_plane_through_three_points(vector<Coordinates> pts);
    bool check_point_on_plane(vector <float> coeffs, Coordinates point);
    bool check_point_within_triangle(Coordinates* point, vector<Coordinates> triangle);
    bool check_point_same_side_of_line(Coordinates* point, Coordinates* order, bool hyp);
    float round_decimalPlaces(float myValue, float PRECISION);
    Coordinates* crossProduct(Coordinates* one, Coordinates* two);
    float dotProduct(Coordinates* first, Coordinates* second);
    float absVal(Coordinates vec);
    int* findRange(vector<Coordinates> coords);
    float calc_z_given_equ_plane(float X, float Y, vector <float> coeffs);
    Coordinates* pt_of_intersection_plane_line(Coordinates pt_one, Coordinates pt_two, float* plane_coeffs);
    float find_denom(Coordinates one, Coordinates two, float* coeffs);
    void test_plane_line_intersect(void);
    bool trianglePlane_cubeDiagonals_intersect_test(vector<Coordinates> triangle, float x, float y, float z, Coordinates* cube_vertex);
    bool check_point_within_cube(Coordinates* point, float x, float y, float z);
    Coordinates* get_cube_vertices(float x, float y, float z);
    vector <Coordinates> initialise_test_triangle(void);
    bool triangleLines_cubeFacePlane_intersect_test(vector<Coordinates> triangle, float x, float y, float z, Coordinates* cube_vertex);
    void voxelisation(void);
    bool check_point_in_line(Coordinates* point, Coordinates start, Coordinates fin);
    bool voxelise(vector<Coordinates> triangle, float x, float y, float z);
    void voxeliseTriangle(vector<Coordinates> Coords, vector<MemAgent*> triangleNodes, EC* cell, int up);
    Coordinates* calcNormal(Coordinates one, Coordinates two, Coordinates three);
//#ifdef GRAPHICS
    void store_normals(void);
//#endif
    Coordinates calc_point_on_line_through_external_point_giving_perpendicular_line(Coordinates C, Coordinates S, Coordinates En);
    vector <Coordinates> store_cube_normals;
    Coordinates findMidPoint(float x1, float y1, float z1, float x2, float y2, float z2, float Length);
    vector <float> equation_of_line_through_two_points(float x1, float y1, float x2, float y2);
    bool test_triange_Pos_change(vector<MemAgent*>triangle);
    int round(float f);
    void createSurfaceAgent(int X, int Y, int Z, EC* cell, vector<MemAgent*> triangleNodes, int up);
    void gridSurfaceTriangleEdges(Coordinates A, Coordinates B, EC* cell, vector <MemAgent*> triangleNodes, int up);
    float get_perp_distance_from_point_to_line(Coordinates P1, Coordinates P2, MemAgent* mp);

    ///visualisation functions
    void viewWorld(void);
    void viewGrid(void);
    void viewMesh(void);
    void drawSprings(MemAgent* mp, float red, float green, float blue);
    void viewGridLines(void);
    void drawFilAgentsCylinders(float a, float b, float c, int a2, int b2, int c2, float red, float green, float blue, float diam, int X);
    void displayStats(void);
    void viewMesh_diagramSpheres(void);


    ///analysis/quantification
    void evaluate_shuffling(ofstream& fileTo);
    void printScores(ofstream& fileTo);
    void evaluateSandP();
    void filopodia_contacts_Test(void);
    void printContactsInfo(ofstream& fileTo);
    void calcEnvVEGFlevel(void);
    void getCellNeighbours(void);
    void collect_Filopodia_Data(void);
    void evaluate_final_offset_patt(ofstream& fileTo);
    void evaluation(ofstream& fileTo);
    void setInitialVEGF(void); ///set VEGF gradient
    void store_new_fusion_events(EC* cell1, EC* cell2);
    void output_cell_protlevels(ofstream& fileTo);


    ///destructors
    ~World(void); ///destructor
    void destroyWorld(void); ///called when doing multiple runs to make sure no memory leaks, deletes all agents
};
///----------------------------------------


///------------------------------------------------------------------------------------
class Macrophage{

    World* worldP;
public:
    Coordinates coords;

    Macrophage(World* WP);
};
///------------------------------------------------------------------------------------


extern World* WORLDpointer;
#endif /*OBJECTS_H_*/
