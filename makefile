#OS
UNAME_S := $(shell uname -s)

OBJS = main.o analysis.o Grid.o CPM_module.o constructors.o memAgents.o creation.o geometry_functions.o EC.o ECmesh.o environment.o


CC = g++
TARGET = springAgent

ECpack = 2
GRAPHICS = false
MAXtime = 5000
BAHTI_ANALYSIS = false
ANALYSIS_HYSTERESIS = false
ANALYSIS_TIME_TO_PATTERN = false
CAMP = false
GITHUB = false

INC := -I/usr/include -I/usr/include/GL -I/usr/local/include 
CDBG +=
CPPFLAGS =
PYBIND_INC = 

ifeq ($(GITHUB), true)
PYBIND_INC := -fPIC -I/usr/local/include/python2.7 -I/usr/local/lib/python2.7/site-packages/pybind11/include
else ifeq ($(CAMP), true)
PYBIND_INC := -fPIC -I../include/python2.7 -I../lib/python2.7/site-packages/pybind11/include
else
PYBIND_INC := -undefined dynamic_lookup -I/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/pybind11/pybind11
endif

# -Wno-deprecated -O3 -Wall -shared -std=c++11 -fPIC -I/camp/home/vuurenk/pybind_sims/include/python2.7 -I/camp/home/vuurenk/pybind_sims/lib/python2.7/site-packages/pybind11/include 
# CCFLAGS = -g -Wno-deprecated -DGRAPHICS -std=c++11 
ifeq ($(BAHTI_ANALYSIS), true)
	CPPFLAGS := -Wno-deprecated -O3 -Wall -shared -std=c++11 -DECpack=$(ECpack) -DGRAPHICS=$(GRAPHICS) -DMAXtime=$(MAXtime) -DBAHTI_ANALYSIS=$(BAHTI_ANALYSIS) -DANALYSIS_HYSTERESIS=$(ANALYSIS_HYSTERESIS) -DANALYSIS_TIME_TO_PATTERN=$(ANALYSIS_TIME_TO_PATTERN)
	CPPFLAGS += $(PYBIND_INC)
	TARGET := springAgent.so
else
	ifeq ($(GRAPHICS), true)
		ifeq ($(UNAME_S),Linux)
			LIB	:= -L/usr/lib64 -lglui -lGL -lglut -lGLU 
		endif

		ifeq ($(UNAME_S),Darwin)
			LIB 	:= -framework OpenGL -framework GLUT -lglui
		endif
                OBJS += display.o ScreenRecording.o
	endif
	#TODO: could have below as standard val
	CPPFLAGS := -Wno-deprecated -g -std=c++11 -DECpack=$(ECpack) -DGRAPHICS=$(GRAPHICS) -DMAXtime=$(MAXtime) -DBAHTI_ANALYSIS=$(BAHTI_ANALYSIS) -DANALYSIS_HYSTERESIS=$(ANALYSIS_HYSTERESIS) -DANALYSIS_TIME_TO_PATTERN=$(ANALYSIS_TIME_TO_PATTERN) 
endif

CPPFLAGS += $(CDBG)

#for non pybind  use:
${TARGET} : ${OBJS}
	${CC} -o ${TARGET} ${CPPFLAGS} ${INC} ${OBJS} ${LIB}

.cpp.o:
	${CC} ${CPPFLAGS} ${INC} -c $*.cpp

clean:
	rm -f ${OBJS} display.o ScreenRecording.o springAgent springAgent.so
	
	
	
	
