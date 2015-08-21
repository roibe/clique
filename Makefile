#########################################################
## Authors
#########################################################
#Roi Benita
#Ariel Hazan
#Daniel Blank
#Netanel Ben-Haim

#########################################################
# Arguments to the program
#########################################################
INDICATOR= 0.9
LOWER_BOUND= 5
UPPER_BOUND= 8
#GET_MAX= Calculate_Max_Clique
INPUT_FILE= test1.csv
## for valgrind leak check 
#VALGRIND = valgrind --leak-check=full -v

#########################################################
# compiler stuff 
#########################################################
CC= gcc
CFLAGS= -Wall -Wvla -Werror 
COPTFLAGS= -O -DNDEBUG

CXX= g++
CXXFLAGS= $(CFLAGS) 

CEXELINKFLAGS=
CXXTESTLINKFLAGS= -lgtest -lgtest_main -pthread

#########################################################
# Executables names
#########################################################
EXE1= cliques_demo
#TEST= clique_test

##########################################################
# Sources files
##########################################################
SRCSEXENOMAIN= cliques.c
SRCSEXE1MAIN= $(EXE1).c
SRCSEXE1= $(SRCSEXENOMAIN) $(SRCSEXE1MAIN)

# There is no SRCTESTMAIN as we are linking with gtest_main that has main
#SRCSTESTNOMAIN= $(TEST).cpp

##########################################################
# Headers files
##########################################################
HEADERFILES= cliques.h

##########################################################
# Actions
#########################################################
all: $(EXE1) $(TEST)
	time $(VALGRIND) ./$(EXE1) $(INPUT_FILE) $(INDICATOR) $(LOWER_BOUND) $(UPPER_BOUND) $(GET_MAX)

#	@read -p "Press [Enter] key to start memory check of test executable..." MEMCHK
#	valgrind --leak-check=full -v ./$(TEST)

$(EXE1): $(subst .c,.o,$(SRCSEXE1))
	$(CC) $(CFLAGS) $(CEXELINKFLAGS) $^ -o $@

#$(TEST): $(subst .c,.o,$(SRCSEXENOMAIN)) $(subst .cpp,.o,$(SRCSTESTNOMAIN))
#	$(CXX) $(CXXFLAGS) $^ -o $@ $(CXXLINKFLAGS) $(CXXTESTLINKFLAGS)

clean:
	rm *.o $(EXE1)  $(TEST) -f

depend: $(SRCSEXE1) $(SRCSTESTNOMAIN) 
	makedepend -Y -- $(CXXFLAGS) -- $^

zipfile:
	zip $(ID).zip $(SRCSEXENOMAIN) $(SRCSEXE1MAIN) $(SRCSTESTNOMAIN) $(HEADERFILES) $(INPUT_FILE)

checkzipfile:
	rm checkSubmission -fr ; \
	mkdir checkSubmission ; \
	cd checkSubmission ; \
	cp ../$(ID).zip . ;\
	unzip $(ID).zip ;\
	cp ../clique.h . ;\
	cp ../clique.c . ;\
	cp ../clique_demo.c . ;\
	cp ../clique_test.cpp . ;\
	cp ../Makefile . ;\
	make ;\
	cd ..

# DO NOT DELETE THIS LINE -- make depend depends on it.

cliques.o: cliques.h
cliques_demo.o: cliques.h
#cliques_test.o: cliques.h
