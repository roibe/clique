#########################################################
# User Section - Choose arguments to the program
#########################################################
THRESHOLD_VALUE= 0.9
LOWER_BOUND= 5
UPPER_BOUND= 8
INPUT_FILE= $(INPUT_FILE_1)
#yes or no
CALCULATE_|MAX_CLIQUE|=no
RUN_GTEST=yes
MEMORY_LEAK_CHECK=no
PROFILER_OUTPUT=yes

#########################################################
# Input Files
#########################################################
INPUT_FILE_1= test1.csv
INPUT_FILE_2= graph_10V.dot
INPUT_FILE_3= graph_2000V.dot

#########################################################
# Check passed arguments
#########################################################
ifeq ($(PROFILER_OUTPUT),yes)
GPROF= gprof $(EXE) gmon.out > profiler.out
GPROFFLAG= -pg
else
GPROF=
GPROFFLAG=
endif

ifeq ($(MEMORY_LEAK_CHECK),yes) 
VALGRIND= valgrind --leak-check=full -v
GPROF=
GPROFFLAG=
endif

ifeq ($(RUN_GTEST),yes) 
RUNGTEST= $(VALGRIND) ./$(GTEST) gtest_test1.csv 0.9 5 8 $(CALCULATE_MAX_CLIQUE)
endif

ifeq ($(CALCULATE_|MAX_CLIQUE|),yes)
CALCULATE_MAX_CLIQUE= yes
CREATE_FILE=
DELETE_TMP_FILE=
else
CALCULATE_MAX_CLIQUE= 
CREATE_FILE= cp output.csv $(INPUT_FILE)_$(THRESHOLD_VALUE)_$(LOWER_BOUND)_$(UPPER_BOUND).csv
#MOVE_FILE= mv $(INPUT_FILE)_$(THRESHOLD_VALUE)_$(LOWER_BOUND)_$(UPPER_BOUND).csv output_files
DELETE_TMP_FILE= rm output.csv
endif

#########################################################
# compiler stuff 
#########################################################
CC= gcc
CFLAGS= -Wall -Wvla -Werror -pthread
COPTFLAGS= -O -DNDEBUG

CXX= g++
CXXFLAGS= $(CFLAGS) -g 

CXXTESTLINKFLAGS= -lgtest -lgtest_main

#########################################################
# Executables names
#########################################################
EXE= cliques_demo
GTEST= cliques_test

##########################################################
# Sources files
##########################################################
SRCSEXENOMAIN= cliques.c
SRCSEXEMAIN= $(EXE).c
SRCSEXE= $(SRCSEXENOMAIN) $(SRCSEXEMAIN)
SRCSTESTMAIN= $(GTEST).cpp

##########################################################
# Headers files
##########################################################
HEADERFILES= cliques.h

##########################################################
# Actions
##########################################################
all: $(EXE) $(GTEST)
	time $(VALGRIND) ./$(EXE) $(INPUT_FILE) $(THRESHOLD_VALUE) $(LOWER_BOUND) $(UPPER_BOUND) $(CALCULATE_MAX_CLIQUE)
	$(GPROF)
	$(CREATE_FILE)
	$(MOVE_FILE)
	$(RUNGTEST)
	$(DELETE_TMP_FILE)

$(EXE): $(subst .c,.o,$(SRCSEXE))
	$(CC) $(CFLAGS) $(COPTFLAGS) $(GPROFFLAG) $^ -o $@

$(GTEST): $(subst .c,.o,$(SRCSEXENOMAIN)) $(subst .cpp,.o,$(SRCSTESTMAIN))
	  $(CXX) $(CXXFLAGS) $^ -o $@ $(CXXTESTLINKFLAGS)

clean:
	rm *.o $(EXE)  $(GTEST) -f

depend:
	$(SRCSEXE) $(SRCSTESTMAIN)
	makedepend -Y -- $(CXXFLAGS) -- $^

zipfile:
	zip $(ID).zip $(SRCSEXENOMAIN) $(SRCSEXEMAIN) $(SRCSTESTMAIN) $(HEADERFILES) $(INPUT_FILE)

checkzipfile:
	rm checkSubmission -fr ; \
	mkdir checkSubmission ; \
	cd checkSubmission ; \
	cp ../$(ID).zip . ;\
	unzip $(ID).zip ;\
	cp ../cliques.h . ;\
	cp ../cliques.c . ;\
	cp ../cliques_demo.c . ;\
	cp ../cliques_test.cpp . ;\
	cp ../Makefile . ;\
	make ;\
	cd ..


##########################################################
# installonce - gtest, makedepend, valgrind (that should be done once)
##########################################################
installonce: gtestinstall makedependinstall valgrindinstall

gtestinstall: 
	sudo apt-get install libgtest-dev
	sudo apt-get install cmake
	cd /usr/src/gtest; \
	sudo cmake CMakeLists.txt; \
	sudo make; \
	sudo cp *.a /usr/lib
	sudo chmod u+r /usr/lib/libgtest.a
	sudo chmod u+r /usr/lib/libgtest_main.a

makedependinstall:
	sudo apt-get install xutils-dev

valgrindinstall:
	sudo apt-get install valgrind

##########################################################

.PHONY: all clean depend installonce gtestinstall makedependinstall valgrindinstall zipfile

# DO NOT DELETE THIS LINE -- make depend depends on it.

cliques.o: cliques.h
cliques_demo.o: cliques.h
cliques_test.o: cliques.h
