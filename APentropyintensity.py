import springAgent
import dit

import numpy as np

import time
import sys

from copy import deepcopy

from ap_functions import *

#RUNTIME INPUTS:    n Time Epsilon Gradient Filconcst Filtipmax Tokenstrength Randfilextend Randfilretract

#Default: 
# runs: 1
# Epsilon: 0.9
# VconcST: 0.04
# filConstNorm: 2.0
# filTipMax: 10
# tokenStrength: 1
# randFilExtend: -1
# randFilRetract: -1
#w = springAgent.World(0.9, 0.04, 2, 2.0, 10, 1, -1, -1)
n=sys.argv[1]
n = int(n)

Time,epsilon,vconcst,gradienttype,F,filtipmax,tokenstrength,filspacing,randfilextend,randfilretract = int(sys.argv[2]),float(sys.argv[3]),float(sys.argv[4]),int(sys.argv[5]),float(sys.argv[6]),float(sys.argv[7]),float(sys.argv[8]),int(sys.argv[9]),float(sys.argv[10]),float(sys.argv[11])
seed = -1
if len(sys.argv) > 12:
    seed = long(sys.argv[12])

if (randfilextend != -1):
    epsilon = 0


names = ['Vegf','Grid Type','Sub_agents','Cell1 SumVegF', 'Cell1 VegfR', 'Cell1 VegfR Active', 'Cell1 DLL', 'Cell2 SumVegF', 'Cell2 VegfR', 'Cell2 VegfR Active', 'Cell2 DLL']
#gradient types: 2 is linear, 7 is bahti




bool_smap_cube_proper = np.load("bool_smap_cube_proper.npy")
#Splitting a bool smap cube proper into two cells
bool_smap_cube_proper_1 = np.append(bool_smap_cube_proper[:20,:,:],np.zeros_like(bool_smap_cube_proper)[:20,:,:],axis=0)
bool_smap_cube_proper_2 = np.append(np.zeros_like(bool_smap_cube_proper)[20:,:,:],bool_smap_cube_proper[20:,:,:],axis=0)





persistence=True



entropies = []
intensities = []
times = []
for i in range(1):
    # print i,eps
#             world = springAgent.World(eps,grad,springAgent.createSeed(),2,1000000)
#             world = springAgent.World(eps,0,springAgent.createSeed(),7,V,F,filtipmax,tokenstrength)
    world = springAgent.World(epsilon,vconcst,gradienttype,F,filtipmax,tokenstrength,filspacing,randfilextend,randfilretract,seed)
    world.simulateTimestep()
    smap_entropies = []
    smap_entropies_cell1 = []
    smap_entropies_cell2 = []
    
    smap_intensities_both = []
    smap_intensities_cell1 = [] 
    smap_intensities_cell2 = []

    decisiontimes = []
    undecided=True
    timespent=0
    for t in range(Time):
        if t%399==0:
            print t
        world.simulateTimestep()
        worldcube = processdata(world.getGridSiteData(),ymax=1016)
        

        cell1_smap = np.array(sensorymap_from_datacube(np.swapaxes(worldcube[:,:,:,5],1,2),zaxis=1,ymax=1016))
        cell2_smap = np.array(sensorymap_from_datacube(np.swapaxes(worldcube[:,:,:,9],1,2),zaxis=1,ymax=1016))
        real_smap = cell1_smap+cell2_smap
                

        histogram = np.array([real_smap[coord[0],coord[1],coord[2]] for coord in np.argwhere(bool_smap_cube_proper!=0)])

        normed_hist = histogram/np.sum(histogram)
        dit_dist = dit.Distribution.from_ndarray(normed_hist)

                
              
                
                
        smap_entropies.append(dit.shannon.entropy(dit_dist))

                
        smap_intensities_both.append(real_smap.sum())


                
                
        if (persistence):
            if undecided:  
                if (np.abs(worldcube[:,:,:,-1].sum() - worldcube[:,:,:,-5].sum())>600):
                    timespent +=1
                    if timespent >= 100:
                        decisiontimes.append(t)
                        times.append(t)
                        undecided=False
                else:
                    timespent=0
        else:
            if undecided:  
                if (np.abs(worldcube[:,:,:,-1].sum() - worldcube[:,:,:,-5].sum())>600):
                    timespent +=1
                    decisiontimes.append(t)
                    times.append(t)
                    undecided=False
                
    try:  
        ti = decisiontimes.pop()
    except:
        ti=0
        times.append(0)
        print('Decision criterion was not met by this run. run {} vconcst {} epsilon {}'.format(i,str(vconcst)[-1],str(epsilon)[-1],time))

    a = np.array(smap_entropies)
    entropies.append(a)

    b = np.array(smap_intensities_both)
    intensities.append(b)

np.save('run {:g} entropy T {:g} epsilon {:g} vconcst {:g} gradienttype {:g} F {:g} filtipmax {:g} tokenstrength {:g} filspacing {:g} randfilextend {:g} randfilretract {:g} seed {:g}'.format(n,Time,epsilon,vconcst,gradienttype,F,filtipmax,tokenstrength, filspacing, randfilextend, randfilretract, seed),np.array(entropies))
np.save('run {:g} intensity T {:g} epsilon {:g} vconcst {:g} gradienttype {:g} F {:g} filtipmax {:g} tokenstrength {:g} filspacing {:g} randfilextend {:g} randfilretract {:g} seed {:g}'.format(n,Time,epsilon,vconcst,gradienttype,F,filtipmax,tokenstrength,filspacing, randfilextend, randfilretract, seed),np.array(intensities))
np.save('run {:g} time T {:g} epsilon {:g} vconcst {:g} gradienttype {:g} F {:g} filtipmax {:g} tokenstrength {:g} filspacing {:g} randfilextend {:g} randfilretract {:g} seed {:g}'.format(n,Time,epsilon,vconcst,gradienttype,F,filtipmax,tokenstrength,filspacing, randfilextend, randfilretract, seed),np.array(times)-100)


