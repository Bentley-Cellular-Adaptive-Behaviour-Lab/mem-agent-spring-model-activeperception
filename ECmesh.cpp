#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include "objects.h"
#include <math.h>
#include <functional>
#include <numeric>
#include <iterator>


using namespace std;


int countbell = 0;
//-------------------------------------------------------------------------------------------------------------
//base has reached a stalk joint, remove stalk node and reconnect base to next node

void EC::removeStalkNode(Spring *STP) {

    int i, N;
    Spring* nextNodeSpring1;
    Spring*baseNodeSpring1;
    int flag, flag1;
    vector<Spring*>::iterator Q;
    vector<MemAgent*>::iterator L;
    vector<MemAgent*>::iterator T;
    MemAgent * stalkNode;
    MemAgent * baseNode;
    MemAgent * nextNode;
    Spring* stp;

    int pos;

    stalkNode = STP->end;
    baseNode = STP->start;

    if (baseNode->FIL == BASE) {
        for (N = 0; N < baseNode->neighs; N++) {
            if (baseNode->neigh[N] == stalkNode) pos = N;
        }
    } else pos = 0;


    //get the address of next node up to reconnect. and both its spring addresses

    nextNode = stalkNode->neigh[0];
    nextNodeSpring1 = stalkNode->SpringNeigh[0];

    baseNodeSpring1 = baseNode->SpringNeigh[pos];

    //remove stalk node from Cells node list and delete stalk node.
    i = 0;
    flag = 0;
    int be = stalkNode->Cell->nodeAgents.size();

    do {
       
        if (stalkNode->Cell->nodeAgents[i] == stalkNode) {
            flag = 1;
            L = stalkNode->Cell->nodeAgents.begin();
            stalkNode->Cell->nodeAgents.erase(L + i);
        }
        i++;

    } while ((i < (int) stalkNode->Cell->nodeAgents.size()) && (flag == 0));
    if (flag == 0) {
        cout << "cant find stalk node.." << endl;
        cout.flush();
    }

    //create new springs


    createSpringTokenObject(baseNode, nextNode, pos);

    nextNode->filNeigh = baseNode;


    baseNode->neigh[pos] = nextNode;


    //give old springs agents and tokens to the new springs----------------------------------------------
    stp = baseNode->SpringNeigh[pos];

    for (i = 0; i < (int) STP->agents.size(); i++) {
        stp->agents.push_back(STP->agents[i]);
    }
    for (i = 0; i < (int) nextNodeSpring1->agents.size(); i++) {
        stp->agents.push_back(nextNodeSpring1->agents[i]);
    }


    stp->labelled = true;
    //stp->filTokPos.assign(nextNodeSpring1->filTokPos.begin(), nextNodeSpring1->filTokPos.end());
    //--------------------------------------------------------------------------------------------------------------------

    //also the stalk node to be deleted needs to have it tokens saved first. give to basenode as this is nearest to it.
    baseNode->filTokens += stalkNode->filTokens;
    //dont need to worry abuot proteins as they've already had their effect in sumProtLevels();

    //delete old grid ref
    int upto = worldP->grid[(int) stalkNode->Mx][(int) stalkNode->My][(int) stalkNode->Mz].Fids.size();
    T = worldP->grid[(int) stalkNode->Mx][(int) stalkNode->My][(int) stalkNode->Mz].Fids.begin();
    flag1 = 0;
    i = 0;
    do {
        if (worldP->grid[(int) stalkNode->Mx][(int) stalkNode->My][(int) stalkNode->Mz].Fids[i] == stalkNode) {
            worldP->grid[(int) stalkNode->Mx][(int) stalkNode->My][(int) stalkNode->Mz].Fids.erase(T + i);
            flag1 = 1;
        }
        i++;
    } while ((i < upto) && (flag1 == 0));
    if (flag1 == 0) {
        cout << "BBcant find the memagent in that grid site" << endl;
        cout.flush();
    }

    //deletions
    baseNodeSpring1->deletion = true;
    nextNodeSpring1->deletion = true;

    delete stalkNode;




}
//-------------------------------------------------------------------------------------------------------------

void EC::newNodes(void) {

   	int i, j, k;
    float x, y, z, a, b, c, length;
    Spring* stp;
    Coordinates Coord;
    MemAgent* memp;
    float XA;
    int startN, half;
    vector<MemAgent*>::iterator D;
    vector<Spring*>::iterator S;
    int upto = (int) Springs.size();
    int count=0;
	
    for (i = 0; i < Springs.size(); i++) {
		
        count++;
		
        //this counter stops the memory leak infinite looping in here - we dont know what precisely causes it to go wrong, but its definitely in here and an infinitie loop of dividing and creating new springs, this counter will detect it and stop this run and start a new one. Doesnt happen often if at all anymore, just a failsafe incase.
        if(count<upto+1000){
			
			stp = Springs[i];
			//deletion set in remove stalk node
			if (stp->deletion == true) {
				S = Springs.begin();
				Springs.erase(S + i);
				delete stp;
				
				i--;
			} else if ((stp->deletion == false) && (stp->filopodia == true)) {
				
				x = stp->start->Mx;
            y = stp->start->My;
            z = stp->start->Mz;
            a = stp->end->Mx;
            b = stp->end->My;
            c = stp->end->Mz;
            XA = x - a;
            //toroidal adjustments
            if(TOROIDAL_X==true){
            if (sqrt(XA * XA) >= (float) xMAX / 2.0f) {

                if (XA > 0) XA = -((float) xMAX - XA);
                else XA = (float) xMAX - fabs(XA);
                length = sqrt((XA * XA)+((y - b)*(y - b))+((z - c)*(z - c)));

            } else {
                length = worldP->getDist(x, y, z, a, b, c);
            }
            }
            else length = worldP->getDist(x, y, z, a, b, c);
            //if((stp->start->FIL==STALK)&&(stp->end->FIL==STALK)){
            if (stp->end->FIL != TIP) {
                if ((int) length <= 1.0) removeStalkNode(stp);
            }
            //check length, if longer than threshold create a new node in middle of spring
            if ((length >= NewNodeLength) && (stp->veilAdvancing == false)) {
					
					//create new node
					Coord = worldP->findMidPoint(x, y, z, a, b, c, length);
					
					if(Coord.x>=xMAX) Coord.x-=xMAX;
                    else if(Coord.x<0) Coord.x+=xMAX;

					memp = new MemAgent(this, worldP);
					//cout<<"newNode:"<<memp<<endl<<endl;
					if ((Coord.x < 0) || (Coord.x >= xMAX)) cout << "bug " << Coord.x;


					memp->Mx = Coord.x;
					memp->My = Coord.y;
					memp->Mz = Coord.z;
					
					nodeAgents.push_back(memp);
					
					
					worldP->setFilLocation((int) memp->Mx, (int) memp->My, (int) memp->Mz, memp);
					
					memp->FIL = STALK;
					
					if ((stp->start->FIL == BASE) && (stp->end->FIL == TIP)) memp->filPos = 1;
					else if (stp->start->FIL == STALK) memp->filPos = stp->start->filPos + 1;
					
					memp->FA = true;
					
					
					//update memAgents knowledge of connections and springs
					
					memp->filNeigh = stp->start;
					
					memp->neigh[0] = stp->end;
					memp->neighs = 1;
					stp->end->filNeigh = memp;
					
					
					//start
					for (j = 0; j < stp->start->neighs; j++) {
						if (stp->start->neigh[j] == stp->end) {
							stp->start->neigh[j] = memp;
							startN = j;
							break;
						}
					}
					
					half = (int) ((float) stp->agents.size() / 2.0f);
					D = stp->agents.begin();
					
					// create new spring token objects for each of the 4 new springs
					createSpringTokenObject(stp->start, memp, startN);
					createSpringTokenObject(memp, stp->end, 0);
					
					
					stp->start->SpringNeigh[startN]->agents.assign(stp->agents.begin(), D + half);
					memp->SpringNeigh[0]->agents.assign(D + half, stp->agents.end());
					
					//reassign agents minus and plus sites to new node
					if (stp->start->SpringNeigh[startN]->agents.size() > 0) {
						stp->start->SpringNeigh[startN]->agents[stp->start->SpringNeigh[startN]->agents.size() - 1]->plusSite = memp;
						memp->minusSite = stp->start->SpringNeigh[startN]->agents[stp->start->SpringNeigh[startN]->agents.size() - 1];
					} else {
						stp->start->plusSite = memp;
						memp->minusSite = stp->start;
					}
					
					if (memp->SpringNeigh[0]->agents.size() > 0) {
						memp->SpringNeigh[0]->agents[0]->minusSite = memp;
						memp->plusSite = memp->SpringNeigh[0]->agents[0];
						if (memp->plusSite == NULL) {
							int kate = 0;
						}
					} else {
						stp->end->minusSite = memp;
						memp->plusSite = stp->end;
						if (memp->plusSite == NULL) {
							int kate = 0;
						}
					}
					
					if (memp->plusSite == NULL) {
						int kate = 0;
					}
					//delete old spring objs, remove from EC list
					S = Springs.begin();
					Springs.erase(S + i);
					delete stp;
					
					i--;
				}
			}
			
		}else{
			//it must be memory leaking and spilling out - so kill this run and start a new one.
			MEM_LEAK_OCCURRING=true;
			i = Springs.size();
			
		}
    }
}
//-------------------------------------------------------------------------------------------------------------
/*void EC::NodeAdditionDeletion(void){
 *
 * int i,j;
 * float x,y,z,a,b,c, length;
 * Spring* stp;
 * Spring* stp2;
 * float* Coord=0;
 * MemAgent* memp;
 * float XA;
 * int startN, endN, half;
 * vector<MemAgent*>::iterator D;
 * vector<Spring*>::iterator S;
 *
 * //add in a bit to set new node springJunction
 * //as true if its created on a spring where the
 * // start and end nodes are in different cells.
 *
 * for(i=0;i<(int)Springs.size();i++){
 *
 * stp = Springs[i];
 *
 * if((stp->deletion==false)&&(stp->filopodia==true)){
 *
 * x = stp->start->Mx; y = stp->start->My; z = stp->start->Mz; a = stp->end->Mx; b = stp->end->My; c = stp->end->Mz;
 * XA = x-a;
 * //toroidal adjustments
 * if(sqrt(XA*XA)>=(float)xMAX/2.0f){
 *
 * if(XA>0) XA= -((float)xMAX- XA);
 * else XA=(float)xMAX- fabs(XA);
 * length=sqrt((XA*XA)+((y-b)*(y-b))+((z-c)*(z-c)));
 *
 * }
 * else{
 * length = worldP->getDist(x,y,z,a,b,c);
 * }
 *
 * if((stp->start->FIL==BASE)&&(stp->end->FIL==STALK)){
 * if((int)length<=1) removeStalkNode(stp);
 * }
 * //check length, if longer than threshold create a new node in middle of spring
 * if(length>=NewNodeLength){
 * //newNodes(stp);
 *
 * //delete old spring objs, remove from EC list
 * S = Springs.begin();
 * Springs.erase(S+i);
 * delete stp;
 * i--;
 * }
 * }
 * }
 *
 * }*/
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

bool EC::testSpringLength(float P[3], float N[3], Spring* stp) {

    int A, B, D;
    bool need = false;

    A = ((int) P[0]-(int) N[0])*((int) P[0]-(int) N[0]);
    B = ((int) P[1]-(int) N[1])*((int) P[1]-(int) N[1]);
    D = ((int) P[2]-(int) N[2])*((int) P[2]-(int) N[2]);

    if (A >= xMAX / 2.0f) {
        if (P[0] > N[0]) A = ((int) (P[0] - xMAX)-(int) N[0])*((int) (P[0] - xMAX)-(int) N[0]);
        else A = ((int) (P[0] - xMAX)-(int) N[0])*((int) (P[0] - xMAX)-(int) N[0]);
    }
    //if the spring stretches past its nearest neighs
    if (sqrt((double) A) >= 1) need = true;
    if (sqrt((double) B) >= 1) need = true;
    if (sqrt((double) D) >= 1) need = true;

    if (need == true)
        return (true);
        //}
    else {

        //if spring is so small it no longer needs spring agents
        if (stp->oldSize > 0) {
            //pass tokens to a node neighbour so they arent lost from the system.
            stp->start->filTokens = accumulate(stp->filTokPos.begin(), stp->filTokPos.end(), 0);

        }

        return (false);
    }

}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

bool World::toroidalTest(float P[3], float N[3]) {

    if ((sqrt((P[0] - N[0])*(P[0] - N[0]))<(float) xMAX / 2.0f)) return (false);
    else return (true);
}
//-------------------------------------------------------------------------------------------------------------

bool EC::test_Pos_change(MemAgent* first, MemAgent* second) {

    if (((int) first->Mx == (int) second->Mx) && ((int) first->My == (int) second->My) && ((int) first->Mz == (int) second->Mz)) {
        return (false);
    } else return (true);

}
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------

void EC::gridAgents(void) {

    float P[3]; // = {1.5,2.0,2.0};
    float N[3]; // = {2.5,2.0,1.0};
    int sumfilTok;

    int i, j, k, J;

    int upto = nodeAgents.size();

    countbell = 0;

    //got through each node agent and check for each neighbour if the spring is long enough to need a spring agent placed on it.
    for (i = 0; i < upto; i++) {

        if ((nodeAgents[i]->FIL == NONE) || (nodeAgents[i]->FIL == BASE)) {

            nodeAgents[i]->gridSurfaceAgents();

        }

        if ((nodeAgents[i]->FIL == BASE) || (nodeAgents[i]->FIL == STALK)) {

            P[0] = nodeAgents[i]->Mx;
            P[1] = nodeAgents[i]->My;
            P[2] = nodeAgents[i]->Mz;

            j = -1;
            J = 0;
            MemAgent* blemb = nodeAgents[i];
            do {

                if (nodeAgents[i]->SpringNeigh[J]->filopodia == true) {
                    j = J;
                }
                J++;

            } while ((j == -1) && (J < nodeAgents[i]->neighs));

            //if(test_Pos_change(nodeAgents[i],nodeAgents[i]->neigh[j])==true){

            sumfilTok = 0;

            if(j!=-1){
            N[0] = nodeAgents[i]->neigh[j]->Mx;
            N[1] = nodeAgents[i]->neigh[j]->My;
            N[2] = nodeAgents[i]->neigh[j]->Mz;

            if (testSpringLength(P, N, nodeAgents[i]->SpringNeigh[j]) == true) {
                //cout<<"gridding "<<nodeAgents[i]<<" "<<nodeAgents[i]->neigh[j]<<endl;
                if (worldP->toroidalTest(P, N) == false) gridSpringAgents(P, N, false, nodeAgents[i]->SpringNeigh[j]);
                else gridSpringAgents(P, N, true, nodeAgents[i]->SpringNeigh[j]);


            } else {
                if ((nodeAgents[i]->filPos < nodeAgents[i]->neigh[j]->filPos) || (nodeAgents[i]->neigh[j]->FIL == TIP)) {
                    nodeAgents[i]->plusSite = nodeAgents[i]->neigh[j];
                    nodeAgents[i]->neigh[j]->minusSite = nodeAgents[i];
                }
                if (nodeAgents[i]->SpringNeigh[j]->oldSize != 0) {
                    //if spring is so small it no longer needs spring agents
                    //pass tokens to a node neighbour so they arent lost from the system.
                    for (k = 0; k < nodeAgents[i]->SpringNeigh[j]->oldSize; k++) {
                        sumfilTok += nodeAgents[i]->SpringNeigh[j]->filTokPos[k];
                    }

                    nodeAgents[i]->SpringNeigh[j]->end->filTokens += sumfilTok;
                }
            }


            if (nodeAgents[i]->plusSite == NULL) {
                int kate = 0;
            }

        }
        }
    }

    //cout<<"vox tried: "<<countbell<<endl;
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

void EC::gridSpringAgents(float P[3], float N[3], bool toroidal, Spring* stp) {

    float PN[3];
    float x1, y1, z1, x2, y2, z2;
    float steps = 0.5f;
    float x, y, z;
    int flag = 0;

    int i;
    int sumfilTok = 0;


    //wrap round for springs that cros x axis toroidal boundary-------------------------------------
    //displace N to outside of grid to calculate then create spring ni correct position
    if ((toroidal == true) && (N[0] > P[0])) {
        N[0] -= xMAX;
        flag = 1;
    } else if ((toroidal == true) && (N[0] < P[0])) {
        N[0] += xMAX;
        flag = 2;
    }
    //-------------------------------------------------------------------------------------------------------------

    //check if the grid sites inbetween along that spring are empty,

    PN[0] = P[0] - N[0];
    PN[1] = P[1] - N[1];
    PN[2] = P[2] - N[2];

    x1 = P[0];
    y1 = P[1];
    z1 = P[2];

    x2 = N[0];
    y2 = N[1];
    z2 = N[2];

    if ((int) x2 > (int) ((int) x1 + steps)) {
        x = (int) x1 + steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) x != (int) x2) {
                if (x > x1) {
                    y = (((x - x1) / PN[0]) * PN[1]) + y1;
                    z = (((x - x1) / PN[0]) * PN[2]) + z1;

                    if ((x >= 0) && (x < xMAX)) createSpringAgent((int) x, (int) y, (int) z, stp);
                        //have to do the extra -1 here as otherwise it rounds -0.5 to 0 instead of -1..
                    else if (flag == 1) createSpringAgent((int) (x - 1) + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                x += steps;
            }

            //cout<<"x2>x1 ";
        } while ((int) x < (int) x2);

    } else if ((int) x2 < (int) ((int) x1 - steps)) {
        x = (int) x1 - steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) x != (int) x2) {
                if (x < x1) {
                    y = (((x - x1) / PN[0]) * PN[1]) + y1;
                    z = (((x - x1) / PN[0]) * PN[2]) + z1;

                    if ((x >= 0) && (x < xMAX)) createSpringAgent((int) x, (int) y, (int) z, stp);
                    else if (flag == 1) createSpringAgent((int) x - 1 + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                x -= steps;
            }
            //cout<<"x2<x1 ";
        } while ((int) x > (int) x2);
    }

    if ((int) y2 > (int) ((int) y1 + steps)) {
        y = (int) y1 + steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) y != (int) y2) {
                if (y > y1) {
                    x = (((y - y1) / PN[1]) * PN[0]) + x1;
                    z = (((y - y1) / PN[1]) * PN[2]) + z1;

                    if ((x >= 0) && (x < xMAX)) createSpringAgent((int) x, (int) y, (int) z, stp);
                    else if (flag == 1) createSpringAgent((int) x - 1 + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                y += steps;
            }
            // cout<<"y2>y1 ";
        } while ((int) y < (int) y2);
    } else if ((int) y2 < (int) ((int) y1 - steps)) {

        y = (int) y1 - steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) y != (int) y2) {
                if (y < y1) {
                    x = (((y - y1) / PN[1]) * PN[0]) + x1;
                    z = (((y - y1) / PN[1]) * PN[2]) + z1;

                    if ((x >= 0) && (x < xMAX)) createSpringAgent((int) x, (int) y, (int) z, stp);
                    else if (flag == 1) createSpringAgent((int) x - 1 + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                y -= steps;
            }
            //cout<<"y2<y1 ";
        } while ((int) y > (int) y2);
    }
    if ((int) z2 > (int) ((int) z1 + steps)) {
        z = (int) z1 + steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) z != (int) z2) {
                if (z > z1) {
                    x = (((z - z1) / PN[2]) * PN[0]) + x1;
                    y = (((z - z1) / PN[2]) * PN[1]) + y1;

                    if ((x >= 0) && (x < xMAX))createSpringAgent((int) x, (int) y, (int) z, stp);
                    else if (flag == 1) createSpringAgent((int) x - 1 + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                z += steps;
            } //cout<<"z2>z1 ";
        } while ((int) z < (int) z2);
    } else if ((int) z2 < (int) ((int) z1 - steps)) {

        z = (int) z1 - steps;
        //we know this is the axis along which the vector increases
        do {
            if ((int) z != (int) z2) {
                if (z < z1) {
                    x = (((z - z1) / PN[2]) * PN[0]) + x1;
                    y = (((z - z1) / PN[2]) * PN[1]) + y1;

                    if ((x >= 0) && (x < xMAX)) createSpringAgent((int) x, (int) y, (int) z, stp);
                    else if (flag == 1) createSpringAgent((int) x - 1 + xMAX, (int) y, (int) z, stp);
                    else if (flag == 2) createSpringAgent((int) x - xMAX, (int) y, (int) z, stp);
                }
                z -= steps;
            }
            //cout<<"z2<z1 ";
        } while ((int) z > (int) z2);
    }


    //update the new spring agents with the old ones on this springs tokens
    if (stp->agents.size() > 0) {

        if (stp->oldSize > 0) {
            if (((int) stp->agents.size() == stp->oldSize) || ((int) stp->agents.size() > stp->oldSize)) {
                for (i = 0; i < stp->oldSize; i++) {
                    stp->agents[i]->filTokens = stp->filTokPos[i];

                }
            } else if ((int) stp->agents.size() < stp->oldSize) {
                //if not the same number of agents then give each the correct value for its position, and then give the remainder to the last agent
                for (i = 0; i < (int) stp->agents.size(); i++) {
                    stp->agents[i]->filTokens = stp->filTokPos[i];

                }
                for (i = (int) stp->agents.size(); i < stp->oldSize; i++) {
                    sumfilTok += stp->filTokPos[i];

                }
                stp->agents[(int) stp->agents.size() - 1]->filTokens += sumfilTok;

            }

            stp->oldSize = 0;
        }
    } else {
        //if spring is so small it no longer needs spring agents
        //pass tokens to a node neighbour so they arent lost from the system.
        for (i = 0; i < stp->oldSize; i++) {
            sumfilTok += stp->filTokPos[i];

        }

        stp->end->filTokens += sumfilTok;

    }

    //if in a filopdia, link the memAgents for token passing
    if (stp->filopodia == true) {
        //cout<<"linking spring agents in fil"<<endl;
        //cout.flush();
        if ((int) stp->agents.size() > 1) {

            for (i = 0; i < (int) stp->agents.size(); i++) {

                if (i == 0) {
                    stp->agents[i]->minusSite = stp->start;
                    stp->agents[i]->plusSite = stp->agents[i + 1];
                    stp->start->plusSite = stp->agents[i];
                } else if (i == (int) stp->agents.size() - 1) {

                    stp->agents[i]->minusSite = stp->agents[i - 1];
                    stp->agents[i]->plusSite = stp->end;
                    stp->end->minusSite = stp->agents[i];
                } else {
                    stp->agents[i]->minusSite = stp->agents[i - 1];
                    stp->agents[i]->plusSite = stp->agents[i + 1];
                }

                stp->agents[i]->FIL = STALK;
            }
        } else if (stp->agents.size() == 1) {
            stp->agents[0]->FIL = STALK;
            stp->agents[0]->minusSite = stp->start;
            stp->agents[0]->plusSite = stp->end;
            stp->start->plusSite = stp->agents[0];
            stp->end->minusSite = stp->agents[0];
        } else if (stp->agents.size() == 0) {

            //reset both nodes to be each others minus and plus sites
            if ((stp->start->FIL == BASE) || (stp->start->FIL == STALK)) {
                stp->start->plusSite = stp->end;
                stp->start->minusSite = NULL;

                stp->end->minusSite = stp->start;
                stp->end->plusSite = NULL;
            }

        }
    }

}
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------

void EC::createSpringAgent(int x, int y, int z, Spring* stp) {

    bool allow;
    int i, flag;
    flag = 0;

    //create object
    MemAgent* memp = new MemAgent(this, worldP);

    memp->Mx = (float) x;
    memp->My = (float) y;
    memp->Mz = (float) z;
    memp->vonNeu = true;
    //set it to be a spring rather than node agent
    memp->node = false;

    //spring agents need to know their position along the spring and which spring they belong to so their tokens can be realocated to the correct new spring agents on the next 			round, even if spring has moved - need tokens to stay in same vicinity. This is needed as every timestep all spring agents are removed and then recreated - so their tokens 		must be stored elsewhere imbetween.

    memp->SpringBelong = stp;
    stp->agents.push_back(memp);

    //add it to ECs list of spring agents
    springAgents.push_back(memp);

    worldP->setFilLocation(x, y, z, memp);

    //memp->JunctionTest();

}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

void EC::removeSpringAgents(void) {

    int i, k, uptoS;
    vector<Spring*>::iterator T;

    int uptoL = Springs.size();

    Spring* stp;


    //go through all springs and remove spring agents from grid

    for (i = 0; i < uptoL; i++) {

        stp = Springs[i];
         if(stp==NULL) {

			T = Springs.begin();
			Springs.erase(T+i);
			i--;
			uptoL--;
		}
        //doesnt actually delete springs within new nodes function as it still has to go through all the springs so
        //would hit a spring that's been deleted, instead flags it as to be deleted and then deletes here

        if (stp->deletion == false) {
            //clearing the last arrangement of token postitions of agents from last removal ready to be reset in this round

            
            stp->filTokPos.clear();

            uptoS = stp->agents.size();

            for (k = 0; k < uptoS; k++) {

                worldP->deleteOldGridRef(stp->agents[k], true);

                //upon removal store info about this agents tokens in the correct position in the correct spring,
                //so they can be redistributed local to that location, also reset that spring to be in state ungridded,
                //so it will be filled up with agents on next timestep

                stp->filTokPos.push_back(stp->agents[k]->filTokens);

                stp->oldSize++;
                delete stp->agents[k];

            }
            stp->agents.clear();
        }

    }

    //then remove all pointers from vector
    springAgents.clear();

    //remove surface agents*********************
    if (on_the_fly_surface_agents == false) {
        MemAgent* sp;
        //remove all surface agents from grid
        for (i = 0; i < (int) surfaceAgents.size(); i++) {

            sp = surfaceAgents[i];
            worldP->deleteOldGridRef(sp, false);

            delete sp;
        }

        surfaceAgents.clear();
    }

}
//-------------------------------------------------------------------------------------------------------------

MemAgent* MemAgent::findConnectedNode(bool Hor1, bool Right1, bool Hor2, bool Right2, int distance) {
    int i;
    MemAgent* chosen = NULL;
    MemAgent* checkPoint = NULL;
    MemAgent* next;
    int flag = 0;
    int flag2=0;

    if (distance == 2) {
        for (i = 0; i < neighs; i++) {
            if (SpringNeigh[i]->filopodia == false) {
                if ((SpringNeigh[i]->right == Right1) && (SpringNeigh[i]->horizontal == Hor1)) {
                    checkPoint = neigh[i];
                }
            }
        }

        

        if (checkPoint != NULL) {
            
            if(checkPoint->mesh3SpringsOnly==true){
                
            do{
                next = checkPoint;
                flag2=0;
            for (i = 0; i < next->neighs; i++) {
            if (next->SpringNeigh[i]->filopodia == false) {
                if ((next->SpringNeigh[i]->right == Right1) && (next->SpringNeigh[i]->horizontal == Hor1)) {
                    checkPoint = next->neigh[i];
                    flag2=1;
                }
            }
        }
            if(checkPoint->mesh3SpringsOnly==false) flag=1;
                if(flag2==0) flag=1;
                
            }while(flag==0);
            }
            
            flag=0;

            flag2=0;
            for (i = 0; i < checkPoint->neighs; i++) {
                if (checkPoint->SpringNeigh[i]->filopodia == false) {
                    if ((checkPoint->SpringNeigh[i]->right == Right2) && (checkPoint->SpringNeigh[i]->horizontal == Hor2)) {
                        chosen = checkPoint->neigh[i];
                    }
                }
            }

            //for case 2 and 4 it wants to connect to a 3mesher, only go through to 4mesher for other cases...
            if((chosen!=NULL)&&(Hor1==false)&&(Right1==true)&&(Hor2 ==true)&&(Right2==true)){}
            else if((chosen!=NULL)&&(Hor1==false)&&(Right1==false)&&(Hor2 ==true)&&(Right2==false)){

            }
            else{

            if ((chosen!=NULL)&&(chosen->mesh3SpringsOnly == true)) {
                next = chosen;

                do {
                    flag2=0;
                    for (i = 0; i < next->neighs; i++) {
                        if (next->SpringNeigh[i]->filopodia == false) {
                            if ((next->SpringNeigh[i]->right == Right2) && (next->SpringNeigh[i]->horizontal == Hor2)) {
                                chosen = next->neigh[i];
                                flag2=1;
                            }
                        }
                    }

                    //hasnt found a node to connect to
                    if(flag2==0) flag=1;
                    if(chosen->mesh3SpringsOnly==true) next = chosen;
                    else flag=1;

                    }
                    while (flag == 0);

                }
            }
        }
            //------------------------------------------------------------------------------------------
        } else {

            for (i = 0; i < neighs; i++) {
                if (SpringNeigh[i]->filopodia == false) {
                    if ((SpringNeigh[i]->right == Right1) && (SpringNeigh[i]->horizontal == Hor1)) {
                        chosen = neigh[i];
                    }
                }
            }
        }

        return (chosen);
    }
    //-------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------

    vector <MemAgent*> MemAgent::three_erTriangleFix(void) {

        MemAgent* nodeA;
        MemAgent* nodeB;
        bool missingHor, missingRight;
        vector <MemAgent*> answer;
        int i, j;
        //if node has only 3 springs coming from it, due to insertion of a new row in recruit nodes, cell growth,
        //it means a surface triangle will be missing

        //then find which spring is missing.. this tells which of the four cases it is, and then which nodes to pick to make a fix triangle
        //see VBL book 1 page 136-137 for explanantion of cases.

        //find missing spring

        int HorC = 0;
        int VerC = 0;
        int LeftC = 0;
        int RightC = 0;


        //go through mesh springs, store which Hor, Right is missing.
        for (j = 0; j < neighs; j++) {
            if (SpringNeigh[j]->horizontal == true) HorC++;
            else VerC++;
            if (SpringNeigh[j]->right == true) RightC++;
            else LeftC++;

        }

        //cout<<HorC<<" "<<VerC<<" "<<RightC<<" "<<LeftC<<endl;
        if (HorC != 2) missingHor = true;
        else missingHor = false;
        if (RightC != 2) missingRight = true;
        else missingRight = false;

        //setup cases to give correct nodes to connect to.

        //CASE 1
        if ((missingHor == false) && (missingRight == false)) {

            
            nodeB = findConnectedNode(true, false, false, false, 2);
            nodeA = findConnectedNode(true, true, false, false, 1);


        }

        //CASE 2
        if ((missingHor == true) && (missingRight == true)) {

            nodeB = findConnectedNode(false, false, false, false, 1);
            nodeA = findConnectedNode(false, true, true, true, 2);

        }

        //CASE 3
        if ((missingHor == false) && (missingRight == true)) {

            nodeA = findConnectedNode(true, false, false, false, 1);
            nodeB = findConnectedNode(true, true, false, true, 2);

        }

        //CASE 4
        if ((missingHor == true) && (missingRight == false)) {

            nodeA = findConnectedNode(false, false, true, false, 2);
            nodeB = findConnectedNode(false, true, false, false, 1);

        }

        answer.push_back(this);

        if ((nodeA != NULL) && (nodeA->Cell == Cell)) answer.push_back(nodeA);
        if ((nodeB != NULL) && (nodeB->Cell == Cell)) answer.push_back(nodeB);


        //labelled = true;
        return (answer);


    }
    //-------------------------------------------------------------------------------------------------------------

    vector <MemAgent*> MemAgent::getTriangle(bool up) {

        MemAgent* checkPoint;
        MemAgent* diagPoint;
        vector <MemAgent*> triangle;
        int i, j;
        //divide up squares between four nodes into two triangles so as to cover the surface with agents.
        //notes in soph's nice book pg.
        // this-0----0 CP
        //      |\ U |
        //      | \  |
        //      |D \ |
        //      0----0 DP

        triangle.push_back(this);
        //upper triangle
        if (up == true) {

            for (i = 0; i < neighs; i++) {
                if ((SpringNeigh[i]->Junction == false) && (SpringNeigh[i]->filopodia == false)) {
                    if ((SpringNeigh[i]->right == true) && (SpringNeigh[i]->horizontal == true)) {
                        checkPoint = neigh[i];


                        triangle.push_back(checkPoint);


                        for (j = 0; j < neigh[i]->neighs; j++) {
                            if (neigh[i]->SpringNeigh[j] == NULL)neigh[i]->neighs--;
                            else {
                                if ((neigh[i]->SpringNeigh[j]->Junction == false) && (neigh[i]->SpringNeigh[j]->filopodia == false)) {
                                    if ((neigh[i]->SpringNeigh[j]->right == true) && (neigh[i]->SpringNeigh[j]->horizontal == false)) {
                                        diagPoint = neigh[i]->neigh[j];



                                        triangle.push_back(diagPoint);

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }            //downward triangle
        else {
            for (i = 0; i < neighs; i++) {
                if ((SpringNeigh[i]->Junction == false) && (SpringNeigh[i]->filopodia == false)) {
                    if ((SpringNeigh[i]->right == true) && (SpringNeigh[i]->horizontal == false)) {
                        checkPoint = neigh[i];


                        triangle.push_back(checkPoint);


                        for (j = 0; j < neigh[i]->neighs; j++) {
                            if (neigh[i]->SpringNeigh[j] == NULL)neigh[i]->neighs--;
                            else {
                                if ((neigh[i]->SpringNeigh[j]->Junction == false) && (neigh[i]->SpringNeigh[j]->filopodia == false)) {
                                    if ((neigh[i]->SpringNeigh[j]->right == true) && (neigh[i]->SpringNeigh[j]->horizontal == true)) {
                                        diagPoint = neigh[i]->neigh[j];


                                        triangle.push_back(diagPoint);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }



        return (triangle);
    }
    //-------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------

    bool World::test_triange_Pos_change(vector<MemAgent*> triangle) {

        //int version = 0;

        //basic test - just movement of the root memAgent
        //if (version == 0) {
            if (((int) triangle[0]->Mx == (int) triangle[0]->previous.x) && ((int) triangle[0]->My == (int) triangle[0]->previous.y) && ((int) triangle[0]->Mz == (int) triangle[0]->previous.z)) {
                if (((int) triangle[1]->Mx == (int) triangle[1]->previous.x) && ((int) triangle[1]->My == (int) triangle[1]->previous.y) && ((int) triangle[1]->Mz == (int) triangle[1]->previous.z)) {
                    if (((int) triangle[2]->Mx == (int) triangle[2]->previous.x) && ((int) triangle[2]->My == (int) triangle[2]->previous.y) && ((int) triangle[2]->Mz == (int) triangle[2]->previous.z)) {
                        //cout<<"no need"<<endl;
                        return (false);
                    } else return (true);
                } else return (true);
            } else return (true);
        //}

    }
    //-------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------

    bool MemAgent::VoxTriag_sizeTest(vector <Coordinates> nodes) {

        bool allow;
        float dist1, dist2, dist3;
        //spring lengths (distance) should all be < or = to 1???
        dist1 = worldP->getDist((int) nodes[0].x, (int) nodes[0].y, (int) nodes[0].z, (int) nodes[1].x, (int) nodes[1].y, (int) nodes[1].z);

        dist2 = worldP->getDist((int) nodes[0].x, (int) nodes[0].y, (int) nodes[0].z, (int) nodes[2].x, (int) nodes[2].y, (int) nodes[2].z);

        dist3 = worldP->getDist((int) nodes[1].x, (int) nodes[1].y, (int) nodes[1].z, (int) nodes[2].x, (int) nodes[2].y, (int) nodes[2].z);

        //cout<<dist1<<" "<<dist2<<" "<<dist3<<endl;
        //what if two of them are in the same grid site

        //for a triange of neighbours in same plane
        if (dist1 == 1) {
            if (dist2 == 1) {
                if (dist3 == (float) sqrt(2)) allow = true;
                else allow = false;
            } else if (dist2 == (float) sqrt(2)) {
                if (dist3 == 1) allow = true;
                else allow = false;
            } else allow = false;

        } else if (dist1 == (float) sqrt(2)) {
            if ((dist2 == 1) && (dist3 == 1)) allow = true;
            else allow = false;
        } else allow = false;

        if (allow == true) return (true);
        else {

            //for triangle with tweo nodes in line and other one layer below offset but all nieghs
            if (worldP->round_decimalPlaces(dist1 + dist2 + dist3, 0.0001) == worldP->round_decimalPlaces(1 + 1.41421 + 1.73205, 0.0001)) allow = true;
            else allow = false;

        }

        if (allow == true) return (true);
        else {

            //for triangle with all nodes only sharing  a vertex, across different layers and y and x
            if (worldP->round_decimalPlaces(dist1 + dist2 + dist3, 0.0001) == worldP->round_decimalPlaces(1.41421 * 3, 0.0001)) allow = true;
            else allow = false;

        }
        if (allow == true) return (true);
        else {
            if (dist1 < 1) {
                if ((dist2 == 1) || (dist2 == 1.41421) || (dist2 == 1.73205)) allow = true;
                else allow = false;
            }
            if (dist2 < 1) {
                if ((dist3 == 1) || (dist3 == 1.41421) || (dist3 == 1.73205)) allow = true;
                else allow = false;
            }
            if (dist3 < 1) {
                if ((dist1 == 1) || (dist1 == 1.41421) || (dist1 == 1.73205)) allow = true;
                else allow = false;
            }

        }

        if (allow == true) return (true);
        return (false);




        //check first with second
        //overlap

        //if()

        //neighbour

        //check first with third

        //check second with third




    }

    //-------------------------------------------------------------------------------------------------------------

    //-------------------------------------------------------------------------------------------------------------

    void MemAgent::gridSurfaceAgents(void) {

        int i, j, k;

        vector<Coordinates> Coords;
        Coordinates current;
        bool up = true;
        bool down = false;
        std::vector<MemAgent*>::iterator T;
        int flagS = 0;
        current.x = Mx;
        current.y = My;
        current.z = Mz;
        int flagFound = 0;
        int FlagS = 0;
        int flagDo = 0;
        int flagA, flagB;
        bool changed;
        std::vector <MemAgent*> triangleAgents;
        std::vector <MemAgent*> triangleNodes;
        int t;
        //cout<<"actual"<<Mx<<" "<<My<<" "<<Mz<<endl;
        //cout<<"current"<<current.x<<" "<<current.y<<" "<<current.z<<endl;
        bool dontDo;

       //need to change for on the fly voxelisation in diffAd...
surfaceAgentsUP.clear();
                    surfaceAgentsDOWN.clear();
                    surfaceAgents3mesher.clear();
        for (i = 0; i < 3; i++) {
            flagDo = 0;
            dontDo = false;
            //only for hexagonal cells
            //if((labelled2==true)&&(i==0)) dontDo=true;
            //if((labelled==true)&&(i==1)) dontDo=true;

            if (dontDo == false) {
                //if(i==2){
                if (i == 0) {
                    //for (t = 0; t < surfaceAgentsUP.size(); t++)
                    //    triangleAgents.push_back(surfaceAgentsUP[t]);

                    triangleNodes = getTriangle(up);
                } else if(i==1) {
                   // for (t = 0; t < surfaceAgentsDOWN.size(); t++)
                     //   triangleAgents.push_back(surfaceAgentsDOWN[t]);

                    triangleNodes = getTriangle(down);
                }
                else{ 
                                if(mesh3SpringsOnly==true){
                                    triangleNodes = three_erTriangleFix();
                                    
                              //      for (t = 0; t < surfaceAgents3mesher.size(); t++)
                       // triangleAgents.push_back(surfaceAgents3mesher[t]);
                                }

                            }


                if (on_the_fly_surface_agents == true) {

                    changed = false;
                    if (triangleNodes.size() == 3) changed = worldP->test_triange_Pos_change(triangleNodes);
                    //---------------------------------------------------------------------------------------
                    //on the fly voxelisation of only triangles that have changed. not fool proof - misses some, cant use on the fly in actual simulations
                    if ((triangleAgents.size() > 0) && (worldP->timeStep > 0) && (changed == true)) {
                        //cout<<"changed is true"<<endl;
                        //remove the surface agents and re voxelise as triangle has moved, otherwise dont bother doing anything, leave spring agents as they are
                        //remove all surface agents from grid


                        j = 0;
                        do {

                            k = 0;
                            flagFound = 0;
                            //if(Cell->surfaceAgents.size()>0){
                            do {

                                if (Cell->surfaceAgents[j] == triangleAgents[k]) {

                                    T = Cell->surfaceAgents.begin();


                                    Cell->surfaceAgents.erase(T + j);

                                    j--;

                                    worldP->deleteOldGridRef(triangleAgents[k], false);

                                    T = triangleAgents.begin();

                                    delete triangleAgents[k];
                                    triangleAgents.erase(T + k);
                                    k--;

                                    flagFound = 1;
                                }
                                k++;
                            } while ((k < triangleAgents.size()) && (flagFound == 0));
                            //}

                            if (triangleAgents.size() == 0) FlagS = 1;
                            j++;

                        } while ((j < (int) Cell->surfaceAgents.size()) && (flagS == 0));

                        if (i == 0) {

                            surfaceAgentsUP.clear();
                        } else {

                            surfaceAgentsDOWN.clear();
                        }
                    }


                    if ((worldP->timeStep == 0) || (changed == true) || ((changed == false) && (triangleAgents.size() == 0))) {
                        //cout<<"trying to voxelise"<<endl;
                        //some on the edge wont have a surface to cover - not covering junctions?
                        if (triangleNodes.size() == 2)
                            if ((worldP->getDist(current.x, current.y, current.z, triangleNodes[1]->Mx, triangleNodes[1]->My, triangleNodes[1]->Mz) > 1.0)) {
                                //figure out the procedure here - for ones where there isnt a diag point to join the triangle to - still need to poss cover this spring using the original spring coverage method - only an issue in wierd initial set ups or when cell growth implemented
                                //worldP->gridTriangle(current, coords[0], NULL);
                                //coords = nodeAgents[i]->getTriangle(down)
                            }
                        if (triangleNodes.size() == 3) {
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

                            second.x = triangleNodes[1]->Mx;
                            second.y = triangleNodes[1]->My;
                            second.z = triangleNodes[1]->Mz;

                            third.x = triangleNodes[2]->Mx;
                            third.y = triangleNodes[2]->My;
                            third.z = triangleNodes[2]->Mz;

                            Coords.push_back(current);
                            Coords.push_back(second);
                            Coords.push_back(third);


                            worldP->voxeliseTriangle(Coords, triangleNodes, Cell, i);

                            //cout<<"voxelised"<<endl;
                            Coords.clear();


                            //}
                            //}

                        }
                    }

                } else {
                    
                    //some on the edge wont have a surface to cover - not covering junctions?
                    if (triangleNodes.size() == 2)
                        if ((worldP->getDist(current.x, current.y, current.z, triangleNodes[1]->Mx, triangleNodes[1]->My, triangleNodes[1]->Mz) > 1.0)) {
                            //figure out the procedure here - for ones where there isnt a diag point to join the triangle to - still need to poss cover this spring using the original spring coverage method - only an issue in wierd initial set ups or when cell growth implemented
                            //worldP->gridTriangle(current, coords[0], NULL);
                            //coords = nodeAgents[i]->getTriangle(down)
                        }
                    if (triangleNodes.size() == 3) {
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
                        if (VoxTriag_sizeTest(Coords) == false) {
                            // cout<<"false";
                            //int B;
                            //for(B=0;B<3;B++)
                            //cout<<"coords "<<B<<": "<<Coords[i].x<<" "<<Coords[i].y<<" "<<Coords[i].z<<" "<<endl;

                            worldP->voxeliseTriangle(Coords, triangleNodes, Cell, i);
                        }
                        Coords.clear();

                        //}
                        //}
                    }
                }

                triangleNodes.clear();
                triangleAgents.clear();
            }
        }
        //}
    }

    //-------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------

    void World::voxeliseTriangle(vector<Coordinates> Coords, vector<MemAgent*> triangleNodes, EC* cell, int up) {

        int* range;
        int X, Y, Z;
        int i;
        bool dont_bother = false;
        int flag = 0;

        bool grid_it;
        bool toroidal = false;
        int diff;
        int r0;
        int flagDone = 0;
        bool really_dont_bother;
        int flagA, flagB;



        //first walk along each triangle edge and place surace agents there... then voxelise triangle interior...

        gridSurfaceTriangleEdges(Coords[0], Coords[1], cell, triangleNodes, up);
        gridSurfaceTriangleEdges(Coords[1], Coords[2], cell, triangleNodes, up);
        gridSurfaceTriangleEdges(Coords[0], Coords[2], cell, triangleNodes, up);

        range = findRange(Coords);

        if (abs(range[0] - range[1]) >= xMAX / 2) {

            diff = xMAX - abs(range[0] - range[1]);
            range[0] = range[1];
            range[1] = range[0] + diff;
            r0 = range[0];
            toroidal = true;

        } else {


            for (X = range[0]; X <= range[1]; X++) {
                for (Y = range[2]; Y <= range[3]; Y++) {
                    for (Z = range[4]; Z <= range[5]; Z++) {

                        if (toroidal == true) {
                            if (X >= xMAX) {
                                range[1] = r0;
                                range[0] = 0;
                                X = range[0];
                            }
                        }

                        dont_bother = false;
                        really_dont_bother = false;
                        //if(grid[X][Y][Z].type==AS) dont_bother=true;

                        //-------------------------------------------------------
                        //for on-the fly-- if another memagent there already , from same cell, then double up - flag as doubled up and remove after...
                        if (grid[X][Y][Z].type == M) {
                            i = 0;
                            flag = 0;

                            do {
                                if (grid[X][Y][Z].Mids[i]->Cell == cell) {
                                    flag = 1;
                                    dont_bother = true;
                                    //cout<<"got a double up!"<<endl;
                                    /* if((grid[X][Y][Z].Mids[i]->node==true)||(grid[X][Y][Z].Mids[i]->FIL==STALK)) really_dont_bother=true;
                                     //1)check if it was created this timestep then it wont be dleted, and no need to grid this one
                                     else if(grid[X][Y][Z].Mids[i]->created==timeStep) really_dont_bother = true;
                                     //2) if not check if this one is likely to get deleted later - i.e. has its triangle pos changed?
                                     else{
                                        // cout<<"old one"<<endl;

                                         if(test_triange_Pos_change(grid[X][Y][Z].Mids[i]->triangle)==false) really_dont_bother = true;
                                     }*/
                                }
                                i++;
                            } while ((i < (int) grid[X][Y][Z].Mids.size()) && (flag == 0));

                            //if (flag == 1)
                            //  dont_bother = true;
                        }


                        if (dont_bother == false) {


                            countbell++;
                            //in same plane

                            grid_it = voxelise(Coords, (float) X + 0.5, (float) Y + 0.5, (float) Z + 0.5);


                            if (grid_it == true) {
                                flagDone = 1;
                                createSurfaceAgent(X, Y, Z, cell, triangleNodes, up);
                                //memp->JunctionTest();
                            }
                        }
                    }
                }
            }
        }
        //cout<<"in here"<<endl;
        /*if(flagDone==0){
            flagA=0;
            flagB=0;
            count2bell++;
                            if(sqrt((Coords[0].x-Coords[1].x)*(Coords[0].x-Coords[1].x))>(float)xMAX/2.0f){
                                    if(Coords[0].x>Coords[1].x) Coords[0].x-=xMAX;
                                    else Coords[1].x-=xMAX;
                            }
                            if(getDist(fabs(Coords[0].x),fabs(Coords[0].y),fabs(Coords[0].z),fabs(Coords[1].x),fabs(Coords[1].y),fabs(Coords[1].z ))>1.1f) got2++;
                            else{

                            if(flagA==1) Coords[0].x+=xMAX;
                            else if(flagB==1) Coords[1].x+=xMAX;

                            if(sqrt((Coords[1].x-Coords[2].x)*(Coords[1].x-Coords[2].x))>(float)xMAX/2.0f){
                                    if(Coords[1].x>Coords[2].x) Coords[1].x-=xMAX;
                                    else Coords[2].x-=xMAX;
                            }
                                    if(getDist(fabs(Coords[1].x),fabs(Coords[1].y),fabs(Coords[1].z),fabs(Coords[2].x),fabs(Coords[2].y),fabs(Coords[2].z ))>1.1f){
                                            got2++;
                                    }
                                    else
                                    notGot2++;
                                    }
                      }
                     else{
                                            flagA=0;
            flagB=0;
                                            countbell++;
                                            if(sqrt((Coords[0].x-Coords[1].x)*(Coords[0].x-Coords[1].x))>(float)xMAX/2.0f){
                                            if(Coords[0].x>Coords[1].x){
                                            flagA=1; Coords[0].x-=xMAX;}
                                    else{
                                    flagB=1; Coords[1].x-=xMAX;}
                            }
                            if(getDist(fabs(Coords[0].x),fabs(Coords[0].y),fabs(Coords[0].z),fabs(Coords[1].x),fabs(Coords[1].y),fabs(Coords[1].z ))>1.1f) got++;
                            else{

                            if(flagA==1) Coords[0].x+=xMAX;
                            else if(flagB==1) Coords[1].x+=xMAX;
                            flagA=0;
                              flagB=0;
                            if(sqrt((Coords[1].x-Coords[2].x)*(Coords[1].x-Coords[2].x))>(float)xMAX/2.0f){
                                    if(Coords[1].x>Coords[2].x){
                                    flagA=1;Coords[1].x-=xMAX;
                                    }
                                    else{
                                    flagB=1;
                                    Coords[2].x-=xMAX;
                                    }
                            }
                                    if(getDist(fabs(Coords[1].x),fabs(Coords[1].y),fabs(Coords[1].z),fabs(Coords[2].x),fabs(Coords[2].y),fabs(Coords[2].z ))>1.1f){
                                            got++;
                                    }
                                    else{
                                    //cout<<flagA<<" "<<flagB<<" "<<Coords[0].x<<" "<<Coords[1].x<<" "<<Coords[2].x<<" "<<endl;
                                    //cout<<Coords[0].y<<" "<<Coords[1].y<<" "<<Coords[2].y<<" "<<endl;
                                    //cout<<Coords[0].z<<" "<<Coords[1].z<<" "<<Coords[2].z<<" "<<endl;
                                    notGot++;
                                    }
                                    }
                                    }
         */

        delete[] range;
    }
    //-------------------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------------------

    void EC::remove_DoubledUp_SurfaceAgents(void) {

        int i, j, S;
        bool flag = false;
        MemAgent* mp;
        MemAgent* memp;
        std::vector<MemAgent*>::iterator T;
        int flagFound = 0;

        for (i = 0; i < doubled_up_surface_agents_list.size(); i++) {

            mp = doubled_up_surface_agents_list[i];

            for (j = 0; j < worldP->grid[(int) mp->Mx][(int) mp->My][(int) mp->Mz].Mids.size(); j++) {
                memp = worldP->grid[(int) mp->Mx][(int) mp->My][(int) mp->Mz].Mids[j];

                if ((memp->Cell == mp->Cell) && (memp != mp)) {

                    S = 0;
                    flagFound = 0;
                    //delete extra ones...
                    do {

                        if (mp->Cell->surfaceAgents[S] == memp) {

                            T = mp->Cell->surfaceAgents.begin();


                            mp->Cell->surfaceAgents.erase(T + S);

                            S--;

                            worldP->deleteOldGridRef(memp, false);

                            delete memp;
                            flagFound = 1;

                        }
                        S++;
                    } while ((S < surfaceAgents.size()) && (flagFound == 0));

                }
            }

        }
    }
    //-------------------------------------------------------------------------------------------------------------

    void World::gridSurfaceTriangleEdges(Coordinates A, Coordinates B, EC* cell, vector <MemAgent*> triangleNodes, int up) {

        float PN[3];
        float P[3];
        float N[3];
        float x1, y1, z1, x2, y2, z2;
        float steps = 0.5f;
        float x, y, z;
        int flag = 0;

        int i;
        int sumfilTok = 0;

        P[0] = A.x;
        P[1] = A.y;
        P[2] = A.z;

        N[0] = B.x;
        N[1] = B.y;
        N[2] = B.z;

        bool toroidal = toroidalTest(P, N);
        //wrap round for springs that cros x axis toroidal boundary-------------------------------------
        //displace N to outside of grid to calculate then create spring ni correct position
        if ((toroidal == true) && (N[0] > P[0])) {
            N[0] -= xMAX;
            flag = 1;
        } else if ((toroidal == true) && (N[0] < P[0])) {
            N[0] += xMAX;
            flag = 2;
        }
        //-------------------------------------------------------------------------------------------------------------

        //check if the grid sites inbetween along that spring are empty,

        PN[0] = P[0] - N[0];
        PN[1] = P[1] - N[1];
        PN[2] = P[2] - N[2];

        x1 = P[0];
        y1 = P[1];
        z1 = P[2];

        x2 = N[0];
        y2 = N[1];
        z2 = N[2];

        if ((int) x2 > (int) ((int) x1 + steps)) {
            x = (int) x1 + steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) x != (int) x2) {
                    if (x > x1) {
                        y = (((x - x1) / PN[0]) * PN[1]) + y1;
                        z = (((x - x1) / PN[0]) * PN[2]) + z1;

                        if ((x >= 0) && (x < xMAX)) createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                            //have to do the extra -1 here as otherwise it rounds -0.5 to 0 instead of -1..
                        else if (flag == 1) createSurfaceAgent((int) (x - 1) + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    x += steps;
                }

                //cout<<"x2>x1 ";
            } while ((int) x < (int) x2);

        } else if ((int) x2 < (int) ((int) x1 - steps)) {
            x = (int) x1 - steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) x != (int) x2) {
                    if (x < x1) {
                        y = (((x - x1) / PN[0]) * PN[1]) + y1;
                        z = (((x - x1) / PN[0]) * PN[2]) + z1;

                        if ((x >= 0) && (x < xMAX)) createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 1) createSurfaceAgent((int) x - 1 + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    x -= steps;
                }
                //cout<<"x2<x1 ";
            } while ((int) x > (int) x2);
        }

        if ((int) y2 > (int) ((int) y1 + steps)) {
            y = (int) y1 + steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) y != (int) y2) {
                    if (y > y1) {
                        x = (((y - y1) / PN[1]) * PN[0]) + x1;
                        z = (((y - y1) / PN[1]) * PN[2]) + z1;

                        if ((x >= 0) && (x < xMAX)) createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 1) createSurfaceAgent((int) x - 1 + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    y += steps;
                }
                // cout<<"y2>y1 ";
            } while ((int) y < (int) y2);
        } else if ((int) y2 < (int) ((int) y1 - steps)) {

            y = (int) y1 - steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) y != (int) y2) {
                    if (y < y1) {
                        x = (((y - y1) / PN[1]) * PN[0]) + x1;
                        z = (((y - y1) / PN[1]) * PN[2]) + z1;

                        if ((x >= 0) && (x < xMAX)) createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 1) createSurfaceAgent((int) x - 1 + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    y -= steps;
                }
                //cout<<"y2<y1 ";
            } while ((int) y > (int) y2);
        }
        if ((int) z2 > (int) ((int) z1 + steps)) {
            z = (int) z1 + steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) z != (int) z2) {
                    if (z > z1) {
                        x = (((z - z1) / PN[2]) * PN[0]) + x1;
                        y = (((z - z1) / PN[2]) * PN[1]) + y1;

                        if ((x >= 0) && (x < xMAX))createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 1) createSurfaceAgent((int) x - 1 + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    z += steps;
                } //cout<<"z2>z1 ";
            } while ((int) z < (int) z2);
        } else if ((int) z2 < (int) ((int) z1 - steps)) {

            z = (int) z1 - steps;
            //we know this is the axis along which the vector increases
            do {
                if ((int) z != (int) z2) {
                    if (z < z1) {
                        x = (((z - z1) / PN[2]) * PN[0]) + x1;
                        y = (((z - z1) / PN[2]) * PN[1]) + y1;

                        if ((x >= 0) && (x < xMAX)) createSurfaceAgent((int) x, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 1) createSurfaceAgent((int) x - 1 + xMAX, (int) y, (int) z, cell, triangleNodes, up);
                        else if (flag == 2) createSurfaceAgent((int) x - xMAX, (int) y, (int) z, cell, triangleNodes, up);
                    }
                    z -= steps;
                }
                //cout<<"z2<z1 ";
            } while ((int) z > (int) z2);
        }




    }
    //-------------------------------------------------------------------------------------------------------------

    void World::createSurfaceAgent(int X, int Y, int Z, EC* cell, vector<MemAgent*> triangleNodes, int up) {


        int i = 0;
        bool allow = true;
        if (grid[X][Y][Z].type == M) {
            do {
                if (grid[X][Y][Z].Mids[i]->Cell == cell) {
                    allow = false;
                }
                i++;
            } while ((i < (int) grid[X][Y][Z].Mids.size()) && (allow == true));

        }

        if (allow == true) {
            if (grid[X][Y][Z].type == E) {
                delete grid[X][Y][Z].Eid;
                grid[X][Y][Z].Eid = NULL;
            }



            MemAgent* memp = new MemAgent(cell, this);



            memp->Mx = (float) X;
            memp->My = (float) Y;
            memp->Mz = (float) Z;
            memp->created = timeStep;

            //set it to be a spring rather than node agent
            memp->node = false;
            //cout<<"b "<<endl;
            //add it to ECs list of spring agents
            cell->surfaceAgents.push_back(memp);

            //let it know which triangle its part of for token passing
            memp->triangle.push_back(triangleNodes[0]);
            memp->triangle.push_back(triangleNodes[1]);
            memp->triangle.push_back(triangleNodes[2]);

            if((triangleNodes[0]->neighs>100)||(triangleNodes[1]->neighs>100)||(triangleNodes[2]->neighs>100)) cout<<"aha! in ECmesh old prob.."<<endl;

            //if(memp->triangle.size()==0) cout<<"ohhhhhhhh"<<endl;
            //cout<<"c "<<endl;
            setMLocation(X, Y, Z, memp);

           

            if (up == 0) triangleNodes[0]->surfaceAgentsUP.push_back(memp);
            if (up == 1) triangleNodes[0]->surfaceAgentsDOWN.push_back(memp);
            if (up == 2) triangleNodes[0]->surfaceAgents3mesher.push_back(memp);

        }
    }

