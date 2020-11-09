#include <iostream>
#include <cstdlib>
#include <vector>
#include <string.h>
#include "objects.h"
#include <math.h>

using namespace std;
float storedOffset;
//-------------------------------------------------------------------------
void World::label_env_exposed_von_neu_agents(void){

    //not all memagents can activate their receptors, as membrane gets ruffled, those with diag env neighs only arent avble to "see enough" of the env to activate.
    //- this is in part to match with first version of model which was set up to not have any ruffled bits, all memagents
    //have to have a face adjacent to env grid site to 'see the vegf ligands' otherwise vertx is not enough... this leaves some vr-2 in reserve
    //in a sense, until they become exposed... although check because maybe we only allocate vr-2 to the von neu neihbours..in which case all vr2 is
    //available to be activated...
    int i,j,k;

 for(i=0;i<xMAX;i++){
            for(j=0;j<yMAX;j++){
                for(k=0;k<zMAX;k++){
                    if(grid[i][j][k].Eid!=NULL){

                        grid[i][j][k].Eid->calcInside();
                    }
                }
            }
        }
        
        int count=0;
        for(j=0;j<(int)ECagents.size();j++){
            count=0;
            for(k=0;k<ECagents[j]->nodeAgents.size();k++)
                if(ECagents[j]->nodeAgents[k]->checkNeighsVonForEnv()==true) count++;
            for(k=0;k<ECagents[j]->surfaceAgents.size();k++)
                if(ECagents[j]->surfaceAgents[k]->checkNeighsVonForEnv()==true) count++;


        }
}
        //-------------------------------------------------------------------------
void World::createMonolayer(void){

    int i,j, I;

    j=0;
    I=0;
    for(i=0;i<ECELLS;i++){

        //creates new object dynamically of type EC (ecp is the e cell pointer)
        EC * ecp = new EC((World*) this);

        //put the address into the vector Ecells
        ECagents.push_back(ecp);
		
     }

   
    int k;
    int chose;

     for(i=0;i<ECELLS;i++){
    
        if(I==MONOLAYERcells){
            I=0;
          j++;
        }
        k=j%2;
        

       create_2D_square_cell(i, (int)(Medium_width+(ECwidth/2.0f)+(I*ECwidth)),(int)(Medium_width+(ECwidth/2.0f)+(j*ECwidth)));
       
        I++;
    }

   
    connectMonolayer();

    //check for junctions to make junction springs
    for(i=0;i<ECagents.size();i++){
        for(j=0;j<ECagents[i]->nodeAgents.size();j++){
            ECagents[i]->nodeAgents[j]->connectJunctions(false);
        }

        
    }


}
//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------
void MemAgent::connectJunctions(bool alsoNormalSprings){

   

     int x=0;
    int flag=0;
    int m, n, p,s;
    float sumV=0;

    int i, j, k;
    i=Mx; j=My; k=Mz;

    //same layer
    for(x=0;x<6;x++){
        if(x==0){ m=i;n=j+1;p=k;}
        else if(x==1){ m=i;n=j-1;p=k;  }
        else if(x==2){ m=i+1;n=j;p=k;  }
        else if(x==3){ m=i;n=j;p=k+1;  }
        else if(x==4){ m=i;n=j;p=k-1;  }
        else if(x==5){ m=i-1;n=j;p=k;  }


        //-------------------------------
        //toroidal only X
        if(m>=xMAX) m=0;
        if(m<0) m=xMAX-1;
        if(n>=yMAX) n=0;
        if(n<0) n=yMAX-1;

        if(worldP->insideWorld(m, n, p)==true){
            if(worldP->grid[m][n][p].type==M){
                for(s=0;s<worldP->grid[m][n][p].Mids.size();s++){
                    if(worldP->grid[m][n][p].Mids[s]->Cell!=Cell){
                        
                        
                        neigh[neighs]=worldP->grid[m][n][p].Mids[s];
                Cell->createSpringTokenObject(this, worldP->grid[m][n][p].Mids[s] , neighs);
                
               

                     if(x==0){ Cell->Springs[Cell->Springs.size()-1]->horizontal = false;
                Cell->Springs[Cell->Springs.size()-1]->right = false; }

                
        else if(x==1){ Cell->Springs[Cell->Springs.size()-1]->horizontal = false;
                Cell->Springs[Cell->Springs.size()-1]->right = true;  }
        else if(x==2){ Cell->Springs[Cell->Springs.size()-1]->horizontal = true
                ;
                Cell->Springs[Cell->Springs.size()-1]->right = true;  }
                        else if(x==3){ Cell->Springs[Cell->Springs.size()-1]->horizontal = false;
                Cell->Springs[Cell->Springs.size()-1]->right = false;  }
        else if(x==4){Cell->Springs[Cell->Springs.size()-1]->horizontal = false;
                Cell->Springs[Cell->Springs.size()-1]->right = false; }
        else if(x==5){ Cell->Springs[Cell->Springs.size()-1]->horizontal = true;
               Cell->Springs[Cell->Springs.size()-1]->right = false;  }
               

        neighs++;
                    }
                }
                if(alsoNormalSprings==true){
                    if(worldP->grid[m][n][p].Mids[s]->Cell==Cell){


                        neigh[neighs]=worldP->grid[m][n][p].Mids[s];
                Cell->createSpringTokenObject(this, worldP->grid[m][n][p].Mids[s] , neighs);



                      if(x==0){ SpringNeigh[neighs]->horizontal = false;
                SpringNeigh[neighs]->right = false; }
        else if(x==1){ SpringNeigh[neighs]->horizontal = false;
                SpringNeigh[neighs]->right = true;  }
        else if(x==2){ SpringNeigh[neighs]->horizontal = true;
                SpringNeigh[neighs]->right = true;  }
        else if(x==3){ SpringNeigh[neighs]->horizontal = true;
                SpringNeigh[neighs]->right = false;  }
        else if(x==4){SpringNeigh[neighs]->horizontal = false;
                SpringNeigh[neighs]->right = false; }
        else if(x==5){ SpringNeigh[neighs]->horizontal =true;
                SpringNeigh[neighs]->right = false;  }

        neighs++;
                    }

                }
            }
        }

    }

}

//-----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------

void World::createHexagonCell(int cell, int centreX, int centreY){

     int i, j;
     int I,J;

     I=0;
    MemAgent* memp;

    cout<<(int)(centreX-(float)ECwidth/2.0f)+2<<" "<<(int)(centreX+(float)ECwidth/2.0f)-2-1<<endl;

    for(i=(int)(centreX-(float)ECwidth/2.0f);i<(int)(centreX+(float)ECwidth/2.0f)-1;i++){
        for(j=(int)(centreY-(float)ECwidth/2.0f);j<(int)(centreY+(float)ECwidth/2.0f);j++){

            if((I==0)||(I==6)) J=2;
            else if((I==1)||(I==5)) J=1;
            else J=0;

            if(I==3)cout<<j<<" "<<J<<" "<<I<<(int)(centreX-(float)ECwidth/2.0f)+J<<" "<<(int)(centreX+(float)ECwidth/2.0f)-J-1<<endl;
            if((j>=(int)(centreY-(float)ECwidth/2.0f)+J)&&(j<(int)(centreY+(float)ECwidth/2.0f)-J)){
            //    cout<<i<<" "<<J<<" "<<I<<endl;
            memp = new MemAgent(ECagents[cell], (World*) this);
            memp->Mx = (float)i;
            memp->My = (float)j;
            memp->Mz = 0.0f;
            ECagents[cell]->nodeAgents.push_back(memp);
            setMLocation(int(i), int(j), 0, memp);
            memp->node=true;
            //will need to generalise this if diff sized cells - see VBL book 1 pg 78 for number refs
            if(((I==0))&&(j==(int)(centreY-(float)ECwidth/2.0f)+2)) memp->labelled=true;
            if(((I==1))&&(j==(int)(centreY-(float)ECwidth/2.0f)+1)) memp->labelled=true;
            if(((I==4))&&(j==(int)(centreY-(float)ECwidth/2.0f)+7)) memp->labelled2=true;
            if(((I==5))&&(j==(int)(centreY-(float)ECwidth/2.0f)+6)) memp->labelled2=true;
            }
            
        }
        I++;
        cout<<I<<endl;
    }

    //connect mesh
    /*int uptoM=ECagents[cell]->nodeAgents.size();
    MemAgent* mp;
    MemAgent* nmp;
    int N;
    int kelp;

    for(i=0;i<uptoM;i++){
        mp = ECagents[cell]->nodeAgents[i];
        N=0;

        if(insideWorld(mp->Mx-1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx-1][(int)mp->My][1].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx-1][(int)mp->My][1].Mids.size();kelp++){
                nmp = grid[(int)mp->Mx-1][(int)mp->My][1].Mids[kelp];

                mp->neigh[N]=nmp;
                ECagents[cell]->createSpringTokenObject(mp, nmp, N);
                mp->SpringNeigh[N]->horizontal = true;
                mp->SpringNeigh[N]->right = false;
                N++;
                mp->neighs++;
            }
        }
        }
        if(insideWorld(mp->Mx+1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx+1][(int)mp->My][1].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx+1][(int)mp->My][1].Mids.size();kelp++){
            nmp = grid[(int)mp->Mx+1][(int)mp->My][1].Mids[kelp];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
            }
        }
        }
        if((insideWorld(mp->Mx, mp->My-1, mp->Mz)==true)){
        if(grid[(int)mp->Mx][(int)mp->My-1][1].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx][(int)mp->My-1][1].Mids.size();kelp++){
            nmp = grid[(int)mp->Mx][(int)mp->My-1][1].Mids[kelp];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
            }
        }
        }
        if((insideWorld(mp->Mx, mp->My+1, mp->Mz)==true)){
        if(grid[(int)mp->Mx][(int)mp->My+1][1].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx][(int)mp->My+1][1].Mids.size();kelp++){
            nmp = grid[(int)mp->Mx][(int)mp->My+1][1].Mids[kelp];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
            }
        }
        }
    }*/
}

//-------------------------------------------------------------------------
//connect mesh
void World::connectMonolayer(void){
    int e,i;
    int uptoM;
    MemAgent* mp;
    MemAgent* nmp;
    int N;
    int kelp;

    for(e=0;e<ECELLS;e++){

        uptoM=ECagents[e]->nodeAgents.size();

    for(i=0;i<uptoM;i++){
        mp = ECagents[e]->nodeAgents[i];
        N=0;


        if(insideWorld(mp->Mx-1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx-1][(int)mp->My][0].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx-1][(int)mp->My][0].Mids.size();kelp++){

                nmp = grid[(int)mp->Mx-1][(int)mp->My][0].Mids[kelp];
                if(mp->Cell==nmp->Cell){
                mp->neigh[N]=nmp;
                ECagents[e]->createSpringTokenObject(mp, nmp, N);
                mp->SpringNeigh[N]->horizontal = true;
                mp->SpringNeigh[N]->right = false;
                N++;
                mp->neighs++;
                }
            }
        }
        }
        /*else{
        
            if(grid[xMAX-1][(int)mp->My][0].type==M){
            for(kelp=0;kelp<grid[xMAX][(int)mp->My][0].Mids.size();kelp++){

                nmp = grid[(int)mp->Mx-1][(int)mp->My][0].Mids[kelp];
                if(mp->Cell==nmp->Cell){
                mp->neigh[N]=nmp;
                ECagents[e]->createSpringTokenObject(mp, nmp, N);
                mp->SpringNeigh[N]->horizontal = true;
                mp->SpringNeigh[N]->right = false;
                N++;
                mp->neighs++;
                }
            }
        }
        }*/
        if(insideWorld(mp->Mx+1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx+1][(int)mp->My][0].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx+1][(int)mp->My][0].Mids.size();kelp++){

            nmp = grid[(int)mp->Mx+1][(int)mp->My][0].Mids[kelp];
            if(mp->Cell==nmp->Cell){
            mp->neigh[N]=nmp;
            ECagents[e]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
            }
            }
        }
        }
        if((insideWorld(mp->Mx, mp->My-1, mp->Mz)==true)){
        if(grid[(int)mp->Mx][(int)mp->My-1][0].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx][(int)mp->My-1][0].Mids.size();kelp++){
            nmp = grid[(int)mp->Mx][(int)mp->My-1][0].Mids[kelp];
            if(mp->Cell==nmp->Cell){
            mp->neigh[N]=nmp;
            ECagents[e]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
            }
        }
        }
        }
        if((insideWorld(mp->Mx, mp->My+1, mp->Mz)==true)){
        if(grid[(int)mp->Mx][(int)mp->My+1][0].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx][(int)mp->My+1][0].Mids.size();kelp++){
            nmp = grid[(int)mp->Mx][(int)mp->My+1][0].Mids[kelp];
            if(mp->Cell==nmp->Cell){
            mp->neigh[N]=nmp;
            ECagents[e]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
            }
            }
        }
        }
        if(insideWorld(mp->Mx, mp->My, mp->Mz)==true){
        //if(grid[(int)mp->Mx][(int)mp->My][1].type==M){
            for(kelp=0;kelp<grid[(int)mp->Mx][(int)mp->My][0].Mids.size();kelp++){

                nmp = grid[(int)mp->Mx][(int)mp->My][0].Mids[kelp];
                if(mp->Cell!=nmp->Cell){
                mp->neigh[N]=nmp;
                ECagents[e]->createSpringTokenObject(mp, nmp, N);
                mp->SpringNeigh[N]->horizontal = true;
                mp->SpringNeigh[N]->right = false;
                N++;
                mp->neighs++;
                }
            }
        
        }
    }
    }
}
//-----------------------------------------------------------------------------------
//create single cell 2D
void World::create_2D_square_cell(int cell, int centreX, int centreY){
    
//create node agents ina  squareand connect up in a square
    int i, j;

  

    MemAgent* memp;
    
    for(i=(int)(centreX-(float)ECwidth/2.0f);i<(int)(centreX+(float)ECwidth/2.0f);i++){
        for(j=(int)(centreY-(float)ECwidth/2.0f);j<(int)(centreY+(float)ECwidth/2.0f);j++){
            
            memp = new MemAgent(ECagents[cell], (World*) this);
            memp->Mx = (float)i;
            memp->My = (float)j;
            memp->Mz = 0.0f;
            ECagents[cell]->nodeAgents.push_back(memp);
            setMLocation(int(i), int(j), 0, memp);
            memp->node=true;
            
        }
    }
    
    //connect mesh
   /* int uptoM=ECagents[cell]->nodeAgents.size();
    MemAgent* mp;
    MemAgent* nmp;
    int N;
    
    for(i=0;i<uptoM;i++){
        mp = ECagents[cell]->nodeAgents[i];
        N=0;
        if(insideWorld(mp->Mx-1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx-1][(int)mp->My][1].type==M){
            
            nmp = grid[(int)mp->Mx-1][(int)mp->My][1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        }
        if(insideWorld(mp->Mx+1, mp->My, mp->Mz)==true){
        if(grid[(int)mp->Mx+1][(int)mp->My][1].type==M){
            nmp = grid[(int)mp->Mx+1][(int)mp->My][1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
        }
        }
        if(insideWorld(mp->Mx, mp->My-1, mp->Mz)==true){
        if(grid[(int)mp->Mx][(int)mp->My-1][1].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My-1][1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
        }
        }
        if(insideWorld(mp->Mx, mp->My+1, mp->Mz)==true){
        if(grid[(int)mp->Mx][(int)mp->My+1][1].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My+1][1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[cell]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        }
    }*/
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//create single cell 2D
void World::create_3D_round_cell(void){
    
    int radius = (float)ECwidth/2.0f;
    int height = 8;
    int i, j, k;

     for(i=0;i<ECELLS;i++){

        //creates new object dynamically of type EC (ecp is the e cell pointer)
        EC * ecp = new EC((World*) this);

        //put the address into the vector Ecells
        ECagents.push_back(ecp);

         
		 //if(chose%3==0)



         ECagents[i]->VEGFRtot = 0;


     }
    
//create node agents ina  squareand connect up in a square
    
    float dist;
    bool allow=false;
    MemAgent* memp;
    int centreX = (int)((float)xMAX/2.0f);
    int centreY = 10;//(int)((float)yMAX/2.0f);
    cout<<centreX<<" "<<centreY<<endl;
    
    for(i=(int)(centreX-(float)ECwidth/2.0f);i<=(int)(centreX+(float)ECwidth/2.0f);i++){
        for(j=(int)(centreY-(float)ECwidth/2.0f);j<=(int)(centreY+(float)ECwidth/2.0f);j++){
            for(k=1;k<height;k++){
                allow=false;
                if((k==1)||(k==height-1)){
                    if(getDist(i, j, k, centreX, centreY, k)<=radius+0.5)
                        allow=true;
                }
                else{
                    dist=getDist(i, j, k, centreX, centreY, k);
                    
                    if((dist<=radius+0.5)&&(dist>=radius-0.8f))
                        allow=true;
                    
                }
                
                if(allow==true){
                    //lower disc
                    memp = new MemAgent(ECagents[0], (World*) this);
                    memp->Mx = (float)i;
                    memp->My = (float)j;
                    memp->Mz = (float)k;
                    ECagents[0]->nodeAgents.push_back(memp);
                    setMLocation(int(i), int(j), k, memp);
                    memp->node=true;
                    
                }
            }
        }
    }
    
    //connect mesh
    int uptoM=ECagents[0]->nodeAgents.size();
    MemAgent* mp;
    MemAgent* nmp;
    int N;
    
    for(i=0;i<uptoM;i++){
        mp = ECagents[0]->nodeAgents[i];
        N=0;
        
        if(grid[(int)mp->Mx-1][(int)mp->My][(int)mp->Mz].type==M){
            
            nmp = grid[(int)mp->Mx-1][(int)mp->My][(int)mp->Mz].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        if(grid[(int)mp->Mx+1][(int)mp->My][(int)mp->Mz].type==M){
            nmp = grid[(int)mp->Mx+1][(int)mp->My][(int)mp->Mz].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = true;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
        }
        if(grid[(int)mp->Mx][(int)mp->My-1][(int)mp->Mz].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My-1][(int)mp->Mz].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = true;
            N++;
            mp->neighs++;
        }
        if(grid[(int)mp->Mx][(int)mp->My+1][(int)mp->Mz].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My+1][(int)mp->Mz].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        if(grid[(int)mp->Mx][(int)mp->My][(int)mp->Mz+1].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My][(int)mp->Mz+1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        
        if(grid[(int)mp->Mx][(int)mp->My][(int)mp->Mz-1].type==M){
            nmp = grid[(int)mp->Mx][(int)mp->My][(int)mp->Mz-1].Mids[0];
            mp->neigh[N]=nmp;
            ECagents[0]->createSpringTokenObject(mp, nmp, N);
            mp->SpringNeigh[N]->horizontal = false;
            mp->SpringNeigh[N]->right = false;
            N++;
            mp->neighs++;
        }
        
        
        
        
    }
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void World::createECagents(int Junc_arrang){
    
    int i, j;
    int chose;

      

    for(i=0;i<ECELLS;i++){
        
        //creates new object dynamically of type EC (ecp is the e cell pointer)
        EC * ecp = new EC((World*) this);
        
        //put the address into the vector Ecells
        ECagents.push_back(ecp);

         //chose = rand();
         chose = new_rand();
	    
         ECagents[i]->VEGFRtot = 0;

         //ECagents[i]->red = ((float)rand()/(float)RAND_MAX);
        //ECagents[i]->green = ((float)rand()/(float)RAND_MAX);
        //ECagents[i]->blue = ((float)rand()/(float)RAND_MAX);

        ECagents[i]->red = ((float)new_rand()/(float)NEW_RAND_MAX);
        ECagents[i]->green = ((float)new_rand()/(float)NEW_RAND_MAX);
        ECagents[i]->blue = ((float)new_rand()/(float)NEW_RAND_MAX);

        
        for(j=0;j<ECwidth;j++){
          
            drawMeshFirst(i, j, ecp, Junc_arrang);
            
        }
        
        if(ANALYSIS_HYSTERESIS==true){
            ECagents[i]->hyst.Cell = ECagents[i];
            
        }
        
    }
    
    //chooseMutants();
     
    
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void World::createBlood(void){
    
    int i, j, k, J, K;
    
    //Vessel * vp = new Vessel((World*) this);
    
    //Vessels.push_back(vp);
    
    //mid points of lumen ends
    //vp-> Sx = 0 ;
    //vp-> Sy = vesselCentreY;//+bloodRadius;
    //vp-> Sz= vesselCentreZ ;
    
    
    /*vp-> EndX = xMAX-1;
     * vp-> EndY = vesselCentreY+bloodRadius;
     * vp-> EndZ = vesselCentreZ;*/
    
    //vp->LongAxis_Theta = ;
    
    for(i=0;i<(ECpack*ECwidth)-1;i++)
        for(j=0;j<(vesselRadius-1)*2;j++){
            for(k=0;k<(vesselRadius-1)*2;k++){
                J=j+vesselCentreY-(vesselRadius-1);
                K=k+vesselCentreZ-(vesselRadius-1);
                
                if(insideWorld(i, J, K)==true){
                    
                    //create blood in vessel
                    if(distTest2D(J, K, vesselCentreY, vesselCentreZ, vesselRadius-1)==true){
                        //if (grid[j+depth][k+Y][l+Z].Eid!=NULL){
                        if(grid[i][J][K].type==E){
                        grid[i][J][K].Eid->blood=1.0f;
                        //grid[i][J][K].Vid=vp
                        //grid[i][J][K].type = BLOOD;
                        grid[i][J][K].Eid->VEGF=0.0f;
                        }
                        
                    }
                    //else if((grid[i][J][K].Mids.size()>0)&&(distTest2D(J,K,vesselCentreY,vesselCentreZ, bloodRadius+1)==true)) grid[i][J][K].Mids[0]->Lumen=true;
                }
            }
        }
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------
void World::replaceBlood(int i, int j, int k){
    
    if(insideWorld(i, j, k)==true){
        
        //create blood in vessel
        if(distTest2D(j, k, vesselCentreY, vesselCentreZ, bloodRadius)==true){
            
            grid[i][j][k].type = BLOOD;
            grid[i][j][k].Eid->VEGF=0.0f;
        }
        
    }
    
}
//--------------------------------------------------------------------------------------------------------------------------------------
void World::drawMeshFirst(int i, int j, EC* ecp, int JunctArrangement){

    float theta;
    int totalAbSteps = (int)(2.0f*(float)Pi*((float)vesselRadius));
    int ablumenalSteps=(int)((float)totalAbSteps/(float)ECcross);//30.0f/(float)ECcross;
    float delta;
    float r, k, l, X, Y, Z, J, rL;
    int lx;
    float thetaStart, deltaSteps;
    float depth;
    MemAgent* memp;
    int circlePosStartAb=0;

    r=vesselRadius;
    rL=bloodRadius;

    delta=((2.0f*(float)Pi)/(float)ECcross);
    deltaSteps = delta/float(ablumenalSteps);
    float offset;
    float offsetPercentage;

    float x=(float)i/(float)ECcross;
    lx=(int)x;
    depth= ECwidth*lx;
    //cout<<"ab steps"<<ablumenalSteps<<endl;
    thetaStart=delta*(float)i;
    circlePosStartAb=ablumenalSteps*i;

    if(lx%2!=0){

     offset=0.5;
     /*if((i==0)||(i==1)) offset=0.1f;
     else if((i==2)||(i==3)) offset=0.5f;
     else if((i==4)||(i==5)) offset = 0.2f;
     else if((i==6)||(i==7)) offset = 0.5f;
     else if((i==8)||(i==9)) offset = 0.7f;*/

     
      thetaStart+=offset*delta;
      circlePosStartAb+=(int)(offset*ablumenalSteps);
    }

    circlePosStartAb=circlePosStartAb%int(ablumenalSteps*ECcross);
    if(thetaStart>=2*Pi) thetaStart-=2*Pi;

    X=(float)j;Y=(float)vesselCentreY;Z=(float)vesselCentreZ;


    for(J=0;J<ablumenalSteps;J++){

        theta=thetaStart+(deltaSteps*(float)J);

        k=((float)r*cos(theta));
        l=((float)r*sin(theta));

        //if(insideWorld(int(j+depth), int(k+Y), int(l+Z))==true){

        //for(r=0;r<=vesselRadius+1;r++){
        // k=(int)((float)r*cos(theta));
        //l=(int)((float)r*sin(theta));

        if(insideWorld(j+depth, k+Y, l+Z)==true){

            //if within vessel but not blood create a cell site here
            //if((getDist((int)k,(int)l,0,0,0,0)>vesselRadius-1)&&(getDist((int)k,(int)l,0,0,0,0)<vesselRadius+1)){
            //if(grid[(int)(j+depth)][(int)(k+Y)][(int)(l+Z)].type!=M){
            memp = new MemAgent(ecp, (World*) this);

            memp->Mx = (float)j+(float)depth;
            memp->My = k+Y;

            memp->FA = true;
            //---------------------------------------
            ///for CELL_SETUP=2 (lars and rearrangement paper models with sewn up front.
            ///label the front row of nodes so they can be sewn up once gridded
            if(BLINDENDED_SPROUT==true){
            if((i==ECELLS-1)||(i==ECELLS-2)){    
                if(j==ECwidth-1) memp->labelledBlindended =  true;
                
            }
            }
            //---------------------------------------
            
            memp->Mz = l+Z;

            memp->previous.x = memp->Mx;
            memp->previous.y = memp->My;
            memp->previous.z = memp->Mz;

            memp->circlePos=(int)(J+circlePosStartAb);

            if(memp->circlePos>=ablumenalSteps*ECcross) {
                memp->circlePos-=ablumenalSteps*ECcross;
            }

            ecp->nodeAgents.push_back(memp);

            setMLocation(int(j+depth), int(k+Y), int(l+Z), memp);

            memp->JunctionTest( true);
        }
        //}
        //}
    }

    //-------------------------------------------------------------------------------------

    


}

//-------------------------------------------------------------------------//-------------------------------------------------------------------------
void World::connectMesh(void){

    int i, j, m, n;
    int uptoE, uptoM, uptoM2;
    EC* ecp;
    EC* ecp2;
    MemAgent* mp;
    MemAgent* nmp;
    int N, JN;
    uptoE = ECagents.size();
    int POS, POSplus, POSminus, Xplus, Xminus;
    int totalAbSteps = (int)(2.0f*(float)Pi*(float)vesselRadius);
    int ablumenalSteps=(int)((float)totalAbSteps/(float)ECcross);

    if(meshNeighs==4){

        for(i=0;i<uptoE;i++){

            ecp=ECagents[i];
            uptoM=ecp->nodeAgents.size();

            for(j=0;j<uptoM;j++){

                mp=ecp->nodeAgents[j];
                N=0;
                JN=0;
                POS = mp->circlePos;
                POSplus=POS+1;
                POSminus=POS-1;
                Xplus = (int)mp->Mx+1;
                Xminus = (int)mp->Mx-1;


                if(POS==0) POSminus=ablumenalSteps*ECcross-1;
                else if(POS==ablumenalSteps*ECcross-1) POSplus=0;
                if(Xplus==xMAX) Xplus = 0;
                else if(Xminus<0) Xminus = xMAX-1;

                //go through all other agents, in each cell and find the ones it should be neighs with
                for(m=0;m<uptoE; m++){
                    ecp2=ECagents[m];
                    uptoM2=ecp2->nodeAgents.size();

                    for(n=0;n<uptoM2;n++){

                        nmp=ecp2->nodeAgents[n];

                        if((nmp->circlePos==POS)&&(((int)nmp->Mx==Xplus)||((int)nmp->Mx==Xminus))){
                            mp->neigh[N]=nmp;
                            if(mp->Cell!=nmp->Cell){
                                //mp->Cell->nodesToRetract.push_back(mp);
                                mp->springJunction=true;
                                nmp->springJunction = true;
                            }
                            ecp->createSpringTokenObject(mp, nmp, N);
                            mp->SpringNeigh[N]->horizontal = true;
                            if((int)nmp->Mx==Xplus) mp->SpringNeigh[N]->right = true;
                            else mp->SpringNeigh[N]->right = false;

                            N++;

                        }
                        else if(((int)nmp->Mx==(int)mp->Mx)&&((nmp->circlePos==POSplus)||(nmp->circlePos==POSminus))){

                            mp->neigh[N]=nmp;
                            if(mp->Cell!=nmp->Cell){
                                //mp->Cell->nodesToRetract.push_back(mp);
                                mp->springJunction=true;
                                nmp->springJunction = true;
                            }


                            ecp->createSpringTokenObject(mp, nmp, N);
                            if(nmp->circlePos==POSplus) mp->SpringNeigh[N]->right=true;
                            else mp->SpringNeigh[N]->right=false;
                            N++;

                        }
                    }
                    mp->neighs=N;

                }


            }

        }

    }
    //check all springs are correctly labelled left right and hor/ver
    int k;
    int flag1=0; int flag2=0; int flag3=0; int flag4=0;

    for(i=0;i<uptoE;i++){
        ecp=ECagents[i];
        uptoM=ecp->nodeAgents.size();

        for(j=0;j<uptoM;j++){
            flag1 = 0;
            flag2 = 0;
            flag3 = 0;
            flag4 = 0;
            mp=ecp->nodeAgents[j];
            for(k=0;k<mp->neighs;k++){
                if((flag1==0)&&(mp->SpringNeigh[k]->horizontal==true)&&(mp->SpringNeigh[k]->right==true)){
                    flag1=1;
                }
                if((flag2==0)&&(mp->SpringNeigh[k]->horizontal==true)&&(mp->SpringNeigh[k]->right==false)){
                    flag2=1;
                }
                if((flag3==0)&&(mp->SpringNeigh[k]->horizontal==false)&&(mp->SpringNeigh[k]->right==true)){
                    flag3=1;
                }
                if((flag4==0)&&(mp->SpringNeigh[k]->horizontal==false)&&(mp->SpringNeigh[k]->right==false)){
                    flag4=1;
                }
            }

            if(flag4!=1){
                cout<<"cell: "<<i<<"cirPos "<<mp->circlePos<<" x "<<(int)mp->Mx<<endl;
            }
        }
    }

}
//-------------------------------------------------------------------------
//------------------------------------------------------------------------
//-------------------------------------------------------------------------
void World::createNewEnvAgent(int x, int y, int z){
    
    
   
    
        if(grid[x][y][z].Eid!=NULL){ 
            cout<<"already got an env agent in createNew"<<endl;
        }
        Env * ep = new Env((World*) this);
        
        ep->Ex=x; ep->Ey=y;ep->Ez=z;
        grid[x][y][z].Eid=ep;
        grid[x][y][z].type=E;
        
        //if((timeStep>0)&&(y<vesselRadius*2+gap)) replaceBlood(x, y, z);
        
        if(timeStep!=0)calcEnvAgentVEGF(ep);
        ep->calcInside();
    
    
   
}
//-------------------------------------------------------------------------


    void World::createAstrocytes(void){
        
        
        int i, j, k, m;
        int centreX, centreY;
        
        int howMany =(int)((float)xMAX/(float)(astroWidth+astroSpacer));
        
        int W=(int)((float)astroWidth/2.0f);
        int V=(int)((float)astroSpacer/2.0f);
        int p1[howMany];int p2[howMany];
        int b1[howMany];int b2[howMany];
        //-------------------------------------------
        //uniform layer of astrocytes
        if(ASTRO==UNIFORM){
            for(i=0;i<xMAX;i++)
                for(j=0;j<yMAX;j++){
                    for(m=0;m<Zgap;m++){
                        grid[i][j][m].Eid->Astro=true;
                        grid[i][j][m].Eid->OldAstro=true;
                        //cout<<"made astrocyte..."<<endl;
                    }
                }
        }
        //-------------------------------------------
        //-------------------------------------------
        //curved circles
        if(ASTRO==CURVED_CIRCLES){
#define Cradius 8
#define Cgap 4
            
            int howManyH = (int)((float)xMAX/(float)((Cradius*2)+Cgap));
            int howManyV = (int)((float)yMAX/(float)((Cradius*2)+Cgap));
            
            for(i=0;i<xMAX;i++)
                for(j=0;j<yMAX;j++){
                    //grid[i][j][0].type=AS;
                }
            
            for(i=0;i<howManyH;i++)
                for(j=0;j<howManyV;j++){
                    centreX = (int)((float)Cgap/2.0f+Cradius+(i*(Cgap+(Cradius*2))));
                    //centreX = m*(Cgap+(Cradius*2));
                    centreY = (int)((float)Cgap/2.0f+(2*vesselRadius)-4+Cgap+Cradius+(j*(Cgap+(Cradius*2))));
                    for(k=0;k<2*Cradius;k++){
                        for(m=0;m<2*Cradius;m++){
                            if(getDist(centreX, centreY, 0, centreX-Cradius+k, centreY-Cradius+m, 0)<Cradius){
                                if(insideWorld(centreX-Cradius+k, centreY-Cradius+m, 0)==true)
                                    grid[centreX-Cradius+k][centreY-Cradius+m][0].type=E;
                            }
                        }
                    }
                }
            
            
        }
        //----------------------------------------------
        //regular criss-cross network
        if(ASTRO==DIAG){
            for(j=0;j<yMAX;j++){
                
                
                for(i=0;i<xMAX;i++){
                    
                    for(k=0;k<howMany;k++){
                        
                        p1[k]=j+(k*(astroWidth+astroSpacer))-W;
                        p2[k]=j+astroWidth+(k*(astroWidth+astroSpacer))-W;
                        
                        b1[k]=(k*(astroWidth+astroSpacer))-j-W;
                        b2[k]=astroWidth+(k*(astroWidth+astroSpacer))-j-W;
                        
                        if(p1[k]>xMAX)
                            p1[k] = p1[k]%xMAX;
                        
                        if(p2[k]>xMAX)
                            p2[k] = p2[k]%xMAX;
                        
                        if(b1[k]<0)
                            b1[k] = xMAX+(b1[k]%xMAX);
                        
                        if(b2[k]<0)
                            b2[k] = xMAX+(b2[k]%xMAX);
                        
                        if(p1[k]<0)
                            p1[k] = xMAX+(p1[k]%xMAX);
                        
                        if(p2[k]<0)
                            p2[k] = xMAX+(p2[k]%xMAX);
                        //-------------------------------
                        //normal case
                        if((p2[k]>p1[k])){
                        //    if((i>=p1[k])&&(i<p2[k])) grid[i][j][0].type=AS;
                        }
                        //-------------------------------
                        //has wrapped around only p2
                        else{
                        //    if((i>=p1[k])||(i<p2[k])) grid[i][j][0].type=AS;
                        }
                        //-------------------------------
                        //-------------------------------
                        //normal case
                        if((b2[k]>b1[k])){
                        //    if((i>=b1[k])&&(i<b2[k])) grid[i][j][0].type=AS;
                        }
                        //-------------------------------
                        //has wrapped around only p2
                        else{
                        //   if((i>=b1[k])||(i<b2[k])) grid[i][j][0].type=AS;
                        }
                        //-------------------------------
                    }
                    
                }
            }
        }
        //-------------------------------------------
        //-------------------------------------------
        //regular square network
        if(ASTRO==SQUARES){
           
            for(k=0;k<howMany;k++){
                
                /*Coordinates One;
                 * Coordinates Two;
                 * Coordinates Anchor;
                 *
                 * Anchor.x = 0.0f+k*(astroWidth+astroSpacer)+2*V;
                 * Anchor.y = 10.0f;
                 * Anchor.z = 0.0f;
                 *
                 * Two.x =  Anchor.x+V+astroWidth/2.0f+2*V;
                 * Two.y =Vspacer;
                 * Two.z = 0.0f;
                 *
                 * One.x =  Anchor.x-(V+astroWidth/2.0f)+2*V;
                 * One.y = Vspacer;
                 * One.z = 0.0f;
                 *
                 * curvedAstrocytes(One, Two, Anchor);*/
                
                p1[k]=(k*(astroWidth+astroSpacer))+V;
                p2[k]=astroWidth+(k*(astroWidth+astroSpacer))+V;
                
                if(p1[k]>xMAX)
                    p1[k] = p1[k]%xMAX;
                
                if(p2[k]>xMAX)
                    p2[k] = p2[k]%xMAX;
                
                if(p1[k]<0)
                    p1[k] = xMAX+(p1[k]%xMAX);
                
                if(p2[k]<0)
                    p2[k] = xMAX+(p2[k]%xMAX);
                
                
            }
            for(j=0;j<yMAX;j++){
                //for(j=0;j<53;j++){
                for(i=0;i<xMAX;i++){
                    for(k=0;k<howMany;k++){
                        
                        //-------------------------------
                        //normal case
                        if((p2[k]>p1[k])){
                            if((i>=p1[k])&&(i<p2[k])){
                                for(m=0;m<4;m++){
                                    grid[i][j][m].Eid->Astro=true;
                                    grid[i][j][m].Eid->OldAstro=true;
                                }
                            }
                            
                        }
                        //-------------------------------
                        //has wrapped around only p2
                        else{
                            if((i>=p1[k])||(i<p2[k])){
                                for(m=0;m<4;m++){
                                    grid[i][j][m].Eid->Astro=true;
                                    grid[i][j][m].Eid->OldAstro=true;
                                }
                            }
                        }
                        //-------------------------------
                    }
                    /*if((j%Vspacer==-2+6)||(j%Vspacer==-1+6)||(j%Vspacer==0+6)||(j%Vspacer==1+6
                            )){
                        for(m=0;m<4;m++){
                            grid[i][j][m].type=AS;
                            grid[i][j][m].oldType=AS;
                        }
                    }*/
                }
            }
            
        }
        //-------------------------------------------
        //regular square network
        if(ASTRO==ZEBRAFISH){
            for(k=0;k<howMany;k++){
                
                /*Coordinates One;
                 * Coordinates Two;
                 * Coordinates Anchor;
                 *
                 * Anchor.x = 0.0f+k*(astroWidth+astroSpacer)+2*V;
                 * Anchor.y = 10.0f;
                 * Anchor.z = 0.0f;
                 *
                 * Two.x =  Anchor.x+V+astroWidth/2.0f+2*V;
                 * Two.y =Vspacer;
                 * Two.z = 0.0f;
                 *
                 * One.x =  Anchor.x-(V+astroWidth/2.0f)+2*V;
                 * One.y = Vspacer;
                 * One.z = 0.0f;
                 *
                 * curvedAstrocytes(One, Two, Anchor);*/
                
                p1[k]=(k*(astroWidth+astroSpacer))+V;
                p2[k]=astroWidth+(k*(astroWidth+astroSpacer))+V;
                
                if(p1[k]>xMAX)
                    p1[k] = p1[k]%xMAX;
                
                if(p2[k]>xMAX)
                    p2[k] = p2[k]%xMAX;
                
                if(p1[k]<0)
                    p1[k] = xMAX+(p1[k]%xMAX);
                
                if(p2[k]<0)
                    p2[k] = xMAX+(p2[k]%xMAX);
                
                
            }
            //for(j=0;j<yMAX;j++){
            for(j=0;j<Vspacer+13;j++){
                for(i=0;i<xMAX;i++){
                    for(m=0;m<4;m++){
                        for(k=0;k<howMany;k++){
                            
                            //-------------------------------
                            //normal case
                            if((p2[k]>p1[k])){
                            //    if((i>=p1[k])&&(i<p2[k])) grid[i][j][m].type=AS;
                            }
                            //-------------------------------
                            //has wrapped around only p2
                            else{
                            //    if((i>=p1[k])||(i<p2[k])) grid[i][j][m].type=AS;
                            }
                            //-------------------------------
                        }
                        //if((j%Vspacer==-3+10)||(j%Vspacer==-2+10)||(j%Vspacer==-1+10)||(j%Vspacer==0+10)||(j%Vspacer==1+10)||(j%Vspacer==2+10)) grid[i][j][m].type=AS;
                    }
                }
            }
            
        }
        
		//cout<<"astros"<<endl;
        
        //-------------------------------------------
    }
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
    bool World::replaceAstrocytes(int i, int j, bool checkOnly){
        
        
        int k, m, n;
        bool flag=false;
#define Cradius 8
#define Cgap 4
        int flagA=0;
        int howMany =(int)((float)xMAX/(float)(astroWidth+astroSpacer));
        
        int W=(int)((float)astroWidth/2.0f);
        int V=(int)((float)astroSpacer/2.0f);
        int p1[howMany];int p2[howMany];
        int b1[howMany];int b2[howMany];
        int centreX, centreY;
        
        //-------------------------------------------
        //uniform layer of astrocytes
        if(ASTRO==UNIFORM){
            
            if(checkOnly==false) grid[i][j][3].Eid->Astro=true;
            flag=true;
            
        }
        //-------------------------------------------
        //-------------------------------------------
        //curved circles
        if(ASTRO==CURVED_CIRCLES){
            
            
            int howManyH = (int)((float)xMAX/(float)((Cradius*2)+Cgap));
            int howManyV = (int)((float)yMAX/(float)((Cradius*2)+Cgap));
            
            for(m=0;m<howManyH;m++)
                for(n=0;n<howManyV;n++){
                    centreX = (int)((float)Cgap/2.0f+Cradius+(m*(Cgap+(Cradius*2))));
                    //centreY = (float)Cgap/2.0f+(2*vesselRadius)-4+Cgap+Cradius+(n*(Cgap+(Cradius*2)));
                    //centreX = m*(Cgap+(Cradius*2));
                    centreY = (int)((float)Cgap/2.0f+(2*vesselRadius)-4+Cgap+Cradius+(n*(Cgap+(Cradius*2))));
                    
                    if(getDist(centreX, centreY, 0, (float)i, (float)j, 0)<Cradius){
                        flagA=1;
                        break;
                    }
                    
                    
                }
            
            if(flagA==0){
                if(checkOnly==true) flag=true;
                else grid[i][j][3].Eid->Astro=true;
            }
            
        }
        //----------------------------------------------
        //----------------------------------------------
        //regular criss-cross network
        if(ASTRO==DIAG){
            
            for(k=0;k<howMany;k++){
                
                p1[k]=j+(k*(astroWidth+astroSpacer))-W;
                p2[k]=j+astroWidth+(k*(astroWidth+astroSpacer))-W;
                
                b1[k]=(k*(astroWidth+astroSpacer))-j-W;
                b2[k]=astroWidth+(k*(astroWidth+astroSpacer))-j-W;
                
                if(p1[k]>xMAX)
                    p1[k] = p1[k]%xMAX;
                
                if(p2[k]>xMAX)
                    p2[k] = p2[k]%xMAX;
                
                if(b1[k]<0)
                    b1[k] = xMAX+(b1[k]%xMAX);
                
                if(b2[k]<0)
                    b2[k] = xMAX+(b2[k]%xMAX);
                
                if(p1[k]<0)
                    p1[k] = xMAX+(p1[k]%xMAX);
                
                if(p2[k]<0)
                    p2[k] = xMAX+(p2[k]%xMAX);
                //-------------------------------
                //normal case
                if((p2[k]>p1[k])){
                    if((i>=p1[k])&&(i<p2[k])){
                        if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                        flag=true;
                    }
                }
                //-------------------------------
                //has wrapped around only p2
                else{
                    if((i>=p1[k])||(i<p2[k])){
                        if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                        flag=true;
                    }
                }
                //-------------------------------
                //-------------------------------
                //normal case
                if((b2[k]>b1[k])){
                    if((i>=b1[k])&&(i<b2[k])){
                        if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                        flag=true;
                    }
                }
                //-------------------------------
                //has wrapped around only p2
                else{
                    if((i>=b1[k])||(i<b2[k])){
                        if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                        flag=true;
                    }
                }
                //-------------------------------
            }
            
        }
        
        //-------------------------------------------
        //-------------------------------------------
        //regular square network
        if(ASTRO==SQUARES){
            for(k=0;k<howMany;k++){
                
                p1[k]=(k*(astroWidth+astroSpacer))+V;
                p2[k]=astroWidth+(k*(astroWidth+astroSpacer))+V;
                
                if(p1[k]>xMAX)
                    p1[k] = p1[k]%xMAX;
                
                if(p2[k]>xMAX)
                    p2[k] = p2[k]%xMAX;
                
                if(p1[k]<0)
                    p1[k] = xMAX+(p1[k]%xMAX);
                
                if(p2[k]<0)
                    p2[k] = xMAX+(p2[k]%xMAX);
            }
            //for(j=0;j<yMAX;j++){
            if(j<yMAX){
                for(k=0;k<howMany;k++){
                    
                    //-------------------------------
                    //normal case
                    if((p2[k]>p1[k])){
                        if((i>=p1[k])&&(i<p2[k])){
                            if(checkOnly==false)grid[i][j][3].Eid->Astro = true;
                            flag=true;
                        }
                    }
                    //-------------------------------
                    //has wrapped around only p2
                    else{
                        if((i>=p1[k])||(i<p2[k])){
                            if(checkOnly==false)grid[i][j][3].Eid->Astro = true;
                            flag=true;
                        }
                    }
                    //-------------------------------
                    
                }
                /*if((j%Vspacer==-3+10)||(j%Vspacer==-2+10)||(j%Vspacer==-1+10)||(j%Vspacer==0+10)||(j%Vspacer==1+10)||(j%Vspacer==2+10)){
                    if(checkOnly==false)grid[i][j][3].type=AS;
                    flag=true;
                }*/
            }
        }
//-------------------------------------------
        //regular square network
        if(ASTRO==ZEBRAFISH){
            for(k=0;k<howMany;k++){
                
                p1[k]=(k*(astroWidth+astroSpacer))+V;
                p2[k]=astroWidth+(k*(astroWidth+astroSpacer))+V;
                
                if(p1[k]>xMAX)
                    p1[k] = p1[k]%xMAX;
                
                if(p2[k]>xMAX)
                    p2[k] = p2[k]%xMAX;
                
                if(p1[k]<0)
                    p1[k] = xMAX+(p1[k]%xMAX);
                
                if(p2[k]<0)
                    p2[k] = xMAX+(p2[k]%xMAX);
            }
            //for(j=0;j<yMAX;j++){
            if(j<Vspacer+13){
                for(k=0;k<howMany;k++){
                    
                    //-------------------------------
                    //normal case
                    if((p2[k]>p1[k])){
                        if((i>=p1[k])&&(i<p2[k])){
                            if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                            flag=true;
                        }
                    }
                    //-------------------------------
                    //has wrapped around only p2
                    else{
                        if((i>=p1[k])||(i<p2[k])){
                            if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                            flag=true;
                        }
                    }
                    //-------------------------------
                    
                }
                if((j%Vspacer==-3+10)||(j%Vspacer==-2+10)||(j%Vspacer==-1+10)||(j%Vspacer==0+10)||(j%Vspacer==1+10)||(j%Vspacer==2+10)){
                    if(checkOnly==false)grid[i][j][0].Eid->Astro = true;
                    flag=true;
                }
            }
        }
        /*if(ASTRO==RETINA){
    Coordinates start1;
    Coordinates start2;
    Coordinates end;
    //int branch = vesselRadius+gap+30;
    Coordinates branch;
    float width =12;
    int drop = 1;
    int dropSpacer = 9;
     vector <float> equ1;
     vector <float> equ2;
     vector <float> equ3;
    int m, n, in;
    float x, x2;
     
    start1.x = ECwidth*0.7;
    start1.y = 0;
    start1.z = 0;
     
    start2.x = ECwidth*3.5;
    start2.y = 0;
    start2.z = 0;
     
    end.x = ECwidth*2.5;
    end.y  = 110;
    end.z = 0;
     
    branch.x = 9;
    branch.y = 55;
    branch.z = 0;
     
    int diff=25;
    //get equation of the line through start1 and end
    equ1 = equation_of_line_through_two_points(start1.x, start1.y, end.x, end.y);
    equ2 = equation_of_line_through_two_points(start2.x, start2.y, end.x, end.y);
    equ3 = equation_of_line_through_two_points(branch.x, branch.y, start2.x+5, start2.y+diff-5);
    //for each y between start1.y and end.y calc x and z and create astro in all width upto width, reduce widtrh by drop spacer..
    in=0;
    int count=0;
     
            if(j<vesselRadius*2+2){
                if(checkOnly==false)grid[i][j][0].type=AS;
                        flag=true;
            }
     
     
     
    for(m=start1.y;m<end.y;m++){
     
     
        if(count%dropSpacer==0) width-=drop;
        if(width<2) width=2;
        if((m>=end.y-35)&&(m<=end.y-30)) in++;
        if(m>=end.y-20) in=3;
        if(m>=end.y-5) in=0;
        x = ((float)m-equ1[1])/equ1[0];
     
        for(n=x-(width);n<=x+(width)+in;n++){
            if((n==i)&&(m==j)){
                if(checkOnly==false)grid[i][j][0].type=AS;
                        flag=true;
            }
        }
        count++;
    }
    width =12;
    count=0;
    in =0;
    for(m=start2.y;m<end.y;m++){
     
        if(count%dropSpacer==0) width-=drop;
        if(width<2) width=2;
        if((m>=end.y-35)&&(m<=end.y-30)) in++;
        if(m>=end.y-20) in=3;
        if(m>=end.y-5) in=0;
        if(in<0) in=0;
        x2 = ((float)m-equ2[1])/equ2[0];
     
        for(n=x2-(width)-in;n<=x2+(width);n++){
            if((n==i)&&(m==j)){
                if(checkOnly==false)grid[i][j][0].type=AS;
                        flag=true;
            }
        }
     
        //if((i>=branch)&&(i<branch+branchWidth)){
        //    for(j=10;j<x2;j++){
        //        grid[j][i][0].type=AS;
        //    }
        //}
     
         count++;
    }
     
    width =8;
    count=0;
    for(m=start2.y+diff; m<branch.y; m++){
     
        if(count%dropSpacer==0) width++;
        //if(width>3) width=4;
        x2 = ((float)m-equ3[1])/equ3[0];
     
        for(n=x2-(width);n<=x2+(width);n++){
            if((n==i)&&(m==j)){
                if(checkOnly==false)grid[i][j][0].type=AS;
                        flag=true;
            }
        }
     
         count++;
    }
    }*/
        
        //create branch
        
        return(flag);
        //-------------------------------------------
    }
//-------------------------------------------------------------------------
    void World::createEnvironment(void){
        
        int i, j, k;
        Env* ep;
        //create environment onjects and place on grid++++++++++++++++++++++++++++++++++++++
        for(i=0;i<xMAX;i++)
            for(j=0;j<yMAX;j++){
                for(k=0;k<zMAX;k++){
                    if((grid[i][j][k].type==E)&&(grid[i][j][k].Eid==NULL)){
                        createNewEnvAgent(i, j, k);
                        //ep=grid[i][j][k].Eid;
                        
                    }
                    
                }
            }
        
        
        //createBlood();
        
        //if(VEGFgradient==FIXED_MACROS) initialise_macrophage_VEGF();
        
        //createTumourCells();
        
        //createECM();
        
        //calcVEGFgradient();    //--does in createEnvAgent now, no need to go through them all again
        
        
    }
//-------------------------------------------------------------------------
    void World::setInitialVEGF(void){

        int i, j, k;
        Env* ep;
        
        //create environment onjects and place on grid++++++++++++++++++++++++++++++++++++++
        for(i=0;i<xMAX;i++)
            for(j=0;j<yMAX;j++){
                for(k=0;k<zMAX;k++){
                    
                    if((grid[i][j][k].type==E)&&(grid[i][j][k].Eid!=NULL)){
                        
                        ep = grid[i][j][k].Eid;
                        calcEnvAgentVEGF(ep);
                        //if(ep->VEGF>0)
                    }
                }
            }

    }
//-------------------------------------------------------------------------------------------------------------
    void EC::createSpringTokenObject(MemAgent* start, MemAgent* end, int neigh){
        
        int flag=0;
        int i;
        Spring* stp;
        int flag2=0;
        int flag3=0;
        
        stp= new Spring();
        
        stp->start = start;
        stp->end = end;
        
        if((start->FIL==BASE)&&(end->FIL==TIP)) flag=1;
        
        else if((start->FIL==BASE)&&(end->FIL==STALK))flag=1;
        
        else if((start->FIL==STALK)||(end->FIL==STALK))flag=1;
        
        if(flag==1) stp->filopodia=true;
        
        //set nodeagents to know this is the spring object they belong to
        
        start->SpringNeigh[neigh]=stp;
        
        
        Springs.push_back(stp);
        
        if(start->Cell!=end->Cell){
            stp->Junction=true;
            stp->filopodia=false;
            flag=0;
            /*i=0;
            //add start and end to junction agent list if not already in there..
            if(worldP->JunctionAgents.size()>0){
            do{
                if(worldP->JunctionAgents[i]==start) flag2=1;
                if(worldP->JunctionAgents[i]==end) flag3=1;
                if((flag2==1)&&(flag3==1)) flag-1;
                i++;

            }while((flag==0)&&(i<worldP->JunctionAgents.size()));

            if(flag2==0) worldP->JunctionAgents.push_back(start);
            if(flag3==0) worldP->JunctionAgents.push_back(end);
            }*/
        }
        
        
    }
//-------------------------------------------------------------------------------------------------------------
    
//-------------------------------------------------------------------------
    void World::createTestCase(void){
        
        int i, j;
        int MEMS=2;
        MemAgent* memp;
        EC* ecp;
        
        cout<<"creating..."<<endl;
        cout.flush();
        
        //create EC agents-----------------------
        for(i=0;i<2;i++){
            
            //creates new object dynamically of type EC (ecp is the e cell pointer)
            EC * ecp = new EC((World*) this);
            
            //put the address into the vector Ecells
            ECagents.push_back(ecp);
        }
        //---------------------------------------
        cout<<"created cells"<<endl;
        cout.flush();
        //---------------------------------------
        //create nodeagents
        for(i=0;i<MEMS;i++){
            
            if(i==0) ecp=ECagents[0];
            else ecp=ECagents[1];
            
            
            for(j=0;j<MEMS;j++){
                memp = new MemAgent(ecp, (World*) this);
                cout<<memp<<endl;
                
                memp->Mx = i+(i*10);
                memp->My = j+(j*10);
                memp->Mz = 2;
                
                memp->neighs=2;
                
                ecp->nodeAgents.push_back(memp);
                
                setMLocation((int)memp->Mx, (int)memp->My, 2, memp);
                
                memp->JunctionTest(true);
            }
        }
        //---------------------------------------
        cout<<"created nodes"<<endl;
        cout.flush();
        
        //---------------------------------------
        //connect mesh
        ECagents[0]->nodeAgents[0]->neigh[0] = ECagents[0]->nodeAgents[1];
        ECagents[0]->nodeAgents[1]->neigh[0] = ECagents[0]->nodeAgents[0];
        
        ECagents[1]->nodeAgents[0]->neigh[0] = ECagents[1]->nodeAgents[1];
        ECagents[1]->nodeAgents[1]->neigh[0] = ECagents[1]->nodeAgents[0];
        
        ECagents[0]->nodeAgents[0]->neigh[1] = ECagents[1]->nodeAgents[0];
        ECagents[0]->nodeAgents[1]->neigh[1] = ECagents[1]->nodeAgents[1];
        
        ECagents[1]->nodeAgents[0]->neigh[1] = ECagents[0]->nodeAgents[0];
        ECagents[1]->nodeAgents[1]->neigh[1] = ECagents[0]->nodeAgents[1];
        
        cout<<"connected springs"<<endl;
        cout.flush();
        //after connecting, create the springToken objects.
        ECagents[0]->createSpringTokenObject(ECagents[0]->nodeAgents[0], ECagents[0]->nodeAgents[0]->neigh[0], 0);
        ECagents[0]->createSpringTokenObject(ECagents[0]->nodeAgents[0], ECagents[0]->nodeAgents[0]->neigh[1], 1);
        ECagents[0]->createSpringTokenObject(ECagents[0]->nodeAgents[1], ECagents[0]->nodeAgents[1]->neigh[0], 0);
        ECagents[0]->createSpringTokenObject(ECagents[0]->nodeAgents[1], ECagents[0]->nodeAgents[1]->neigh[1], 1);
        
        ECagents[1]->createSpringTokenObject(ECagents[1]->nodeAgents[0], ECagents[1]->nodeAgents[0]->neigh[0], 0);
        ECagents[1]->createSpringTokenObject(ECagents[1]->nodeAgents[0], ECagents[1]->nodeAgents[0]->neigh[1], 1);
        ECagents[1]->createSpringTokenObject(ECagents[1]->nodeAgents[1], ECagents[1]->nodeAgents[1]->neigh[0], 0);
        ECagents[1]->createSpringTokenObject(ECagents[1]->nodeAgents[1], ECagents[1]->nodeAgents[1]->neigh[1], 1);
        
        cout<<"created spring objects"<<endl;
        cout.flush();
        /*for(i=0;i<2;i++){
         * for(j=0;j<2;j++){
         * for(k=0;k<2;k++){
         *
         * cout<<"node cell: "<<ECagents[i]->nodeAgents[j]->Cell<<" neigh cell "<<ECagents[i]->nodeAgents[j]->neigh[k]->Cell<<" junction "<<ECagents[i]->nodeAgents[j]->SpringNeigh[k]->Junction<<" start cell "<<ECagents[i]->nodeAgents[j]->SpringNeigh[k]->start->Cell<<" end cell "<<ECagents[i]->nodeAgents[j]->SpringNeigh[k]->end->Cell<<endl;
         *
         *
         * }
         * }
         * }*/
        
        
        
        


    }
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
    void World::createHaptoTest(void){
        
        int i, j, k;
        EC* ecp =  new EC((World*) this);
        ECagents.push_back(ecp);
        ecp->VEGFRtot = 46000;
        for(i=0;i<xMAX;i++){
            for(j=0;j<9;j++){
                for(k=0;k<zMAX;k++){
                    //create env objs
                    if(j<4){
                        Env * ep = new Env((World*) this);
                        ep->Ex=i; ep->Ey=j;ep->Ez=0;
                        grid[i][j][k].Eid=ep;
                        grid[i][j][k].Eid->Astro = true;
                    }
                    //create blood
                    else{
                        grid[i][j][0].Eid->Astro = true;
                    }
                }
            }
        }
        
        //create and place memAgent
        MemAgent* memp = new MemAgent(ecp, (World*) this);
        ecp->nodeAgents.push_back(memp);
        
        memp->Mx = 4.0f;
        memp->My = 6.0f;
        memp->Mz = 1.0f;
        memp->node=true;
        setMLocation((int)memp->Mx, (int)memp->My, (int)memp->Mz, memp);
        
    }
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
    void World::runHapto(void){
        
        //set new pos
        float newPos[3] = {4.0f, 0.0f, 0.0f};
        grid[(int)newPos[0]][(int)newPos[1]][(int)newPos[2]].type = BLOOD;
        
        if(timeStep==1){
            //ECagents[0]->nodeAgents[0]->haptotaxis(newPos[0], newPos[1], newPos[2], AS, false);
        }
    }
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
    void World:: curvedAstrocytes(Coordinates One, Coordinates Two, Coordinates Anchor){
        
        //for test only**************88
        //createEnvironment();
        //**************************8
        
        float A1, B1, C1;
        float x, y;
        
        std:: vector <Coordinates> points;
        
        Coordinates Mid;
        Coordinates Median;
        Coordinates D;
        Coordinates sD;
        Coordinates horizontal;
        Coordinates vertical;
        
        float denom, curve_x_1, curve_x_2, nearest_curve_x, Anchor_hor_y, curve_y ;
        int flagY, flagX;
        
        //define points
        
        /*One.x = 10.0f;
         * One.y = 40.0f;
         * One.z = 0.0f;
         *
         * Two.x = 30.0f;
         * Two.y = 40.0f;
         * Two.z = 0.0f;
         *
         * Anchor.x = 20.0f;
         * Anchor.y = 0.0f;
         * Anchor.z = 0.0f;*/
        
        //calc median
        
        Mid.x = (One.x +Two.x)/2.0f;
        Mid.y = (One.y +Two.y)/2.0f;
        Mid.z = 0.0f;
        
        Median.x = Anchor.x-Mid.x;
        Median.y = Anchor.y-Mid.y;
        Median.z= 0.0f;
        
        denom = sqrt((Median.x*Median.x)+(Median.y*Median.y)+(Median.z*Median.z));
        
        sD.x = astroCurveDepth*(Median.x/denom);
        sD.y = astroCurveDepth*(Median.y/denom);
        sD.z = 0.0f;
        
        D.x = Anchor.x - sD.x;
        D.y = Anchor.y - sD.y;
        D.z = Anchor.z - sD.z;
        
        //calc curve equation between pairs of pairs
        float denom2;
        denom2 = ((One.x-D.x)*((Two.x*Two.x)-(One.x*One.x)))+((Two.x-One.x)*((D.x*D.x)-(One.x*One.x)));
        A1 = ((Two.y - One.y)*(One.x-D.x)+(D.y - One.y)*(Two.x-One.x))/denom2;
        
        B1 = ((Two.y-One.y)-A1*((Two.x*Two.x)-(One.x*One.x)))/(Two.x-One.x);
        
        C1 = One.y - A1*(One.x*One.x)-(B1*One.x);
        
        //test - draw line
        /*for(x = 0;x<100;x+=0.1){
         * y = (A1*x*x)+(B1*x)+C1;
         * if((x>=0)&&(x<xMAX)&&(y>=0)&&(y<yMAX))
         * grid[(int)x][(int)y][0].type = AS;
         * }*/
        
        //for all points defined by a b and c check which are enclosed by line One-D, Two-D and the curve.
        
        //check which is most vertical..
        
        //if(fabs(Anchor.y-One.y)>=fabs(Anchor.y-Two.y)){
        vertical = One;
        horizontal = Two;
        /*}
         * else{
         * vertical = Two;
         * horizontal = One;
         * }*/
        
        for(x=0;x<xMAX;x+=0.1){
            for(y=0;y<yMAX;y+=0.1){
                flagY=0;
                flagX=0;
                
                //Anchor_hor_x = equation_of_line_through_two_points_given_Coord(horizontal.x, horizontal.y, Anchor.x, Anchor.y, y ,1);
                //Anchor_ver_x = equation_of_line_through_two_points_given_Coord(vertical.x, vertical.y, Anchor.x, Anchor.y, y ,1);
                
                Anchor_hor_y = equation_of_line_through_two_points_given_Coord(horizontal.x, horizontal.y, Anchor.x, Anchor.y, x, 0);
                //two roots always, must choose the correct one
                curve_x_1 = -(B1/A1)+sqrt((y-C1)/A1); //pg 150 book 3
                curve_x_2 = -(B1/A1)-sqrt((y-C1)/A1);
                
                //find nearest root
                if(getDist(curve_x_1, y, 0.0f, x, y, 0.0f)<=getDist(curve_x_2, y, 0.0f, x, y, 0.0f)) nearest_curve_x = curve_x_1;
                else nearest_curve_x = curve_x_2;
                
                curve_y = (A1*x*x)+(B1*x)+C1;
                
                if(((int)y<=(int)horizontal.y)&&((int)y>=(int)Anchor.y)){
                    if((y>=Anchor.y)&&(y<=curve_y)){
                        flagY=1;
                    }
                    if((x>=vertical.x)&&(x<=horizontal.x)){
                        flagX=1;
                    }
                    
                    if((flagY==1)&&(flagX==1)){
                        if(insideWorld((int)x, (int)y, 0)==true)grid[(int)x][(int)y][0].Eid->Astro = true;
                    }
                }
            }
        }
        
        
        //grid astro
        
        /*grid[(int)One.x][(int)One.y][(int)One.z].type = C;
         * grid[(int)Two.x][(int)Two.y][(int)Two.z].type = C;
         * grid[(int)Anchor.x][(int)Anchor.y][(int)Anchor.z].type = C;
         *
         * cout<<Mid.x<<" "<<Mid.y<<" "<<Mid.z<<endl;
         * cout<<D.x<<" "<<D.y<<" "<<D.z<<endl;
         *
         * grid[(int)Mid.x][(int)Mid.y][(int)Mid.z].type = BLOOD;
         * grid[(int)D.x][(int)D.y][(int)D.z].type = C;*/
        
    }
    
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
    void World::createMacrophages(void){
        
        int i;
        Macrophage* macp;
        float offset;
        int MACS;
        
        if(MACROS!=0)
            MACS = MACROS;
        else MACS = 1;
        
        offset = ((float)xMAX/((float)MACS))/2.0f+6;
        
        for(i=0;i<MACROS;i++){
            
            //random
            
            //fixed
            macp = new Macrophage(this);
            /*if(ECpack>1){
            if(i==0) macp->coords.x = 30.0f;//offset+(float)xMAX/((float)MACROS)*i;
            else macp->coords.x = xMAX-30.0f;
        }
        else macp->coords.x = 10.0f;
        macp->coords.y = 70.0f;//40.0f;
        macp->coords.z = 0.0f;*/
            
            //grid[(int)macp->coords.x][(int)macp->coords.y][(int)macp->coords.z].type = Mac;
            macrophages.push_back(macp);
            if(i==0){
                macp->coords.x=50.0f;//((float)xMAX/2.0f)+7;//49.0f
                macp->coords.y=90.0f;//0.0f;//110.0f;//50.0f;
                macp->coords.z=0.0f;//{((float)xMAX/2.0f)+7, 110.0f, 0.0f};
            }
            else{
                macp->coords.x=25;
                macp->coords.y=140.0f;
                macp->coords.z=0.0f;
            }
            
        }
    }
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//initialise peaked VEGF concentration
    void World::initialise_macrophage_VEGF(void){
        
        int i, j, m;
        float Dist[MACROS];
        float accum;
        
        
        
        for(i=0; i<xMAX; i++){
            for(j=0; j<yMAX; j++){
                
                accum = 0.0f;
                
                for(m=0;m<MACROS;m++){
                    Dist[m] = getDist(macrophages[m]->coords.x, macrophages[m]->coords.y, macrophages[m]->coords.z, (float)i, (float)j, 1 );
                    //CD[m] = CentreVEGF-Dist[m];
                    //if(CD[m]<0) CD[m] = 0;
                    //accum+=CD[m];
                }
                
                if((checkForAstro(i, j, 1)==1)&&(grid[i][j][1].type==E)){
                    grid[i][j][1].Eid->VEGF=accum*VconcST;
                }
            }
        }
    }
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
    void World::create_astro_retina_section(void){
        
        Coordinates start1;
        Coordinates start2;
        Coordinates end;
        //int branch = vesselRadius+gap+30;
        Coordinates branch;
        float width =12;
        int drop = 1;
        int dropSpacer = 20;
        vector <float> equ1;
        vector <float> equ2;
        vector <float> equ3;
        int i, j, in;
        float x, x2;
        
        start1.x = ECwidth*0.7;
        start1.y = 0;
        start1.z = 0;
        
        start2.x = ECwidth*3.5;
        start2.y = 0;
        start2.z = 0;
        
        end.x = ECwidth*2.5;
        end.y  = 150;
        end.z = 0;
        
        branch.x = 9;
        branch.y = 65;
        branch.z = 0;
        
        int k,m;
        
        
        int diff=35;
        //get equation of the line through start1 and end
        equ1 = equation_of_line_through_two_points(start1.x, start1.y, end.x, end.y);
        equ2 = equation_of_line_through_two_points(start2.x, start2.y, end.x, end.y);
        equ3 = equation_of_line_through_two_points(branch.x, branch.y, start2.x+5, start2.y+diff-5);
        //for each y between start1.y and end.y calc x and z and create astro in all width upto width, reduce widtrh by drop spacer..
        in=0;
        int count=0;
        for(i=0;i<vesselRadius*2+2;i++){
            for(j=0;j<xMAX;j++){
                for(m=0;m<4;m++){
                grid[j][i][m].Eid->Astro = true;
                grid[j][i][m].Eid->OldAstro = true;
                }
            }
        }
        for(i=start1.y;i<end.y;i++){
            
            
            if(count%dropSpacer==0) width-=drop;
            if(width<2) width=2;
            if((i>=end.y-35)&&(i<=end.y-30)) in++;
            if(i>=end.y-20) in=3;
            if(i>=end.y-5) in=0;
            x = ((float)i-equ1[1])/equ1[0];
            
            for(j=x-(width);j<=x+(width)+in;j++){
                if(insideWorld(j, i, 0)==true){
                    for(m=0;m<4;m++){
                    grid[j][i][m].Eid->Astro = true;
                    grid[j][i][m].Eid->OldAstro = true;
                    }
                }
            }
            count++;
        }
        width =12;
        count=0;
        in =0;
        for(i=start2.y;i<end.y;i++){
            
            if(count%dropSpacer==0) width-=drop;
            if(width<2) width=2;
            if((i>=end.y-35)&&(i<=end.y-30)) in++;
            if(i>=end.y-20) in=3;
            if(i>=end.y-5) in=0;
            if(in<0) in=0;
            x2 = ((float)i-equ2[1])/equ2[0];
            
            for(j=x2-(width)-in;j<=x2+(width);j++){
                if(insideWorld(j, i, 0)==true){
                    for(m=0;m<4;m++){
                    grid[j][i][m].Eid->Astro = true;
                    grid[j][i][m].Eid->OldAstro = true;
                    }
                }
            }
            
            //if((i>=branch)&&(i<branch+branchWidth)){
            //    for(j=10;j<x2;j++){
            //        grid[j][i][0].type=AS;
            //    }
            //}
            
            count++;
        }
        
        width =10;
        count=0;
        for(i=start2.y+diff; i<branch.y; i++){
            
            if(count%4==0) width++;
            //if(width>3) width=4;
            x2 = ((float)i-equ3[1])/equ3[0];
            
            for(j=x2-(width);j<=x2+(width);j++){
                if(insideWorld(j, i, 0)==true){
                    for(m=0;m<4;m++){
                    grid[j][i][m].Eid->Astro = true;
                    grid[j][i][m].Eid->OldAstro = true;
                    }
                }
            }
            
            count++;
        }
        
        //create branch
        
        
    }
    
    
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------

void World::chooseMutants(void){
	
	
    //-----------------------------------
    //random choice.
    int i;
    int* pastNo;
	int CHOSE =5;
    pastNo=(int*)calloc(CHOSE, sizeof(int));
    time_t t;
    int a;
    int tot = 10;
    int ranNo;
    int k = 0;
    float b;
    float c;
    //b = ((float)rand()/(float)RAND_MAX);
    b = ((float)new_rand()/(float)NEW_RAND_MAX);
	
	
	//for(i=0;i<5;i++)
    //Ecells[i]->mutant=true;
    bool random = true;
	
    //if (b<=0.0){  //half the time it selects five random numbers as below
    if(random==true){
		for(i=0;i<CHOSE;i++)pastNo[i]=-1;
		
		
		do
		{
            //a = (int)(((float)rand()*tot)/(float)RAND_MAX);
            a = (int)(((float)new_rand()*tot)/(float)NEW_RAND_MAX);
			
			if(a==0){
				
			}
			if (AlreadyPicked(a, pastNo, CHOSE) == 0)//&&(noneAdj(pastNo) == 0))
			{
				pastNo[k] = a;
				k++;
				
				
				ECagents[a]->mutant=true;
			}
		}
		while (k < CHOSE);
    }
	
	else{   //otherwise selects even spaced. Only two predefined options, so half the time picks one, half other..
		
        //pastNo[0]=0;
		//pastNo[1]=2;
		//pastNo[2]=4;
		//pastNo[3]=6;
		// pastNo[4]=8;
		ECagents[0]->mutant=true;
		ECagents[2]->mutant=true;
		ECagents[4]->mutant=true;
		ECagents[6]->mutant=true;
		ECagents[8]->mutant=true;
	}
	
	
	
	
	
    free(pastNo);
    pastNo=NULL;
	
	
	
    //-----------------------------------
	
    //-----------------------------------
    //three in a row, 5 cells
	
    //-----------------------------------
	
    //-----------------------------------
	
    //-----------------------------------
}
//----------------------------------------------------------------------------------------------

int World::AlreadyPicked(int a, int* pastNo, int chose) {
	
    int i;
    int flag=0;
    int answer;
    //int percent = 70; //70%
    //int totalCells = ((float)ECagents.size()/100.0f)*percent;
	
    for (i = 0; i < chose ; i++)
    {   if (pastNo[i] == a)
	{ flag=1; } /*If any of the array is the same as the random no. just generated, flag is turned to 1*/
    }
	
    if (flag==1)
    {answer=1;}
    else
    {answer=0;}
	
    return(answer);
	
}
//-----------------------------------------------------------------------------------------------------



