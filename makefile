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

CDBG +=
CPPFLAGS =
LIBS = 
PYBIND_INC = 
ifeq ($(CAMP), true)
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
                OBJS += display.o ScreenRecording.o
		LIBS := -framework OpenGL -framework GLUT -lglui
	endif
	#TODO: could have below as standard val
	CPPFLAGS := -Wno-deprecated -g -std=c++11 -DECpack=$(ECpack) -DGRAPHICS=$(GRAPHICS) -DMAXtime=$(MAXtime) -DBAHTI_ANALYSIS=$(BAHTI_ANALYSIS) -DANALYSIS_HYSTERESIS=$(ANALYSIS_HYSTERESIS) -DANALYSIS_TIME_TO_PATTERN=$(ANALYSIS_TIME_TO_PATTERN) 
endif

CPPFLAGS += $(CDBG)

#for non pybind  use:
${TARGET} : ${OBJS}
	${CC} -o ${TARGET} ${CPPFLAGS} ${OBJS} ${LIBS}

.cpp.o:
	${CC} ${CPPFLAGS} -c $*.cpp

clean:
	rm -f ${OBJS} display.o ScreenRecording.o
	
	
	
	
