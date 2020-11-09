#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include "objects.h"
#include <math.h>

using namespace std;

float steps = 0.5f;

int right1 = 0;
int left1 = 0;
int right2 = 0;
int left2 = 0;
int middle = 0;
int count3 = 0;

float overallVEGF = 0.0f;
int overallflag = 0;
int Dolaycount = 0;
//----------------------------------------------------------------------------------
/**
 *
 * This is needed to tell if triangle positions have changed when 
 * on the fly surface agent coverage flag is switched ON (quick calculatoin of voxelisatoin though incomplete and shouldnt be used durnig real simulations, only in development to see faster, then retest with on the fly surface OFF to ensure full surface created.
 */
void MemAgent::store_previous_triangle_pos(void) {
    if ((node == true) && ((FIL == NONE) || (FIL == BASE))) {
        previous.x = Mx;
        previous.y = My;
        previous.z = Mz;
    }
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/**
 * 
 * Calculate memAgents Notch receptor activation from local Dll4 ligands seen in grid
 */
void MemAgent::NotchResponse(void) {

    int i = 0;
    int j;
    int flag = 0;

    

    do {
        if (worldP->neigh[i].type == M) {

            for (j = 0; j < (int) worldP->neigh[i].Mids.size(); j++) {

                if (flag == 0) {
                    if (worldP->neigh[i].Mids[j]->Cell != Cell) {
                        //if more than number of notch receptors  only take amount needed to activate notches
                        if (worldP->neigh[i].Mids[j]->Dll4 > Notch1) {
                            worldP->neigh[i].Mids[j]->Dll4 -= Notch1;
                            activeNotch = activeNotch + Notch1;
                            Notch1 = 0.0f;
                            flag = 1;

                        }//take all of it if less than it has notch receptors
                        else {
                            Notch1 = Notch1 - worldP->neigh[i].Mids[j]->Dll4;
                            activeNotch = activeNotch + worldP->neigh[i].Mids[j]->Dll4;
                            worldP->neigh[i].Mids[j]->Dll4 = 0.0f;
                        }
                    }
                }
            }
        }

        i++;

    } while ((flag == 0) && (i < NEIGH));

}

//------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
/**
 * 
 * only used for TIP memAgents (at the tip of a filopodium) to calculate length of current filopodium from spring lengths of all springs in this fil.
 * 
 * filNeigh refers to the node agent one the other end of the current filopdia spring being measured.
 * 
 *
 * @param type
 * @return float
 */
float MemAgent::FilLength(int type) {

    int score = 0;
    MemAgent* Mcurrent;
    float length = 0.0f;
 
    vector <MemAgent*> stored;

    if (type == BASE) Mcurrent = plusSite;

    else Mcurrent = this;

    if (Mcurrent != NULL) {
        do {
            //score++;
            if (type == TIP) {
                stored.push_back(Mcurrent);
                if (Mcurrent->Mx - Mcurrent->filNeigh->Mx >= (float) xMAX / 2.0f)
                    length += worldP->getDist(Mcurrent->Mx - xMAX, Mcurrent->My, Mcurrent->Mz, Mcurrent->filNeigh->Mx, Mcurrent->filNeigh->My, Mcurrent->filNeigh->Mz);
                else if (Mcurrent->filNeigh->Mx - Mcurrent->Mx >= (float) xMAX / 2.0f)
                    length += worldP->getDist(Mcurrent->Mx, Mcurrent->My, Mcurrent->Mz, Mcurrent->filNeigh->Mx - xMAX, Mcurrent->filNeigh->My, Mcurrent->filNeigh->Mz);
                else
                    length += worldP->getDist(Mcurrent->Mx, Mcurrent->My, Mcurrent->Mz, Mcurrent->filNeigh->Mx, Mcurrent->filNeigh->My, Mcurrent->filNeigh->Mz);
                Mcurrent = Mcurrent->filNeigh;
            } else if (type == BASE) Mcurrent = Mcurrent->plusSite;

        } while (Mcurrent->FIL != BASE);
    }

    return (length);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
/**
 * When a memAgent at the tip of a filopodium (FIL=TIP) touches another cells filopodium it triggers both fils nodeAgents to lose all their focal adhesions (FAs) and advance the veil.
 * 
 */
void MemAgent::veilAdvance(void) {

    int flag = 0;
    int i;
    MemAgent* currentNode;
    MemAgent* nextNode;
    
    int count = 0;
    //go back through connected spring nodes and release their FAs
    currentNode = filNeigh;

    do {

        if (currentNode->FIL != BASE) {
            count++;
            currentNode->FA = false;
            currentNode->SpringNeigh[0]->veilAdvancing = true; 
            /** 
             * important to flag as veil advancing here so that newNodes() doesnt insert a new node in the middle of the spring when it goes over threshold legnth for an adhesion node as per filopodia extension*/
            nextNode = currentNode->filNeigh;
            currentNode = nextNode;
        } else if (count > 0) {
            flag = 1;
            for (i = 0; i < currentNode->neighs; i++)
                if (currentNode->SpringNeigh[i]->filopodia == true) {
                    currentNode->SpringNeigh[i]->veilAdvancing = true;
                }
            currentNode->veilAdvancing = true;
        } else flag = 1;

        if (currentNode->FIL == BASE)currentNode->veilAdvancing = true;


    } while (flag == 0);


}
//----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
/**
 * Retract filopodium
 * 
 * @return bool retracted
 */
bool MemAgent::filRetract(void) {

    int flag = 0;
    int i, k;
    float B, D;
    int FLAG = 0;

    vector<Spring*>::iterator Q;
    vector<MemAgent*>::iterator L;
    Spring* neighStp;
    int XA;
    float length;
    MemAgent* memp;
    vector<MemAgent*>::iterator T;

    ///release this memAgents adhesion (FIL=TIP for this node) 
    FA = false;
    ///flag it for deletion, which will also stop it being assessed in any further update functions e.g. receptor activation
    deleteFlag = true;

    ///locate its nearest nodeAgent back in the fil then calculate length of its spring.
    MemAgent* mp = filNeigh;

    XA = (int) Mx - (int) mp->Mx;
    B = (int) My - (int) mp->My;
    D = (int) Mz - (int) mp->Mz;

    //toroidal adjustments
    if(TOROIDAL_X==true){
    if (sqrt(XA * XA) >= (int) ((float) xMAX / 2.0f)) {

        if (XA > 0) XA = -(xMAX - XA);
        else XA = xMAX - abs(XA);
        length = sqrt((XA * XA)+(B * B)+(D * D));

    } else {
        length = worldP->getDist(Mx, My, Mz, mp->Mx, mp->My, mp->Mz);
    }
    }
    else {
        length = worldP->getDist(Mx, My, Mz, mp->Mx, mp->My, mp->Mz);
    }

    ///if its spring length>1 (so nodeAgents either end of spring are not nearest neighbours in grid, return flase and stop function. It will reassess next timestep after the spring has retracted further
    if ((int) length > 1) {

        return (false);
    }
    ///else, the current tip has retracted the spring back to the node agents adhered at the other end of the current spring
    else {
        i = 0;
        flag = 0;
        this->Cell->filopodiaRetractions.push_back(std::array<int, 3>{ (int)mp->Mx, (int)mp->My, (int)mp->Mz });
        ///update current actin usage by minusing the current length of the spring from cells list, as this spring is now going to be deleted
         Cell->actinUsed -= length;
         
         ///if the nodeAgent at the other end of the spring is the BASE of the filopodium then reset it to NONE state and delete all springs and agents associated
        if (mp->FIL == BASE) {
            
            mp->FIL = NONE;
            
            ///if vessel is blindended dont release adhesion, otherwise do. keeps it fixed and sewn up at front of sprout in this setup
            ///releasing adhesion keeps cell brod moving freely, filopida adheisn can root it stuck while they exist
            if(BLINDENDED_SPROUT==true){
                if(mp->labelledBlindended!=true) mp->FA=false;
            }
            //else mp->FA=false;
            
            //send the actin back to the filNeigh as tipnode to be deleted
            mp->filTokens += filTokens;
            
            //ANALYSIS of filopodia can be done here
            if (ANALYSIS_contactsTest == true) {
                mp->base_fil_belong->time_retract_complete = worldP->timeStep;
                mp->base_fil_belong->retracted = true;
                mp->base_fil_belong = NULL;
            }

            //the BASE->NONE state returns to not veiladvancing state incase it had been previously advancing
            mp->veilAdvancing = false; 
            
            //find where this spring is listed in each memAgent and remove
            for (i = 0; i < mp->neighs; i++) {

                if (mp->neigh[i] == this) {

                    neighStp = mp->SpringNeigh[i];
                    mp->neigh[i] = NULL;
                    mp->SpringNeigh[i] = NULL;

                    FLAG = 1;
                }
                if ((FLAG == 1) && (mp->neighs > i + 1)) {
                    mp->neigh[i] = mp->neigh[i + 1];
                    mp->SpringNeigh[i] = mp->SpringNeigh[i + 1];
                    mp->neigh[i + 1] = NULL;
                    mp->SpringNeigh[i + 1] = NULL;
                }
            }

            mp->neighs--;
        } else {
           ///if filNeigh !=BASE then it is a stalk node within the filopodium. so just rename it the new current tip and delete the old tipnode
            mp->FIL = TIP;
            //set its filtip timer so that it will be retracted in this same way on next assessment
            mp->filTipTimer = FILTIPMAX + 1;
            neighStp = mp->SpringNeigh[0];
            mp->neigh[0] = NULL;
            mp->SpringNeigh[0] = NULL;
            mp->neighs = 0;
            //flag as deleted so dont assess receptors etc. 
            mp->deleteFlag = true;
//pass down tipnodes actin to this new tip as the old will be deleted
            mp->filTokens += filTokens;

        }

        mp->plusSite = NULL;

        //remove the spring from list
        i = 0;
        flag = 0;
        do {

            if (Cell->Springs[i] == neighStp) {
                flag = 1;
                Q = Cell->Springs.begin();
                Cell->Springs.erase(Q + i);
                i--;
            }
            i++;

        } while ((i < (int) Cell->Springs.size()) && (flag == 0));
        if (flag == 0) {
            cout << "filRetract: hasnt found spring in list" << endl;
            cout.flush();
        }

        //remove the tip node from cells list
        i = 0;
        flag = 0;
        do {

            if (Cell->nodeAgents[i] == this) {
                flag = 1;
                L = Cell->nodeAgents.begin();
                Cell->nodeAgents.erase(L + i);
            }
            i++;
        } while ((i < (int) Cell->nodeAgents.size()) && (flag == 0));
        if (flag == 0) {
            cout << "filRetract: hasnt found in node list" << endl;
            cout.flush();
        }

        //need to go through and remove springAgents from grid, though really shouldnt have any as its less than 1 distance. just to be sure...-------------------
        //delete old grid ref
        if (neighStp->agents.size() > 0) {

            for (i = 0; i < (int) neighStp->agents.size(); i++) {
                memp = neighStp->agents[i];
                worldP->deleteOldGridRef(memp, true);
                memp->deleteFlag = true;

                if (memp->assessed == true) {
                    k = 0;
                    flag = 0;
                    do {

                        if (memp->Cell->springAgents[k] == memp) {
                            flag = 1;
                            L = memp->Cell->springAgents.begin();
                            memp->Cell->springAgents.erase(L + k);
                        }
                        k++;
                    } while ((k < (int) memp->Cell->springAgents.size()) && (flag == 0));
                    if (flag == 0) {
                        cout << "deleting spring agent in main: hasnt found in list" << endl;
                        cout.flush();
                    }
                    delete memp;
                }
            }
        }
        

        delete neighStp;
        return (true);
    }

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
/**
 * Used in TokenTrading to allocate actin to nearby node if current agent is a surface node, which are unable to extend filopodia themselves
 * @return MemAgent* to nearest node in surface triangle
 */
MemAgent* MemAgent::find_nearest_triangle_node(void) {

    int dist1, dist2, dist3;

    MemAgent* nearest;

    if (fabs(Mx - triangle[0]->Mx) >= (float) xMAX / 2.0f) {
        if (Mx > triangle[0]->Mx) dist1 = worldP->getDist(Mx - xMAX, My, Mz, triangle[0]->Mx, triangle[0]->My, triangle[0]->Mz);
        else dist1 = worldP->getDist(Mx, My, Mz, triangle[0]->Mx - xMAX, triangle[0]->My, triangle[0]->Mz);
    } else dist1 = worldP->getDist(Mx, My, Mz, triangle[0]->Mx, triangle[0]->My, triangle[0]->Mz);

    if (fabs(Mx - triangle[1]->Mx) >= (float) xMAX / 2.0f) {
        if (Mx > triangle[1]->Mx) dist2 = worldP->getDist(Mx - xMAX, My, Mz, triangle[1]->Mx, triangle[1]->My, triangle[1]->Mz);
        else dist2 = worldP->getDist(Mx, My, Mz, triangle[1]->Mx - xMAX, triangle[1]->My, triangle[1]->Mz);
    } else dist2 = worldP-> getDist(Mx, My, Mz, triangle[1]->Mx, triangle[1]->My, triangle[1]->Mz);

    if (fabs(Mx - triangle[2]->Mx) >= (float) xMAX / 2.0f) {
        if (Mx > triangle[2]->Mx) dist3 = worldP->getDist(Mx - xMAX, My, Mz, triangle[2]->Mx, triangle[2]->My, triangle[2]->Mz);
        else dist3 = worldP->getDist(Mx, My, Mz, triangle[2]->Mx - xMAX, triangle[2]->My, triangle[2]->Mz);
    } else dist3 = worldP->getDist(Mx, My, Mz, triangle[2]->Mx, triangle[2]->My, triangle[2]->Mz);

    if ((dist1 <= dist2) && (dist1 <= dist3)) nearest = triangle[0];
    else if ((dist2 <= dist1) && (dist2 <= dist3))nearest = triangle[1];
    else if ((dist3 <= dist2) && (dist3 <= dist1))nearest = triangle[2];
    else cout << "cant find a nearest triangle node!" << endl;

    return (nearest);

}
//---------------------------------------------------------------------------------------------
//--------------------
void MemAgent:: TokenTrading(void){
    //older version of actinFlow used in original papers.
    int flag=0;
    MemAgent* nearest;
    
    //int diffFlag=0;
    //int i=0;
    //int chose;
    
    //lose old tokens------------------------
    if(VRinactiveCounter>TokDisRate){
        filTokens=0;
        flag=1;
        VRinactiveCounter=0;
        //cout<<"deactivated!"<<endl;
        
    }
    //----------------------------------------
    
    if(flag==0){
        
        //pass tokens up filopodia
        if((FIL==STALK)||(FIL==BASE)){
            if(plusSite==NULL){
                //cout<<"no plusSite"; cout.flush();
            }
            else{
                
                plusSite->filTokens+=filTokens;
                filTokens=0;
                
                
            }
        }
        else if((triangle.size()!=0)&&(filTokens>0)){
            //pick nearest node in triangle to give tokens to.
            
            nearest = find_nearest_triangle_node();
            nearest->filTokens+=filTokens;
            filTokens=0;
            
        }
        
        
    }
    
}
////---------------------------------------------------------------------------------------------
/**
 * move actin up filopodia to the tip and also from surface agents to their nearest node agent as they cant extend their own filopdium, only nodeAgents can
 */
void MemAgent::ActinFlow(void) {

    int flag = 0;
    MemAgent* nearest;


    //lose all actin tokens (opposite of recruitment also simplified degradation) if VEGFR-2 receptors havent activated for a while------------------------
    if (VRinactiveCounter > TokDisRate) {
        filTokens = 0;
        flag = 1;
        VRinactiveCounter = 0;
        //cout<<"deactivated!"<<endl;

    }
    //!WORTH BUG TESTING!if in a stalk and hemmed in all around so doesnt activate much then will stop passing others tokens up.. - although it will for a while, one timer is reset.
    //----------------------------------------

    if (flag == 0) {

        //pass tokens up filopodia towards tipnode
        if ((FIL == STALK) || (FIL == BASE)) {
            if (plusSite == NULL) {
                cout<<"no plusSite_BUG DETECTED in memAgents.cpp line 429"; cout.flush(); ///!WORTH BUG TESTING! if this shows up then plusSite is not labelling correctly 
            } else {

                plusSite->filTokens += filTokens;
                filTokens = 0;


            }
        } else if ((triangle.size() != 0) && (filTokens > 0)) {
            ///!WORTH BUG TESTING! is this def sending all surface agents accumulated actin around? and not any tipnodes in filopodia? Prob ok, but worth a recheck
            ///pick nearest node in triangle to give tokens to if a surface agent (which should have triangle.size()!=0

            nearest = find_nearest_triangle_node();
            nearest->filTokens += filTokens;
            filTokens = 0;

        }


    }

}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
/**
 * 
 * this function will inhibit filopodia existing with  radius N of another filopodia
 * N = 1 for CELL_SETUP 2 (as corases grid and smaller cells. Also for CELL_SETUP = 1 when ENV  =1 (2D) as fils nedd to line up closer to get some in the thin astrocyte sections
 * N=4 with CELL SETUP 1 and Env 3D VEGF as fils can then grow all over the cell.. 
 * !WORTH A BUG TEST! N prob doesnt actually matter now, could set for =1 under all conditions as actinMAX keeps fils in check.. but test properly to be sure.
 * @param x position
 * @param y position
 * @param z position
 * @param N radius
 */
void MemAgent::tryActinPassRadiusN(int x, int y, int z, int N) {
    int i, j, k, X, Y, Z, m;
    int flag = 0;
    int flag2 = 0;
    //checks the cube of neighbours surrounding a point - rather than the sphere of neighs

    X = x - N;
    Y = y - N;
    Z = z - N;

    for (i = X; i <= X + (2 * N); i++) {
        for (j = Y; j <= Y + (2 * N); j++) {
            for (k = Z; k <= Z + (2 * N); k++) {
                if (flag == 0) {

                    if (worldP->insideWorld(i, j, k) == true) {
                        if (worldP->grid[i][j][k].type == M) {
                            for (m = 0; m < worldP->grid[i][j][k].Mids.size(); m++) {
                                if (flag2 == 0) {
                                    if ((worldP->grid[i][j][k].Mids[m]->FIL != NONE) && (worldP->grid[i][j][k].Mids[m]->Cell == Cell)) {
                                        ///only passes one token as called in VEGFR response when only one new one has been allocated
                                        worldP->grid[i][j][k].Mids[m]->filTokens++; 
                                        filTokens--;
                                        flag2 = 1;

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}
//---------------------------------------------------------------------------------------------

/**
 * 
 * checks if agent has a face (not vertice) exposed to the environment, so that if membrane is densely ruffled and amemAgent is blocked in it wont be ableot activate its receptors via just a corner exposure to the environment. 
 * thus function is important for alterations in coarseness of the grid and different geometries for intialising cells on - to match same receptor activity behaviour incase the voxelisation of the mesh happens to be more ruffled. 
 * !WORTH BUG TESTING! is this bit robust, especially when switching between config template CELL_SETUPS. test output of receptors overall with and without it. clarify in documentation
 * @return true/false
 */
bool MemAgent::checkNeighsVonForEnv(void) {
    //von neumann neighbourhood used

    int x = 0;
    int flag = 0;
    int m, n, p;
    float sumV = 0;
    int i, j, k;
    i = Mx;
    j = My;
    k = Mz;
    //same layer
    do {
        if (x == 0) {
            m = i;
            n = j + 1;
            p = k;
        } else if (x == 1) {
            m = i;
            n = j - 1;
            p = k;
        } else if (x == 2) {
            m = i + 1;
            n = j;
            p = k;
        } else if (x == 3) {
            m = i;
            n = j;
            p = k + 1;
        } else if (x == 4) {
            m = i;
            n = j;
            p = k - 1;
        } else if (x == 5) {
            m = i - 1;
            n = j;
            p = k;
        }


        //-------------------------------
        //toroidal only X
        if(TOROIDAL_X_env==true){
        if (m >= xMAX) m = 0;
        if (m < 0) m = xMAX - 1;
        }
        if (worldP->insideWorld(m, n, p) == true) {
            if (worldP->grid[m][n][p].type == E) {
                
                flag = 1;
            } else if ((worldP->grid[m][n][p].type == E)&&(worldP->grid[m][n][p].Eid->Astro==true)) {
                flag = 1;
            }
        }
        
        x++;
    } while ((x < 6) && (flag == 0));

    if (flag == 1) return true;
    else {
       
        return false;

    }
   

}
//-----------------------------------
//------------------------------------------------------------------------------
/**
 * 
 * main VEGFR activation function (applies VEGFR-1 as a simple sink parameter taking away VEGF from VEGFR-2
 */
void MemAgent::VEGFRresponse(void) {

    float Prob, chance;

    int upto = Cell->VonNeighs; 
    float VEGFRactiveProp;
    int i, j, k;
    i = (int) Mx;
    j = (int) My;
    k = (int) Mz;
    bool moved = false;
   
    //calculate the active VEGFR level as a function of VEGFR-2, VEGFR1 level and vEGF.. 
    VEGFRactiveProp = (VEGFR / ((float) VEGFRNORM / (float) upto));
    VEGFRactive = (SumVEGF / Cell->Vsink) * VEGFRactiveProp;

    //done exceed max level
    if (VEGFRactive > VEGFR) {
        
        VEGFRactive = VEGFR;
    }

    //calculate probability of extending a filopdium as a function of VEGFR activity, if no filopodia needed set to 0
    if (FILOPODIA == true){
    //***** RANDFIL here
    if(randFilExtend >= 0 && randFilExtend <= 1)
      Prob = randFilExtend; //0-1 continuous value input at runtime. if randFil!=-1 - token Strength forced to 0, and epsilon forced to 0.0 (fully random direction and extension, no bias from VR->actin or VR gradient to direction.
    else
      Prob = ((float) VEGFRactive / ((float) Cell->VEGFRnorm / (float) upto)) * Cell->filCONST;
        //else Prob = ((float) VEGFRactive / (((float) VEGFRnorm/2.0f) / (float) upto)) * Cell->filCONST;
    }
    else Prob = 0;

   
    //chance = (float) rand() / (float) RAND_MAX;
    chance = (float) new_rand() / (float) NEW_RAND_MAX;

    //-----------------------------------------------------------------------
    if (chance < Prob) {
    
        //award actin tokens

        filTokens++;

        
        if (FIL == NONE) tryActinPassRadiusN((int) Mx, (int) My, (int) Mz, FIL_SPACING);

        if (oldVersion == true) {
            if (FIL == STALK) {
                //passes its filExtend token to Magent in its plusSite
                plusSite->filTokens++;
                filTokens--;
            }
        }

        //--------------------------------------------------------------------------------------------
        //filopodia extension
        if (((FIL == TIP) || (FIL == NONE)) && (filTokens >= tokenStrength)) {
            if (deleteFlag == false) moved = extendFil();
        }
        //--------------------------------------------------------------------------------------------

        //reset VRinactive counter as now activated
        VRinactiveCounter = 0;

    } else VRinactiveCounter++;

    if (moved == false) filTipTimer++;
    else filTipTimer = 0;


}
//----------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void MemAgent::calcForce(void) {

    int i, k, T;
    float denom, length;
    float sumDN[3];
    int upto = meshNeighs + 5;
    int DONE = 0;
    float newX, newY, newZ;
    float SL = 0;
    float sConst = 0;

    int flagFil = 0;
    float oldDist, newDist;
    float PN[upto][3];
    float SN[upto][3];
    float DN[upto][3];


    for (k = 0; k < 3; k++) Force[k] = 0;
    for (k = 0; k < 3; k++) sumDN[k] = 0.0f;

    //have different lengths and constants for different types of spring
    if ((FIL == TIP)&&(FA==false)) {

        sConst = filSpringConstant;
        SL = filSpringLength;
    }
    //calculate new force by summing neighbour vectors minused from current point PN1, PN2... then calculate the projection of S onto spring direction and get the difference, then 		  sum the elongated regions of each and times by k constant.n

    SL = springLength;
    i = 0;
    do {
        if (neigh[i] != NULL) {
            PN[i][0] = Mx - neigh[i]->Mx;
            PN[i][1] = My - neigh[i]->My;
            PN[i][2] = Mz - neigh[i]->Mz;
            length = worldP->getDist(Mx, My, Mz, neigh[i]->Mx, neigh[i]->My, neigh[i]->Mz);

            /*if(surgeSpringConst==true){
             * sConst = surgeConstant;
             * SL =  surgeSpringLength;
             * surgeSpringConst = false;
             * }*/
            if (neigh[i]->FA == true) {
                sConst = FAspringConstant; //filBaseConstant;
                SL = springLength;
            }
            if (((FIL == BASE) || (FIL == STALK)) && (veilAdvancing == true) && ((neigh[i]->FIL == STALK) || (neigh[i]->FIL == TIP))) {
                sConst = filBaseConstant;
                SL = filSpringLength;
            } else if (neigh[i]->Cell != Cell) {
                sConst = junctionConstant;
                SL = JunctionSpringLength;
            } else {
                sConst = springConstant;
                SL = springLength;
            }


        } else if ((FIL == TIP) && (flagFil == 0)) {
            flagFil = 1;
            PN[i][0] = Mx - filNeigh->Mx;
            PN[i][1] = My - filNeigh->My;
            PN[i][2] = Mz - filNeigh->Mz;
            length = worldP->getDist(Mx, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);


        } else {
            DONE = 1;
        }

        if (DONE == 0) {

            if (sqrt(PN[i][0] * PN[i][0]) >= (float) xMAX / 2.0f) {

                if (PN[i][0] > 0) PN[i][0] = -((float) xMAX - PN[i][0]);
                else PN[i][0] = (float) xMAX - fabs(PN[i][0]);
                length = fabs(xMAX - PN[i][0]);

            }
            denom = sqrt((PN[i][0] * PN[i][0])+(PN[i][1] * PN[i][1])+(PN[i][2] * PN[i][2]));

            //only apply force when spring is longer than it should be, not smaller -as membranes dont ping outwards, they ruffle - should avoid 'sagging of membrane'
            if (length > SL) {
                for (k = 0; k < 3; k++) {
                    SN[i][k] = SL * (PN[i][k] / denom);
                }

                for (k = 0; k < 3; k++) {
                    DN[i][k] = PN[i][k] - SN[i][k];
                }

                for (k = 0; k < 3; k++) sumDN[k] += (sConst * DN[i][k]);
            }
        }
        i++;
    } while ((i < upto) && (DONE == 0));


    for (k = 0; k < 3; k++) {
        Force[k] = sumDN[k];

    }


    newX = Mx - (Force[0] / 2.0f);
    newY = My - (Force[1] / 2.0f);
    newZ = Mz - (Force[2] / 2.0f);

    //toroidal on x axis
    if(TOROIDAL_X==true){
    if(newX<0){
        newX =  (float)xMAX+newX;
    }
    if(newX>=(float)xMAX){
        
        newX = newX - (float)xMAX;
    }
    }
    if(TOROIDAL_Y==true){
    
    //roof on y ans z axis
    if(newY<0) newY = 0;
    else if(newY>yMAX-1) newY = yMAX-1;
    
    if(newZ<0) newZ=0;
    else if(newZ>zMAX-1) newZ = zMAX-1;
    }
    //----------------------------------------------------------------
 
    moveAgent(newX, newY, newZ, false);
    
    

    

}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

void MemAgent::moveAgent(float newX, float newY, float newZ, bool FAset) {

    int newMx = (int) newX;
    int newMy = (int) newY;
    int newMz = (int) newZ;
    int X, Y, Z;
    X = (int) Mx;
    Y = (int) My;
    Z = (int) Mz;


    //only bother to move grid version if its moving to another grid site - this is the int version of the float model for ease of local rule look up
    if ((newMx == X) && (newMy == Y) && (newMz == Z)) {
        //do nothing its in the same place;

        //move agent in continuous space
        Mx = newX;
        My = newY;
        Mz = newZ;
    } else {

        //--------------------------------------------------------------------------------------------------------------------------
        //non filopodia agents change grid state to M
        if ((FIL == BASE) || (FIL == NONE)) {
            if ((worldP->grid[newMx][newMy][newMz].type == M) || (worldP->grid[newMx][newMy][newMz].type == E) || (worldP->grid[newMx][newMy][newMz].type == BLOOD)) {

                worldP->deleteOldGridRef(this, false);

                worldP->setMLocation(newMx, newMy, newMz, this);


                //move agent in continuous space
                Mx = newX;
                My = newY;
                Mz = newZ;
            }
        }            //------------------------------------------------------------------------------------------------------------------------------
            //------------------------------------------------------------------------------------------------------------------------------
            //filopodia agents exist in either M or E state grid sites as they are thin.. only end up in M state if the veil somhow pulls up over them
        else if ((FIL == TIP) || (FIL == STALK)) {

            worldP->deleteOldGridRef(this, true);

            worldP->grid[newMx][newMy][newMz].Fids.push_back(this);

            //move agent in continuous space
            Mx = newX;
            My = newY;
            Mz = newZ;
        }
        //------------------------------------------------------------------------------------------------------------------------------
    }

    FA = FAset;

}
//-----------------------------------------------------------------------------------------------------------------------
//anastamosis new junction

void MemAgent::anastomosis(MemAgent* other) {

    int S, z;
    int flag, flagN, flagE, flagA;

    flagA = 1;
    flag = 0;
    flagN = 0;
    flagE = 0;
    S = 0;
    MemAgent* node1;
    MemAgent* node2;
    bool allow = true;


    if ((other->Cell->tipCellTest() == true) && (Cell->tipCellTest() == true)) {
        //if (((other->Cell == worldP->ECagents[1]) && (Cell == worldP->ECagents[3])) || ((other->Cell == worldP->ECagents[3]) && (Cell == worldP->ECagents[1]))) {
            if (node == true) node1 = this;
            else {
                if (triangle.size() == 2)
                    node1 = find_nearest_triangle_node();
                else
                    allow = false;
            }


            if (other->node == true) {
                node2 = other;

            } else {
                if (triangle.size() == 2)
                    node2 = find_nearest_triangle_node();
                else
                    allow = false;
            }

            if (allow == true) {
                //dont create a new spring if this node already has a junction spring to that cell
                do {
                    if (node1->neigh[S]->Cell == node2->Cell) {
                        flagN = 1;
                    }
                    S++;
                } while ((S < node1->neighs) && (flagN == 0));

                S = 0;
                if (flagN == 0) {
                    //dont create a new spring if the other node already has a junction spring to this cell
                    do {
                        if (node2->neigh[S]->Cell == node1->Cell) {
                            flagE = 1;
                        }
                        S++;
                    } while ((S < node2->neighs) && (flagE == 0));
                }
                //-----------------------------------------------------------------------------------
                //dont connect up if no fused loops and this node has a neigh node who's connected to the other cell, as it means its
                //not a real fusion event but just nodes at the corners of three cell junctions (when ECcross =2) trying to form a diagonal junction
                //spring to the third cell
                if ((flagN == 0) && (flagE == 0)) {
                    S = 0;
                    if (worldP->fusedCells.size() == 0) {
                        //cout<<"target "<<node2->Cell<<endl;
                        do {
                            for (z = 0; z < node1->neigh[S]->neighs; z++) {
                                //cout<<node1->neigh[S]->neigh[z]->Cell<<endl;
                                if (node1->neigh[S]->neigh[z]->Cell == node2->Cell) flagE = 1;
                            }
                            S++;
                        } while ((S < node1->neighs) && (flagE == 0));
                    }
                }
                //-----------------------------------------------------------------------------------
                if ((flagE == 1) || (flagN == 1)) flag = 1;

                if (flag == 0) {

                    //has no spring already to this cell so create one

                    //only if its right next to it, not on diagonal - to preclude bunching of three way junctions..tho may prehibit anast..

                    node1->neigh[neighs] = other;
                    other->neigh[other->neighs] = node1;
                    node1->Cell->createSpringTokenObject(node1, other, node1->neighs);
                    other->Cell->createSpringTokenObject(other, node1, other->neighs);
                    node1->springJunction = true;
                    other->springJunction = true;
                    worldP->store_new_fusion_events(node1->Cell, other->Cell);

                    cout << "creating new spring " << node1 << " " << other << " " << " " << SpringNeigh[neighs] << endl;
                    node1->neighs++;
                    other->neighs++;

                    Cell->newJunction++;
                    node1->triggeredAnastSurge(other);

                }
            }
       // }
    }

}
//-------------------------------------------------------------------------------------------------------------

void World::store_new_fusion_events(EC* cell1, EC* cell2) {//store which cells have fused

    int j;
    bool allow = false;
    bool found = false;


    if (fusedCells.size() > 0) {
        for (j = 0; j < fusedCells.size() / 2; j++) {

            //check this is a new fusion event, and the two cells havent already formed a new junction
            if ((fusedCells[(j * 2)] == cell1) && (fusedCells[(j * 2) + 1] == cell2)) found = true;
            else if ((fusedCells[(j * 2)] == cell2) && (fusedCells[(j * 2) + 1] == cell1)) found = true;

        }
        if (found == true) allow = false;
        else allow = true;
    } else allow = true;

    if (allow == true) {
        fusedCells.push_back(cell1);
        fusedCells.push_back(cell2);
        fusedCells_times.push_back(timeStep);

    }

}
//----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

void MemAgent::JunctionTest( bool StoreInJunctionList) {
    int x, m, n, p, y, S, z, a, b, c;
    int i, j, k;
    int flagA = 0;
    int flag = 0;
    vector<EC*> cellNeighs;
    vector<float> smallestDist;
    vector<MemAgent*> nearestNode;
    vector <EC*> CellsConnected;
    float distance;
    int pos;
    int flag2 = 0;

    int flag3;

    bool previousJunction = junction;


    i = (int) Mx;
    j = (int) My;
    k = (int) Mz;
    //cout<<this<<endl;
    


    if ((FIL == NONE) || (FIL == BASE)) {
        //same layer
        for (x = 0; x < 27; x++) {

            if (x == 0) {
                m = i + 1;
                n = j - 1;
                p = k;
            } else if (x == 1) {
                m = i + 1;
                n = j;
                p = k;
            } else if (x == 2) {
                m = i + 1;
                n = j + 1;
                p = k;
            } else if (x == 3) {
                m = i;
                n = j - 1;
                p = k;
            } else if (x == 4) {
                m = i;
                n = j + 1;
                p = k;
            } else if (x == 5) {
                m = i - 1;
                n = j - 1;
                p = k;
            } else if (x == 6) {
                m = i - 1;
                n = j;
                p = k;
            } else if (x == 7) {
                m = i - 1;
                n = j + 1;
                p = k;
            }                //layer below
            else if (x == 8) {
                m = i + 1;
                n = j - 1;
                p = k - 1;
            } else if (x == 9) {
                m = i + 1;
                n = j;
                p = k - 1;
            } else if (x == 10) {
                m = i + 1;
                n = j + 1;
                p = k - 1;
            } else if (x == 11) {
                m = i;
                n = j - 1;
                p = k - 1;
            } else if (x == 12) {
                m = i;
                n = j + 1;
                p = k - 1;
            } else if (x == 13) {
                m = i - 1;
                n = j - 1;
                p = k - 1;
            } else if (x == 14) {
                m = i - 1;
                n = j;
                p = k - 1;
            } else if (x == 15) {
                m = i - 1;
                n = j + 1;
                p = k - 1;
            } else if (x == 16) {
                m = i;
                n = j;
                p = k - 1;
            }                //layer above
            else if (x == 17) {
                m = i + 1;
                n = j - 1;
                p = k + 1;
            } else if (x == 18) {
                m = i + 1;
                n = j;
                p = k + 1;
            } else if (x == 19) {
                m = i + 1;
                n = j + 1;
                p = k + 1;
            } else if (x == 20) {
                m = i;
                n = j - 1;
                p = k + 1;
            } else if (x == 21) {
                m = i;
                n = j + 1;
                p = k + 1;
            } else if (x == 22) {
                m = i - 1;
                n = j - 1;
                p = k + 1;
            } else if (x == 23) {
                m = i - 1;
                n = j;
                p = k + 1;
            } else if (x == 24) {
                m = i - 1;
                n = j + 1;
                p = k + 1;
            } else if (x == 25) {
                m = i;
                n = j;
                p = k;
            } else {
                m = i;
                n = j;
                p = k + 1;
            }


            //-------------------------------
            //toroidal only X
            if (m >= xMAX) m = 0;
            if (m < 0) m = xMAX - 1;
            if (n >= yMAX) n = 0;
            if (n < 0) n = yMAX - 1;

            //-------------------------------

            if (worldP->insideWorld(m, n, p) == true) {
                if (worldP->grid[m][n][p].type == M) {
                    for (y = 0; y < (int) worldP->grid[m][n][p].Mids.size(); y++) {


                        if ((worldP->grid[m][n][p].Mids[y]->Cell != Cell) && (worldP->grid[m][n][p].Mids[y]->FIL != STALK) && (worldP->grid[m][n][p].Mids[y]->FIL != TIP)) {

                            junction = true;
                            flagA = 1;
                            worldP->grid[m][n][p].Mids[y]->junction = true;



                            if (worldP->timeStep == 0) {

                            }

                            if (worldP->timeStep > 0) {

                                //-------------------------------------------------------------------------------------------------------------
                                //Anastamosis: create new spring junction to allow fusion, only on two tip cells
                                if(StoreInJunctionList!=true){
                                    if(ANASTOMOSIS==true)
                                anastomosis(worldP->grid[m][n][p].Mids[y]);
                                }
                                //-------------------------------------------------------------------------------------------------------------
                             

                            }

                        }

                    }
                }

            }
        }



    


    }


    

    if (flagA == 0) junction = false;
    //StoreInJunctionList=false;
    if ((StoreInJunctionList == true) && (node == true) && (FIL == NONE)) {//&&(previousJunction!=junction)){
        //vector<MemAgent*>::iterator L;
        //L = worldP->JunctionAgents.begin();
        //flag=0;
        //add to list
        //m=0;
        //if(worldP->JunctionAgents.size()>0){
        if ((junction == true) && (addedJunctionList == false)) {
            //  do{
            //      if(worldP->JunctionAgents[m]==this) flag=1;
            //      m++;
            //  }while((flag==0)&&(m<worldP->JunctionAgents.size()));
            // if(flag==0)
            worldP->JunctionAgents.push_back(this);
            addedJunctionList = true;
        }

        //remove from list

        /*else{
            m=0;
        flag=0;
           do{
               if(worldP->JunctionAgents[m]==this){ worldP->JunctionAgents.erase(L+m); flag=1;}
               m++;
           }while((flag==0)&&(m<worldP->JunctionAgents.size()));
        }
    }
    else{
        if(junction==true){
            worldP->JunctionAgents.push_back(this);
        }
    }*/
    }


    //-------------------------------------------------------------------------------------------------------------



}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

void MemAgent::triggeredAnastSurge(MemAgent* junctionedAgent) {

    //when a memAgent forms a junction with another cell, it should cause a local surge of the body that way

    int i;
    int flag = 0;
    MemAgent* current;
    MemAgent* Jcurrent;
    int flagN = 0;
    int S = 0;
    int flagE = 0;
    int flagDo = 0;
    //first version+ just increase spring constant of the neigh nodes--------------------------------------------------
    /*for(i=0;i<neighs;i++){
     * neigh[i]->surgeSpringConst=true;
     * }
     * for(i=0;i<junctionedAgent->neighs;i++){
     * junctionedAgent->neigh[i]->surgeSpringConst=true;
     * }*/
    //---------------------------------------------------------------

    //second version+ move niegh nodes to nearest high VEGF conc - like a local actin lamella podia surge--------------------------------------------------
    for (i = 0; i < neighs; i++) {

        if ((mesh3SpringsOnly == true) && (i == 3)) flag = 1;

        if (flag == 0) {
            current = neigh[i];
            Jcurrent = junctionedAgent->neigh[i];

            if (((current->FIL == BASE) || (current->FIL == NONE)) && ((Jcurrent->FIL == NONE) || (Jcurrent->FIL == BASE))) {

                //limit junction springs to one per node for a given cell
                S = 0;
                do {
                    if (current->neigh[S]->Cell == Jcurrent->Cell) {
                        flagN = 1;
                    }
                    S++;
                } while ((S < current->neighs) && (flagN == 0));

                S = 0;
                //dont create a new spring if the other node already has a junction spring to this cell
                do {
                    if (Jcurrent->neigh[S]->Cell == current->Cell) {
                        flagE = 1;
                    }
                    S++;
                } while ((S < Jcurrent->neighs) && (flagE == 0));

                if ((flagE == 1) || (flagN == 1)) flagDo = 1;

                if (flagDo == 0) {
                    current->neigh[current->neighs] = Jcurrent;
                    Jcurrent->neigh[Jcurrent->neighs] = current;

                    Cell->createSpringTokenObject(current, Jcurrent, current->neighs);
                    Jcurrent->Cell->createSpringTokenObject(Jcurrent, current, Jcurrent->neighs);
                    cout << "creating new surge spring " << current << " " << Jcurrent << " " << " " << endl;

                    current->neighs++;
                    Jcurrent->neighs++;

                    Cell->newJunction++;
                }

            }

        }
        //neigh[i]->surgeSpringConst=true;
        //if(neigh[i]->EnvNeighs.size()!=0) highest=neigh[i]->findHighestConc();
        //neigh[i]->moveAgent(highest->Ex, highest->Ey, highest->Ez,false);

    }
    //for(i=0;i<junctionedAgent->neighs;i++){
    //junctionedAgent->neigh[i]->surgeSpringConst=true;
    //if(junctionedAgent->neigh[i]->EnvNeighs.size()!=0) highest=junctionedAgent->neigh[i]->findHighestConc();
    //junctionedAgent->neigh[i]->moveAgent(highest->Ex, highest->Ey, highest->Ez,false);
    //}
    //---------------------------------------------------------------


}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

bool MemAgent::form_filopodia_contact(void) {
    int x, y;
    int i, j, k;
    i = (int) Mx;
    j = (int) My;
    k = (int) Mz;
    int flagA = 0;

    //check if the memAgent is on a junction with another filopodia-at any point in the fil
    for (x = 0; x < 26; x++) {
        for (y = 0; y < (int) worldP->neigh[x].Fids.size(); y++) {
            if ((worldP->neigh[x].Fids[y]->Cell != Cell)) {
                flagA = 1;
            }

        }
    }

    if (flagA == 0) return (false);
    else return (true);

}
//------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

bool testFilMax(EC* cell) {

    /*int i;
     * int count=0;
     * int springAgentsMax;
     * springAgentsMax = cell->springAgents.size();
     *
     *
     * for(i=0;i<cell->nodeAgents.size();i++){
     * if((cell->nodeAgents[i]->FIL==STALK)||(cell->nodeAgents[i]->FIL==TIP)) count++;
     * }
     * if(count+springAgentsMax+cell->no_of_filExtensions_this_timestep>=512)
     *
     * */
    if (actinMax - cell->actinUsed >= 1.0f)return (false);
    else return (true);
}
//---------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------

bool MemAgent::extendFil(void) {

    //cout<<"extending!"<<endl;
    MemAgent* mp;
    Env * highest;
    bool ans = false;
    Filopodia* fp;
    float distNeeded;
    float newDist, oldDist;
    int i;
    bool allow;
    if (node == true) {

        //only extend if enough membrane - not limiting individual filopodia size, but overall cell stretch
        //if((int)(Cell->nodeAgents.size()+Cell->springAgents.size()+Cell->surfaceAgents.size())<memMax){

        //find new position----------------------------------------------
        if (EnvNeighs.size() != 0) {
            if (Cell->actinUsed < actinMax) {
                //if(LUMEN_BM){ for(i=0;i<EnvNeighs.size();i++){
                 //   allow = false;
                 //   if((EnvNeighs[i]->inside==false)&&(EnvNeighs[i]->BM!=true)) allow = true;
                //}
                //}
                //else{
                    allow = true;
                //}
                if(allow==true){
                highest = findHighestConc();
                if ((highest != NULL) && (highest->VEGF != 0)) {

                    //-----------------------------------------------------------------------
                    if (FIL == NONE) {
                        //if(testFilMax(Cell)==false){
                        //basal focal adhesion - inhibits veil and all cell body advance


                        if (sqrt((highest->Ex - Mx)*(highest->Ex - Mx)) > xMAX / 2.0f) {
                            if (highest->Ex > Mx)
                                distNeeded = worldP->getDist(highest->Ex - xMAX, highest->Ey, highest->Ez, Mx, My, Mz);
                            else
                                distNeeded = worldP->getDist(highest->Ex, highest->Ey, highest->Ez, Mx - xMAX, My, Mz);

                        } else
                            distNeeded = worldP->getDist(highest->Ex, highest->Ey, highest->Ez, Mx, My, Mz);


                        if ((actinMax - Cell->actinUsed) >= distNeeded) {

                            //cout<<distNeeded<<" B "<<Cell;
                            Cell->actinUsed += distNeeded;
                            FA=true;
                            //Cell->fil = true;
                            //create new node, only attached to the current guy. create it in hghest VEGF site.
                            mp = new MemAgent(Cell, worldP);
                            mp->FA = true;

                            
                            mp->Mx = highest->Ex;
                            mp->My = highest->Ey;
                            mp->Mz = highest->Ez;

                            mp->previous.x = mp->Mx;
                            mp->previous.y = mp->My;
                            mp->previous.z = mp->Mz;

                            mp->FIL = TIP;
                            FIL = BASE;


                            Cell->nodeAgents.push_back(mp);

                            worldP->setFilLocation((int) mp->Mx, (int) mp->My, (int) mp->Mz, mp);

                            //connect the two nodes

                            neigh[neighs] = mp;
                            Cell->createSpringTokenObject(this, mp, neighs);
                            neighs++;

                            //this is so the tip knows which node it is connected to, rather than having a full spring as we dont want the tip to be pulled back down.
                            mp->filNeigh = this;

                            //link the two for polarity for passing of tokens up filopodia (always passes up to plus site)
                            plusSite = mp;
                            mp->minusSite = this;

                            //confirms the extension has succeeded
                            ans = true;

                            //spend the actin tokens
                            filTokens -= tokenStrength;
                            this->Cell->filopodiaExtensions.push_back(std::array<int,3>{(int)mp->Mx, (int)mp->My, (int)mp->Mz});
                            //mp->filTokens=filTokens;
                            //focalAdhesions();

                            //for testing filopodia contacts (giovanni data comparison from PLoS CB paper)------------
                            if (ANALYSIS_contactsTest == true) {

                                Filopodia* fp = new Filopodia(worldP);

                                worldP->filopodia.push_back(fp);

                                base_fil_belong = fp;
                                fp->time_created = worldP->timeStep;
                                fp->base = this;
                                fp->Cell = this->Cell;
                            }
                            //------------------------------------------
                            

                        }

                    } else {

                        if (highest->Ex - filNeigh->Mx > xMAX / 2.0f)
                            newDist = worldP->getDist(highest->Ex - xMAX, highest->Ey, highest->Ez, filNeigh->Mx, filNeigh->My, filNeigh->Mz);
                        else if (filNeigh->Mx - highest->Ex > xMAX / 2.0f)
                            newDist = worldP->getDist(highest->Ex, highest->Ey, highest->Ez, filNeigh->Mx - xMAX, filNeigh->My, filNeigh->Mz);
                        else
                            newDist = worldP->getDist(highest->Ex, highest->Ey, highest->Ez, filNeigh->Mx, filNeigh->My, filNeigh->Mz);


                        if (Mx - filNeigh->Mx > xMAX / 2.0f)
                            oldDist = worldP->getDist(Mx - xMAX, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);
                        else if (filNeigh->Mx - Mx > xMAX / 2.0f)
                            oldDist = worldP->getDist(Mx, My, Mz, filNeigh->Mx - xMAX, filNeigh->My, filNeigh->Mz);
                        else
                            oldDist = worldP->getDist(Mx, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);

                        distNeeded = newDist - oldDist;


                        if ((actinMax - Cell->actinUsed) >= distNeeded) {
                            
                            Cell->actinUsed += distNeeded;

                            moveAgent(highest->Ex, highest->Ey, highest->Ez, true);
                            this->Cell->filopodiaExtensions.push_back(std::array<int,3>{(int)Mx, (int)My, (int)Mz});
                            ans = true;
                            filTokens -= tokenStrength;
                            

                            //filTokens=0;
                        }
                    }
                }
            }
            }


        }

    }
    return (ans);

}
//----------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MemAgent::checkNeighs(bool called_fron_differentialAdhesion) {
    int x, m, n, p, zed;
    int i, j, k, mleb;
    i = (int) Mx;
    j = (int) My;
    k = (int) Mz;
    Env* Eagent;
    bool connected;

    MneighCount = 0;

    EnvNeighs.clear();
    SumVEGF = 0.0f;

    mediumNeighs = 0;
    //--------------
    DiffAd_neighs.clear();
    mediumNeighs = 0;
    //--------------

    //same layer
    for (x = 0; x < 26; x++) {
        if (x == 0) {
            m = i + 1;
            n = j - 1;
            p = k;
        } else if (x == 1) {
            m = i + 1;
            n = j;
            p = k;
        } else if (x == 2) {
            m = i + 1;
            n = j + 1;
            p = k;
        } else if (x == 3) {
            m = i;
            n = j - 1;
            p = k;
        } else if (x == 4) {
            m = i;
            n = j + 1;
            p = k;
        } else if (x == 5) {
            m = i - 1;
            n = j - 1;
            p = k;
        } else if (x == 6) {
            m = i - 1;
            n = j;
            p = k;
        } else if (x == 7) {
            m = i - 1;
            n = j + 1;
            p = k;
        }            //layer below
        else if (x == 8) {
            m = i + 1;
            n = j - 1;
            p = k - 1;
        } else if (x == 9) {
            m = i + 1;
            n = j;
            p = k - 1;
        } else if (x == 10) {
            m = i + 1;
            n = j + 1;
            p = k - 1;
        } else if (x == 11) {
            m = i;
            n = j - 1;
            p = k - 1;
        } else if (x == 12) {
            m = i;
            n = j + 1;
            p = k - 1;
        } else if (x == 13) {
            m = i - 1;
            n = j - 1;
            p = k - 1;
        } else if (x == 14) {
            m = i - 1;
            n = j;
            p = k - 1;
        } else if (x == 15) {
            m = i - 1;
            n = j + 1;
            p = k - 1;
        } else if (x == 16) {
            m = i;
            n = j;
            p = k - 1;
        }            //layer above
        else if (x == 17) {
            m = i + 1;
            n = j - 1;
            p = k + 1;
        } else if (x == 18) {
            m = i + 1;
            n = j;
            p = k + 1;
        } else if (x == 19) {
            m = i + 1;
            n = j + 1;
            p = k + 1;
        } else if (x == 20) {
            m = i;
            n = j - 1;
            p = k + 1;
        } else if (x == 21) {
            m = i;
            n = j + 1;
            p = k + 1;
        } else if (x == 22) {
            m = i - 1;
            n = j - 1;
            p = k + 1;
        } else if (x == 23) {
            m = i - 1;
            n = j;
            p = k + 1;
        } else if (x == 24) {
            m = i - 1;
            n = j + 1;
            p = k + 1;
        } else {
            m = i;
            n = j;
            p = k + 1;
        }


        //-------------------------------
        //toroidal only X
        if(TOROIDAL_X==true){
        if (m >= xMAX) m = 0;
        if (m < 0) m = xMAX - 1;
        }

        if(TOROIDAL_Y==true){
        if (n >= yMAX) n = 0;
        if (n < 0) n = yMAX - 1;
        }


        if (worldP->insideWorld(m, n, p) == true) {
            worldP->neigh[x].Mids = worldP->grid[m][n][p].Mids;
            worldP->neigh[x].Eid = worldP->grid[m][n][p].Eid;

            worldP->neigh[x].type = worldP->grid[m][n][p].type;
            worldP->neigh[x].Fids = worldP->grid[m][n][p].Fids;

            if (worldP->neigh[x].type == M) {
                MneighCount++;
            }


            else if (worldP->neigh[x].type == E) {
                Eagent = worldP->neigh[x].Eid;
                SumVEGF += Eagent->VEGF;
                EnvNeighs.push_back(Eagent);
            }
            
            /*else if((node==true)&&(worldP->neigh[x].type==AS)){
             *
             * if(FATimer>=0) FA=true;
             *
             * }*/
            //-----------NEW BIT!

            //for differential adhesion..
            //will need to add a lot to make sure dont think filagents are neighbours here for vessel version etc...!!!!
            if (called_fron_differentialAdhesion == true) {
                if (worldP->grid[m][n][p].type == M) {
                    if (diffAd_replaced_cell != NULL) {
                        for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {
                            if ((worldP->grid[m][n][p].Mids[zed]->FIL != TIP) && (worldP->grid[m][n][p].Mids[zed]->FIL != STALK)) {
                                connected = meshConnected(worldP->grid[m][n][p].Mids[zed]);
                                if (connected == true) {

                                    if ((worldP->grid[m][n][p].Mids[zed]->Cell != this->diffAd_replaced_cell)) {
                                        DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                                        //worldP->grid[m][n][p].Mids[zed]->labelled2 = true;
                                    }
                                }
                            }
                        }
                    } else if (diffAd_replaced_med != NULL) {

                        
                        for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {

                            DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                        }

                    } else {
                        for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {
                            if ((worldP->grid[m][n][p].Mids[zed]->FIL != TIP) && (worldP->grid[m][n][p].Mids[zed]->FIL != STALK)) {
                                connected = meshConnected(worldP->grid[m][n][p].Mids[zed]);
                                if (connected == true) {
                                    //cout<<"*"<<endl;
                                    if (worldP->grid[m][n][p].Mids[zed]->diffAd_replaced_cell != NULL) {
                                        if ((worldP->grid[m][n][p].Mids[zed]->diffAd_replaced_cell != this->Cell)) {
                                            DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                                            //worldP->grid[m][n][p].Mids[zed]->labelled2 = true;
                                        }
                                    } else {
                                        if ((worldP->grid[m][n][p].Mids[zed]->Cell != this->Cell)) {
                                            DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                                            //worldP->grid[m][n][p].Mids[zed]->labelled2 = true;
                                        }

                                    }
                                }
                            }
                        }
                    }
                } else if ((worldP->grid[m][n][p].type == MED) && (diffAd_replaced_med == NULL) && (worldP->grid[m][n][p].med->diffAd_replaced == NULL)) {


                    mediumNeighs++;

                } else if ((worldP->grid[m][n][p].type == MED) && (diffAd_replaced_med == NULL) && (worldP->grid[m][n][p].med->diffAd_replaced != NULL)) {
                    if (worldP->grid[m][n][p].med->diffAd_replaced->Cell != this->Cell) DiffAd_neighs.push_back(worldP->grid[m][n][p].med->diffAd_replaced);

                }

            }



            //_____________________
        } else {

            worldP->neigh[x].Eid = NULL;
            worldP->neigh[x].type = -1;
            worldP->neigh[x].Fids.clear();
            worldP->neigh[x].Mids.clear();
        }
        //-------------------------------

    }

    /*if(diffAd_replaced!=NULL){
            //out<<"D:";
            for(i=0;i<DiffAd_neighs.size();i++){
                    //cout<<DiffAd_neighs[i]->Cell->VEGFRtot<<endl;
            }
            //cout<<endl;
    }*/

}
//-------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
Env * MemAgent::findHighestConc(void){
    
    int i, direction;
    int start, picked;
    int upto;
    Env * highest;
    Env * chosen;
    float chance, prob;
    Env* furthest;
    float dist;
    float furthestDist=0;
    Env* straight=NULL;
    float currLength;
    
   /* if(BAHTI_ANALYSIS==true){
        
        //memagent ID
        worldP->dataFile2<<this<<"\t";
        
        //env neighbour coords and vegf level.
        for(i=0;i<EnvNeighs.size();i++)
        {
            worldP->dataFile2<<EnvNeighs[i]->Ex<<"\t"<<EnvNeighs[i]->Ey<<"\t"<<EnvNeighs[i]->Ez<<"\t"<<EnvNeighs[i]->VEGF<<"\t";
            
        }
        
        worldP->dataFile2<<endl;
    }*/
    
    //if(FIL==TIP){
    //   currLength = worldP->getDist(Mx, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);
    //}
    //start=(int)((double)rand()*(double)EnvNeighs.size()/(double)RAND_MAX);
    
    //direction=((double)rand()*2.00/(double)RAND_MAX);
    //random_shuffle(EnvNeighs.begin(), EnvNeighs.end());
    new_random_shuffle(EnvNeighs.begin(), EnvNeighs.end());
    highest=EnvNeighs[0];
    
    if (EnvNeighs[0]->VEGF > 0)
    {
        furthest = EnvNeighs[0];
        if (FIL == NONE)
            furthestDist = worldP->getDist(furthest->Ex, furthest->Ey, furthest->Ez, (int)Mx, (int)My, (int)Mz);
        else
            furthestDist = worldP->getDist(furthest->Ex, furthest->Ey, furthest->Ez, (int)filNeigh->Mx, (int)filNeigh->My, (int)filNeigh->Mz);
    }
    
    else furthest=NULL;
    
    upto=EnvNeighs.size();
    
    //direction = (int)((double)rand()*(double)2/(double)RAND_MAX);
    
    //if(direction==0) picked = start++;
    //else picked = start--;
    
    //cout<<" D= "<<direction<<" S= "<<start;
    
    for(i=1;i<upto;i++){
        
        
        //if(picked==upto)picked=0;
        //else if(picked<0)picked=upto-1;
        //if((EnvNeighs[i]->Ex==Mx)&&(EnvNeighs[i]->Ey==My+1)&&(EnvNeighs[i]->Ez==Mz)) straight = EnvNeighs[i];
        //if(FIL==TIP)
        //   if((EnvNeighs[i]->VEGF>=highest->VEGF)&&(worldP->getDist(EnvNeighs[i]->Ex, EnvNeighs[i]->Ey, EnvNeighs[i]->Ez, filNeigh->Mx, filNeigh->My, filNeigh->Mz)>currLength))
        //       highest=EnvNeighs[i];
        //  else
        if(EnvNeighs[i]->VEGF>=highest->VEGF)
            highest=EnvNeighs[i];
        if(EnvNeighs[i]->VEGF>0){
            if(FIL==NONE)
                dist = worldP->getDist(EnvNeighs[i]->Ex, EnvNeighs[i]->Ey, EnvNeighs[i]->Ez, (int)Mx, (int)My, (int)Mz);
            else
                dist = worldP->getDist(EnvNeighs[i]->Ex, EnvNeighs[i]->Ey, EnvNeighs[i]->Ez, (int)filNeigh->Mx, (int)filNeigh->My, (int)filNeigh->Mz);
            
            if(dist>=furthestDist){
                furthestDist = dist;
                furthest = EnvNeighs[i];
            }
        }
        
        //if(direction==0)
        
        //else picked--;
        
    }
    //chance = (float)rand()/(float)RAND_MAX;
    chance = (float)new_rand()/(float)NEW_RAND_MAX;
    prob = EPSILON; //epsilon high (1) = greedy, always choses highest otherwise random.
    if(chance<prob){
        chosen = highest;
        //cout<<"highest"<<endl;
    }
    
    //picking one that extends length, not necessarily the furthest one tho..
    
    //if((FIL==TIP)&&(worldP->getDist(highest->Ex, highest->Ey, highest->Ez, filNeigh->Mx, filNeigh->My, filNeigh->Mz)>currLength))
    //chosen=highest;
    //else if(FIL==NONE)
    //chosen = highest;
    else{
        //pick a random direction to extend fil in -more realistic than always picking correct direction
        //could randomly pick the correct one of course..
        
        //chosen  = EnvNeighs[(int)(((float)rand()*(float)EnvNeighs.size()/(float)RAND_MAX))];
        chosen  = EnvNeighs[(int)(((float)new_rand()*(float)EnvNeighs.size()/(float)NEW_RAND_MAX))];
        //chosen = furthest;
        //if(straight!=NULL){
        //cout<<"found straight!"<<endl;
        //chosen = straight;
        //}
        //else
        
        
        //chosen = furthest;
        
        //cout<<furthestDist<<endl;
    }
    
    
    //stats
    /*if((chosen->Ex==Mx+1)&&(chosen->Ey==My+1)) right1++;
     * if((chosen->Ex==Mx)&&(chosen->Ey==My+1)) middle++;
     * if((chosen->Ex==Mx-1)&&(chosen->Ey==My+1)) left1++;
     * if((chosen->Ex==Mx-1)&&(chosen->Ey==My)) left2++;
     * if((chosen->Ex==Mx+1)&&(chosen->Ey==My)) right2++;
     * count3++;*/
    if(BAHTI_ANALYSIS==true){ //worldP->dataFile2<<"chose "<<chosen->Ex<<"\t"<<chosen->Ey<<"\t"<<chosen->Ez<<endl;
        
    }
    
    return(chosen);
    
    
}

void World::displayStats(void) {
    cout << "Dir right: " << 100 * ((float) right2 / ((float) count3));
    cout << "Dir left: " << 100 * ((float) left2 / ((float) count3));
    cout << "right: " << 100 * ((float) right1 / ((float) count3));
    cout << " middle: " << 100 * ((float) middle / ((float) count3));
    cout << " left: " << 100 * ((float) left1 / ((float) count3)) << endl;

}

//------------------------------------------------------------------------------------------

void MemAgent::calcRetractDist(void) {

    int i, k;
    float denom, length;
    float sumDN[3];
    int upto = meshNeighs + 5;
    int DONE = 0;
    float newX, newY, newZ;
    float SL;
    float sConst;

    int flagFil = 0;
    float ForceTemp[3];
    float oldDist;


    if (Mx - filNeigh->Mx >= xMAX / 2.0f)
        oldDist = worldP->getDist(Mx - xMAX, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);
    else if (filNeigh->Mx - Mx >= xMAX / 2.0f)
        oldDist = worldP->getDist(Mx, My, Mz, filNeigh->Mx - xMAX, filNeigh->My, filNeigh->Mz);
    else
        oldDist = worldP->getDist(Mx, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);

    float newDist;
    float PN[upto][3];
    float SN[upto][3];
    float DN[upto][3];


   
    for (k = 0; k < 3; k++) sumDN[k] = 0.0f;

    //have different lengths and constants for different types of spring
    if (FIL == TIP) {

        sConst = filSpringConstant;
        SL = filSpringLength;
    }
    //calculate new force by summing neighbour vectors minused from current point PN1, PN2... then calculate the projection of S onto spring direction and get the difference, then 		  sum the elongated regions of each and times by k constant.n

    i = 0;
    do {
        if (neigh[i] != NULL) {
            PN[i][0] = Mx - neigh[i]->Mx;
            PN[i][1] = My - neigh[i]->My;
            PN[i][2] = Mz - neigh[i]->Mz;
            length = worldP->getDist(Mx, My, Mz, neigh[i]->Mx, neigh[i]->My, neigh[i]->Mz);

            /*if(surgeSpringConst==true){
             * sConst = surgeConstant;
             * SL =  surgeSpringLength;
             * surgeSpringConst = false;
             * }*/
            if (neigh[i]->FA == true) {
                sConst = FAspringConstant; //filBaseConstant;
                SL = springLength;
            }
            if (((FIL == BASE) || (FIL == STALK)) && (veilAdvancing == true) && ((neigh[i]->FIL == STALK) || (neigh[i]->FIL == TIP))) {
                sConst = filBaseConstant;
                SL = filSpringLength;
            } else if (neigh[i]->Cell != Cell) {
                sConst = junctionConstant;
                SL = JunctionSpringLength;
            } else {
                sConst = springConstant;
                SL = springLength;
            }


        } else if ((FIL == TIP) && (flagFil == 0)) {
            flagFil = 1;
            PN[i][0] = Mx - filNeigh->Mx;
            PN[i][1] = My - filNeigh->My;
            PN[i][2] = Mz - filNeigh->Mz;
            length = worldP->getDist(Mx, My, Mz, filNeigh->Mx, filNeigh->My, filNeigh->Mz);


        } else {
            DONE = 1;
        }

        if (DONE == 0) {

            if (sqrt(PN[i][0] * PN[i][0]) >= (float) xMAX / 2.0f) {

                if (PN[i][0] > 0) PN[i][0] = -((float) xMAX - PN[i][0]);
                else PN[i][0] = (float) xMAX - fabs(PN[i][0]);
                length = fabs(xMAX - PN[i][0]);

            }
            denom = sqrt((PN[i][0] * PN[i][0])+(PN[i][1] * PN[i][1])+(PN[i][2] * PN[i][2]));

            //only apply force when spring is longer than it should be, not smaller -as membranes dont ping outwards, they ruffle - should avoid 'sagging of membrane'
            if (length > SL) {
                for (k = 0; k < 3; k++) {
                    SN[i][k] = SL * (PN[i][k] / denom);
                }

                for (k = 0; k < 3; k++) {
                    DN[i][k] = PN[i][k] - SN[i][k];
                }

                for (k = 0; k < 3; k++) sumDN[k] += (sConst * DN[i][k]);
            }
        }
        i++;
    } while ((i < upto) && (DONE == 0));


    for (k = 0; k < 3; k++) {
        ForceTemp[k] = sumDN[k];

    }


    newX = Mx - (ForceTemp[0] / 2.0f);
    newY = My - (ForceTemp[1] / 2.0f);
    newZ = Mz - (ForceTemp[2] / 2.0f);


    if (newX - filNeigh->Mx >= xMAX / 2.0f)
        newDist = worldP->getDist(newX - xMAX, newY, newZ, filNeigh->Mx, filNeigh->My, filNeigh->Mz);
    else if (filNeigh->Mx - newX >= xMAX / 2.0f)
        newDist = worldP->getDist(newX, newY, newZ, filNeigh->Mx - xMAX, filNeigh->My, filNeigh->Mz);
    else
        newDist = worldP->getDist(newX, newY, newZ, filNeigh->Mx, filNeigh->My, filNeigh->Mz);



    Cell->actinUsed -= (oldDist - newDist);
    //cout<<(oldDist-worldP->getDist(newX, newY, newZ, filNeigh->Mx, filNeigh->My, filNeigh->Mz))<<endl;
}
//----------------------------------------------------------------------------------


bool MemAgent::meshConnected(MemAgent* tocheck) {

     int i = 0;
    int j = 0;
    int k=0;
    int flag = 0;
    MemAgent* tp;
    MemAgent* np;
	bool returnType = false;
    bool flagDont=false;
    //check if the neighbour agent is either,
    //a node agent and has a spring connecting to this one,
    //or a surface agent and belongs to any neigh node triangle..
    //cout<<"|";


    if(((node==false)&&((triangle[0]->neighs>500)||(triangle[1]->neighs>500)||(triangle[2]->neighs>500)))||((tocheck->node==false)&&((tocheck->triangle[0]->neighs>500)||(tocheck->triangle[1]->neighs>5000)||(tocheck->triangle[2]->neighs>5000)))){
        cout<<"hmmm"<<this<<" "<<tocheck<<endl;
        flagDont = true;
    }

    if (node == true) {
        if (tocheck->node == true) {
            //is it in direct 4 neighhs list?
            do {

                if (neigh[i] == tocheck) flag = 1;
                i++;
            } while ((i < neighs) && (flag == 0));

            //check 2nd level out neighs, are they connected?
            if (flag == 0) {
                i = 0;
                j = 0;

                do {
                    tp = neigh[i];
                    j=0;
                    do {

                        if (tp->neigh[j] == tocheck) flag = 1;
                        j++;
                    } while ((j < tp->neighs) && (flag == 0));
                    i++;
                } while ((i < neighs) && (flag == 0));

                if(flag==0)
					returnType= false;
                else returnType =true;
            } else returnType = true;
        } else {


			//  return(false);
			// }
			// }
            if(flagDont==false){

				do {
					tp = tocheck->triangle[i];
					if (tp == this) flag = 1;
					else {
						j = 0;
						do {

							if (tp->neigh[j] == this) flag = 1;
							j++;
						} while ((j < tp->neighs) && (flag == 0));
					}
					i++;

				} while ((i < 3) && (flag == 0));
				//check if its in 2nd level of connected neighs, this gives moore neighbourhood - if a little extra, tho if they are in grid sites nearby this is ok...
				if (flag == 0) {

					i=0;
					do {

						tp = tocheck->triangle[i];

						j = 0;
						do {
							k=0;
							do{
								if(tp->neigh[j]->neigh[k] == this) flag = 1;
								k++;
							}while((k<tp->neigh[j]->neighs)&&(flag==0));
							j++;
						} while ((j < tp->neighs) && (flag == 0));

						i++;

					} while ((i < 3) && (flag == 0));

					if(flag==0)returnType = false;
					else returnType = true;
				}

				else returnType = true;
            }
        }
    }//if this guy is a surface agent...
    else {
        //need to test if any of this guys triangle nodes are a) mesh connected to the tocheck node, if its a node, or mesh connected to any of the other guys triangle nodes

        if(flagDont==false){
			if (tocheck->node == true) {

				i = 0;
				do {
					tp = triangle[i];
					if (tp == tocheck) flag = 1;
					else {
						j = 0;
						do {

							if (tp->neigh[j] == tocheck) flag = 1;
							j++;
						} while ((j < tp->neighs) && (flag == 0));



					}
					i++;
				} while ((i < 3) && (flag == 0));

				//check 2nd level out neighs...
				if (flag == 0) {

					i = 0;
					do {
						tp = triangle[i];

						j = 0;
						do {
							k = 0;
							do {
								if (tp->neigh[j]->neigh[k] == tocheck) flag = 1;
								k++;
							} while ((k < tp->neigh[j]->neighs) && (flag == 0));
							j++;
						} while ((j < tp->neighs) && (flag == 0));




						i++;
					} while ((i < 3) && (flag == 0));


					if (flag == 0)returnType = false;
					else returnType = true;
				} else returnType = true;
			}
        }
		else {

            if(flagDont==false){
				//only allowing if it shares a triangle node in common - but could add or triangle node is connected to the other node in its neighs...
				for (i = 0; i < 3; i++) {
					tp = triangle[i];
					for (j = 0; j < 3; j++) {
						np = tocheck->triangle[j];

						if (tp == np) flag = 1;
					}
				}



				if (flag == 0) {
					//check 2nd level neighs...
					for (i = 0; i < 3; i++) {
						tp = triangle[i];
						for (k = 0; k < tp->neighs; k++) {
							for (j = 0; j < 3; j++) {
								np = tocheck->triangle[j];

								if (tp->neigh[k] == np) flag = 1;
							}
						}
					}
					if (flag == 0) {

						for (i = 0; i < 3; i++) {
							tp = tocheck->triangle[i];
							for (k = 0; k < tp->neighs; k++) {
								for (j = 0; j < 3; j++) {
									np = triangle[j];

									if (tp->neigh[k] == np) flag = 1;
								}
							}
						}
						if (flag == 0)returnType = false;
						else returnType = true;
					} else returnType = true;
				} else returnType = true;

			}
		}
	}
	
    if(flagDont==true) return(true);
	else return(returnType);
	//return (true);




}
