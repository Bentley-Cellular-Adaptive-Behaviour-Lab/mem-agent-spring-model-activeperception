graphics=false
camp=false
analysis="time_to_pattern"

all_args=()
while [[ $# -gt 0 ]]
do
key="$1"
case $key in
	--analysis)
    analysis="$2"
    # break
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    all_args+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
all_args="${all_args[@]}"

for arg in $all_args
do
    if [ "$arg" == "--graphics" ]
    then
        graphics=true
    fi

    if [ "$arg" == "--camp" ]
    then
    	camp=true
    fi
done

#check values for graphics flag correct
if [ "$graphics" != "true" ] && [ "$graphics" != "false" ] && [ "$graphics" != "" ]
then
	echo "Invalid graphics option! Pass in either true or false."
	exit 1
elif [ "$graphics" == "" ]
then
	graphics="false"
fi
#check values for camp flag correct
if [ "$camp" != "true" ] && [ "$camp" != "false" ] && [ "$camp" != "" ]
then
	echo "Invalid graphics option! Pass in either true or false."
	exit 1
elif [ "$camp" == "" ]
then
	camp="false"
fi

echo "graphics = $graphics"
echo "camp = $camp"

if [ "$analysis" == "pybind" ]
then
	echo "building pybind analysis..."
	make clean; make CDBG=-w ECpack=2 BAHTI_ANALYSIS=true GRAPHICS=false CAMP=$camp
elif [ "$analysis" == "bistability" ]
then
	echo "building bistability analysis..."
	make clean; make CDBG=-w ECpack=3 MAXtime=10000000 ANALYSIS_HYSTERESIS=true GRAPHICS=$graphics
elif [ "$analysis" == "time_to_pattern" ]
then
	echo "building time_to_pattern analysis..."
	make clean; make CDBG=-w ECpack=10 MAXtime=10000 ANALYSIS_TIME_TO_PATTERN=true GRAPHICS=$graphics
else
	echo "$analysis is not a valid analysis. Enter either pybind, bistability or time_to_pattern with the -a flag."
	exit 1
fi