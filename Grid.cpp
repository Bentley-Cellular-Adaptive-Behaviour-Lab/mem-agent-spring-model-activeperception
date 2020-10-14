

#include <math.h>
#include <iomanip>
#include "objects.h"
#include <fstream>
using namespace std;
//---------------------------------------------------------------------------------------------

void World::deleteOldGridRef(MemAgent* memp, bool fil) {

    vector<MemAgent*>::iterator T;
    int upto;
    int flagthere = 0;
    int j;

    if (fil == true) {

        upto = grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Fids.size();
        T = grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Fids.begin();
        j = 0;
        do {

            if (grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Fids[j] == memp) {
                grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Fids.erase(T + j);
                flagthere = 1;

            }
            j++;
        } while ((flagthere == 0) && (j < upto));

        if (flagthere == 0) {
            cout << "ohhh..." << endl;
        }
        /*if((grid[(int)memp->Mx][(int)memp->My][(int)memp->Mz].Fids.size()==0)&&(grid[(int)memp->Mx][(int)memp->My][(int)memp->Mz].type!=AS)){
         * grid[(int)memp->Mx][(int)memp->My][(int)memp->Mz].type = E;
         * createNewEnvAgent((int)memp->Mx, (int)memp->My, (int)memp->Mz);
         *
         * }*/


    }
    else {
        upto = grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Mids.size();
        T = grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Mids.begin();
        j = 0;
        do {
            if (grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Mids[j] == memp) {
                grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Mids.erase(T + j);
                flagthere = 1;
                //break;
            }
            j++;
        } while ((flagthere == 0) && (j < upto));
        if (flagthere == 0) cout << "not found agent in that grid site..." << endl;
        if (grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Mids.size() == 0) {
            //grid[(int)memp->Mx][(int)memp->My][(int)memp->Mz].type = E;
            if (grid[(int) memp->Mx][(int) memp->My][(int) memp->Mz].Eid != NULL) cout << "already got an env agent in deleteOldGridRef " << memp->junction << endl;
            createNewEnvAgent((int) memp->Mx, (int) memp->My, (int) memp->Mz);

        }
    }

    //error message
    if (flagthere == 0) {
        cout << "AAcant find the memAgent in that grid site" << endl;
        cout.flush();
    }
}

//------------------------------------------------------------

void World::setMLocation(int x, int y, int z, MemAgent * ident) {

    grid[x][y][z].Mids.push_back(ident);
    grid[x][y][z].type = M;
    delete grid[x][y][z].Eid;
    grid[x][y][z].Eid = NULL;
}
//------------------------------------------------------------
//------------------------------------------------------------

void World::setFilLocation(int x, int y, int z, MemAgent * ident) {


    grid[x][y][z].Fids.push_back(ident);
}
//------------------------------------------------------------
//------------------------------------------------------------

bool World::insideWorld(int i, int j, int k) {

    if ((i < 0) || (j < 0) || (k < 0) || (i >= xMAX) || (j >= yMAX) || (k >= zMAX)) return (false);
    else return (true);
}


int World::checkForAstro(int i, int j, int k) {
    int x = 0;
    int m, n, p;

    int flag = 0;
    //same layer
    do {
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
        } else if (x == 25) {
            m = i;
            n = j;
            p = k + 1;
        }


        //-------------------------------
        //toroidal only X
        //if(m>=xMAX) m=0;
        //if(m<0) m=xMAX-1;
        //-------------------------------

        if (insideWorld(m, n, p) == true) {

            if ((grid[m][n][p].type == E) && (grid[m][n][p].Eid->Astro == true)) {
                flag = 1;
                //cout<<"astro"<<endl;
            } else if (p == 0) {
                //may have a fil there even though it strictly speaking should have astro there - so do replace with the check only flag on so it doesnt actually replace
                if ((grid[m][n][p].type == E) && (grid[m][n][p].Eid->OldAstro == true)) flag = 1;
            }
        }
        //-------------------------------

        x++;

    } while ((x < NEIGH) && (flag == 0));

    return (flag);
}
//-------------------------------------------------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int Env::checkForEnv(void) {
    int x = 0;
    int m, n, p;
    int i = Ex;
    int j = Ey;
    int k = Ez;

    int flag = 0;
    //same layer
    do {
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
        } else if (x == 25) {
            m = i;
            n = j;
            p = k + 1;
        }


        //-------------------------------
        //toroidal only X
        if (m >= xMAX) m = 0;
        if (m < 0) m = xMAX - 1;
        //-------------------------------

        if (worldP->insideWorld(m, n, p) == true) {

            if ((worldP->grid[m][n][p].type == E) && (worldP->grid[m][n][p].Eid->VEGF > 0.0f)) {
                flag = 1;
            }
        }
        //-------------------------------

        x++;

    } while ((x < NEIGH) && (flag == 0));

    return (flag);
}
//-------------------------------------------------------------------------------

