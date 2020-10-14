analysis="time_to_pattern"
vary1_param=
vary1_vals=()

vary2_param=
vary2_vals=()

numberOfRuns=1
epsilon=0.9
vconcst=0.04
gradient=2
filconstnorm=2.0
filtipmax=15
tokenstrength=1
filspacing=2
randFilExtend=-1
randFilRetract=-1
maxtime=5000 #just for pybind version

# other_args=()
POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"
case $key in
    --runs)
    numberOfRuns="$2"
    shift # past argument
    shift # past value
    ;;
    --analysis)
    analysis="$2"
    # break
    shift # past argument
    shift # past value
    ;;
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
    --maxtime)
    maxtime="$2"
    shift # past argument
    shift # past value
    ;;
    --vary1)
    vary1_param="$2"
    vary1_vals+="$3"
    shift # past argument
    shift # past value
    shift # past value
    ;;
    --vary2)
    vary2_param="$2"
    vary2_vals+="$3"
    shift # past argument
    shift # past value
    shift # past value
    ;;
    --vary3)
    vary3_param="$2"
    vary3_vals+="$3"
    shift # past argument
    shift # past value
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done

timestamp=$(date "+%Y.%m.%d-%H.%M.%S")
local_output_foldername="camp_output_analysis_"$analysis"_"$timestamp
camp_subfolder_name="APSingleCodebase/"$analysis"_"$timestamp

echo "camp folder name: $camp_subfolder_name"

mkdir $local_output_foldername
camp_home="/camp/lab/bentleyk/home/shared/$USER"
echo "analysis type: $analysis"
echo "number of runs: $numberOfRuns"
echo "uploading to simulation files CAMP..."
# rsync -r --include="*/" --include='*.'{sh,cpp,h,py,npy,pyc} --include="makefile" --include="requirements" --exclude="*" --delete-excluded ./ login.camp.thecrick.org:~/"$camp_subfolder_name"/
rsync -r --include='*.'{sh,cpp,h,py,npy,pyc} --include="makefile" --include="requirements" --exclude="*" --delete-excluded ./ login.camp.thecrick.org:"$camp_home"/"$camp_subfolder_name"/
echo "uploaded files to CAMP... logging in"

if [ "$analysis" == "pybind" ]
then 
    ssh login.camp.thecrick.org "echo \"setting up virtual env \"; cd $camp_home/APSingleCodebase; ml Python/2.7.16-GCCcore-8.3.0; python -m virtualenv ./; source bin/activate; pip install dit; pip install numpy; pip install sklearn; pip install pybind11; echo  \"building pybind springAgent...\"; cd ../$camp_subfolder_name; ./buildSpringAgent.sh --analysis $analysis --camp; echo \"built pybind\"; exit;"
else
    ssh login.camp.thecrick.org  "echo  \"log in successful... setting up environment\"; cd $camp_home/$camp_subfolder_name; ml purge; ml foss; echo \"running make... \"; ./buildSpringAgent.sh --analysis \"$analysis\"; echo \" finished building spring agent\"; exit;"
fi

for vary1_val in $vary1_vals
do
    if [ "$vary1_param" == "epsilon" ]
    then
        epsilon=$vary1_val
    elif [ "$vary1_param" == "vconcst" ]
    then
        vconcst=$vary1_val
    elif [ "$vary1_param" == "gradient" ]
    then
        gradient=$vary1_val
    elif [ "$vary1_param" == "filconstnorm" ]
    then
        filconstnorm=$vary1_val
    elif [ "$vary1_param" == "filtipmax" ]
    then
        filtipmax=$vary1_val
    elif [ "$vary1_param" == "tokenstrength" ]
    then
        tokenstrength=$vary1_val
    elif [ "$vary1_param" == "filspacing" ]
    then
        filspacing=$vary1_val
    elif [ "$vary1_param" == "randFilExtend" ]
    then
        randFilExtend=$vary1_val
    elif [ "$vary1_param" == "randFilExtend" ]
    then
        randFilRetract=$vary1_val
    fi

    for vary2_val in $vary2_vals
    do
        if [ "$vary2_param" == "epsilon" ]
        then
            epsilon=$vary2_val
        elif [ "$vary2_param" == "vconcst" ]
        then
            vconcst=$vary2_val
        elif [ "$vary2_param" == "gradient" ]
        then
            gradient=$vary2_val
        elif [ "$vary2_param" == "filconstnorm" ]
        then
            filconstnorm=$vary2_val
        elif [ "$vary2_param" == "filtipmax" ]
        then
            filtipmax=$vary2_val
        elif [ "$vary2_param" == "tokenstrength" ]
        then
            tokenstrength=$vary2_val
        elif [ "$vary2_param" == "filspacing" ]
        then
            filspacing=$vary2_val
        elif [ "$vary2_param" == "randFilExtend" ]
        then
            randFilExtend=$vary2_val
        elif [ "$vary2_param" == "randFilExtend" ]
        then
            randFilRetract=$vary2_val
        fi
        for vary3_val in $vary3_vals
        do
            if [ "$vary3_param" == "epsilon" ]
            then
                epsilon=$vary3_val
            elif [ "$vary3_param" == "vconcst" ]
            then
                vconcst=$vary3_val
            elif [ "$vary3_param" == "gradient" ]
            then
                gradient=$vary3_val
            elif [ "$vary3_param" == "filconstnorm" ]
            then
                filconstnorm=$vary3_val
            elif [ "$vary3_param" == "filtipmax" ]
            then
                filtipmax=$vary3_val
            elif [ "$vary3_param" == "tokenstrength" ]
            then
                tokenstrength=$vary3_val
            elif [ "$vary3_param" == "filspacing" ]
            then
                filspacing=$vary3_val
            elif [ "$vary3_param" == "randFilExtend" ]
            then
                randFilExtend=$vary3_val
            elif [ "$vary3_param" == "randFilExtend" ]
            then
                randFilRetract=$vary3_val
            fi
            if [ "$analysis" == "pybind" ]
            then
                echo "calling slurm script for pybind, passing in flags: --analysis $analysis --epsilon $epsilon --vconcst $vconcst --gradient $gradient --filconstnorm $filconstnorm --filtipmax $filtipmax --tokenstrength $tokenstrength --filspacing $filspacing --randFilExtend $randFilExtend --randFilRetract $randFilRetract --maxtime $maxtime"
                ssh login.camp.thecrick.org  "cd $camp_home/$camp_subfolder_name; echo \"running slurm script... \"; sbatch --array 1-$numberOfRuns slurmScript.sh --analysis $analysis --epsilon $epsilon --vconcst $vconcst --gradient $gradient --filconstnorm $filconstnorm --filtipmax $filtipmax --tokenstrength $tokenstrength --filspacing $filspacing --randFilExtend $randFilExtend --randFilRetract $randFilRetract --maxtime $maxtime; exit;"
            else
                echo "calling slurm script for $analysis, passing in flags: --analysis $analysis --epsilon $epsilon --vconcst $vconcst --gradient $gradient --filconstnorm $filconstnorm --filtipmax $filtipmax --tokenstrength $tokenstrength --filspacing $filspacing --randFilExtend $randFilExtend --randFilRetract $randFilRetract"
                ssh login.camp.thecrick.org  "cd $camp_home/$camp_subfolder_name; echo \"running slurm script... \"; sbatch --array 1-$numberOfRuns slurmScript.sh --analysis $analysis --epsilon $epsilon --vconcst $vconcst --gradient $gradient --filconstnorm $filconstnorm --filtipmax $filtipmax --tokenstrength $tokenstrength --filspacing $filspacing --randFilExtend $randFilExtend --randFilRetract $randFilRetract --maxtime $maxtime; exit;"
            fi
        done
    done
done
echo "All camp jobs submitted. Wait for runs to finish (check with sacct or squeue -u <username>) then copy output files from $camp_home/$camp_subfolder_name"