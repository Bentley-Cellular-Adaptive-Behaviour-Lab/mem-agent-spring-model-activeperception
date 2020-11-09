

from __future__ import division

import springAgent
import dit

import numpy as np

import time

from copy import deepcopy
from sklearn.preprocessing import KBinsDiscretizer  


from itertools import product
from random import randint, sample

import random
from copy import deepcopy

from dit.inference import distribution_from_data
from dit.multivariate import coinformation as I
from dit.multivariate import intrinsic_total_correlation as IMI
from itertools import combinations





def sensorymap_from_datacube(datacube,maptype='cylindrical',zaxis=3,zrange=40,ymax=80):
    """
    2 cells gets map
    parameters
    -----------
    datacube: 
        40x80x20x11 cube of values 
        'Vegf','Grid Type','Sub_agents',
        'Cell1 SumVegF', 'Cell1 VegfR', 'Cell1 VegfR Active', 'Cell1 DLL',
        'Cell2 SumVegF', 'Cell2 VegfR', 'Cell2 VegfR Active', 'Cell2 DLL'
    maptype:
        'Cylyndrical' or 'Rectangular'... etc
    case:
        Which axis is the z direction (80)
        

        
    returns 
    sensorymap:
        one for each cell?
    """
    
    sensorymap = []
    for i in range(zrange):
        if zaxis==1:
            square = sensorymap_from_datasquare(datacube[i,:,:],ymax=ymax)
        elif zaxis==2:
            square = sensorymap_from_datasquare(datacube[:,i,:],ymax=ymax)
        elif zaxis==3:
            square = sensorymap_from_datasquare(datacube[:,:,i],ymax=ymax)
        sensorymap.append(square)
    
    
    return sensorymap
    




def processdata(data,filopodia=False,ymax=80):
    """
    Turns data into cube
    """
    a = data
    b = np.array(a)   
    
    if filopodia:
        c = np.zeros((40,ymax,20,2))
        for i in range(len(a[0])):
            c[int(np.around(a[0][i][0])),int(np.around(a[0][i][1])),int(np.around(a[0][i][2])),0] = a[0][i][-1]
            
        for i in range(len(a[1])):
            c[int(np.around(a[1][i][0])),int(np.around(a[1][i][1])),int(np.around(a[1][i][2])),1] = a[1][i][-1]
    else:
        c = np.zeros((40,ymax,20,11))-1
        for i in range(len(a)):
            assert(int(a[i][0])==a[i][0] and int(a[i][1])==a[i][1] and int(a[i][2])==a[i][2]) #make sure indices are ints


            c[int(a[i][0]),int(a[i][1]),int(a[i][2])] = b[i,3:]  #populate worldcube
        assert c[c<0].shape == (0,)

    
    return c


def sensorymap_from_datasquare(square,maptype='circular',ymax=80):
    """
    A first step for the cube.
    """

    squarecopy = deepcopy(square)
    listtoadd = []
    centerx = 10.0
    centery = 10.0
    radius = 5.0
    for x in range(20):
        for y in range(ymax):
            if squarecopy[x,y]>=1:
                xc,yc = np.array([centerx,centery]) + radius *np.array([x-centerx,y-centery])/(np.sqrt((x-centerx)**2 + (y-centery)**2))
                for _ in range(int(np.around(squarecopy[x,y]))):
    #                 print square[x,y]
#                     print x,y,'Free'
#                     print xc,yc,'Captured'
                    listtoadd.append([xc,yc])
                squarecopy[x,y] = 0
    for coord in listtoadd:
        coord = np.around(coord)

        squarecopy[int(coord[0]),int(coord[1])]+=1
    return squarecopy
    
def generatefakedata(N=1000,xmin=0,ymin=0,xmax=20,ymax=20,cylinder=True,cube=False,circle=True):
    """
    Outdated Function 

    outputs square with scattered data
    
    If cube and cylinder, returns 20,40,80 populated data cube
    
    if not cube, returns 20,40 square
    
    
    Circle means circularly configured data
    """
    if cube and cylinder:
        cube = np.zeros((20,40,80))
        
        xs = np.random.randint(xmin,xmax,N)
        ys = np.random.randint(ymin,ymax,N)
        zs = np.random.randint(0,80,N)
        
        datacoords = []
        for x,y,z in zip(xs,ys,zs):
            if (x-10)**2 + (y-10)**2 >25:
                if circle:
                    if (x-10)**2 + (y-10)**2 <100:  #To generate inside of circle
                        datacoords.append((x,y,z))
                else:
                    datacoords.append((x,y,z))

        for coord in datacoords:
            cube[coord[0],coord[1],coord[2]] += 1
            
        return cube
    else:
        square = np.zeros((20,40))

        xs = np.random.randint(xmin,xmax,N)
        ys = np.random.randint(ymin,ymax,N)

        datacoords = []
        for x,y in zip(xs,ys):
            if (x-10)**2 + (y-10)**2 >25:
                if circle:
                    if (x-10)**2 + (y-10)**2 <100:  #To generate inside of circle
                        datacoords.append((x,y))
                else:
                    datacoords.append((x,y))

        for coord in datacoords:
            square[coord[0],coord[1]] += 1
        return square

def asymmetric_std(X):
    """
    Returns Upper and Lower std over first axis in shape of second axis of input.
    """
    X += 1e-3 * np.random.randn(*X.shape)  #Adding noise to prevent 0 count on first datapoint

    errors = (X - X.mean(axis=0))

    errorsplus = np.where(errors>0,errors,0)
    errorspluscount = errorsplus.astype(bool).astype(int).sum(axis=0)

    errorsminus = np.where(errors<0,errors,0)
    errorsminuscount = errorsminus.astype(bool).astype(int).sum(axis=0)


    # errorspluscount = np.where(errorspluscount>.5,errorspluscount,1)
    # errorsminuscount = np.where(errorsminuscount>.5,errorsminuscount,1)

    upper_std = np.sqrt(np.square(errorsplus).sum(axis=0) / errorspluscount)
    lower_std = np.sqrt(np.square(errorsminus).sum(axis=0) / errorsminuscount)


    
    return(upper_std,lower_std)


def getTE(timeseriesa,timeseriesb,history=1):
    """
    Takes two time series and calculates the pairwise transfer entropies 
    
    Parameters
    ----------
    a,b: np.array
        Two time series
    history: int
        Markov Order

    
    Returns
    ---------
    list of Floats containing Transfer Entropy, Intrinsic Transfer Entropy, Transfer Entropy,
    backwards Transfer Entropy, backwards intrinsic Transfer Entropy of the pairs of nodes addressed by input a,b
    """
    pairofnodes = list(zip(timeseriesa,timeseriesb))
    #list(zip(two lists of ints))
#     pairofnodes=list(zip(rbn.histories[a],rbn.histories[b]))
    distribution=distribution_from_data(pairofnodes,history+1,base='linear')  
    dist=dit.distconst.modify_outcomes(distribution,lambda o: (tuple([oo[0] for oo in o[:-1]]), tuple([oo[1] for oo in o[:-1]]), o[-1][0], o[-1][1]))
    I0,J0,I1,J1=([0],[1],[2],[3])
    TE=I(dist,[J1,I0],J0)
#     ITE=IMI(dist,[J1,I0],J0)
    backTE=I(dist,[I1,J0],I0)
#     backITE=IMI(dist,[I1,J0],I0)
    return [TE,backTE,0,0]


def global_transfer_entropy(M,S,history):
    """
    All to All TE
    
    M->S
    S->M
    
    Input
    ------------
    M:
       ndarray Mdim x T
        Time series of Sensory Map of Movements.
    S: 
       ndarray Mdim x T
        Time series Sensory Map of VEGFR Activations
    History:
        int
        Time delay hyperparameter for TE.
        
    Returns
    --------------
    TEgrid:
        Shape like S and M
        
    """
    
    msize = len(M)
    TEgrid = np.zeros((msize,msize,2))
    
    for i in range(msize):
        for j in range(msize):
            TEtemp = getTE(M[i],S[j],history)
            TEgrid[i,j,0] = TEtemp[0] #forward
            TEgrid[i,j,1] = TEtemp[1] #backward
    
    
    return TEgrid




def get_SM_series(time=800,epsilon=.9,gradient=.04,gradienttype=2,yBaseline=10000,F=2,filtipmax=20,tokenstrength=1,randfilextension=-1,randfilretract=-1,want_RGB=True,NewVersion=True,ymax=116):
    """
    Crucial Assumption: This counts all VEGFR activations in specific grid sites as discrete units!
    
    parameters
    ----------
    time: int
        How many time steps of simulation
    epsilon: float
        Guidance Parameter for msm model
    gradient: float
        Gradient Steepness parameter (Vconcst)
    gradienttype: int
        Gradient Type. 2 for normal, 5 for semi-uniform?
    yBaseline: float
        Baseline y parameter for gradient type 5.
    F: float
        Filopodia extension parameter. More = more filopodia extension. standard 2. 
    filtipmax: float
        parameter determining How long filopodia wait till retracting
    tokenStrength: float
        Some other parameter for msm that I don't know the details of.
        
    NOT COARSE GRAINED AT ALL?
    Outputs Mseries and Sseries, a flattened list of time series for the entire sensory map
    """
    if NewVersion:

        time_of_patterning = time
        world = springAgent.World(epsilon,gradient,gradienttype,F,filtipmax,tokenstrength,randfilextension,randfilretract)
        world.simulateTimestep()
        alternative_argmask = np.load('alternative_argmask.npy')


                #Gathering Time Series. Don't discretize here, so you can try different methods later.
        Mseries = [[] for _ in range(40)]
        Sseries = [[] for _ in range(40)]
        SseriesRGB = [[] for _ in range(40)]
        uniqueargs=[]
        uniqueargsRGB=[]
        undecided=True
        for t in range(time):
            if t<(100000000000+50):
                if t%100==0: print t
                world.simulateTimestep()
                    #     Sworldcube = pass
                Mworldcube = np.array(world.getGridMapOfFilopodiaMovement())
                Mworldcube = np.swapaxes(Mworldcube[:,:,:,0],1,2) #This shouldnt need to happen in the future
                Msensorymap = np.array(sensorymap_from_datacube(Mworldcube,zaxis=1,ymax=ymax))
                flattened_Msensorymap = Msensorymap.sum(axis=0) #Coarse Graining Space to lower dimensionality (cube to square)
                        #try vectorizing all of that, below:
                Msensorymap_masked = np.array([flattened_Msensorymap[alternative_argmask[i][0],alternative_argmask[i,1]] for i in range(len(alternative_argmask))])
                for i in range(len(Mseries)):
                    Mseries[i].append(Msensorymap_masked[i])

                Sdata = world.getGridSiteData()
                Sworldcube = np.array(processdata(Sdata,ymax=ymax))
                if undecided:
                    if (np.abs(Sworldcube[:,:,:,-1].sum() - Sworldcube[:,:,:,-5].sum())>600):
                        time_of_patterning = t
                        undecided=False
                #         if t==(time-1):
                            
                #             return Sworldcube.astype(bool).astype(int)
                #             pass
                SworldcubeRGB = np.swapaxes(Sworldcube[:,:,:,2],1,2)
        #         print SworldcubeRGB.shape
                Sworldcube = Sworldcube.astype(bool).astype(int)  #Counting all activations as 1
                Sworldcube = np.swapaxes(Sworldcube[:,:,:,5],1,2) + np.swapaxes(Sworldcube[:,:,:,-2],1,2)   #both cells. Also deleting other data than activations

                summedcube = Sworldcube.sum(axis=0)   #flatten it in order to see where to normalize later.
                summedcubeRGB = SworldcubeRGB.sum(axis=0)

                args = np.argwhere(summedcube>=30)    # normalize the membrane layer bits to focus on Filopodia

                argsRGB = np.argwhere(summedcubeRGB >= np.max(summedcubeRGB)/2)
                
                uniqueargs.append(np.unique(np.concatenate(np.array(args))))
                uniqueargsRGB.append(np.unique(np.concatenate(np.array(argsRGB))))
                for i in range(len(args)):
                    Sworldcube[:,args[i][0],args[i][1]] = 0 # or 1/40
                for i in range(len(argsRGB)):
                    SworldcubeRGB[:,argsRGB[i][0],argsRGB[i][1]] = 0 
                SsensorymapRGB = np.array(sensorymap_from_datacube(SworldcubeRGB,zaxis=1,ymax=ymax))
                Ssensorymap = np.array(sensorymap_from_datacube(Sworldcube,zaxis=1,ymax=ymax))

                flattened_Ssensorymap = Ssensorymap.sum(axis=0)
                flattened_SsensorymapRGB = SsensorymapRGB.sum(axis=0)

                Ssensorymap_masked = np.array([flattened_Ssensorymap[alternative_argmask[i][0],alternative_argmask[i,1]] for i in range(len(alternative_argmask))])
                Ssensorymap_maskedRGB = np.array([flattened_SsensorymapRGB[alternative_argmask[i][0],alternative_argmask[i,1]] for i in range(len(alternative_argmask))])
                for i in range(len(Sseries)):
                    Sseries[i].append(Ssensorymap_masked[i])   
                for i in range(len(SseriesRGB)):
                    SseriesRGB[i].append(Ssensorymap_maskedRGB[i])
            else: break 
            

    Mseries = np.array(Mseries)  
    Sseries = np.array(Sseries)
    SseriesRGB = np.array(SseriesRGB)
    
    if want_RGB==True:
        return Mseries,Sseries,SseriesRGB
    else:
        return Mseries,Sseries

    

    #     Sworldcube = 

        #don't forget to count both cells    



class coarsegrain_smap_sensing(object):
    """
    This class coarse grains the vegfr activations in the way that they are currently counted (as 1's)
    
    Is there a reason to keep counting them as 1's if I am just gonna coarse grain anyway? Maybe I should just
    coarse grain the raw activation values directly...  edit: For now, I will continue counting as 1s because it makes 
                                                        it easier to normalize the membrane activation values.
    
    Will use Sklearn Kbins discretizer to bin sensory map data.
    
    I suppose I should have either one discretization to account for all of the parameters of World, or
    a custom discretization for each combination (Probably Custom if quantile)
    
    """
    def __init__(self,N=4,binningstrategy='quantile',load_data='discretizer_data.npy'):
        """
        Load in densely sampled values that you wanna coarse grain, then based on those statistics it will
        coarse grain your sparsely sampled stuff.
        
        Try the different binning strategies from the docs of KBinsDiscretizer!
        
        """
        if type(load_data)==type('stringtype'):
            samples = np.concatenate(np.load(load_data)) #load and flatten
        else:
            samples = load_data
            everything = []
            for i in range(len(samples)):
                for j in range(40):
                    for k in range(len(samples[i][j])):
                        everything.append(samples[i][j][k])
            samples = np.array(everything)

        self.Kbins = KBinsDiscretizer(n_bins=N,encode='ordinal',strategy=binningstrategy)
        self.Kbins.fit(samples.reshape(-1,1))  #fits discretizer to relevant samples.
        
    def coarsegrain(self,x):
        """
        x: Numpy array 1d
        """
        return(self.Kbins.transform(x.reshape(-1,1)))



