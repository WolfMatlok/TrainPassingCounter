#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/TimeServer.o \
	${OBJECTDIR}/TimeServerClock.o \
	${OBJECTDIR}/cCamera.o \
	${OBJECTDIR}/cCommonTools.o \
	${OBJECTDIR}/cDetector.o \
	${OBJECTDIR}/cGyroServer.o \
	${OBJECTDIR}/cUnitTest.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`pkg-config opencv --cflags --libs` -std=c++11 -L /opt/vc/lib -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_imgproc -lopencv_core -lboost_thread -lboost_system -lboost_chrono -lwiringPi 
CXXFLAGS=`pkg-config opencv --cflags --libs` -std=c++11 -L /opt/vc/lib -lraspicam -lraspicam_cv -lmmal -lmmal_core -lmmal_util -lopencv_imgproc -lopencv_core -lboost_thread -lboost_system -lboost_chrono -lwiringPi 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trainpassingcounter

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trainpassingcounter: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trainpassingcounter ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/TimeServer.o: TimeServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TimeServer.o TimeServer.cpp

${OBJECTDIR}/TimeServerClock.o: TimeServerClock.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TimeServerClock.o TimeServerClock.cpp

${OBJECTDIR}/cCamera.o: cCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cCamera.o cCamera.cpp

${OBJECTDIR}/cCommonTools.o: cCommonTools.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cCommonTools.o cCommonTools.cpp

${OBJECTDIR}/cDetector.o: cDetector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cDetector.o cDetector.cpp

${OBJECTDIR}/cGyroServer.o: cGyroServer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cGyroServer.o cGyroServer.cpp

${OBJECTDIR}/cUnitTest.o: cUnitTest.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cUnitTest.o cUnitTest.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -I/usr/include/eigen3 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/trainpassingcounter

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
