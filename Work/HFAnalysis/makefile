DIR = $(shell pwd)

CXX           = g++
CCFLAGS       = $(shell root-config --cflags) -I$(DIR) -I$(DIR)/.. -std=c++0x
CXXFLAGS      = $(shell root-config --cflags) -I$(DIR) -I$(DIR)/..
LDFLAGS       = 
LD            = g++

LIBS          = $(shell root-config --glibs)   


OBJS1         = HFanalysis.o ../Events.o 
PROGRAM1      = run


OBJS2         = CalibAnalysis.o ../Events.o
PROGRAM2      = runCal

HEADERS       = $(DIR)/../Events.h

 
%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CCFLAGS) -c $<

%.o : %.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

all : $(PROGRAM1) $(PROGRAM2)
 
clean : *.o $(PROGRAM1) 
	$(RM) *.o $(PROGRAM1) 

.PHONY : all 

$(PROGRAM1): $(OBJS1)
	@echo "Linking $(PROGRAM1) ..."
	@$(LD) $(OBJS1) $(LIBS) -o $(PROGRAM1)
	@echo "done"

$(PROGRAM2): $(OBJS2)
	@echo "Linking $(PROGRAM2) ..."
	@$(LD) $(OBJS2) $(LIBS) -o $(PROGRAM2)
	@echo "done"

