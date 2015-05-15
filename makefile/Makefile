#specify comile environment
OS_NAME := $(shell uname -s)
CWD := $(shell pwd)

#set command
AR := ar            
SED:= sed          
AWK:= awk
MV := mv
RM := rm -f
ECHO := echo

#prepare C compile tools
CC := gcc

#prepare C comile options
CFLAGS := -c -O2 -g -w -Wall

#prepare C pre-process options
CPP :=
CPPFLAGS :=

#prepare C++ environment
CXX := g++
CXXFALGS := -c -g -w Wall
CXXPP := 
CXXPPFLAGS :=

#set header files path
INCLUDES := -I$(CWD)/include -I./

#set lib search path
LIBDIRS := -L$(CWD)/lib

#set special value with different OS 
ifeq ($(OS_NAME), 'Linux')
CLDFLAGS :=
endif

#specify source dir
SRC_DIR := . ./src ./src/apis ./src/utils
VPATH := $(SRC_DIR)

#define install bin dir
BIN := ./bin

#secify source extension
SFIX := .c .cpp .C .cc .c++ .CPP .cp .cxx

#get all source file
SOURCES := $(foreach sdir, ${SRC_DIR}, \
	$(wildcard $(addprefix ${sdir}/*, ${SFIX})))

#get object files
OBJS := $(addsuffix .o, $(basename $(notdir ${SOURCES})))

#get depend files
DEPENDS := $(addsuffix .d, $(basename ${SOURCES}))

#ultimate target name 
PROGRAM := example

#define target 
.PHONY : all check clean install rebuild

all : ${PROGRAM}

#get C++ source files
LDCXX := $(strip $(filter-out %.c, ${SOURCES}))

#reset compiler
ifdef LDCXX
	CC := ${CXX}
	CFLAGS := ${CXXFALGS}
	CPP := ${CXXPP}
	CPPFLAGS := ${CXXPPFLAGS}
endif

#product ultimate program
${PROGRAM} : ${DEPENDS} ${OBJS}
ifeq ($(strip $(filter-out %.c, ${SOURCES})),)
	${CC} ${LIBDIRS} ${OBJS} -o $@
else
	${CXX} ${LIBDIRS} ${OJBS} -o $@
endif

#or include dependence files
#sinclude ${DEPENDS}

#rules for producing the object files
%.o : %.c
	${CC} ${CFLAGS} ${INCLUDES} $<

%.o : %.C
	${CC} ${CFLAGS} ${INCLUDES} $<

%.o : %.cc
	${CXX} ${CFLAGS} ${INCLUDES} $<

%.o : %.cpp
	${CXX} ${CFLAGS} ${INCLUDES} $<

#create dependence files
%.d : %.c
	@${CC} -M -MD ${INCLUDES} $<

%.d : %.C
	@${CC} -MM -MD ${INCLUDES} $<

%.d : %.cc
	@${CXX} -MM -MD ${INCLUDES} $<

%.d : %.cpp
	@${CXX} -MM -MD ${INCLUDES} $<

#check valuables
check:
	@${ECHO}  MAKEFILES : ${MAKEFILES}
	@${ECHO}  MAKECMDGOALS : ${MAKECMDGOALS}
	@${ECHO}  SHELL  : ${SHELL}
	@${ECHO}  OS_NAME  : ${OS_NAME}
	@${ECHO}  SRC_DIR : ${SRC_DIR}
	@${ECHO}  SFIX : ${SFIX}
	@${ECHO}  VPATH : ${VPATH}
	@${ECHO}  BIN : ${BIN}
	@${ECHO}  SOURCES : ${SOURCES}
	@${ECHO}  OBJS : ${OBJS}
	@${ECHO}  DEPENDS : ${DEPENDS}
	@${ECHO}  PROGRAM : ${PROGRAM}
	@${ECHO}  CC :  ${CC}
	@${ECHO}  CFLAGS : ${CFLAGS}
	@${ECHO}  CPP : ${CPP}
	@${ECHO}  CPPFLAGS : ${CPPFLAGS}
	@${ECHO}  CXX :  ${CXX}
	@${ECHO}  CXXFLAGS : ${CXXFLAGS}
	@${ECHO}  CXXPP : ${CXXPP}
	@${ECHO}  CXXPPFLAGS : ${CXXPPFLAGS}
	@${ECHO}  INCLUDES : ${INCLUDES}
	@${ECHO}  LIBDIRS : ${LIBDIRS}
	@${ECHO}  CLDFLAGS : ${CLDFLAGS}
	@${ECHO}  CXXLDFLAGS : ${CXXLDFLAGS}
	@${ECHO}  DCPPFLAGS : ${DCPPFLAGS}

clean :
	-${RM} ${BIN}/${PROGRAM}
	-${RM} ${BIN}/*.o
	-${RM} ${BIN}/*.d
	-${RM} *.o
	-${RM} *.d

install :
	-${MV} ${PROGRAM} ${BIN}
	-${MV}  *.o ${BIN}
	-${MV}  *.d ${BIN}

rebuild : clean all
