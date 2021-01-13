#   ============================================================================
#   Youhua Xu Dec-13-2017
#   ============================================================================
#   Make a build dir for compilation
MDIR := $(shell pwd)
WRKDIR = $(MDIR)/build
BINDIR = $(MDIR)/bin
# DEBUG  = $(MDIR)/debug

#	make build & binary dirs
.base:
	if ! [ -e $(WRKDIR) ]; then mkdir $(WRKDIR) ; fi;
	touch $(WRKDIR)/.base;
	if ! [ -e $(BINDIR) ]; then mkdir $(BINDIR) ; fi;
	touch $(BINDIR)/.base;
	# if ! [ -e $(DEBUG) ]; then mkdir $(DEBUG) ; fi;
	# touch $(DEBUG)/.base;
#   ============================================================================
#   Set the source file path
vpath %.c   main:src
vpath %.h   include
vpath %.o build
vpath .base build

INCLUDES = -I $(MDIR)/include

CC			= arm-linux-gnueabihf-gcc
CCFLAG  	= -Wall
OPTFLAG		= #-O2 #-ffast-math #( not recognized by intel compiler )
LIBS		= #-lgsl -lgslcblas -lm
LDFLAG      = 

#   http://www.tuicool.com/articles/NBfeYj
ifeq ($(shell uname -s),Linux)
	LDFLAG	+= -Wl,--no-as-needed
endif

ifeq ($(shell uname -s),Darwin)
	LDFLAG	+= -framework Accelerate #(-framework Accelerate is for Mac OSX)
endif

#%.o: %.cpp .base
#	cd $(WRKDIR);$(CPP) $(OMPFLAG) $(OPTFLAG) $(CCFLAG) $(INCLUDES) -c ../$< -o $*.o

# %.o: %.cc .base
# 	cd $(WRKDIR);$(CC) $(OMPFLAG) $(OPTFLAG) $(CCFLAG) $(INCLUDES) -c ../$< -o $*.o

%.o: %.c .base
	cd $(WRKDIR);$(CC) $(OMPFLAG) $(OPTFLAG) $(CCFLAG) $(INCLUDES) -c ../$< -o $*.o

MAIN 	= ad5628_test.o
DEPS	= xil_printf.o xil_assert.o xspips.o xspips_sinit.o xspips_options.o \
			outbyte.o xspips_selftest.o xspips_g.o xuartps_hw.o

all: ad5628_test

ad5628_test: ${MAIN} ${DEPS}
	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $(BINDIR)/$@

#ad5628_test: ${MAIN} ${DEPS}
#	${CC} ${OPTFLAG} ${LDFLAG} $(addprefix build/,$(notdir $^)) ${LIBS} -o $@

.PHONY:clean tidy run
clean: .base
	rm -rf $(WRKDIR);
tidy:
	make clean; rm -rf $(BINDIR) $(WRKDIR)
