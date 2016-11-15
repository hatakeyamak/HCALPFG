DIR = $(shell pwd)

CXX           = g++
CCFLAGS       = $(shell root-config --cflags) -I$(DIR) -std=c++0x
CXXFLAGS      = $(shell root-config --cflags) -I$(DIR)  
LDFLAGS       = 
LD            = g++

LIBS          = $(shell root-config --glibs)   


OBJS1         = analysis.o Events.o 
PROGRAM1      = run


HEADERS       = $(DIR)/Events.h

 
%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CCFLAGS) -c $<

%.o : %.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

all : $(PROGRAM1)
 
clean : *.o $(PROGRAM1) 
	$(RM) *.o $(PROGRAM1) 

.PHONY : all 

$(PROGRAM1): $(OBJS1)
	@echo "Linking $(PROGRAM1) ..."
	@$(LD) $(OBJS1) $(LIBS) -o $(PROGRAM1)
	@echo "done"


