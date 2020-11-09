#!/bin/bash
#SBATCH --job-name=APSingleCodebase_Simulation
#SBATCH --nodes=1                   
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=72:00:00
#SBATCH --mem=5980mb

echo "Total $# arguments passed to me are: $*"

epsilon=0.9
vconcst=0.04
gradient=2
filconstnorm=2.0
filtipmax=15
tokenstrength=1
filspacing=2
randFilExtend=-1
randFilRetract=-1
analysis="time_to_pattern"
maxtime=5000 #just for pybind version
seed=-1

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    --epsilon)
    epsilon="$2"
    shift # past argument
    shift # past value
    ;;
    --vconcst)
    vconcst="$2"
    shift # past argument
    shift # past value
    ;;
    --gradient)
    gradient="$2"
    shift # past argument
    shift # past value
    ;;
    --filconstnorm)
    filconstnorm="$2"
    shift # past argument
    shift # past value
    ;;
    --filtipmax)
    filtipmax="$2"
    shift # past argument
    shift # past value
    ;;
    --tokenstrength)
    tokenstrength="$2"
    shift # past argument
    shift # past value
    ;;
    --filspacing)
    filspacing="$2"
    shift # past argument
    shift # past value
    ;;
    --randFilExtend)
    randFilExtend="$2"
    shift # past argument
    shift # past value
    ;;
    --randFilRetract)
    randFilRetract="$2"
    shift # past argument
    shift # past value
    ;;
    --analysis)
    analysis="$2"
    shift # past argument
    shift # past value
    ;;
    --maxtime)
    maxtime="$2"
    shift # past argument
    shift # past value
    ;;
    --seed)
    seed="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;; 
esac
done

echo "analysis: $analysis"
echo "epsilon: $epsilon"
echo "vconcst: $vconcst"
echo "gradient: $gradient"
echo "filconstnorm: $filconstnorm"
echo "filtipmax: $filtipmax"
echo "tokenstrength: $tokenstrength"
echo "filspacing: $filspacing"
echo "randFilExtend: $randFilExtend"
echo "randFilRetract: $randFilRetract"
echo "maxtime: $maxtime (only if pybind version)"

# ml purge
ml foss
if [ "$analysis" == "pybind" ]
then
    ml Python/2.7.16-GCCcore-8.3.0
    source /camp/lab/bentleyk/home/shared/"$USER"/APSingleCodebase/bin/activate
    echo "running pybind version, passing in the following arguments: run: $SLURM_ARRAY_TASK_ID maxtime: $maxtime epsilon: $epsilon vconcst: $vconcst gradient: $gradient filconstnorm: $filconstnorm filtipmax: $filtipmax tokenstrength: $tokenstrength filspacing: $filspacing randFilExtend: $randFilExtend randFilRetract: $randFilRetract seed: $seed"
    python APentropyintensity.py $SLURM_ARRAY_TASK_ID $maxtime $epsilon $vconcst $gradient $filconstnorm $filtipmax $tokenstrength $filspacing $randFilExtend $randFilRetract $seed
else
    echo "running executable, passing in the following arguments: run: $SLURM_ARRAY_TASK_ID epsilon: $epsilon vconcst: $vconcst gradient: $gradient filconstnorm: $filconstnorm filtipmax: $filtipmax tokenstrength: $tokenstrength filspacing: $filspacing randFilExtend: $randFilExtend randFilRetract: $randFilRetract seed: $seed"
    ./springAgent $SLURM_ARRAY_TASK_ID $epsilon $vconcst $gradient $filconstnorm $filtipmax $tokenstrength $filspacing $randFilExtend $randFilRetract $seed
fi