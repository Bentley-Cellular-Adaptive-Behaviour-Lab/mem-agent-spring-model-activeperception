#include <math.h>
#include <iomanip>
#include "objects.h"
#include <fstream>
using namespace std;

bool found_sp = false;
vector <int> current_order;
vector <int> shuffles;
vector <int> headShuffles_COM;
vector <int> orderFar;
vector <int> orderNear;
vector <int> stored_Far;
vector <int> stored_Near;
int oldHeadNode;
int overtakeTime = 1;
int frequencyHeads[ECELLS] = {0};
int frequencyHeadsDom[ECELLS] = {0};
int SnapShot_frequencyHeads[ECELLS] = {0};
int SnapShot_frequencyHeadsDom[ECELLS] = {0};
int battleTime = 0;
int domReighMutant=0;
int lastHead;
int totalTime = 0;
int snapShot_oldHead;
int dominant_reign = 0;
int countDomRei = 0;
int dom_head = -1;
int battling_reign = 0;
int battlers = 0;
int DOM_reigners[ECELLS] = {0};
int Battle_reigners[ECELLS] = {0};
int snapShot_count = 0;
bool snapShot_start = false;
int countActual = 0;
int TotalBattlingTime = 0;
int battles = 0;
int AILars = 0;
int countHET = 0;
int countHETact = 0;


std::vector< std::vector<float> > World::getGridSiteData()
{
    std::vector< std::vector<float> > retval;

//    for(int x = 0; x < xMAX; x++)
//    {
//        for(int y = 0; y < yMAX; y++)
//        {
//            for(int z = 0; z < zMAX; z++)
//            {
//                cout << "x: " << x << " y: " << y << " z: " << z << " type= " << grid[x][y][z].type << endl;
//            }
//        }
//    }

    for (int x = 0; x < xMAX; x++)
    {
        for (int y = 0; y < yMAX; y++)
        {
            for (int z = 0; z < zMAX; z++)
            {
                std::vector<float> gridSiteValues;
                gridSiteValues.push_back(x);
                gridSiteValues.push_back(y);
                gridSiteValues.push_back(z);

                if (grid[x][y][z].Eid != NULL)
                {
                    gridSiteValues.push_back(grid[x][y][z].Eid->VEGF);
                }
               else {
                    gridSiteValues.push_back(0);
                }

                float vegfTotal = 0;
                float vegfrTotal = 0;
                float activeVegfrTotal = 0;
                float dll4Total = 0;

                if (grid[x][y][z].type == E)
                {
                    gridSiteValues.push_back(0); //for environment type
                    gridSiteValues.push_back(grid[x][y][z].Fids.size());

                    for (int endothelialCellNumber = 0 ; endothelialCellNumber < ECELLS; endothelialCellNumber++)
                    {
                        vegfTotal = 0;
                        vegfrTotal = 0;
                        activeVegfrTotal = 0;
                        dll4Total = 0;

                        if (grid[x][y][z].Fids.size() > 0) // Check if grid site contains filapodia
                        {
                            for (int filopodiaID = 0; filopodiaID < grid[x][y][z].Fids.size(); filopodiaID++)
                            {
                                // Check if filapodium belongs to this EC
                                if (grid[x][y][z].Fids[filopodiaID]->Cell == ECagents[endothelialCellNumber])
                                {
                                    vegfTotal += grid[x][y][z].Fids[filopodiaID]->SumVEGF;
                                    vegfrTotal += grid[x][y][z].Fids[filopodiaID]->VEGFR;
                                    activeVegfrTotal += grid[x][y][z].Fids[filopodiaID]->VEGFRactive;
                                    dll4Total += grid[x][y][z].Fids[filopodiaID]->Dll4;
                                }
                            }
                        }
                        gridSiteValues.push_back(vegfTotal);
                        gridSiteValues.push_back(vegfrTotal);
                        gridSiteValues.push_back(activeVegfrTotal);
                        gridSiteValues.push_back(dll4Total);
                    }
                }
                else if (grid[x][y][z].type == M)
                {
                    gridSiteValues.push_back(1); // for membrane type
                    gridSiteValues.push_back(grid[x][y][z].Fids.size() + grid[x][y][z].Mids.size());

                    for (int endothelialCellNumber = 0 ; endothelialCellNumber < ECELLS; endothelialCellNumber++)
                    {
                        vegfTotal = 0;
                        vegfrTotal = 0;
                        activeVegfrTotal = 0;
                        dll4Total = 0;

                        if (grid[x][y][z].Fids.size() > 0)
                        {
                            for (int filopodiaID = 0; filopodiaID < grid[x][y][z].Fids.size(); filopodiaID++)
                            {
                                if (grid[x][y][z].Fids[filopodiaID]->Cell == ECagents[endothelialCellNumber])
                                {
                                    vegfTotal += grid[x][y][z].Fids[filopodiaID]->SumVEGF;
                                    vegfrTotal += grid[x][y][z].Fids[filopodiaID]->VEGFR;
                                    activeVegfrTotal += grid[x][y][z].Fids[filopodiaID]->VEGFRactive;
                                    dll4Total += grid[x][y][z].Fids[filopodiaID]->Dll4;
                                }
                            }
                        }
                        if (grid[x][y][z].Mids.size() > 0)
                        {
                            for (int memAgentID = 0; memAgentID < grid[x][y][z].Mids.size(); memAgentID++)
                            {
                                if (grid[x][y][z].Mids[memAgentID]->Cell == ECagents[endothelialCellNumber])
                                {
                                    vegfTotal += grid[x][y][z].Mids[memAgentID]->SumVEGF;
                                    vegfrTotal += grid[x][y][z].Mids[memAgentID]->VEGFR;
                                    activeVegfrTotal += grid[x][y][z].Mids[memAgentID]->VEGFRactive;
                                    dll4Total += grid[x][y][z].Mids[memAgentID]->Dll4;
                                }
                            }
                        }
                        gridSiteValues.push_back(vegfTotal);
                        gridSiteValues.push_back(vegfrTotal);
                        gridSiteValues.push_back(activeVegfrTotal);
                        gridSiteValues.push_back(dll4Total);
                    }
                }
                retval.push_back(gridSiteValues);
            }
        }
    }
    return retval;
}

std::vector< std::vector< std::vector < std::array<int,2> > > > World::getGridMapOfFilopodiaMovement()
 {
    vector < vector < vector < array<int, 2> > > > retval(xMAX, vector < vector < array<int, 2> > >(yMAX, vector < array<int, 2> >(zMAX, {0,0})));

    int totalExtenstions = 0;
    int totalRetractions = 0;

    for (EC* ec : ECagents)
    {
        for (std::array<int, 3> filExtension : ec->filopodiaExtensions)
        {
            totalExtenstions++;
            retval[filExtension[0]][filExtension[1]][filExtension[2]][0] += 1;
        }
        for (std::array<int, 3> filRetractions : ec->filopodiaRetractions)
        {
            totalRetractions++;
            retval[filRetractions[0]][filRetractions[1]][filRetractions[2]][1] += 1;
        }
    }

    return retval;
}


std::vector< std::vector< std::vector<float> > > World::getFilopodiaBaseLocationsAndTotalVegfr()
{
    std::vector< std::vector< std::vector<float> > > retval;

    std::vector< std::vector<float> > cell1Values;
    std::vector< std::vector<float> > cell2Values;

    if (this->timeStep == 800)
    {
        cout << "stop";
    }

    for (Filopodia* filopodia : this->filopodia)
    {
        if (!filopodia->retracted)
        {
            MemAgent* currentMemAgentInFilopodia = filopodia->base;
            if (currentMemAgentInFilopodia != nullptr)
            {
                float x = currentMemAgentInFilopodia->Mx;
                float y = currentMemAgentInFilopodia->My;
                float z = currentMemAgentInFilopodia->Mz;
                float totalVegfr = currentMemAgentInFilopodia->VEGFRactive;

                totalVegfr += currentMemAgentInFilopodia->VEGFRactive;
                while (currentMemAgentInFilopodia->plusSite != nullptr)
                {
                    currentMemAgentInFilopodia = currentMemAgentInFilopodia->plusSite;
                    totalVegfr += currentMemAgentInFilopodia->VEGFRactive;
                }

                std::vector<float> values;
                values.push_back(x); values.push_back(y); values.push_back(z); values.push_back(totalVegfr);

                if (currentMemAgentInFilopodia->Cell == ECagents[0])
                {
                    cell1Values.push_back(values);
                }
                else if (currentMemAgentInFilopodia->Cell == ECagents[1])
                {
                    cell2Values.push_back(values);
                }
            }
            else {
                cout << "filopodia base is null what is going on ?????";
            }
        }
    }
    retval.push_back(cell1Values); retval.push_back(cell2Values);
    return retval;
}

void World::simulateTimestep(std::vector< std::vector<float> > cellIncrements)
{
    for (unsigned i = 0; i < cellIncrements.size(); i++)
    {
        EC* currentCell = ECagents[i];
        currentCell->VEGFRtot += cellIncrements[i][0];
        currentCell->Dll4tot += cellIncrements[i][1];
        currentCell->Notchtot += cellIncrements[i][2];
    }
    simulateTimestep();
}


//---------------------------------------------------------------------------------------------
bool Hysteresis::evaluate_hysteresis(ofstream& fileTo) {

    
    int i;
    
    //calc stability of current cell properties//
    
    //Dll4
    if((Cell->Dll4tot<stableDll4+Dll4_SigRange)&&(Cell->Dll4tot>stableDll4-Dll4_SigRange)){
        //stable, so increment timer
        //cout<<"stable.."<<stabilityTimer_latest<<endl;
        stabilityTimer_latest++;
        stabilityTimer_overall++;
    }
    else{
        //unstable so use current level as new start point to measure stability from and reset timer
        // cout<<"UNstable.."<<endl;
        stableDll4 = Cell->Dll4tot;
        stabilityTimer_latest=0;
    }
    //filopodia (use actin params)
        
    if((stabilityTimer_latest>=CELL_STABLE)||(stabilityTimer_overall>=bio_time_window)){
    //cell is deemed stable, so output results 
        
        cout<<"Stabilised! "<<Cell->Dll4tot<<"\t"<<stabilityTimer_overall<<endl;
        //cout<<"stable! "<<Current_Dll4_incremented_level<<endl;
        storeDll4.push_back(Cell->Dll4tot);
        storeTimes.push_back(stabilityTimer_overall);
        
        
        //increment fixed neigbour dll4 levels
        if(Current_Dll4_incremented_level==HYST_INCREMENT_MAX) direction=false;
        
        if(direction==false)//MAX_dll4)
                Current_Dll4_incremented_level-=HYST_INCREMENT;
        else 
            Current_Dll4_incremented_level+=HYST_INCREMENT;
        
        cout<<"hysteresis Dext = "<<Current_Dll4_incremented_level<<endl;
       
        
        //clear timers for next round.
        stabilityTimer_latest=0;
        stabilityTimer_overall=0;
        
    }
         if(Current_Dll4_incremented_level<0){ 
      
             //write data to file
             for(i=0;i<HYST_INCREMENT_MAX/HYST_INCREMENT;i++){
                 
             fileTo<<(i*HYST_INCREMENT)<<"\t"<<storeDll4[i]<<"\t"<<storeTimes[i]<<"\t"<<storeDll4[storeDll4.size()-1-i]<<"\t"<<storeTimes[storeDll4.size()-1-i]<<endl;
             }
             
             fileTo<<endl;
             fileTo<<storeDll4[HYST_INCREMENT_MAX/HYST_INCREMENT]<<"\t"<<storeTimes[HYST_INCREMENT_MAX/HYST_INCREMENT]<<endl;
             
             
             direction=true;
             
             storeDll4.clear();
             storeTimes.clear();
             
             return(false);
         }
         else return(true);

}



void World::printScores(ofstream& fileTo) {





    int i, j, k;
    int sumNewJunction = 0;
    int name, name2;
    int adjacentLoops = 0;
    int nonAdjacentLoops = 0;
    int c, s, uptoS;
    int uptoC = ECELLS;
    int count = 0;
    int flag;
    int neighCell = 0;
    EC* ecp;
    int Xsum = 0;
    int Patt = 1;
    int tipCount = 0;


    for (c = 0; c < uptoC; c++) {

        fileTo << ECagents[c]->VEGFRtot << "\t";
    }

    fileTo << endl;
    /*	count=0;
            flag=0;
            ecp=ECagents[c];
            uptoS=ecp->stableArray.size();
		
		
            //------------------------------------------------------------------
            for(s=uptoS-1;s>=0;s--){
			
                    if((flag==0)&&(ecp->stableArray[s]==0)) flag=1;
			
                    if(flag==0) count++;
            }
		
            //cout<<"C= "<<count;
            //------------------------------------------------------------------
		
            //prints out how long it took to stabilize fully
            Xsum=Xsum+count;
		
		
            //print out how many tip cells and whether any are next to each other.
            //define tip cell as having over half full amount of VEGFR and more than initial amount of memAgents
		
            //fileTo<<ecp->VEGFRtot<<"\t"<<ecp->actVEGFRArray[0];//<<ecp->Dll4tot<<"\t"<<ecp->actNotCurrent<<"\t";
		
            if(ecp->tipCellTest()==true){
                    //it is a tip cell
                    //fileTo<<"1\t";
                    //if its next to a tip cell score a bad point - not the pattern we want.
                    if(neighCell==1) Patt=0;
                    if(c==uptoC-1){
                            if(ECagents[0]->tipCellTest()==true) Patt=0;
    }
                    neighCell=1;
                    tipCount++;
            }
            else{
                    neighCell=0;
                    //fileTo<<"0\t";
            }
		
    }
    //fileTo<<endl;
    if(tipCount==0){Patt=0;}
	
    if(Patt==1){
            if((tipCount==4)||(tipCount==5)) Patt=1;
            else Patt=0;
    }
	
    fileTo<<100*(((float)Xsum/(float)uptoC)/(float)MAXtime)<<"\t"<<Patt<<"\t"<<tipCount<<endl;
     */



}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

void World::getCellNeighbours(void) {

    int i, j, k, y, x, m, n, p, r, s, t, flag;
    EC* ecp;
    MemAgent* np;

    vector <EC*> neighbours;

    if (cellNeighbourhoods.size() != 0) {
        for (r = 0; r < ECagents.size(); r++) {

            do {
                cellNeighbourhoods[r].pop_back();
            } while (cellNeighbourhoods[r].size() != 0);

        }
        for (r = 0; r < ECagents.size(); r++) {
            cellNeighbourhoods.pop_back();
        }
    }
    //hypothesis - why equal boundaries no offset fails more than equal boundaries offset - offset one is fighting on more fronts - or bigger fronts so has distributed its dll4 thinner all over as it were - so is essentially using less to inhibit the ones more likely to become tips - facing the vegf. We know that turing down dll4 helps the pattern to stabilise in high vegf...
    for (r = 0; r < ECagents.size(); r++) {
        ecp = ECagents[r];

        //group agents into which boundary they are in, with which cell,
        for (s = 0; s < ecp->nodeAgents.size(); s++) {
            np = ecp->nodeAgents[s];

            // - show how many neighbour cells it has  and how big each bounday is.. in percentage of agents..
            //and show total level of active notch on each boundary.
            if (np->junction == true) {
                flag = 0;
                if ((np->FIL == NONE) || (np->FIL == BASE)) {
                    i = np->Mx;
                    j = np->My;
                    k = np->Mz;
                }
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
                    }
                        //layer below
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
                    }
                        //layer above
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
                    if (m >= xMAX) m = 0;
                    if (m < 0) m = xMAX - 1;
                    //-------------------------------

                    if (insideWorld(m, n, p) == true) {
                        if (grid[m][n][p].type == M) {
                            for (y = 0; y < (int) grid[m][n][p].Mids.size(); y++) {
                                if (grid[m][n][p].Mids[y]->Cell != ecp) {
                                    for (t = 0; t < neighbours.size(); t++) {
                                        if (grid[m][n][p].Mids[y]->Cell == neighbours[t]) {
                                            flag = 1;
                                        }
                                    }
                                    if (flag == 0) neighbours.push_back(grid[m][n][p].Mids[y]->Cell);

                                }

                            }
                        }

                    }

                }
            }
        }
        cellNeighbourhoods.push_back(neighbours);
        neighbours.clear();

    }

}
int patternHistory = 0;
bool patterned = false;
//---------------------------------------------------------------------------------------------
void World::evaluateSandP()
{
    int Patt = 0;
    int c, s, r;
    int count;
    int flag = 0;
    int neighCell = 0;

    int uptoS = ECELLS;
    int tipCount = 0;
    int cont = 0;
    EC* ecp;

    for (c = 0; c < ECELLS; c++)
    {
        ecp = ECagents[c];

        if (ecp->tipCellTest())
        {
            tipCount++;
            if (neighCell == 1)
            {
                Patt = 0;
                break;
            } else if (tipCount == 4 || tipCount == 5)
            {
                Patt = 1;
            }
            neighCell = 1;
        } else {
            neighCell = 0;
        }
    }

    if (Patt == 1)
        patternHistory++;
    else
        patternHistory = 0;

    if (patternHistory == 150) {
        cout << "patterned!" << endl;
        patterned = true;
        RUNSfile << timeStep;
        timeStep = MAXtime;
    }

    if (timeStep == MAXtime - 1 && !patterned)
        RUNSfile << "-1";
}


//---------------------------------------------------------------------------------------------
/*void World::filopodia_contacts_Test(void){
 *
 * int i, j, k;
 * Filopodia* fp;
 * Filopodia* fpCurr;
 * vector <Filopodia*> current_contacts;
 * vector <Contact*> copy_previous_contacts;
 * vector<Contact*>::iterator T;
 * Contact* cp;
 * bool found=false;
 * //every time step go through all current non-retracted filopodia,
 *
 * for(i=0;i<filopodia.size();i++){
 *
 * copy_previous_contacts.clear();
 * fp = filopodia[i];
 *
 * if(fp->retracted==false){
 * //copy current fils contact list
 * copy_previous_contacts.assign(fp->contactList.begin(), fp->contactList.end());
 *
 * //see if they have crossings with other filopodia from different cells
 * current_contacts = fp->checkForContact();
 * //cout<<"checkedForContacts"<<endl;
 *
 * //cout<<"curr_con "<<current_contacts.size()<<endl;
 * for(j=0;j<current_contacts.size();j++){
 *
 * fpCurr = current_contacts[j];
 * found=false;
 * //cout<<"copy_prev_con "<<copy_previous_contacts.size()<<endl;
 * for(k=0;k<copy_previous_contacts.size();k++){
 *
 * if(copy_previous_contacts[k]->finished==false){
 * //if yes - update that contact in list (increase lifetime) and remove
 * if((copy_previous_contacts[k]->fil1==fpCurr)||(copy_previous_contacts[k]->fil2==fpCurr)){
 *
 * copy_previous_contacts[k]->lifeTime++;
 * T = copy_previous_contacts.begin();
 * copy_previous_contacts.erase(T+k);
 * k--;
 * found=true;
 *
 * }
 * }
 * }
 * //if thats a new contact add it to the list (fil in start time and fil details)
 * if(found==false){
 * cp = new Contact();
 * fp->contactList.push_back(cp);
 * cp->fil1 = fp;
 * cp->fil2 = fpCurr;
 * cp->startTime = timeStep;
 * contacts.push_back(cp);
 * fpCurr->contactList.push_back(cp);
 *
 * }
 *
 *
 *
 * }
 *
 *
 * //set all remaining contacts to finished and remove from list
 * for(k=0;k<copy_previous_contacts.size();k++){
 * copy_previous_contacts[k]->finished=true;
 *
 * }
 *
 * }
 * }
 * }
 * //---------------------------------------------------------------------------------------------
 * //---------------------------------------------------------------------------------------------
 * vector <Filopodia*> Filopodia:: checkForContact(void){
 *
 * int i, j, k;
 * vector <Filopodia*> filsContacted;
 * MemAgent* Mcurrent;
 * Filopodia* fil;
 * int count=0;
 * Mcurrent=base->plusSite;
 *
 * if(Mcurrent!=NULL){
 * do{
 * //only treats a fil from a different cell in same grid site as a contact
 * //(could widen the search to all neighs...)
 * if(worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids.size()>1){
 * //could have more than one contact in same location
 * for(j=0;j<(int)worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids.size();j++){
 * if(worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids[j]!=Mcurrent){
 * if(worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids[j]->Cell!=Mcurrent->Cell){
 * fil = findFil(worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids[j]);
 *
 * //cout<<"thinks: "<<worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids[j]->Cell<<endl;
 * //cout<<"this one "<<Mcurrent->Cell;
 * filsContacted.push_back(fil);
 * }
 * }
 * }
 * }
 * Mcurrent=Mcurrent->plusSite;
 * count++;
 * //cout<<count<<"\t"<<endl;
 * }while(Mcurrent!=NULL);
 * }
 *
 * return(filsContacted);
 * }*/
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

void World::filopodia_contacts_Test(void) {

    int i, j, k;
    Filopodia* fp;
    Filopodia* fpCurr;
    vector <Filopodia*> current_contacts;
    vector <Contact*> copy_previous_contacts;
    vector<Contact*>::iterator T;
    Contact* cp;
    bool found = false;
    //every time step go through all current non-retracted filopodia,

    for (i = 0; i < filopodia.size(); i++) {
        for (j = 0; j < filopodia[i]->contactList.size(); j++) {
            filopodia[i]->contactList[j]->updated = false;
        }
    }

    for (i = 0; i < filopodia.size(); i++) {

        copy_previous_contacts.clear();
        fp = filopodia[i];

        if ((fp->retracted == false) && ((fp->Cell == ECagents[1]) || (fp->Cell == ECagents[3]))) {
            //copy current fils contact list
            copy_previous_contacts.assign(fp->contactList.begin(), fp->contactList.end());
            if (copy_previous_contacts.size() > 0) {
                for (k = 0; k < copy_previous_contacts.size(); k++) {
                    //cout<<fp<<" size "<<copy_previous_contacts.size()<<" "<<copy_previous_contacts[k]->fil1<<" "<<copy_previous_contacts[k]->fil2<<" "<<copy_previous_contacts[k]->finished<<endl;
                }

            }
            //see if they have crossings with other filopodia from different cells
            current_contacts = fp->checkForContact();
            //cout<<"checkedForContacts"<<endl;

            //cout<<"curr_con "<<current_contacts.size()<<endl;
            for (j = 0; j < current_contacts.size(); j++) {

                fpCurr = current_contacts[j];
                found = false;
                if ((fpCurr->Cell == ECagents[1]) || (fpCurr->Cell == ECagents[3])) {
                    //cout<<"copy_prev_con "<<copy_previous_contacts.size()<<endl;
                    for (k = 0; k < copy_previous_contacts.size(); k++) {

                        if (copy_previous_contacts[k]->finished == false) {
                            //if yes - update that contact in list (increase lifetime) and remove
                            if ((copy_previous_contacts[k]->fil1 == fpCurr) || (copy_previous_contacts[k]->fil2 == fpCurr)) {

                                if (copy_previous_contacts[k]->updated == false) {
                                    copy_previous_contacts[k]->lifeTime++;
                                    copy_previous_contacts[k]->updated = true;
                                }
                                T = copy_previous_contacts.begin();
                                copy_previous_contacts.erase(T + k);
                                k--;
                                found = true;

                            }
                        }
                    }

                    //if thats a new contact add it to the list (fil in start time and fil details)
                    if (found == false) {
                        cp = new Contact();
                        fp->contactList.push_back(cp);
                        cp->fil1 = fp;
                        cp->fil2 = fpCurr;
                        cp->startTime = timeStep;
                        contacts.push_back(cp);
                        fpCurr->contactList.push_back(cp);
                        cp->updated = true;
                    }
                }



            }


            //set all remaining contacts to finished and remove from list
            for (k = 0; k < copy_previous_contacts.size(); k++) {
                copy_previous_contacts[k]->finished = true;

            }

        }
    }


}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

vector <Filopodia*> Filopodia::checkForContact(void) {

    int i, j, k;
    vector <Filopodia*> filsContacted;
    MemAgent* Mcurrent;
    Filopodia* fil;
    int count = 0;
    Mcurrent = base->plusSite;
    int flag = 0;
    if (Mcurrent != NULL) {
        do {
            //only treats a fil from a different cell in same grid site as a contact
            //(could widen the search to all neighs...)
            if (worldP->grid[(int) Mcurrent->Mx][(int) Mcurrent->My][(int) Mcurrent->Mz].Fids.size() > 1) {
                //could have more than one contact in same location
                for (j = 0; j < (int) worldP->grid[(int) Mcurrent->Mx][(int) Mcurrent->My][(int) Mcurrent->Mz].Fids.size(); j++) {
                    if (worldP->grid[(int) Mcurrent->Mx][(int) Mcurrent->My][(int) Mcurrent->Mz].Fids[j] != Mcurrent) {
                        if (worldP->grid[(int) Mcurrent->Mx][(int) Mcurrent->My][(int) Mcurrent->Mz].Fids[j]->Cell != Mcurrent->Cell) {
                            flag = 0;
                            fil = findFil(worldP->grid[(int) Mcurrent->Mx][(int) Mcurrent->My][(int) Mcurrent->Mz].Fids[j]);
                            for (k = 0; k < filsContacted.size(); k++) {
                                if (fil == filsContacted[k]) flag = 1;
                            }
                            //cout<<"thinks: "<<worldP->grid[(int)Mcurrent->Mx][(int)Mcurrent->My][(int)Mcurrent->Mz].Fids[j]->Cell<<endl;
                            //cout<<"this one "<<Mcurrent->Cell;
                            if (flag == 0) {
                                filsContacted.push_back(fil);

                            }
                        }
                    }
                }
            }
            Mcurrent = Mcurrent->plusSite;
            count++;
            //cout<<count<<"\t"<<endl;
        } while (Mcurrent != NULL);
    }

    return (filsContacted);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

Filopodia* Filopodia::findFil(MemAgent* mp) {

    MemAgent* Mcurrent;
    int flag = 0;
    if (mp->filNeigh != NULL)
        Mcurrent = mp->filNeigh;
    else
        Mcurrent = mp->SpringBelong->start;
    //int count=0;
    if (Mcurrent != NULL) {
        do {
            if (Mcurrent->FIL == BASE) {
                flag = 1;
            } else {

                Mcurrent = Mcurrent->filNeigh;

            }
            //count++;
            //cout<<count<<"\t"<<endl;

        } while ((Mcurrent != NULL) && (flag == 0));
    } else {
        cout << "oh dear, think this is the base already, or no minus site  - in findFil" << endl;
    }
    //cout<<"cell in findFil: "<<Mcurrent->Cell<<endl;
    if (Mcurrent == NULL) cout << "not finding fil base" << endl;
    return (Mcurrent->base_fil_belong);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------

void World::printContactsInfo(ofstream& fileTo) {

    int i, j;
    int count = 0;
    int count2 = 0;
    int count2a = 0;
    int count4 = 0;
    int count6 = 0;
    int count8 = 0;
    int count10 = 0;
    int count12 = 0;
    int count14 = 0;
    int count16 = 0;
    int count18 = 0;
    int count20 = 0;
    int count20p = 0;

    //dataFile<<"&&&&& "<<timeStep<<endl;
    /*for(i=0;i<contacts.size();i++){
     *
     * //check all ok
     *
     * if(contacts[i]->fil1==contacts[i]->fil2) dataFile<<"same fill!"<<endl;
     * if(contacts[i]->fil1->Cell==contacts[i]->fil2->Cell) dataFile<<"same Cell!"<<endl;
     *
     * if(contacts[i]->lifeTime>5){
     * dataFile<<"\n*-----\n";
     * dataFile<<contacts[i]->startTime<<"\t"<<contacts[i]->lifeTime<<endl;
     *
     * if(contacts[i]->finished==true) dataFile<<"finished"<<endl;
     * }
     *
     * }*/

    cout << "printing!" << endl;
    int sum = 0;
    float mean;
    float stdev = 0;
    float meanOver8 = 0;
    float meanUnder20 = 0;
    float sumOver8 = 0;
    float stdevOver8 = 0;
    float stdevUnder20 = 0;
    float sumUnder20 = 0.0f;
    int countUnder20 = 0;
    //how many total

    if (fusedCells.size() != 0) {
        fileTo << contacts.size() << "\t";
        int earliest = 10000;
        int latest = 10000;
        //how many over 2 mins
        for (i = 0; i < contacts.size(); i++) {

            if ((contacts[i]->startTime < earliest) && (contacts[i]->lifeTime >= 8)) {
                earliest = contacts[i]->startTime;
            }
            if ((contacts[i]->startTime < latest) && (contacts[i]->finished == false)) {
                latest = contacts[i]->startTime;
            }

            if (contacts[i]->lifeTime >= 8) {
                count++;
                sumOver8 += contacts[i]->lifeTime;
                if (contacts[i]->lifeTime < 88) {
                    sumUnder20 += contacts[i]->lifeTime;
                    countUnder20++;
                }
            }
            //if((contacts[i]->lifeTime<16)) count2a++;
            if ((contacts[i]->lifeTime >= 8) && (contacts[i]->lifeTime < 16)) count2++;
            if ((contacts[i]->lifeTime >= 16) && (contacts[i]->lifeTime < 24)) count4++;
            if ((contacts[i]->lifeTime >= 24) && (contacts[i]->lifeTime < 32)) count6++;
            if ((contacts[i]->lifeTime >= 32) && (contacts[i]->lifeTime < 40)) count8++;
            if ((contacts[i]->lifeTime >= 40) && (contacts[i]->lifeTime < 48)) count10++;
            if ((contacts[i]->lifeTime >= 48) && (contacts[i]->lifeTime < 56)) count12++;
            if ((contacts[i]->lifeTime >= 56) && (contacts[i]->lifeTime < 64)) count14++;
            if ((contacts[i]->lifeTime >= 64) && (contacts[i]->lifeTime < 72)) count16++;
            if ((contacts[i]->lifeTime >= 72) && (contacts[i]->lifeTime < 80)) count18++;
            if ((contacts[i]->lifeTime >= 80) && (contacts[i]->lifeTime < 88)) count20++;
            if ((contacts[i]->lifeTime >= 88)) count20p++;

            //to calc mean and st dev
            sum += contacts[i]->lifeTime;

        }
        fileTo << count << "\t" << count2 << "\t" << count4 << "\t" << count6 << "\t" << count8 << "\t" << count10 << "\t" << count12 << "\t" << count14 << "\t" << count16 << "\t" << count18 << "\t" << count20 << "\t" << count20p << "\t";

        mean = (float) sum / (float) contacts.size();
        meanOver8 = sumOver8 / (float) count;
        meanUnder20 = sumUnder20 / (float) countUnder20;
        for (i = 0; i < contacts.size(); i++) {


            stdev += ((contacts[i]->lifeTime - mean)*(contacts[i]->lifeTime - mean));
            if (contacts[i]->lifeTime >= 8) {
                stdevOver8 += ((contacts[i]->lifeTime - meanOver8)*(contacts[i]->lifeTime - meanOver8));
                if (contacts[i]->lifeTime < 88) stdevUnder20 += ((contacts[i]->lifeTime - meanUnder20)*(contacts[i]->lifeTime - meanUnder20));
            }
        }

        stdev = sqrt(stdev / (float) contacts.size());
        stdevOver8 = sqrt(stdevOver8 / (float) count);
        stdevUnder20 = sqrt(stdevUnder20 / (float) countUnder20);
        fileTo << mean << "\t" << stdev << "\t" << meanOver8 << "\t" << stdevOver8 << "\t" << meanUnder20 << "\t" << stdevUnder20 << "\t";



        fileTo << fusedCells_times[0] - earliest << "\t" << fusedCells_times[0] - latest << endl;
    }

}
//-----------------------------------------------------------------------------------

void World::collect_Filopodia_Data(void) {

    int i;
    int sum = 0;
    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
    int count5 = 0;
    int count6 = 0;
    int count7 = 0;

    for (i = 0; i < filopodia.size(); i++) {
        if (filopodia[i]->retracted == true) {
            sum += (filopodia[i]->time_retract_complete - filopodia[i]->time_created);


            //if((filopodia[i]->time_retract_complete-filopodia[i]->time_created>=0)&&(contacts[i]->lifeTime<)) count2++;
            //if((filopodia[i]->time_retract_complete-filopodia[i]->time_created>=24)&&(contacts[i]->lifeTime<40)) count4++;
            //if((cfilopodia[i]->time_retract_complete-filopodia[i]->time_created>=40)&&(contacts[i]->lifeTime<56)) count6++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 0) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 40)) count1++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 40) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 80)) count2++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 80) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 120)) count3++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 120) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 160)) count4++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 160) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 200)) count5++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 200) && (filopodia[i]->time_retract_complete - filopodia[i]->time_created < 240)) count6++;
            if ((filopodia[i]->time_retract_complete - filopodia[i]->time_created >= 240)) count7++;
        } else {
            sum += timeStep - filopodia[i]->time_created;
            if ((timeStep - filopodia[i]->time_created >= 0) && (timeStep - filopodia[i]->time_created < 40)) count1++;
            if ((timeStep - filopodia[i]->time_created >= 40) && (timeStep - filopodia[i]->time_created < 80)) count2++;
            if ((timeStep - filopodia[i]->time_created >= 80) && (timeStep - filopodia[i]->time_created < 120)) count3++;
            if ((timeStep - filopodia[i]->time_created >= 120) && (timeStep - filopodia[i]->time_created < 160)) count4++;
            if ((timeStep - filopodia[i]->time_created >= 160) && (timeStep - filopodia[i]->time_created < 200)) count5++;
            if ((timeStep - filopodia[i]->time_created >= 200) && (timeStep - filopodia[i]->time_created < 240)) count6++;
        }
    }

    cout << "fils: " << filopodia.size() << "average fil lifetime = " << (float) sum / (float) filopodia.size() << endl;
    cout << count1 << "\t" << count2 << "\t" << count3 << "\t" << count4 << "\t" << count5 << "\t" << count6 << "\t" << count7 << "\t" << endl;
    dataFile << "fils: " << filopodia.size() << "average fil lifetime = " << (float) sum / (float) filopodia.size() << endl;
    dataFile << count1 << "\t" << count2 << "\t" << count3 << "\t" << count4 << "\t" << count5 << "\t" << count6 << "\t" << count7 << "\t" << endl;
}

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------

void World::evaluate_final_offset_patt(ofstream& fileTo) {

    int Xsum = 0;
    int uptoC = ECagents.size();
    int Patt = 1;
    int c, s, r, i, j;
    int count;
    int flag = 0;
    int neighCell = 0;
    EC* ecp;
    int uptoS;
    int tipCount = 0;
    int cont = 0;
    int adjSum = 0;
    int adjFlag = 0;

    //prints out 1) the size of all adjacent tip cells junctions and which cell numbers were adjacent
    //2) the stablility of the pattern found at t=2500 and total number of adjacent tip cells
    //3) which cells were tip cells.
    for (c = 0; c < uptoC; c++) {
        adjFlag = 0;
        count = 0;
        flag = 0;
        ecp = ECagents[c];
        uptoS = ecp->stableArray.size();

        //------------------------------------------------------------------
        for (s = uptoS - 1; s >= 0; s--) {

            if ((flag == 0) && (ecp->stableArray[s] == 0)) flag = 1;

            if (flag == 0) count++;
        }
        //------------------------------------------------------------------

        Xsum += count;

        if (ecp->tipCellTest() == true) {
            //cout<<"Tip: "<<ecp->VEGFRtot<<" "<<ecp->actinUsed<<" ";

            for (r = 0; r < cellNeighbourhoods[c].size(); r++) {

                if (cellNeighbourhoods[c][r]->tipCellTest() == true) {

                    //calc size of boundary with this cell.
                    ECagents[c]->characterizeActNotchBoundaries(c, r);

                    fileTo << ECagents[c]->junctionSizes[r] << "\t" << c << "\t";


                    ecp = cellNeighbourhoods[c][r];
                    for (j = 0; j < ECagents.size(); j++)
                        if (ecp == ECagents[j]) fileTo << j << "\t\t";

                    adjSum++;



                }

            }



        }
        //else cout<<"stalk "<<ecp->VEGFRtot<<" "<<ecp->actinUsed<<endl;


    }

    //cout<<patternHistory<<endl;
    fileTo << adjSum << "\t" << Xsum / (float) ECagents.size() << "\t\t";

    for (c = 0; c < uptoC; c++) {
        ecp = ECagents[c];
        if (ecp->tipCellTest() == true) fileTo << c << "\t";

    }
    fileTo << endl;
    //---------------------------------------------------------------------------------------------

}
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------

void World::evaluation(ofstream& fileTo) {

    int Xsum = 0;
    int uptoC = ECagents.size();
    int Patt = 1;
    int c, s, r;
    int count;
    int flag = 0;
    int neighCell = 0;
    EC* ecp;
    int uptoS;
    int tipCount = 0;
    int stuntedTipCount = 0;
    int LessFilsTipCount = 0;
    int cont = 0;
    int sprout = 0;
    Coordinates newCOM;
    float dist;
    float sproutStability = 0;
    float tipStability = 0;
    bool adjacent = false;
    int mutTips = 0;
    int mutSprouts = 0;

    getCellNeighbours();

    for (c = 0; c < uptoC; c++) {
        count = 0;
        flag = 0;
        ecp = ECagents[c];
        uptoS = ecp->stableArray.size();



        //for each cell, if its got over half VEGFR2...
        if (ecp->VEGFRtot > VEGFRNORM / 2.0f) {

            //then store its stability

            //------------------------------------------------------------------
            for (s = uptoS - 1; s >= 0; s--) {

                if ((flag == 0) && (ecp->stableArray[s] == 0)) flag = 1;

                if (flag == 0) count++;
            }
            //------------------------------------------------------------------
            //check its migration distance (define as sprout or tip)
            //newCOM = ecp->calcCOM();
            //dist = getDist(ecp->tipInitPos.x,ecp->tipInitPos.y, ecp->tipInitPos.z, newCOM.x, newCOM.y, newCOM.z);
            //toroidal
            //if(dist>xMAX/2.0f) dist = xMAX-dist;

            for (r = 0; r < cellNeighbourhoods[c].size(); r++) {

                if ((ecp->tipCellTest() == true) && (cellNeighbourhoods[c][r]->tipCellTest() == true)) adjacent = true; //cout<<"T "

                // else cout<<"S ";
            }
            //check is it a straight forward tip cell...
            if (ecp->actinUsed >= 257.0) {
                sprout++;
                sproutStability += count;
                if (ecp->mutant == true) mutSprouts++;
                //if(dist>5) sprout++;
                //cout<<"sproutDist: "<<dist<<endl;

            }                //or is it a stunted tip (less fils but some)
            else if (ecp->actinUsed > 0) {
                //LessFilsTipCount++;
                tipStability += count;
                //if(dist<5)
                stuntedTipCount++;
                if (ecp->mutant == true)mutTips++;
                //cout<<"stuntedDist: "<<dist<<endl;
                //cout<<"stuntedFils: "<<ecp->actinUsed<<endl;
            }




        }
    }


    if ((adjacent == false) && (stuntedTipCount + sprout == 3)) Patt = 1;
    else Patt = 0;
    //adjacent is only sprouts

    if ((adjacent == false) && (stuntedTipCount + sprout == 3)) Patt = 1;
    else Patt = 0;
    //adjacent is only sprouts

    fileTo << sprout << "\t" << stuntedTipCount + sprout << "\t" << Patt << "\t";

    if (sprout > 0) fileTo << ((sproutStability / (float) sprout) / MAXtime)*100 << "\t";
    else fileTo << "-1\t";

    if (stuntedTipCount > 0) fileTo << ((tipStability / (float) stuntedTipCount) / MAXtime)*100 << "\t";
    else fileTo << "-1\t";

    fileTo << adjacent << "\t" << mutSprouts << "\t" << mutTips << "\t";
    if (((mutSprouts > 0) || (mutTips > 0)) && ((sprout > 0) || (stuntedTipCount > 0))) fileTo << ((float) (mutSprouts + mutTips) / (float) (sprout + stuntedTipCount))*100 << endl;
    else fileTo << "-1" << endl;


}





//---------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
/**
 * 
 *Output the current total membrane grid sites and total VEGF level in world
 */
void World::calcEnvVEGFlevel(void) {

    int i, j, k;
    float sum = 0.0f;

    int count = 0;
    int countM = 0;

    for (i = 0; i < xMAX; i++) {
        for (j = 0; j < yMAX; j++) {
            for (k = 0; k < zMAX; k++) {
                if ((grid[i][j][k].type == E) && (grid[i][j][k].Eid->VEGF > 0.0f)) {
                    sum += grid[i][j][k].Eid->VEGF;
                    count++;
                }
                if ((grid[i][j][k].type == M) && (grid[i][j][k].Mids.size() >= 1)) {
                    countM++;
                }
            }
        }
    }

    cout << "VEGF total " << sum << "\t" <<count<<"gridded membrane" << countM << endl;

}
//---------------------------------------------------------------------------------------------------------

void World::output_cell_protlevels(ofstream& fileTo){

    
    
    int i,j;
    int filTokSum=0;
    EC* ecp;
    
    for(i=0;i<ECagents.size();i++){
filTokSum=0;
        ecp = ECagents[i];

//tally up filTokens  
        for(j=0;j<ecp->nodeAgents.size();j++){
filTokSum+=ecp->nodeAgents[j]->filTokens;

        }/*for(j=0;j<ecp->springAgents.size();j++){
filTokSum+=ecp->nodeAgents[j]->filTokens;

        }*/
    
        if(timeStep==0){
                cout<<"CEll: "<<i<<" VEGFRnorm: "<<ecp->VEGFRnorm<<" Vsink: "<<ecp->Vsink<<endl;
        }
        else{
                cout<<timeStep<<" CEll: "<<i<<" actinUsed "<<ecp->actinUsed<<" fiToks "<<filTokSum<<" Dll4tot:"<<ecp->Dll4tot<<" activeVR2 "<<ecp->activeVEGFRtot<<" VEGFRnorm: "<<ecp->VEGFRnorm<<" Vsink: "<<ecp->Vsink<<endl; 
        }
        
        //fileTo<<ecp->Dll4tot<<"\t"<<ecp->activeVEGFRtot<<"\t";
        
    }
    
         cout<<endl; 
         //fileTo<<endl;

}
//---------------------------------------------------------------------------------------------
void EC::store_cell_COM(void){
//centre of mass of cell position - though not toroidal
    float aveX=0;
    float aveY=0;
    float aveZ=0;
    int i;
    int count=0;
    Coordinates* COM = new Coordinates();



    int uptoN=nodeAgents.size();


    for(i=0;i<uptoN;i++){

        if(nodeAgents[i]->minusSite==NULL){
        aveX+=nodeAgents[i]->Mx;
        aveY+=nodeAgents[i]->My;
        aveZ+=nodeAgents[i]->Mz;
        count++;
        }

    }


    COM->x = aveX/(float)count;
    COM->y = aveY/(float)count;
    COM->z = aveZ/(float)count;

    COMstore.push_back(COM);
}
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
void EC::calcStability(void){
    
    // if its significantly different from current stable amount, score 0 and change stableVEGFR to current VEGFR
    if((VEGFRtot>stableVEGFR+SigRange)||(VEGFRtot<stableVEGFR-SigRange)){
        stableArray.push_back(0);
        stableVEGFR=VEGFRtot;
    }
    else stableArray.push_back(1);
    
    
}
//---------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
Coordinates EC::calcCOM_toroidal(void){
    //calculates centre of mass of cell position for cell tracking in toroidal space
    
    float aveX=0;
    float aveY=0;
    float aveZ=0;
    int i;
    Coordinates COM;
    int xstart=0;
    int xend=0;
    
    float X;
    int flag=0;
    int toroidal=0;
    //------------------------------------------------------
    //toroidal check
    int s=0;
    do{
        if(Springs[s]->filopodia==false){
            if(sqrt((Springs[s]->end->Mx-Springs[s]->start->Mx)*(Springs[s]->end->Mx-Springs[s]->start->Mx))>xMAX/2.0f){
                flag=1;
            }
        }
        s++;
    }while((s<Springs.size())&&(flag==0));
    
    //toroidal case, so work out which side of x axis majority of cell is and shift any nodes on the other side to calc proper mid point.
    if(flag==1){
        //cout<<"toroidal..";
        for(i=0;i<nodeAgents.size();i++){
            if(nodeAgents[i]->Mx<xMAX/2.0f) xstart++;
            else xend++;
            
        }
        
        if(xstart>xend){
            //cout<<"left"<<endl;
            toroidal = 1;
        }
        else if(xstart<xend){
            //cout<<"right"<<endl;
            toroidal = 2;
        }
        else{cout<<"wierd, cell seems to be stretched accross the whole x axis! oh dear..."<<endl;}
    }
    else{
        toroidal=0;
    }
//------------------------------------------------------
    //int uptoS=springAgents.size();
    int uptoN=nodeAgents.size();
    int nodesUsed=0;
    int uptoSu=surfaceAgents.size();
    
    for(i=0;i<uptoN;i++){
        if((nodeAgents[i]->FIL==BASE)||(nodeAgents[i]->FIL==NONE)){
            nodesUsed++;
            if((toroidal==1)&&(nodeAgents[i]->Mx>xMAX/2.0f)) X = nodeAgents[i]->Mx-xMAX;
            else if((toroidal==2)&&(nodeAgents[i]->Mx<xMAX/2.0f)) X = nodeAgents[i]->Mx+xMAX;
            else
                X = nodeAgents[i]->Mx;
            
            aveX+=X;
            aveY+=nodeAgents[i]->My;
            aveZ+=nodeAgents[i]->Mz;
        }
        
    }
    /*for(i=0;i<uptoS;i++){
     * aveX+=springAgents[i]->Mx;
     * aveY+=springAgents[i]->My;
     * aveZ+=springAgents[i]->Mz;
     *
     * }*/
    
    for(i=0;i<uptoSu;i++){
        
        if((toroidal==1)&&(surfaceAgents[i]->Mx>xMAX/2.0f)) X = surfaceAgents[i]->Mx-xMAX;
        else if((toroidal==2)&&(surfaceAgents[i]->Mx<xMAX/2.0f)) X = surfaceAgents[i]->Mx+xMAX;
        else X = surfaceAgents[i]->Mx;
        
        aveX+=X;
        aveY+=surfaceAgents[i]->My;
        aveZ+=surfaceAgents[i]->Mz;
        
    }
    int total =nodesUsed+uptoSu;
    
    COM.x = aveX/(float)total;
    COM.y = aveY/(float)total;
    COM.z = aveZ/(float)total;
    
    //cout<<"mid X: "<<COM.x<<endl;
    
    return(COM);
}
//------------------------------------------------------------------------------------------------
