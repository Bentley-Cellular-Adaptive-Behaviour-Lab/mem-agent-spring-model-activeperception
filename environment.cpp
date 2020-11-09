#include <iostream>
#include <vector>
#include "objects.h"
#include <math.h>
#include <cstdlib>

//---------------------------------------------------------------
void World::updateEnvironment(void){

    //check_if_InsideVessel();


//updateVEGF();

//cout<<macrophages[0]->coords.x<<endl;
    //grid[(int)((float)ECwidth/2.0f)][(int)((float)ECwidth/2.0f)][1].Eid->VEGF = 10.0f;



}

//-----------------------------------------------------------------
void World::check_if_InsideVessel(void){

    int i,j,k;
    
    for(i=0;i<xMAX;i++){
                for(j=0;j<yMAX;j++){
                    for(k=0;k<zMAX;k++){
                        if(grid[i][j][k].Eid!=NULL){
                            //only checking if was already inside...
                            if(grid[i][j][k].Eid->inside==true)
                                grid[i][j][k].Eid->calcInside();
                        }

                    }
                }
            }
}
//-----------------------------------------------------------------
//-------------------------------------------------------------------------
void Env::calcInside(void){

    int i;
    int flag=0;
    bool flagInside = false;
    bool flagInsideInside = false;
    EC* cell=NULL;
    i=0;
   
    //y axis----------------------------------
    do{
        if(worldP->grid[Ex][Ey+i][Ez].Mids.size()>0){
            flagInside=true;
            flag=1;
            //cell = worldP->grid[Ex][Ey+i][Ez].Mids[0]->Cell;
        }
        i++;
    }while((Ey+i<yMAX)&&(flag==0));

    if(flagInside==true){
        flag=0;
        flagInside=false;
        i=0;
        do{
            if(worldP->grid[Ex][Ey-i][Ez].Mids.size()>0){
                flag=1;
                //if(worldP->grid[Ex][Ey-i][Ez].Mids[0]->Cell==cell)
                flagInside=true;

            }
            i++;
        }while((Ey-i>=0)&&(flag==0));


        //z axis------------------------------------------------
            if(flagInside==true){
                flag=0;
            flagInside=false;
            i=0;
            do{
                if(worldP->grid[Ex][Ey][Ez+i].Mids.size()>0){

                    flag=1;
                    //if(worldP->grid[Ex][Ey][Ez+i].Mids[0]->Cell==cell)
                    flagInside=true;

                }
                i++;
            }while((Ez+i<zMAX)&&(flag==0));

            if(flagInside==true){
                flag=0;
                flagInside=false;
                i=0;
                do{
                    if(worldP->grid[Ex][Ey][Ez-i].Mids.size()>0){
                        flag=1;
                        //if(worldP->grid[Ex][Ey][Ez-i].Mids[0]->Cell==cell)
                        flagInside=true;

                    }
                    i++;
                }while((Ez-i>=0)&&(flag==0));
                if(flagInside==true) inside=true;


            }
            else{
            inside=false;
        }
        }else{
            inside=false;
        }
    }

        else{
            inside=false;
        }

    //try to fill in the gaps that seems to escape the above algorithm...if has insde blocks in all directions then must be inside...
    /*if(flagInside==false){
    //y axis----------------------------------
    do{
        if(worldP->grid[Ex][Ey+i][Ez].Eid->inside==true){
            flagInside=true;
            flag=1;
            //cell = worldP->grid[Ex][Ey+i][Ez].Mids[0]->Cell;
        }
        i++;
    }while((Ey+i<yMAX)&&(flag==0));

    if(flagInside==true){
        flag=0;
        flagInside=false;
        i=0;
        do{
            if(worldP->grid[Ex][Ey-i][Ez].Mids.size()>0){
                flag=1;
                //if(worldP->grid[Ex][Ey-i][Ez].Mids[0]->Cell==cell)
                flagInside=true;

            }
            i++;
        }while((Ey-i>=0)&&(flag==0));


        //z axis------------------------------------------------
            if(flagInside==true){
                flag=0;
            flagInside=false;
            i=0;
            do{
                if(worldP->grid[Ex][Ey][Ez+i].Mids.size()>0){:)

                    flag=1;
                    //if(worldP->grid[Ex][Ey][Ez+i].Mids[0]->Cell==cell)
                    flagInside=true;

                }
                i++;
            }while((Ez+i<zMAX)&&(flag==0));

            if(flagInside==true){
                flag=0;
                flagInside=false;
                i=0;
                do{
                    if(worldP->grid[Ex][Ey][Ez-i].Mids.size()>0){
                        flag=1;
                        //if(worldP->grid[Ex][Ey][Ez-i].Mids[0]->Cell==cell)
                        flagInside=true;

                    }
                    i++;
                }while((Ez-i>=0)&&(flag==0));
                if(flagInside==true) inside=true;


            }
            else{
            inside=false;
        }
        }else{
            inside=false;
        }
    }

        else{
            inside=false;
        }
*/
    //if(inside==true)cout<<"true"<<endl;
}
//-------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void World::calcEnvAgentVEGF(Env* ep){


        int yDist;
int J;
        float accum;
        int m;
        float Dist[MACROS], CD[MACROS];
        float XA, YA, ZA;
        int CentreVEGF;
        int div =4;
        float linVAR = 0.3f;
        float minus;
        //if(ASTRO==UNIFORM) CentreVEGF=40;
        //else
        CentreVEGF=30;
        int CentreVEGF_B=50;
        
        float random;

        random = 0;//3*(rand()/RAND_MAX);
        
        if(ENV_SETUP==1){
        if(ep->blood==0.0f){
            
            J=ep->Ey;
            
            if(VEGFgradient==STEADY){
                //if(J>vesselRadius+gap){
                //s=(int)((float)J/(float)STEP);
                ep->VEGF=(J)*VconcST;
                //}
            }
            else if(VEGFgradient==FLAT)
                ep->VEGF=VEGFconc;
            else if(VEGFgradient==EXP)
                ep->VEGF=0.0001*exp(J);
            
            else if(VEGFgradient==ASTRO_LINEAR){
                if(J>gap+(2*vesselRadius)-4){
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        //minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                        minus = new_rand()*(float)(linVAR)/(float)NEW_RAND_MAX;
                        ep->VEGF=J*VconcST - minus;
                        if(ep->VEGF<0) ep->VEGF=0.0f;
                        
                    }
                    
                }
            }
            else if(VEGFgradient==ASTRO_UNIFORM){
                
              
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        ep->VEGF=VEGFconc;
                    }
                    
                
            }
            else if(VEGFgradient==FIXED_MACROS){
                accum = 0.0f;
                if(J>gap+(vesselRadius)){
                for(m=0;m<MACROS;m++){
                    
                    //toroidal
                    XA = macrophages[m]->coords.x - ep->Ex;
                    
                    /**(sqrt(XA*XA)>=(float)xMAX/2.0f){
                    
                        if(XA>0) XA= -((float)xMAX- XA);
                        else XA=(float)xMAX- fabs(XA);
                    
                        YA = macrophages[m]->coords.y - ep->Ey;
                        ZA = macrophages[m]->coords.z - ep->Ez;
                    
                        Dist[m]=sqrt((XA*XA)+(YA*YA)+(ZA*ZA));
                    
                    }
                    else*/
                    Dist[m] = getDist(macrophages[m]->coords.x, macrophages[m]->coords.y, macrophages[m]->coords.z, ep->Ex, ep->Ey, ep->Ez);
                    
                    
                    if(Dist[m]<=CentreVEGF){
                        CD[m] = CentreVEGF-Dist[m];
                    }
                    else CD[m]=0.0f;
                    
                    
                    accum+=CD[m];
                
                }
                
                if(ASTRO!=NONE){
                    if(ep->Ez>2){
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        
                        ep->VEGF=accum*VconcSTMACRO;
                        
                    }
                    if(BACKGROUND_VEGF==STEADY){
                        
                        if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                            //minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                            minus = new_rand()*(float)(linVAR)/(float)NEW_RAND_MAX;
                            ep->VEGF+=VEGFbase+(J*VconcST) - minus;
                            if(ep->VEGF<0) ep->VEGF=0.0f;
                            
                        }
                        
                        
                    }
                    else if(BACKGROUND_VEGF==FLAT){
                        if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                            //minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                            minus = new_rand()*(float)(linVAR)/(float)NEW_RAND_MAX;
                            ep->VEGF+=VEGFconc-minus;
                            if(ep->VEGF<0) ep->VEGF=0.0f;
                        }
                        
                    }
                    }
                }
                else{
                    if(ep->Ez==0){
                        ep->VEGF=accum*VconcST;
                        
                        if(BACKGROUND_VEGF==STEADY){
                            if(J*VconcST>10){
                                ep->VEGF+=10.0f;
                            }
                            else{
                                ep->VEGF+=J*VconcST;
                            }
                            
                        }
                        else if(BACKGROUND_VEGF==FLAT){
                            ep->VEGF+=VEGFconc;
                        }
                    }
                }
                
                }
            }
            
            if(getDist(0, ep->Ey, ep->Ez, 0, vesselCentreY, vesselCentreZ)<vesselRadius) ep->VEGF=0.0f;
        }
        }
        else if(ENV_SETUP==6){

        if(ep->blood==0.0f){

                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
           
                        ep->VEGF=VEGFbase+(ep->Ex*VconcST);
                        


                    }
                    else{
                        ep->VEGF=(VEGFbase+(ep->Ex*VconcST))*0.01;
                    }
        }
        }

            

                    
}
//-------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
 /*   void World::calcEnvAgentVEGF(Env* ep){

        int J;
        float accum;
        int m;
        float Dist[MACROS], CD[MACROS];
        float XA, YA, ZA;
        int CentreVEGF=28;
        int div =4;
        float linVAR = 0.3f;
        float minus;
        //if(ASTRO==UNIFORM) CentreVEGF=40;
        //else
        //cout<<CentreVEGF<<endl;

        //float d = ((float)rand()*(float)VconcST/(float)RAND_MAX);
        //d=d-1.0f;
	 if(ep->blood==0.0f){

            J=ep->Ey;

            if(VEGFgradient==STEADY){
                //if(J>vesselRadius+gap){
                //s=(int)((float)J/(float)STEP);
                ep->VEGF=(J)*VconcST;
                //}
            }
            else if(VEGFgradient==FLAT)
                ep->VEGF=VEGFconc;
            else if(VEGFgradient==EXP)
                ep->VEGF=0.0001*exp(J);

            else if(VEGFgradient==ASTRO_LINEAR){
                if(J>gap+(2*vesselRadius)-(vesselRadius-2)){
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                        ep->VEGF=J*VconcST - minus;
                        if(ep->VEGF<0) ep->VEGF=0.0f;

                    }

                }
            }
            else if(VEGFgradient==ASTRO_UNIFORM){
                if(test==0){
                    //if(J>gap+(2*vesselRadius)-4){
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        ep->VEGF=VEGFconc;
                    }
                    //}
                }
                else{
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                        ep->VEGF=VEGFconc;
                    }

                }
            }
            else if(VEGFgradient==FIXED_MACROS){

                accum = 0.0f;
                if(J>gap+2+(vesselRadius)){
                for(m=0;m<MACROS;m++){

                    //toroidal
                    XA = macrophages[m]->coords.x - ep->Ex;

                    (sqrt(XA*XA)>=(float)xMAX/2.0f){

                        if(XA>0) XA= -((float)xMAX- XA);
                        else XA=(float)xMAX- fabs(XA);

                        YA = macrophages[m]->coords.y - ep->Ey;
                        ZA = macrophages[m]->coords.z - ep->Ez;

                        Dist[m]=sqrt((XA*XA)+(YA*YA)+(ZA*ZA));

                    }
                    else*/
    /*
                    Dist[m] = getDist(macrophages[m]->coords.x, macrophages[m]->coords.y,0, ep->Ex, ep->Ey, 0);


                    if(Dist[m]<=CentreVEGF){
                        //cout<<Dist[m]<<endl;
                        CD[m] = CentreVEGF-Dist[m];
                    }
                    else CD[m]=0.0f;


                    accum+=CD[m];

                }

                if(ASTRO!=NONE){
                    if(ep->Ez>2){
                    if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){

                        ep->VEGF=accum*VconcSTMACRO;

                    }
                    if(BACKGROUND_VEGF==STEADY){

                        if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                            minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                            ep->VEGF+=J*VconcST - minus+VEGFbase;
                            if(ep->VEGF<0) ep->VEGF=0.0f;

                        }
                        else{
                            //holger said something like 10% of vegf might be floating about...
                            minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                            ep->VEGF+=0.001*(J*VconcST - minus);
                            if(ep->VEGF<0) ep->VEGF=0.0f;
                        }


                    }
                    else if(BACKGROUND_VEGF==FLAT){
                        if(checkForAstro(ep->Ex, ep->Ey, ep->Ez)==1){
                            minus = rand()*(float)(linVAR)/(float)RAND_MAX;
                            ep->VEGF+=VEGFconc-minus;
                            if(ep->VEGF<0) ep->VEGF=0.0f;
                        }

                    }
                    }
                }
                else{
                    if(ep->Ez==0){
                        ep->VEGF=accum*VconcST;

                        if(BACKGROUND_VEGF==STEADY){
                            if(J*VconcST>10){
                                ep->VEGF+=10.0f;
                            }
                            else{
                                ep->VEGF+=J*VconcST;
                            }

                        }
                        else if(BACKGROUND_VEGF==FLAT){
                            ep->VEGF+=VEGFconc;
                        }
                    }
                }

                }
            }

            if(getDist(0, ep->Ey, ep->Ez, 0, vesselCentreY, vesselCentreZ)<vesselRadius) ep->VEGF=0.0f;
        }
    }
*/
//-------------------------------------------------------------------------
