import pytest, subprocess, shutil, os, sys, filecmp
import numpy as np
import constant

run_number = 1
epsilon = 0.9
vconcst = 0.04
gradient = 2
filconstnorm = 2
filtipmax = 15
tokenstrength = 1
filspacing = 2
randFilExtend = -1
randFilRetract = -1


#below is the full range of values that should be used
#these take a long time however so shorter version in place as default
# seeds = { 10, 1000, 100000 }
# filvary_vals = { 1, 2, 3 }
# vconcst_vals = { 0.02, 0.04 }

seeds = { 1000 }
filvary_vals = { 2 }
vconcst_vals = { 0.04 }

def test_time_to_pattern():
	compileRunAndCheckOutputs(constant.ANALYSIS_TIME_TO_PATTERN)

def test_bistability():
	compileRunAndCheckOutputs(constant.ANALYSIS_BISTABILITY)

def test_pybind():
	compileRunAndCheckOutputs(constant.ANALYSIS_PYBIND)

def compileRunAndCheckOutputs(analysis_type):
	compile(analysis_type)
	for seed in seeds:
		for filvary_val in filvary_vals:
			filconstnorm = filvary_val
			for vconcst_val in vconcst_vals:
				vconcst = vconcst_val
				run(seed, analysis_type)
				checkOutputs(analysis_type, seed)
				if analysis_type != constant.ANALYSIS_PYBIND:
					check_time(analysis_type, seed)

def compile(analysis_type):
	ec_pack = 2
	max_time = 5000

	if analysis_type == constant.ANALYSIS_BISTABILITY:
		ec_pack = 3
		max_time = 10000000
	elif analysis_type == constant.ANALYSIS_TIME_TO_PATTERN:
		ec_pack = 10
		max_time = 100000
		
	print(str(subprocess.call("pip show pybind11", shell=True)))

	compile_command = "make clean; make CDBG=-w " + analysis_type + "=true MAXtime=" + str(max_time) + " ECpack=" + str(ec_pack) + " GRAPHICS=false GITHUB=" + constant.github
	completed_compilation = subprocess.call(
		[compile_command], shell = True
	)
	assert completed_compilation == constant.SUCCESS_RETURN_CODE

def run(seed, analysis_type):

	run_command = "./springAgent 1 " + str(epsilon) + " " + str(vconcst) + " " + str(gradient) + " " + str(filconstnorm) + " " + str(filtipmax) + " " + str(tokenstrength) + " " + str(filspacing) + " " + str(randFilExtend) + " " + str(randFilRetract) + " " + str(seed)
	if analysis_type == constant.ANALYSIS_PYBIND:
		run_command = "python APentropyintensity.py 1 500 " + str(epsilon) + " " + str(vconcst) + " " + str(gradient) + " " + str(filconstnorm) + " " + str(filtipmax) + " " + str(tokenstrength) + " " + str(filspacing) + " " + str(randFilExtend) + " " + str(randFilRetract) + " " + str(seed)
	completed_run = subprocess.call(
		[run_command], shell = True
	)
	assert completed_run == constant.SUCCESS_RETURN_CODE

def checkOutputs(analysis_type, seed):
	if analysis_type == constant.ANALYSIS_PYBIND:
		filenames =  ["run 1 entropy T 500 epsilon "+str(epsilon)+" vconcst "+str(vconcst)+" gradienttype "+str(gradient)+" F "+str(filconstnorm)+" filtipmax "+str(filtipmax)+" tokenstrength "+str(tokenstrength)+" filspacing "+str(filspacing)+" randfilextend "+str(randFilExtend)+" randfilretract "+str(randFilRetract)+" seed "+str(seed)+".npy",
					  "run 1 intensity T 500 epsilon "+str(epsilon)+" vconcst "+str(vconcst)+" gradienttype "+str(gradient)+" F "+str(filconstnorm)+" filtipmax "+str(filtipmax)+" tokenstrength "+str(tokenstrength)+" filspacing "+str(filspacing)+" randfilextend "+str(randFilExtend)+" randfilretract "+str(randFilRetract)+" seed "+str(seed)+".npy",
					  "run 1 time T 500 epsilon "+str(epsilon)+" vconcst "+str(vconcst)+" gradienttype "+str(gradient)+" F "+str(filconstnorm)+" filtipmax "+str(filtipmax)+" tokenstrength "+str(tokenstrength)+" filspacing "+str(filspacing)+" randfilextend "+str(randFilExtend)+" randfilretract "+str(randFilRetract)+" seed "+str(seed)+".npy" ]
		for filename in filenames:
			assert filecmp.cmp(filename, "tests/expected_outputs/" + filename, shallow=False) == True
	else:
		analysis_name = ""
		if analysis_type == constant.ANALYSIS_BISTABILITY:
			analysis_name = "analysis_hysteresis"
		elif analysis_type == constant.ANALYSIS_TIME_TO_PATTERN:
			analysis_name = "time_to_pattern"

		output_filename = analysis_name + "_filvary_"+ str(filconstnorm) + "_epsilon_" + str(epsilon) + "_VconcST" + str(vconcst) + "_GRADIENT" + str(gradient) + "_FILTIPMAX" + str(filtipmax) + "_tokenStrength" + str(tokenstrength) + "_FILSPACING" + str(filspacing) + "_randFilExtend" + str(randFilExtend) + "_randFilRetract" + str(randFilRetract) + "_seed" + str(seed) + "_" + "run1_.txt"
		expected_output_filename = "tests/expected_outputs/expected_"+ analysis_name + "_filvary_"+ str(filconstnorm) + "_epsilon_" + str(epsilon) + "_VconcST" + str(vconcst) + "_GRADIENT" + str(gradient) + "_FILTIPMAX" + str(filtipmax) + "_tokenStrength" + str(tokenstrength) + "_FILSPACING" + str(filspacing) + "_randFilExtend" + str(randFilExtend) + "_randFilRetract" + str(randFilRetract) + "_seed" + str(seed) + "_" + "run1_.txt"
		assert filecmp.cmp(output_filename, expected_output_filename, shallow=False) == True


def check_time(analysis_type, seed):
	analysis_name = ""
	if analysis_type == constant.ANALYSIS_BISTABILITY:
		analysis_name = "hysteresis"
		desired_time = 1600
		tolerance = 0.5
	elif analysis_type == constant.ANALYSIS_TIME_TO_PATTERN:
		analysis_name = "time_to_pattern"
		desired_time = 80
		tolerance = 0.5

	statistics_file = "statistics_" + analysis_name \
					  + "_filvary_" + str(filconstnorm) \
					  + "_epsilon_" + str(epsilon) \
					  + "_VconcST" + str(vconcst) \
					  + "_GRADIENT" + str(gradient) \
					  + "_FILTIPMAX" + str(filtipmax) \
					  + "_tokenStrength" + str(tokenstrength) \
					  + "_FILSPACING" + str(filspacing) \
					  + "_randFilExtend" + str(randFilExtend) \
					  + "_randFilRetract" + str(randFilRetract) \
					  + "_seed" + str(seed) \
					  + "_run1_.csv"

	elapsed_time = get_elapsed_time(statistics_file)
	np.testing.assert_allclose(elapsed_time, desired_time, rtol=tolerance)


def get_elapsed_time(statistics_file):
	elapsed_time = 0
	with open(statistics_file) as file:
		lines = file.readlines()
		for line in lines:
			if line.startswith("Elapsed time"):
				line = line.split(",")
				elapsed_time = int(line[1])
	return elapsed_time
