/*
 *  differentialAdhesion.cpp
 *  
 *
 *  Created by Katie Bentley on 23/11/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "objects.h"
#include <math.h>
#include <stdlib.h>

vector <MemAgent*> deleteList;

//----------------------------------------------------------------------------------

void CPM_module::run_CPM(void) {

    float changeH;
    bool accept;
    int uptoN, upto;
    int choseReplacer;

    MemAgent* replacer_mem;
    MemAgent* replaced_mem;
    MedAgent* replacer_med;
    MedAgent* replaced_med;
    int f;

int steps;


    int MCS_STEPS = MCS;//10*(16*ECELLS);//400; how Glazier and Graner set it...

    int flag = 0;
    int flag2=0;
    int i, j, k,pos;
    int chose;
    int choseReplace;
    int count;
    int counter=0;
    bool biasAccept;
    bool moved;

    //cout<<"starting"<<endl;
    if (worldP->timeStep == 0) {

        //createMedium();
        Temp = 10;
        calc_Cell_areas();


    } else{

        //if (worldP->timeStep > 200) Temp = 15;
        //if(worldP->timeStep>600) Temp = 10;
        for (steps = 0; steps < MCS_STEPS; steps++) {
            //cout<<"Step "<<steps<<endl;

            //pick random memagent to be overwritten by neighbour state..
            //go through all memagents randomised order, pick first one
            //set all agents list---------------------------------------------------------------------------------------------------------------------------------------------------------
            /*for (i = 0; i < ECELLS; i++) {
			
                    uptoN = worldP->ECagents[i]->nodeAgents.size();
			
                    for (j = 0; j < uptoN; j++) worldP->ALLmemAgents.push_back(worldP->ECagents[i]->nodeAgents[j]);
				
            }
                            //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		
            //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //reorder agents randomly
            random_shuffle(worldP->ALLmemAgents.begin(), worldP->ALLmemAgents.end());*/
            //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            //add medium agents to all agents list..

            upto = worldP->JunctionAgents.size();
               flag = 0;
            replaced_med = NULL;
            replaced_mem = NULL;
            replacer_mem = NULL;
            replacer_med = NULL;
            do {


                //chose = rand() % upto;
                chose = new_rand() % upto;
/*upto = worldP->ALLmemAgents.size();// + mediumAgents.size();

            flag = 0;
            replaced_med = NULL;
            replaced_mem = NULL;
            replacer_mem = NULL;
            replacer_med = NULL;
 *
 ***********make sure uncomment this do while loop as soon as not debugging!!!!!!!
            do {


                chose = rand() % upto;

                if (chose < worldP->ALLmemAgents.size()) {

                    worldP->ALLmemAgents[chose]->checkNeighs();


                    if ((worldP->ALLmemAgents[chose]->DiffAd_neighs.size() != 0)){ //|| (worldP->ALLmemAgents[chose]->mediumNeighs!= 0)) {
                        //for (j = 0; j < worldP->ALLmemAgents[chose]->DiffAd_neighs.size(); j++) {

                            choseReplacer = rand() % (int)(worldP->ALLmemAgents[chose]->DiffAd_neighs.size());// + worldP->ALLmemAgents[chose]->mediumNeighs);


                        if (choseReplacer < worldP->ALLmemAgents[chose]->DiffAd_neighs.size()) {


							//make sure it has no filoodia coming from it (for future vessel version...)
                            if ((worldP->ALLmemAgents[chose]->node == true)&&(worldP->ALLmemAgents[chose]->FIL==NONE)) {
                                if (worldP->ALLmemAgents[chose]->DiffAd_neighs[choseReplacer]->node == true) {
                                    flag = 1;
                                    replacer_mem = worldP->ALLmemAgents[chose]->DiffAd_neighs[choseReplacer];
                                    replaced_mem = worldP->ALLmemAgents[chose];
                                    pos = chose;
                                    //replaced->diffAd_replaced = replacer->Cell;
                                    //replaced->labelled = true;

                                }
                            }
                        } else {
                            if(worldP->ALLmemAgents[chose]->mediumNeighs==0) cout<<"no medium neighs!"<<endl;
                            //cout<<"medium Neighs "<<worldP->ALLmemAgents[chose]->mediumNeighs<<endl;
                            replacer_med = mediumAgents[0];
                            replaced_mem = worldP->ALLmemAgents[chose];
                            pos = chose;
                            flag = 1;

                        }
                    }
                } else{
                    cout<<"med some how "<<endl;
                    replaced_med = mediumAgents[chose - worldP->ALLmemAgents.size()];

                    replaced_med->checkNeighs();

                    if(mediumAgents[chose - worldP->ALLmemAgents.size()]->DiffAd_neighs.size() != 0) {


                    choseReplacer = rand() % mediumAgents[chose - worldP->ALLmemAgents.size()]->DiffAd_neighs.size();
                    replacer_mem = mediumAgents[chose - worldP->ALLmemAgents.size()]->DiffAd_neighs[choseReplacer];
                    pos = chose - worldP->ALLmemAgents.size();
                    flag = 1;
                    }
                }

            } while (flag == 0);*/

                    worldP->JunctionAgents[chose]->checkNeighs(true);
                    

               
                        //for (j = 0; j < worldP->ALLmemAgents[chose]->DiffAd_neighs.size(); j++) {

                        //find how many junction springs it has, then pick one to be the replacer on the other end
                    //if it has a filopodia, try to find a neighbour node in the same cell to transfer the fil to
                    //so it can be replaced - this allows the final stage of overtaking to occur!!
                    

                    if(((worldP->JunctionAgents[chose]->FIL==BASE)||(worldP->JunctionAgents[chose]->FIL==NONE))){
                        

                        count=0;
                        flag2=0;
                        for(k=0;k<worldP->JunctionAgents[chose]->neighs;k++){
                            //cout<<worldP->JunctionAgents[chose]->SpringNeigh[k]->Junction<<endl;
                            //cout<<worldP->JunctionAgents[chose]->FIL<<" "<<worldP->JunctionAgents[chose]->neigh[k]<<" "<<worldP->JunctionAgents[chose]->SpringNeigh[k]<<endl;
                            if(worldP->JunctionAgents[chose]->SpringNeigh[k]!=NULL)
                            if(worldP->JunctionAgents[chose]->SpringNeigh[k]->Junction==true){ count++; flag2=1;}
                            //else count++;
                        }
                        if(flag2==1){
                        if(count!=1)
                        //choseReplacer = rand()%count;
                        choseReplacer = new_rand()%count;
                        else choseReplacer=1;
                            
                        count=1;
                        for(k=0;k<worldP->JunctionAgents[chose]->neighs;k++){
                          
                                if(worldP->JunctionAgents[chose]->SpringNeigh[k]!=NULL){
                                if(worldP->JunctionAgents[chose]->SpringNeigh[k]->Junction==true){
                                if(count==choseReplacer){
                                replacer_mem = worldP->JunctionAgents[chose]->neigh[k];
                                replaced_mem = worldP->JunctionAgents[chose];
                                    pos = chose;
                                    flag=1;
                                }
                                else count++;
                            }
                                }
                            
                           
                        }

					
                               
                            
                        }
                    }
                    

                    

            } while (flag == 0);

            if(flag==1){
            //for(i=0;i<upto;i++) worldP->ALLmemAgents[i]->checkNeighs();
                //cout<<"*";
            changeH = calc_local_change(replacer_mem, replaced_mem, replacer_med, replaced_med);

            //cout<<"changeH "<<changeH<<endl;
            //store sum of each pairs J value..
            //calc_J_sum();
            //cout<<"3"<<endl;

            //store sum of differences in area from ideal for each cell
            //calc_area_sum();

            //calc new H value

            //changeH = Hamiltonian_change();
            //cout<<"4"<<endl;

            //calc probability of accepting change - compare to old H
            accept = calcProb(changeH);

            if(worldP->timeStep>100){
            if(biased_mig_diffAd==true){


                biasAccept = check_gradient(replaced_mem, replacer_mem);
                if(biasAccept==true){ 
                    
                    //float prob = (float)rand()/(float)RAND_MAX;
                    float prob = (float)new_rand()/(float)RAND_MAX;
                    if(prob<BIAS_DIFFAD_CHANCE)
                    accept = true;
                }

                //cout<<"biased"<<endl;
                
            }
            }
			

            
            //cout<<"calcPorb"<<endl;
            //if accept, replace, and replace springs(can do without springs for now? just gridded agent version..)
            if (accept == true) {
                //actually replace
                //cout << "replacing" << endl;
                 //cout<<"replace "<<replacer_mem<<" "<<replaced_mem<<" "<<replacer_med<<" "<<replaced_med<<endl;

                if(worldP->JunctionAgents[chose]->FIL==BASE){
                    //cout<<"_";
                        moved = move_fil_base(worldP->JunctionAgents[chose]);
                    }
                else moved = true;

                if(moved==true){
                    // cout<<"-";
                    replace_agent(replacer_mem, replaced_mem, replacer_med, replaced_med, pos);
                }
                else{

                    if (replaced_med != NULL)replaced_med->diffAd_replaced = NULL;
                if (replaced_mem != NULL) {
                    replaced_mem->diffAd_replaced_cell = NULL;
                    replaced_mem->diffAd_replaced_med = NULL;
                }
                }

                //WORLDpointer->Pause=true;
                //cout<<"replaced"<<endl;
                //Hamiltonian = new_Hamiltonian;
            } else {
                //cout<<"not replacing!"<<endl;
                if (replaced_med != NULL)replaced_med->diffAd_replaced = NULL;
                if (replaced_mem != NULL) {
                    replaced_mem->diffAd_replaced_cell = NULL;
                    replaced_mem->diffAd_replaced_med = NULL;
                }
                //cout<<"not replaced"<<endl;
                //replaced->labelled = false;
            }
        }
        
        
    }
    }

    //counld do this only every few timesteps..
    clearUpSmallSeparatedBitsOfCells();
    /*for(i=0;i<deleteList.size();i++)
        delete deleteList[i];

    deleteList.clear();*/
    //worldP->ALLmemAgents.clear();
}
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------

void CPM_module::calc_J_sum(void) {

/*    int i, j, k;

    EC* ecp;
    MemAgent* mp;

    J_sum = 0;
    //float start;
    //float DiffSum2=0.0;
    //go through all memAgents and store which type of J they classify as for each of its neighbours. CAN OPTIMISE THIS FURTHER, IS DOUBLING UP CALCS..
    int upto = worldP->ALLmemAgents.size();

    for (i = 0; i < worldP->ALLmemAgents.size(); i++) {
        mp = worldP->ALLmemAgents[i];

        if (mp->diffAd_replaced != NULL) {
            //cout << "J1 " << J_sum << endl;
            for (j = 0; j < mp->DiffAd_neighs.size(); j++) {
                //cout << j << endl;
                //decide types
                if (mp->diffAd_replaced->VEGFRtot == 0) {
                    if (mp->DiffAd_neighs[j]->Cell->ODE.Variables[VR2] == 0) {
                        J_sum += J_SS;

                        //DiffSum2+=J_SS;
                    } else {
                        J_sum += J_TS;
                        //DiffSum2+=J_TS;

                    }
                } else {
                    if (mp->DiffAd_neighs[j]->Cell->ODE.Variables[VR2] == 0) {
                        J_sum += J_TS;
                        //DiffSum2+=J_TS;

                    } else {
                        J_sum += J_TT;
                        //DiffSum2+=J_TT;

                    }

                }
            }
            //cout << J_sum << endl;
        } else {
            for (j = 0; j < mp->DiffAd_neighs.size(); j++) {
                //cout<<j<<endl;
                if (mp->DiffAd_neighs[j]->diffAd_replaced != NULL) {
                    //cout<<"J2"<<endl;
                    //start = J_sum;
                    //cout << "trying " << J_sum << endl;
                    //decide types
                    if (mp->DiffAd_neighs[j]->diffAd_replaced->ODE.Variables[VR2] == 0) {
                        if (mp->DiffAd_neighs[j]->diffAd_replaced->ODE.Variables[VR2] == 0) {
                            J_sum += J_SS;
                        } else {
                            J_sum += J_TS;
                        }
                    } else {
                        if (mp->DiffAd_neighs[j]->diffAd_replaced->ODE.Variables[VR2] == 0) {
                            J_sum += J_TS;
                        } else {
                            J_sum += J_TT;
                        }

                    }
                    //cout << J_sum << endl;
                } else {

                    //decide types
                    if (mp->DiffAd_neighs[j]->Cell->ODE.Variables[VR2] == 0) {
                        if (mp->DiffAd_neighs[j]->Cell->ODE.Variables[VR2] == 0) {
                            J_sum += J_SS;
                        } else {
                            J_sum += J_TS;
                        }
                    } else {
                        if (mp->DiffAd_neighs[j]->Cell->ODE.Variables[VR2] == 0) {
                            J_sum += J_TS;
                        } else {
                            J_sum += J_TT;
                        }

                    }

                }
            }


        }
    }
    //cout<<"diffSum: "<<DiffSum<<endl;*/

}

//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------

void CPM_module::calc_area_sum(void) {

    int i, j, k;
    EC* ecp;
    //for now define cell area as simply no. of memAgents.. will prob need to redefine for vessel version.. just von neigh agents.. or actual grid sites occupied, once agents bunch up..
    area_sum = 0;
    int Q;
    for (i = 0; i < worldP->ECagents.size(); i++) {
        Q = 0; //rand()%(ECwidth*2);
        ecp = worldP->ECagents[i];
        area_sum += (((ecp->nodeAgents.size() + ecp->surfaceAgents.size()) - (ecp->ideal_Cell_area - ((float) ECwidth / 2.0f) + Q))*((ecp->nodeAgents.size() + ecp->surfaceAgents.size()) - (ideal_Area - ((float) ECwidth / 2.0f) + Q)));

    }

}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

void CPM_module::calc_Cell_areas(void) {

    int i, Q;
    int var = ECwidth * 2;

    for (i = 0; i < ECELLS; i++) {
        //Q = rand() % (var);
        Q = new_rand() % (var);
        worldP->ECagents[i]->ideal_Cell_area = ideal_Area;// - ((float) var / 2.0f) + Q;

    }

}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

float CPM_module::Hamiltonian_change(void) {

    float change;

    new_Hamiltonian = J_sum + area_sum;


    change = new_Hamiltonian - Hamiltonian;

    //cout << Hamiltonian << " " << new_Hamiltonian << " " << change << endl;

    // cout<<"Ham = "<<new_Hamiltonian<<" "<<change<<endl;

    return (change);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

bool CPM_module::calcProb(float change) {

    //float chose = (float) rand() / (float) RAND_MAX;
    float chose = (float) new_rand() / (float) NEW_RAND_MAX;
    float prob;
    bool accept;

    if (change < 0) accept = true;
    else if (change == 0) {
        if (chose < 0.5) accept = true;
        else accept = false;
    } else {
        prob = exp(-change / (float) Temp);
        //cout<<"exp: "<<prob<<endl;
        if (chose < prob) {
            accept = true;

        } else accept = false;
    }
    //else accept = false;


    return (accept);
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

void CPM_module::replace_agent(MemAgent* replacer_mem, MemAgent* replaced_mem, MedAgent* replacer_med, MedAgent* replaced_med, int pos) {

    
    
    int flag = 0;
    int i = 0;
    int m;
    vector<MemAgent*>::iterator L;
    vector<MemAgent*>::iterator Q;
    vector<MemAgent*>::iterator L2;
    vector<MedAgent*>::iterator T;
    float X, Y, Z;
    if(replaced_mem!=NULL){
    X = replaced_mem->Mx;
    Y = replaced_mem->My;
    Z = replaced_mem->Mz;
    }
    else{
    X = replaced_med->Mx;
    Y = replaced_med->My;
    Z = replaced_med->Mz;
    }

    MemAgent* memp  = NULL;
    MedAgent* medp;

    //delete old memagent if a memagent is being replaced.
    if((replaced_mem!=NULL)){
        //cout<<"mem"<<endl;
        i=0;
    //remove memagent from cell list to be replaced
    do {

        if (replaced_mem->Cell->nodeAgents[i] == replaced_mem) {
            flag = 1;
            L = replaced_mem->Cell->nodeAgents.begin();
            replaced_mem->Cell->nodeAgents.erase(L + i);
        }
        i++;

    } while ((i < (int) replaced_mem->Cell->nodeAgents.size()) && (flag == 0));
    if(flag==0) cout<<"hasnt found node in list to remove"<<endl;

    /*i=0;
    flag=0;
    do {

        if (replaced_mem->Cell->nodeAgents[i] == replaced_mem) {
            flag = 1;
            L = replaced_mem->Cell->nodeAgents.begin();
            replaced_mem->Cell->nodeAgents.erase(L + i);
        }
        i++;

    } while ((i < (int) replaced_mem->Cell->nodeAgents.size()) && (flag == 0));
    if(flag==0){// cout<<"def isnt in list twice"<<endl;
    }
    else cout<<"must appear twice!"<<endl;
    */

    i=0;
    flag=0;
    do {

        if (worldP->ALLmemAgents[i] == replaced_mem) {
            flag = 1;
            L2 = worldP->ALLmemAgents.begin();
            worldP->ALLmemAgents.erase(L2 + i);
        }
        i++;

    } while ((i < (int) worldP->ALLmemAgents.size()) && (flag == 0));

    Q = worldP->JunctionAgents.begin();

    //int count=0;
    //for(i=0;i<worldP->JunctionAgents.size();i++) if(worldP->JunctionAgents[i]==replaced_mem) count++;

    //if(count!=1) cout<<"appears more than once!"<<endl;

    worldP->JunctionAgents.erase(Q+pos);
    
    //remove from grid
    worldP->deleteOldGridRef(replaced_mem, false);
    //delete agent
    
    //just add to delete list and flag it delete all at end of MCS
    //deleteList.push_back(replaced_mem);
    //replaced_mem->diffAd_deleteFlag = true;

    }
    //erase old medium agent
    else{
        i=0;
        //cout<<"med"<<endl;
        
            T = mediumAgents.begin();
            mediumAgents.erase(T + pos);
            //cout<<"found"<<endl;
        
       
    //remove from grid
    worldP->grid[(int)replaced_med->Mx][(int)replaced_med->My][(int)replaced_med->Mz].med = NULL;
    //delete agent
        //cout<<"deleted"<<endl;
    }

    if(replacer_mem!=NULL){
        //cout<<"mem replace"<<endl;
    //create new agent in same pos, belonging to other cell..
    memp = new MemAgent(replacer_mem->Cell, worldP);
    //cout<<"newNode:"<<memp<<endl<<endl;

    //cout<<X<<" "<<Y<<" "<<Z<<endl;
    memp->Mx = X;
    memp->My = Y;
    memp->Mz = Z;
    memp->mesh3SpringsOnly=replaced_mem->mesh3SpringsOnly;
    memp->labelled = replaced_mem->labelled;
    memp->node = true;
    memp->FA = replaced_mem->FA;
    replacer_mem->Cell->nodeAgents.push_back(memp);
    worldP->setMLocation((int) X, (int) Y, (int) Z, memp);
    //memp->labelled = true;
    memp->JunctionTest(true);
    //cout<<"set"<<endl;
    
    worldP->ALLmemAgents.push_back(memp);


   // cout<<"replacing with mem"<<endl;
    //cout<<"replaced"<<endl;//will need to do something with springs...
    }
    else{

        cout<<"replacing with med"<<endl;
        //create new agent in same pos, belonging to other cell..
    medp = new MedAgent( worldP);
    //cout<<"newNode:"<<memp<<endl<<endl;

    medp->Mx = X;
    medp->My = Y;
    medp->Mz = Z;


    mediumAgents.push_back(medp);
    worldP->grid[(int) X][(int) Y][(int) Z].med = medp;
    worldP->grid[(int) X][(int) Y][(int) Z].type = MED;
    
    }

    update_displaced_surfaceAgents(replaced_mem, replacer_mem);
    replaceSprings(replaced_mem, memp, replacer_med, replaced_med);

    

    if(replaced_mem!=NULL){
		
        //replaced_mem->labelled2=true;
		delete replaced_mem;
	}

}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void CPM_module::replaceSprings(MemAgent* replaced_mem, MemAgent* replacer_mem, MedAgent* replacer_med, MedAgent* replaced_med){

	int i,j,k,m;
	
	//list of agents connected to is the replaced mems neigh] array
	vector <bool>  horizontals;
    vector <bool>  rights;
	vector <Spring*>::iterator S;
        vector <Spring*>::iterator IT;
	int flag;
	
	//cout<<"starting..."<<replaced_mem<<" "<<replacer_mem<<" "<<replacer_med<<endl;

        if(replaced_mem!=NULL){
        //if((replaced_mem->labelled==true)&&(replacer_mem==NULL)&&(replacer_med!=NULL)) cout<<"medium replacer!!!"<<endl;
        //else if(replaced_mem->labelled==true)cout<<"just replacer labelled first"<<endl;
	
	//first go through this agent to be replaceds springs, store details of hor and right in list and also address of agent connected to. 
	for(i=0;i<replaced_mem->neighs;i++){
		//cout<<i<<"springneigh"<<endl;
		horizontals.push_back(replaced_mem->SpringNeigh[i]->horizontal);
		rights.push_back(replaced_mem->SpringNeigh[i]->right);
		
		//then go through stored ones and remove from spring list in cell
		
			S = replaced_mem->Cell->Springs.begin();
			j=0;
		flag=0;
			do{
				if(replaced_mem->Cell->Springs[j]==replaced_mem->SpringNeigh[i]) {
				replaced_mem->Cell->Springs.erase(S+j);
					flag=1;
					//cout<<"found"<<endl;
				}
				j++;
				
			}while((flag==0)&&(j<replaced_mem->Cell->Springs.size()));
                        if(flag==0){
                            j=0;
                            flag=0;
                            for(k=0;k<worldP->ECagents.size();k++){
                                for(m=0;m<worldP->ECagents[k]->Springs.size();m++)
				if(worldP->ECagents[k]->Springs[m]==replaced_mem->SpringNeigh[i]) {
				//replaced_mem->Cell->Springs.erase(S+j);
					flag=1;
                                        IT = worldP->ECagents[k]->Springs.begin();
                                        worldP->ECagents[k]->Springs.erase(IT+m);
					cout<<"yes you rock!!!"<<endl;
				}
                            }

			
                            cout<<"cant find in list "<<replaced_mem->SpringNeigh[i]<<" "<<i<<" "<<replaced_mem->neighs<<" "<<replaced_mem->SpringNeigh[i]->filopodia<<" "<<replaced_mem->SpringNeigh[i]->Junction<<endl;
                            int kate = 0;
                        }
		
                        if(flag==1)
		delete replaced_mem->SpringNeigh[i];

	}
	
	//cout<<"1"<<endl;
	
	//-----
	if(replacer_mem!=NULL){
		//next go through saved agents and find springs that were connected to this agent, replace their end agent in spring object and neigh ref with new guy
		
		for(i=0;i<replaced_mem->neighs;i++){
			for(j=0;j<replaced_mem->neigh[i]->neighs;j++){
				if(replaced_mem->neigh[i]->neigh[j]==replaced_mem){
                                    replaced_mem->neigh[i]->neigh[j] = replacer_mem;
					replaced_mem->neigh[i]->SpringNeigh[j]->end = replacer_mem;
					//change junction status if necessary
					if(replacer_mem->Cell!=replaced_mem->neigh[i]->Cell) replaced_mem->neigh[i]->SpringNeigh[j]->Junction = true;
					else replaced_mem->neigh[i]->SpringNeigh[j]->Junction = false;
				}
			}
		}
		//create new springs out from new guy to all the stored list. 
		//cout<<"2"<<endl;
		for(i=0;i<replaced_mem->neighs;i++){
			replacer_mem->Cell->createSpringTokenObject(replacer_mem, replaced_mem->neigh[i], i);
			replacer_mem->neighs++;
			replacer_mem->neigh[i] = replaced_mem->neigh[i];
			//set hor and right from list.
			replacer_mem->SpringNeigh[i]->horizontal = horizontals[i];
			replacer_mem->SpringNeigh[i]->right = rights[i];
			
		}
		
		//cout<<"3"<<endl;
		
		//TO DO assign any spring agents to new springs/and or surface agents
		
		
		
	}
        else{
            cout<<"arrrg! shouldnt be in here!"<<endl;
	//TO DO delete springs from neighs if replacing with medium...

            //need to also delete any springs from neigh to replaced mem, no longer needed
            for(i=0;i<replaced_mem->neighs;i++){
			for(j=0;j<replaced_mem->neigh[i]->neighs;j++){
				if(replaced_mem->neigh[i]->neigh[j]==replaced_mem){
                                    S = replaced_mem->neigh[i]->Cell->Springs.begin();
			k=0;
		flag=0;
			do{
				if(replaced_mem->neigh[i]->Cell->Springs[k]==replaced_mem->neigh[i]->SpringNeigh[j]) {
				replaced_mem->neigh[i]->Cell->Springs.erase(S+k);
					flag=1;
					//cout<<"found"<<endl;
				}
				k++;

			}while((flag==0)&&(j<replaced_mem->neigh[i]->Cell->Springs.size()));



		delete replaced_mem->neigh[i]->SpringNeigh[j];
                                }
                        }
            }


        }

        }
        //for replacing a medium agent with a memagent
        else if((replaced_med)&&(replacer_mem!=NULL)){

            //look around and connect to any neighs nearby with springs..
            //memp->connectJunctions(true);

            //for each one create a new spring back out to it..
            //for(i=0;i<memp->neighs;i++){
            //
            //}

        }
	
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

float CPM_module::calc_local_change(MemAgent* replacer_mem, MemAgent* replaced_mem, MedAgent* replacer_med, MedAgent* replaced_med) {


    float old_Jsum, oldAsum, oldPsum;
    float new_Jsum, newAsum, newPsum;
    EC* ecp;
    EC* ecp2;
    int i;
    int oldPsumC, newPsumC, oldPsumD, newPsumD;
    bool Do1 = false;
    bool Do2 = false;
    int first, second;
    int conec_score;
    int area =0;
    int area2=0;
    //calc local Jsum value before change, and cell size area values, store

    //cout<<replacer_mem<<" "<<replaced_mem<<" "<<replacer_med<<" "<<replaced_med<<endl;
    old_Jsum = calcLocal_Jsum(replaced_mem, replaced_med);

    //perimeterCheck----------------------------------------------------
    /*if ((replaced_mem!= NULL)&&(replacer_mem!=NULL)) {
        oldPsumC = 0;
        newPsumC=0;
        oldPsumD = 0;
        newPsumD=0;
        ecp = replaced_mem->Cell;
        ecp2 = replacer_mem->Cell;

        if( ecp->ODE.Variables[VR2] == 0) Do1 = true;
        if( ecp2->ODE.Variables[VR2] == 0) Do2 = true;

        if(Do1==true){
        for(i=0;i<ecp->nodeAgents.size();i++){
            if(ecp->nodeAgents[i]->junction==true) oldPsumC++;
        }

        first = (oldPsumC - ideal_Perimeter)*(oldPsumC - ideal_Perimeter);
        }
        else first = 0;

        if(Do2==true){
        for(i=0;i<ecp2->nodeAgents.size();i++){
            if(ecp2->nodeAgents[i]->junction==true) oldPsumD++;
        }
        second = (oldPsumD - ideal_Perimeter)*(oldPsumD - ideal_Perimeter);
        }
        else second = 0;

        oldPsum = (first)+(second);

        replaced_mem->Cell = ecp2;

        if(Do1==true){
        for(i=0;i<ecp->nodeAgents.size();i++){
            ecp->nodeAgents[i]->JunctionTest(false);
            if(ecp->nodeAgents[i]->junction==true) newPsumC++;
        }
        first = (newPsumC - ideal_Perimeter)*(newPsumC - ideal_Perimeter);
        }
        else first = 0;

        if(Do2==true){
        for(i=0;i<ecp2->nodeAgents.size();i++){
            ecp2->nodeAgents[i]->JunctionTest(false);
            if(ecp2->nodeAgents[i]->junction==true) newPsumD++;
        }
        second = (newPsumD - ideal_Perimeter)*(newPsumD - ideal_Perimeter);
        }
        else second = 0;

        newPsum = (first)+(second);

        replaced_mem->Cell = ecp;

        if(Do1==true){
        for(i=0;i<ecp->nodeAgents.size();i++){
            ecp->nodeAgents[i]->JunctionTest(false);

        }
        }
        if(Do2==true){
        for(i=0;i<ecp2->nodeAgents.size();i++){
            ecp2->nodeAgents[i]->JunctionTest(false);

        }
        }

    }*/

    //-------------------------------------------------------------------
    conec_score = connectivity(replaced_mem, replacer_mem);
    //cout<<"old J"<<old_Jsum<<endl;

    if (replaced_mem != NULL) {

        ecp = replaced_mem->Cell;
        //if(ecp->VEGFRtot<(float)VEGFRnorm/2.0f){
        for(i=0;i<ecp->nodeAgents.size();i++){
            if((ecp->nodeAgents[i]->FIL!=TIP)&&(ecp->nodeAgents[i]->FIL!=STALK)) area++;
        }
        //cout<<"mem replaced area: "<<ecp->nodeAgents.size() + ecp->surfaceAgents.size() <<endl;
            oldAsum = ((area - ecp->ideal_Cell_area)*(area  - ecp->ideal_Cell_area));

        //}

        if (replacer_mem != NULL) {
            ecp = replacer_mem->Cell;
          //   if(ecp->VEGFRtot<(float)VEGFRnorm/2.0f){
            for(i=0;i<ecp->nodeAgents.size();i++){
            if((ecp->nodeAgents[i]->FIL!=TIP)&&(ecp->nodeAgents[i]->FIL!=STALK)) area2++;
        }
            //cout<<"mem replacer area: "<<ecp->nodeAgents.size() + ecp->surfaceAgents.size() <<endl;
            oldAsum += ((area2  - ecp->ideal_Cell_area)*(area2  - ecp->ideal_Cell_area));
        }
        //}
        
            //calc with change
     } 
    //if medium guy is to be assesed
    else {

        ecp = replacer_mem->Cell;
        //cout<<"medium overtaken, mem replacer area: "<<ecp->nodeAgents.size() + ecp->surfaceAgents.size() <<endl;
            oldAsum = ((ecp->nodeAgents.size() + ecp->surfaceAgents.size() - ecp->ideal_Cell_area)*(ecp->nodeAgents.size() + ecp->surfaceAgents.size() - ecp->ideal_Cell_area));
     }

    //cout<<"oldAsum "<<oldAsum<<endl;
    //now replace temporarily
    if(replaced_mem!=NULL){
        if(replacer_mem!=NULL) replaced_mem->diffAd_replaced_cell = replacer_mem->Cell;
        else  replaced_mem->diffAd_replaced_med = replacer_med;
    }
    else{
        replaced_med->diffAd_replaced = replacer_mem;
    }
        //replaced->labelled = true;


        new_Jsum = calcLocal_Jsum(replaced_mem, replaced_med);

        //cout<<"new Jsum "<<new_Jsum<<endl;
        if (replaced_mem != NULL) {

            ecp = replaced_mem->Cell;
           //  if(ecp->VEGFRtot<(float)VEGFRnorm/2.0f){
            newAsum = ((area -1 - ecp->ideal_Cell_area)*(area-1 - ecp->ideal_Cell_area));
            // }

        if (replacer_mem != NULL) {
            ecp = replacer_mem->Cell;
           //  if(ecp->VEGFRtot<(float)VEGFRnorm/2.0f){
            newAsum += ((area2 +1 - ecp->ideal_Cell_area)*(area2 +1  - ecp->ideal_Cell_area));
           //  }
        }
         //calc with change
     }
    //if medium guy is to be assesed
    else {

        ecp = replacer_mem->Cell;
            newAsum = ((ecp->nodeAgents.size() + ecp->surfaceAgents.size()+1 - ecp->ideal_Cell_area)*(ecp->nodeAgents.size() + ecp->surfaceAgents.size()+1 - ecp->ideal_Cell_area));
     }
       

        //cout<<"new Asum "<<newAsum<<endl;
        //return difference
        return ((new_Jsum + newAsum)-(old_Jsum + oldAsum));
        //return ((newAsum)-(oldAsum));
        //return ((new_Jsum)-(old_Jsum));

    }
    //----------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------

    float CPM_module::calcLocal_Jsum(MemAgent * replaced_mem, MedAgent * replaced_med) {

        int x, m, n, p, zed;
        int i, j, k;
        bool connected;
        if(replaced_mem!=NULL){
        i = (int) replaced_mem->Mx;
        j = (int) replaced_mem->My;
        k = (int) replaced_mem->Mz;
        }
        else if(replaced_med!=NULL){
        i = (int) replaced_med->Mx;
        j = (int) replaced_med->My;
        k = (int) replaced_med->Mz;
        }
        float JsumLocal = 0.0f;

        //cout<<i<<" "<<j<<" "<<k<<endl;

        //--------------

        //--------------

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
            }//layer below
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
            }//layer above
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
            p = k + 1;
        } else {
            m = i;
            n = j;
            p = k;
        }


        //-------------------------------
        //toroidal only X
        /*if (m >= xMAX) m = 0;
        if (m < 0) m = xMAX - 1;

        if (n >= yMAX) n = 0;
        if (n < 0) n = yMAX - 1;*/

        if(worldP->insideWorld(m,n,p)==true){
        if (worldP->grid[m][n][p].type == M) {
            //for each neighbour around the one to be replaced, and the guy himself, calc Jsum values and save...
            for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {
                if ((worldP->grid[m][n][p].Mids[zed]->FIL != TIP) && (worldP->grid[m][n][p].Mids[zed]->FIL != STALK)) {
                    connected = replaced_mem->meshConnected(worldP->grid[m][n][p].Mids[zed]);
                    if (connected == true) {
                        worldP->grid[m][n][p].Mids[zed]->checkNeighs(true);
                        //cout<<worldP->grid[m][n][p].Mids[zed]->DiffAd_neighs.size()<<endl;
                        //so for this guy, checkNeighs, then put through Jsum calc..
                        JsumLocal += calc_Jsum_individual(worldP->grid[m][n][p].Mids[zed], NULL);
                    }
                }

            }
        } else if (worldP->grid[m][n][p].type == MED) { //for each neighbour around the one to be replaced, and the guy himself, calc Jsum values and save...

            worldP->grid[m][n][p].med->checkNeighs();
            //cout<<worldP->grid[m][n][p].Mids[zed]->DiffAd_neighs.size()<<endl;
            //so for this guy, checkNeighs, then put through Jsum calc..
            //cout<<m<<" "<<n<<" "<<p<<endl;
            JsumLocal += calc_Jsum_individual(NULL, worldP->grid[m][n][p].med);


        }
        }



    }

    return (JsumLocal);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

float CPM_module::calc_Jsum_individual(MemAgent * individual, MedAgent* medInd) {
	int i, j, k;
    MemAgent* mp = individual;
    float J_sumL = 0;
	
    //medium interaction, simple....
    //J_sumL+=mediumNeighs*
	
	
    if (mp != NULL) {
        //cout<<"mem: "<<mp->mediumNeighs<<" "<<mp->DiffAd_neighs.size()<<endl;
		
        if (mp->diffAd_replaced_cell != NULL) {
            //cout << "J1 " << J_sum << endl;
            J_sumL += mp->mediumNeighs*J_MC;
			
            for (j = 0; j < mp->DiffAd_neighs.size(); j++) {
				
                //decide types
                if (mp->diffAd_replaced_cell->activeVEGFRtot <  M1_neta) {
                    if (mp->DiffAd_neighs[j]->Cell->activeVEGFRtot < M1_neta) {
                        J_sumL += J_SS;
						
                        //cout<<"|"<<endl;
                        //DiffSum2+=J_SS;
                    } else {
                        J_sumL += J_TS;
                        //cout<<"_"<<endl;
                        //DiffSum2+=J_TS;
						
                    }
                } else {
                    if (mp->DiffAd_neighs[j]->Cell->activeVEGFRtot< M1_neta) {
                        J_sumL += J_TS;
                        //cout<<"_"<<endl;
                        //DiffSum2+=J_TS;
						
                    } else {
                        J_sumL += J_TT;
                        //DiffSum2+=J_TT;
                        //cout<<"X "<<endl;
						
                    }
					
                }
            }
			
        } else if (mp->diffAd_replaced_med != NULL) {
            J_sumL += mp->DiffAd_neighs.size() * J_MC;
			
        } else {
            
            J_sumL += mp->mediumNeighs*J_MC;
			
            for (j = 0; j < mp->DiffAd_neighs.size(); j++) {
                //cout<<j<<endl;
                if (mp->DiffAd_neighs[j]->diffAd_replaced_cell != NULL) {
                    //cout<<"J2"<<endl;
                    //start = J_sum;
                    //cout << "trying " << J_sum << endl;
                    //decide types
                    if (mp->Cell->activeVEGFRtot < M1_neta) {
                        if (mp->DiffAd_neighs[j]->diffAd_replaced_cell->activeVEGFRtot < M1_neta) {
                            J_sumL += J_SS;
                        } else {
                            J_sumL += J_TS;
                        }
                    } else {
                        if (mp->DiffAd_neighs[j]->diffAd_replaced_cell->activeVEGFRtot< M1_neta) {
                            J_sumL += J_TS;
                        } else {
                            J_sumL += J_TT;
							
                        }
						
                    }
                    //cout << J_sum << endl;
                } else if (mp->DiffAd_neighs[j]->diffAd_replaced_med != NULL) {
                    J_sumL += J_MC;
					
                } else {
					
                    //decide types
                    if (mp->Cell->activeVEGFRtot < M1_neta) {
                        if (mp->DiffAd_neighs[j]->Cell->activeVEGFRtot< M1_neta) {
                            J_sumL += J_SS;
							
                        } else {
                            J_sumL += J_TS;
							
                        }
                    } else {
                        if (mp->DiffAd_neighs[j]->Cell->activeVEGFRtot < M1_neta) {
                            J_sumL += J_TS;
							
                        } else {
                            J_sumL += J_TT;
							
							
                        }
						
                    }
					
                }
            }
			
        }
    } else {
        //cout<<"med: "<<medInd->DiffAd_neighs.size()<<endl;
        if (medInd->diffAd_replaced == NULL)J_sumL += medInd->DiffAd_neighs.size() * J_MC;
        else J_sumL += medInd->mediumNeighs*J_MC;
		
    }
	
	
	return (J_sumL);
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void MedAgent::checkNeighs(void) {
    int x, m, n, p, zed;
    int i, j, k;
    i = (int) Mx;
    j = (int) My;
    k = (int) Mz;


    //MneighCount = 0;
    mediumNeighs = 0;
    //--------------
    DiffAd_neighs.clear();
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
        }//layer below
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
        }//layer above
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

        if (n >= yMAX) n = 0;
        if (n < 0) n = yMAX - 1;


        if (worldP->insideWorld(m, n, p) == true) {

            //-----------NEW BIT!
            //for differential adhesion..
            //will need to add a lot to make sure dont think filagents are neighbours here for vessel version etc...!!!!
            if (worldP->grid[m][n][p].type == M) {
                if (diffAd_replaced == NULL) {

                    for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {
                        DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                    }
                } else if (diffAd_replaced != NULL) {

                    for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {

                        if ((worldP->grid[m][n][p].Mids[zed]->Cell != diffAd_replaced->Cell)) {
                            DiffAd_neighs.push_back(worldP->grid[m][n][p].Mids[zed]);
                        }

                    }
                }


            } else if ((worldP->grid[m][n][p].type == MED) && (diffAd_replaced != NULL)) {


                mediumNeighs++;

            } else if ((worldP->grid[m][n][p].type == MED) && (worldP->grid[m][n][p].med->diffAd_replaced != NULL)) {


                DiffAd_neighs.push_back(worldP->grid[m][n][p].med->diffAd_replaced);

            }
        }
        //_____________________
    }


}





//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------

void CPM_module::createMedium(void) {

    int i, j, k;
    MedAgent* medp;

    for (i = 0; i < xMAX; i++) {
        for (j = 0; j < yMAX; j++) {
            if (worldP->grid[i][j][0].type != M) {
                worldP->grid[i][j][0].type = MED;
                medp = new MedAgent(worldP);
                medp->Mx = (float) i;
                medp->My = (float) j;
                medp->Mz = 0.0f;
                mediumAgents.push_back(medp);
                worldP->grid[i][j][0].med = medp;
            }
        }
    }
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

int CPM_module::connectivity(MemAgent* replaced, MemAgent* replacer) {

    //check if replacer move leaves any of the neighbouring agents stranded...

    //
}
//-------------------------------------------------------------------------

bool CPM_module::check_gradient(MemAgent* replaced_mem, MemAgent* replacer_mem) {

    //if replacer mem is a tip cell... doesnt matter what other cell is...

    //make inversely proportional too active notch level...
    //float chance = rand() / (float) RAND_MAX;
    float chance = new_rand() / (float) NEW_RAND_MAX;
    float prob;

    //old version - but not inline with findings in Lars paper...
    //if (replacer_mem->Cell->VEGFRtot > (float) VEGFRnorm / 2.0f) {

    //this prob was in Lars paper model - but is te prob of one cell entirely passing another, so too low for this here...
    //will need to optimise this param somehow...

    //prob = 0.015-(0.000005*replacer_mem->Cell->activeNotchtot);
    
    if((replacer_mem->Cell->actNotCurrent<200)&&(replacer_mem->Cell->actNotCurrent>0)) prob = 1.0f-(replacer_mem->Cell->actNotCurrent/200.0f);
    else prob = 0;

    if (chance < prob) {
        
    //if (replacer_mem->Cell->VEGFRtot> (float) VEGFRnorm / 2.0f) {
        /*   int SumVEGF_replacer = 0;
           int SumVEGF_replaced = 0;
           int countA = 0;`
           int countB = 0;

           int x, m, n, p, zed;
           int i, j, k;
           i = (int) replaced_mem->Mx;
           j = (int) replaced_mem->My;
           k = (int) replaced_mem->Mz;

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


               if (worldP->insideWorld(m, n, p) == true) {

                   //-----------NEW BIT!
                   //for differential adhesion..
                   //will need to add a lot to make sure dont think filagents are neighbours here for vessel version etc...!!!!
                   if (worldP->grid[m][n][p].type == M) {
                       for (zed = 0; zed < worldP->grid[m][n][p].Mids.size(); zed++) {
                           if (worldP->grid[m][n][p].Mids[zed]->Cell == replacer_mem->Cell) {
                               SumVEGF_replacer += worldP->grid[m][n][p].Mids[zed]->SumVEGF;
                               countA++;
                           } else if (worldP->grid[m][n][p].Mids[zed]->Cell == replaced_mem->Cell) {
                               SumVEGF_replaced += worldP->grid[m][n][p].Mids[zed]->SumVEGF;
                               countB++;
                           }
                       }
                   }
               }
           }
           //get average vegf level seen by neighs of replacer cell type

           //get average vegf seen by replaced cell type

           if ((float) SumVEGF_replacer / (float) countA < (float) SumVEGF_replaced / (float) countB) return (true);
           else return (false);

       } else return (false);*/
       /* if (replaced_mem->SumVEGF > replacer_mem->SumVEGF)
        {
            cout<<" bias "<<replacer_mem->Mx<<" "<<replaced_mem->Mx<<" "<<replaced_mem->SumVEGF<<" "<<replacer_mem->SumVEGF<<" "<<replacer_mem->My<<" "<<replaced_mem->My;
            return (true);
        }*/
        if(replaced_mem->Mx>=replacer_mem->Mx){
            
            //cout<<" biased ";
            return(true);
        }
        else return (false);
    } else return (false);

}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------

void CPM_module::update_displaced_surfaceAgents(MemAgent* replaced_mem, MemAgent* replacer_mem) {

    int i, j, k, flag;
    int count1 = 0;
    int count2 = 0;
    MemAgent* deleter;
    std::vector<MemAgent*>::iterator T;
    int upto;


    //slow version - WORKS!!!************************************************************************
    /*for(i=0;i<worldP->ECagents.size();i++){
        upto = worldP->ECagents[i]->surfaceAgents.size();
        for(j=0;j<upto;j++){
            //delete any that have the replaced_mem in their triangle nodes...
            flag=0;
            
            if(worldP->ECagents[i]->surfaceAgents[j]->triangle.size()>0){
            for(k=0;k<3;k++){
                
                if(worldP->ECagents[i]->surfaceAgents[j]->triangle[k]==replaced_mem){ flag=1;
                }
            }
            //count1++;
            }
                //else{
                //cout<<"getting one without triangle nodes!!!!"<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
                //count2++;
                //}
            

            if(flag==1){

                //cout<<"deleting "<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
                            T =worldP->ECagents[i]->surfaceAgents.begin();
                            deleter = worldP->ECagents[i]->surfaceAgents[j];
                                    worldP->ECagents[i]->surfaceAgents.erase(T+j);
                                    flag=1;
                               

                            worldP->deleteOldGridRef(deleter, false);
                            delete deleter;
                            j--;
                            upto--;

                            
            }
        }
    }*/
    //****************************************************************************

    //see VBL book 149 for full description of cases for this.. basically the node in question has 6 possible triangles coming from it,
    //so go through the main anchor nodes for each and 1) check if given the change in node, the triangle is now on a junction, or if it was on a junction and now has changed, or if there is no change...
    // if its now on a junction, dleete all surface agents and their refs
    //if its still within same cell, change surface aents knowledge of that triangle node
    //if it was on a junciton but now in same cell, try to voxelise
    int s, t;
    MemAgent* nodeA = NULL;
    MemAgent* nodeB = NULL;
    MemAgent* nodeC = NULL;
    MemAgent* nodeD = NULL;
    MemAgent* neigh1 = NULL;
    MemAgent* neigh2 = NULL;
    MemAgent* anchor = NULL;
    std::vector <MemAgent*> triangleNodes;
    MemAgent* sp;
    int ups;
    bool UP;
    int m;
    //int upto;
    MemAgent* storedNeigh = NULL;
    //std::vector<MemAgent*>::iterator T;
    Coordinates first;
    Coordinates second;
    Coordinates third;
    vector<Coordinates> Coords;

    vector <MemAgent*> m6triagDels;

    //int flag;



    int count = 0;

    //for (i = 0; i < replaced_mem->neighs; i++) {

    //}

    /*for (i = 0; i < replaced_mem->neighs; i++) {
        if ((replaced_mem->neigh[i]->FIL != TIP) && (replaced_mem->neigh[i]->FIL !=STALK)) {
            count++;
            if ((replaced_mem->SpringNeigh[i]->right == false) && (replaced_mem->SpringNeigh[i]->horizontal == false)) nodeA = replaced_mem->neigh[i];
            else if ((replaced_mem->SpringNeigh[i]->right == true) && (replaced_mem->SpringNeigh[i]->horizontal == true)) nodeB = replaced_mem->neigh[i];
            else if ((replaced_mem->SpringNeigh[i]->right == true) && (replaced_mem->SpringNeigh[i]->horizontal == false)) nodeC = replaced_mem->neigh[i];
            else if ((replaced_mem->SpringNeigh[i]->right == false) && (replaced_mem->SpringNeigh[i]->horizontal == true)) nodeD = replaced_mem->neigh[i];
        }
    }


//cout<<"***start: replaced: "<<replaced_mem<<" replacer: "<<replacer_mem<<" nodeA "<<nodeA<<" nodeB "<<nodeB<<" nodeC "<<nodeC<<" nodeD "<<nodeD<<" ";
    for (i = 0; i < 6; i++) {

        neigh1 = NULL;
        neigh2 = NULL;

                //CASE A
        if (i == 0) {
            //neigh1 is the anchor node - triuangle  = down
            neigh1 = nodeA;
            neigh2 = nodeB;
            anchor = neigh1;
            UP = false;
        }
        //CASE B
        if (i == 1) {
            //anchor node = X this replacer - triangle is anchors up triangle
            anchor = replaced_mem;
            UP = true;
            neigh1 = nodeB;
            if(neigh1!=NULL){
            for (j = 0; j < neigh1->neighs; j++) {
                if ((neigh1->SpringNeigh[j]->right == true) && (neigh1->SpringNeigh[j]->horizontal == false)) {
                    neigh2 = neigh1->neigh[j];
                    storedNeigh = neigh2;

                }

            }
            }
            //cout<<"stored1: "<<storedNeigh<<" ";

        }
        //CASE C
        if (i == 2) {
            //anchor node  = X. triangel down.


            neigh1 = nodeC;
            neigh2 = storedNeigh;
            anchor = replaced_mem;
            UP = false;
        }
        //CASE D
        if (i == 3) {
            //neigh2 is anchor - tirangle =up.

            neigh1 = nodeC;
            neigh2 = nodeD;
            anchor = neigh2;
            UP = true;
        }
        //CASE E
        if (i == 4) {
            //anchor node is neigh2 triangle = down
            neigh1 = nodeD;
            UP = false;
            if(neigh1!=NULL){
            for (j = 0; j < neigh1->neighs; j++) {
                if ((neigh1->SpringNeigh[j]->right == false) && (neigh1->SpringNeigh[j]->horizontal == false)) {
                    neigh2 = neigh1->neigh[j];
                    storedNeigh = neigh2;
                }

            }
            }
            //cout<<"stored2 "<<storedNeigh<<" ";
            anchor = neigh2;


        }
        //CASE F
        if (i == 5) {
            //anchor node is neigh2 triangle = up
            neigh1 = nodeA;
            neigh2 = storedNeigh;
            anchor = neigh2;
            UP = true;
        }

        if ((neigh1 != NULL) && (neigh2 != NULL)) {
            //new triangle is within a cell
            if ((neigh1->Cell == neigh2->Cell) && (neigh1->Cell == replacer_mem->Cell)) {
                
                //I think the old one therefore must have been on a junction as this node cant have been from same cell

                //so only thing to do is try to voxelise...:depending on which case we're on, need to feed it the correct node to be anchor node etc...


                if(anchor==replaced_mem) anchor = replacer_mem;
                
                triangleNodes.push_back(anchor);
                if(neigh1!=anchor)
                triangleNodes.push_back(neigh1);
                if(neigh2!=anchor)
                triangleNodes.push_back(neigh2);
                if(triangleNodes.size()!=3) triangleNodes.push_back(replacer_mem);


                 first.x = triangleNodes[0]->Mx;
                        first.y = triangleNodes[0]->My;
                        first.z = triangleNodes[0]->Mz;

                        second.x = triangleNodes[1]->Mx;
                        second.y = triangleNodes[1]->My;
                        second.z = triangleNodes[1]->Mz;

                        third.x = triangleNodes[2]->Mx;
                        third.y = triangleNodes[2]->My;
                        third.z = triangleNodes[2]->Mz;

                        Coords.push_back(first);
                        Coords.push_back(second);
                        Coords.push_back(third);

                        //only bother if triangle nodes dont neighbour/overlap each other, i.e. has a hole that needs voxelising
                        if (anchor->VoxTriag_sizeTest(Coords) == false) {
                            // cout<<"false";
                            //int B;
                            //for(B=0;B<3;B++)
                            //cout<<"coords "<<B<<": "<<Coords[i].x<<" "<<Coords[i].y<<" "<<Coords[i].z<<" "<<endl;

                            if(UP==true) ups = 0;
                            else ups = 1;
                            worldP->voxeliseTriangle(Coords, triangleNodes, anchor->Cell, ups);
                            if(UP==true){
                                for(m=0;m<anchor->surfaceAgentsUP.size();m++)
                                worldP->dataFile<<anchor->surfaceAgentsUP[m]<<" "<<anchor->surfaceAgentsUP[m]->triangle[0]<<" "<<anchor->surfaceAgentsUP[m]->triangle[1]<<" "<<anchor->surfaceAgentsUP[m]->triangle[2]<<" neigh1 "<<neigh1<<" neigh2 "<<neigh2<<" anchor "<<anchor<<" "<<i<<endl;
                            }
                            else{
                                for(m=0;m<anchor->surfaceAgentsDOWN.size();m++)
                                worldP->dataFile<<anchor->surfaceAgentsDOWN[m]<<" "<<anchor->surfaceAgentsDOWN[m]->triangle[0]<<" "<<anchor->surfaceAgentsDOWN[m]->triangle[1]<<" "<<anchor->surfaceAgentsDOWN[m]->triangle[2]<<" neigh1 "<<neigh1<<" neigh2 "<<neigh2<<" anchor "<<anchor<<" "<<i<<endl;
                            }
                        }
                        Coords.clear();

                        for(m=0;m<3;m++) if(triangleNodes[m]==replaced_mem) cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<endl<<endl<<endl<<endl;
                       triangleNodes.clear();
     */

    /* }
     //new triangle is on a junction
     else {

         //old triangle was within a cell
         //if((neigh1->Cell == neigh2->Cell) && (neigh1->Cell == replaced_mem->Cell)){
     if(anchor->surfaceAgents3mesher.size()>0){
         cout<<"3mesher?"<<endl;
         for(s=0;s<anchor->surfaceAgents3mesher.size();s++){

                     sp = anchor->surfaceAgents3mesher[s];
                     worldP->deleteOldGridRef(sp, false);
                     //cout<<"6triagDel "<<sp<<endl;
                     //m6triagDels.push_back(sp);
                     //cout<<"upTriag deleted "<<sp<<endl;
                     //remove from overall surface agent list.
                     //could speed this up...
                     t=0;
                     T = anchor->Cell->surfaceAgents.begin();
                     flag=0;
                     do{
                         if(anchor->Cell->surfaceAgents[t]==sp){
                             anchor->Cell->surfaceAgents.erase(T+t);
                             flag=1;

                         }
                         t++;

                     }while((t<anchor->Cell->surfaceAgents.size())&&(flag==0));
                     delete sp;
                 }
                 anchor->surfaceAgents3mesher.clear();
                    
     }
             //then need to remove all surface agents.
             if(UP==true){
                        
             if(anchor->surfaceAgentsUP.size()>0){
                 //
                 //cout<<"up triag..."<<endl;

                        
                 for(s=0;s<anchor->surfaceAgentsUP.size();s++){

                     sp = anchor->surfaceAgentsUP[s];
                     worldP->deleteOldGridRef(sp, false);
                     //cout<<"6triagDel "<<sp<<endl;
                     //m6triagDels.push_back(sp);
                     //cout<<"upTriag deleted "<<sp<<endl;
                     //remove from overall surface agent list.
                     //could speed this up...
                     t=0;
                     T = anchor->Cell->surfaceAgents.begin();
                     flag=0;
                     do{
                         if(anchor->Cell->surfaceAgents[t]==sp){
                             anchor->Cell->surfaceAgents.erase(T+t);
                             flag=1;
                                   
                         }
                         t++;

                     }while((t<anchor->Cell->surfaceAgents.size())&&(flag==0));
                     delete sp;
                 }
                 anchor->surfaceAgentsUP.clear();
             }
             }
                    
             else{
                 if(anchor->surfaceAgentsDOWN.size()>0){
                 //
                     //cout<<"down triag..."<<endl;
                     upto = anchor->Cell->surfaceAgents.size()-1;
                 for(s=0;s<anchor->surfaceAgentsDOWN.size();s++){
                     //cout<<"downTriag deleted"<<endl;
                     sp = anchor->surfaceAgentsDOWN[s];
                     //cout<<"6triagDel "<<sp<<endl;
                     //m6triagDels.push_back(sp);
                     worldP->deleteOldGridRef(sp, false);

                     //remove from overall surface agent list.
                     //could speed this up...
                     t=upto;
                     T = anchor->Cell->surfaceAgents.begin();
                     flag=0;
                     do{
                         if(anchor->Cell->surfaceAgents[t]==sp){
                             anchor->Cell->surfaceAgents.erase(T+t);
                             flag=1;
                             upto--;
                         }
                         t--;

                     }while((t>-1)&&(flag==0));
                     delete sp;
                 }
                     anchor->surfaceAgentsDOWN.clear();
             }
                       
             }
         }
                
         //else{
             //old triangle could have also been on a junction - but different one.. so may not need to do anything

         //}

            
 //}
    }
 else if((neigh1==NULL)&&(neigh2!=NULL))
{
     if((anchor!=NULL)&&(anchor->surfaceAgents3mesher.size()>0)) cout<<"3mesherA?"<<endl;
 }
 else if((neigh1!=NULL)&&(neigh2==NULL))
{
     if((anchor!=NULL)&&(anchor->surfaceAgents3mesher.size()>0)) cout<<"3mesherB?"<<endl;
 }
 else{
     cout<<"null neighs"<<endl;
 if((anchor!=NULL)&&(anchor->surfaceAgents3mesher.size()>0)) cout<<"3mesher2?"<<endl;
 }

    }*/

    //cout<<"longone"<<endl;

    int flag2;
    /*for(i=0;i<worldP->ECagents.size();i++){
        upto = worldP->ECagents[i]->surfaceAgents.size();
        for(j=0;j<upto;j++){
            //delete any that have the replaced_mem in their triangle nodes...
            flag=0;

            if(worldP->ECagents[i]->surfaceAgents[j]->triangle.size()>0){
            for(k=0;k<3;k++){

                if(worldP->ECagents[i]->surfaceAgents[j]->triangle[k]==replaced_mem){
                    flag2=0;
                    for(m=0;m<m6triagDels.size();m++){
                    if(worldP->ECagents[i]->surfaceAgents[j]==m6triagDels[m]) flag2=1;
                    }
                    if(flag2==0){
                        cout<<endl<<endl<<"**********************************************************************gotone!"<<endl;
                        WORLDpointer->Pause=true;
                    }
                    cout<<worldP->ECagents[i]->surfaceAgents[j]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[0]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[1]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[2]<<endl;
                            flag=1;
                }
            }
            //count1++;
            }
                //else{
                //cout<<"getting one without triangle nodes!!!!"<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
                //count2++;
                //}


            if(flag==1){

                //cout<<"deleting "<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
                            T =worldP->ECagents[i]->surfaceAgents.begin();
                            deleter = worldP->ECagents[i]->surfaceAgents[j];
                                    worldP->ECagents[i]->surfaceAgents.erase(T+j);
                                    flag=1;


                            worldP->deleteOldGridRef(deleter, false);
                            delete deleter;
                            j--;
                            upto--;


            }
        }
    }*/

    //for(i=0;i<worldP->ECagents.size();i++){
    upto = replaced_mem->Cell->surfaceAgents.size();
    for (j = 0; j < upto; j++) {
        //delete any that have the replaced_mem in their triangle nodes...
        flag = 0;

        if (replaced_mem->Cell->surfaceAgents[j]->triangle.size() > 0) {
            for (k = 0; k < 3; k++) {

                if (replaced_mem->Cell->surfaceAgents[j]->triangle[k] == replaced_mem) {
                    /*flag2=0;
                    for(m=0;m<m6triagDels.size();m++){
                    if(replaced_mem->Cell->surfaceAgents[j]==m6triagDels[m]) flag2=1;
                    }
                    if(flag2==0){
                        cout<<endl<<endl<<"**********************************************************************gotone!"<<endl;
                        WORLDpointer->Pause=true;
                    }*/
                    //cout<<worldP->ECagents[i]->surfaceAgents[j]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[0]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[1]<<" "<<worldP->ECagents[i]->surfaceAgents[j]->triangle[2]<<endl;
                    flag = 1;
                }
            }
            //count1++;
        }
        //else{
        //cout<<"getting one without triangle nodes!!!!"<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
        //count2++;
        //}


        if (flag == 1) {

            //cout<<"deleting "<<worldP->ECagents[i]->surfaceAgents[j]<<endl;
            T = replaced_mem->Cell->surfaceAgents.begin();
            deleter = replaced_mem->Cell->surfaceAgents[j];
            replaced_mem->Cell->surfaceAgents.erase(T + j);
            flag = 1;


            worldP->deleteOldGridRef(deleter, false);
            delete deleter;
            j--;
            upto--;


        }
    }

    //   vector<Coordinates> Coords;
    Coordinates current;
    bool up = true;
    bool down = false;
    //std::vector<MemAgent*>::iterator T;
    int flagS = 0;

    int flagFound = 0;
    int FlagS = 0;
    int flagDo = 0;
    int flagA, flagB;
    bool changed;
    bool allow;
    std::vector <MemAgent*> triangleAgents;
    //std::vector <MemAgent*> triangleNodes;
    //int t;
    //cout<<"actual"<<Mx<<" "<<My<<" "<<Mz<<endl;
    //cout<<"current"<<current.x<<" "<<current.y<<" "<<current.z<<endl;
    bool dontDo;

    //need to change for on the fly voxelisation in diffAd...


    for (m = 0; m < replacer_mem->Cell->nodeAgents.size(); m++) {
        //get triangle.. if it has replacer as a node.. try gridding...
        //but need to realise havent deleted legitimate surface agent s from it.. er.. infact can hopefully hack code from grid agents to here..
        //then only replace up or down or 3fixer as need be, which shouldnt have any agents anyway as its changing cell...



        for (i = 0; i < 3; i++) {
            allow = false;

            //only for hexagonal cells
            //if((labelled2==true)&&(i==0)) dontDo=true;
            //if((labelled==true)&&(i==1)) dontDo=true;

            //if (dontDo == false) {
            //if(i==2){
            if (i == 0) {
                //for (t = 0; t < surfaceAgentsUP.size(); t++)
                //    triangleAgents.push_back(surfaceAgentsUP[t]);

                triangleNodes = replacer_mem->Cell->nodeAgents[m]->getTriangle(up);
            } else if (i == 1) {
                // for (t = 0; t < surfaceAgentsDOWN.size(); t++)
                //   triangleAgents.push_back(surfaceAgentsDOWN[t]);

                triangleNodes = replacer_mem->Cell->nodeAgents[m]->getTriangle(down);
            } else {
                if (replacer_mem->Cell->nodeAgents[m]->mesh3SpringsOnly == true) {
                    triangleNodes = replacer_mem->Cell->nodeAgents[m]->three_erTriangleFix();

                    //      for (t = 0; t < surfaceAgents3mesher.size(); t++)
                    // triangleAgents.push_back(surfaceAgents3mesher[t]);
                }

            }




            //some on the edge wont have a surface to cover - not covering junctions?

            if (triangleNodes.size() == 3) {
                for (k = 0; k > triangleNodes.size(); k++) {
                    if (triangleNodes[k] == replaced_mem) allow = true;
                }

                if (allow == true) {
                    //Cell->triangles.push_back(current);
                    //  Cell->triangles.push_back(coords[0]);
                    //Cell->triangles.push_back(coords[1]);
                    //Cell->trianglesUPDOWN.push_back(i);
                    //cout<<"coord 0"<<coords[0].x<<" "<<coords[0].y<<" "<<coords[0].z<<endl;
                    //cout<<"coord 1"<<coords[1].x<<" "<<coords[1].y<<" "<<coords[1].z<<endl;
                    //if((worldP->getDist(current.x, current.y, current.z, coords[0].x, coords[0].y, coords[0].z)>1)||(worldP->getDist(coords[0].x, coords[0].y, coords[0].z, coords[1].x, coords[1].y, coords[1].z)>1.0)){
                    //cout<<worldP->getDist(current.x, current.y, current.z, coords[0].x, coords[0].y, coords[0].z)<<endl;
                    //cout<<worldP->getDist(coords[0].x, coords[0].y, coords[0].z, coords[1].x, coords[1].y, coords[1].z)<<endl;
                    Coordinates second;
                    Coordinates third;

                    current.x = replacer_mem->Cell->nodeAgents[m]->Mx;
                    current.y = replacer_mem->Cell->nodeAgents[m]->My;
                    current.z = replacer_mem->Cell->nodeAgents[m]->Mz;

                    second.x = triangleNodes[1]->Mx;
                    second.y = triangleNodes[1]->My;
                    second.z = triangleNodes[1]->Mz;

                    third.x = triangleNodes[2]->Mx;
                    third.y = triangleNodes[2]->My;
                    third.z = triangleNodes[2]->Mz;

                    Coords.push_back(current);
                    Coords.push_back(second);
                    Coords.push_back(third);

                    //only bother if triangle nodes dont neighbour/overlap each other, i.e. has a hole that needs voxelising
                    if (replacer_mem->Cell->nodeAgents[m]->VoxTriag_sizeTest(Coords) == false) {
                        // cout<<"false";
                        //int B;
                        //for(B=0;B<3;B++)
                        //cout<<"coords "<<B<<": "<<Coords[i].x<<" "<<Coords[i].y<<" "<<Coords[i].z<<" "<<endl;

                        worldP->voxeliseTriangle(Coords, triangleNodes, replacer_mem->Cell, i);
                    }
                    Coords.clear();
                }
                //}
                //}
            }
        }

        triangleNodes.clear();
        triangleAgents.clear();
    }

    //}



}

//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//clear up function to stop broken off parts of cells created by differential adhesion model (occurred in the original glazier graner model the bastards, tho they didnt say about it of course...)

void CPM_module::clearUpSmallSeparatedBitsOfCells(void) {

    //if its on the junction - it could either belong to the main body of the cell, or a small off shoot.. trouble is it might not be completely on its own...

    //for now, start with single nodes with founr junction springs.. could make more sophisticated if needed..

    int i, j, k, r;
    EC* ecp;
    int count = 0;
    int countS;
    int countFil;
    int choseReplacer;
    int pos;
    int upto;
    MemAgent* replacer_mem;
    MemAgent* replaced_mem;
    int neigh;
    int flag=0;
    MemAgent* Mcurrent;
    int flagD=0;
    //cout<<"`";
    upto = worldP->JunctionAgents.size();

    for (j = 0; j < worldP->JunctionAgents.size(); j++) {
        flagD = 0;
        countFil = 0;
        countS = 0;
        k=0;
        do{
            if (((worldP->JunctionAgents[j]->neigh[k]->FIL == BASE) || (worldP->JunctionAgents[j]->neigh[k]->FIL == NONE)) && (worldP->JunctionAgents[j]->SpringNeigh[k]->Junction == false)) flagD=1;
            if ((worldP->JunctionAgents[j]->neigh[k]->FIL == STALK) || (worldP->JunctionAgents[j]->neigh[k]->FIL == TIP)) {
                countFil++;
                neigh = k;
            }
            if (worldP->JunctionAgents[j]->SpringNeigh[k]->Junction == true) countS++;
            k++;

        }while((k<worldP->JunctionAgents[j]->neighs)&&(flagD==0));
        if (flagD == 0) {
            //need to delete this agent by force replacing by a random neigh cell.. it gets engulfed/dies
            if (countFil == 0) {
                count = 0;
                //choseReplacer = rand() % countS;
                choseReplacer = new_rand() % countS;
                for (r = 0; r < worldP->JunctionAgents[j]->neighs; r++) {


                    if (worldP->JunctionAgents[j]->SpringNeigh[r] != NULL) {
                        if (worldP->JunctionAgents[j]->SpringNeigh[r]->Junction == true) {
                            if (count == choseReplacer) {
                                replacer_mem = worldP->JunctionAgents[j]->neigh[r];
                                replaced_mem = worldP->JunctionAgents[j];
                                pos = j;

                            } else count++;
                        }
                    }
                }
                replace_agent(replacer_mem, replaced_mem, NULL, NULL, pos);
                //cout<<"replaced_small_bits_disconnected"<<endl;
            } else {
                //will need to set fil to retract before can replace agent...
                if (worldP->JunctionAgents[j]->neigh[neigh]->FIL == TIP) {
                    worldP->JunctionAgents[j]->neigh[neigh]->filTipTimer = FILTIPMAX + 1;
                } else {

                    Mcurrent = worldP->JunctionAgents[j]->neigh[neigh];
                    flag = 0;

                    do {
                        //score++;

                        if (Mcurrent->FIL == TIP) {
                            Mcurrent->filTipTimer = FILTIPMAX + 1;
                            flag = 1;
                        } else
                            Mcurrent = Mcurrent->plusSite;

                    } while (flag != 1);
                }

            }

        }
    }

}

//---------------------------------------------------------------------------------------------------
bool CPM_module::move_fil_base(MemAgent* mp){

    int i,j,k;
    int pos = -1;
    int flag=0;

    //find the neighbour node that is the fil
    for(i=0;i<mp->neighs;i++){
        if((mp->neigh[i]->FIL==TIP)||(mp->neigh[i]->FIL==STALK)) pos = i;
    }
    if(pos==-1) cout<<"cant find fil?"<<endl;
    //will always move fil to a left hand neigh - so should randomise this...
    for(i=0;i<mp->neighs;i++){
        if(flag==0){
        if((mp->neigh[i]->Cell==mp->Cell)&&(mp->neigh[i]->FIL==NONE)){
            //transfer fil to this node
            mp->neigh[i]->FIL = BASE;
            mp->neigh[i]->plusSite = mp->neigh[pos];
            mp->neigh[pos]->filNeigh = mp->neigh[i];
            mp->FIL = NONE;
            mp->plusSite = NULL;
            
            mp->neigh[pos]->labelled = true; //is this redundant?!! use a specific labelling flag or remove. 
            
            //cout<<"changed a fil position! "<<mp<<" "<<mp->neigh[pos]<<endl;

            //add to other neighbour
            mp->neigh[i]->neigh[mp->neigh[i]->neighs] = mp->neigh[pos];
            mp->neigh[i]->SpringNeigh[mp->neigh[i]->neighs] = mp->SpringNeigh[pos];
            mp->SpringNeigh[pos]->start = mp->neigh[i];

            for(j=0;j<mp->neigh[pos]->neighs;j++){
                if(mp->neigh[pos]->neigh[j]==mp){
                    mp->SpringNeigh[pos]->end = mp->neigh[i];
                }
            }
            mp->neigh[i]->neighs++;
            //remove neigh and spring info fro this agent - shuffle up ones after if need be
            mp->neigh[pos] = NULL;
            mp->SpringNeigh[pos] = NULL;
            if(pos!=mp->neighs-1){
                //shuffle up ones after
                k=pos;
                for(j=0;j<mp->neighs-pos;j++){
                    if(k+1<mp->neighs){
                    mp->neigh[k] = mp->neigh[k+1];
                    mp->SpringNeigh[k] = mp->SpringNeigh[k+1];
                    mp->neigh[k+1] = NULL;
                    mp->SpringNeigh[k+1] = NULL;

                    }

                }
            }
            mp->neighs--;
            flag=1;
        }
        }
    }

    if(flag==1) return(true);
    else return(false);
}
