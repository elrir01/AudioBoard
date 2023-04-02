#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=Drivers/Audio/BM64.c Drivers/Display/ILI9488_driver.c Drivers/Display/XPT2046_driver.c Drivers/Display/font.c Drivers/Sensor/bme280.c configuration_bits.c exceptions.c interrupts.c main.c system.c user.c circularBuff.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/Drivers/Audio/BM64.o ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o ${OBJECTDIR}/Drivers/Display/font.o ${OBJECTDIR}/Drivers/Sensor/bme280.o ${OBJECTDIR}/configuration_bits.o ${OBJECTDIR}/exceptions.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/user.o ${OBJECTDIR}/circularBuff.o
POSSIBLE_DEPFILES=${OBJECTDIR}/Drivers/Audio/BM64.o.d ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o.d ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o.d ${OBJECTDIR}/Drivers/Display/font.o.d ${OBJECTDIR}/Drivers/Sensor/bme280.o.d ${OBJECTDIR}/configuration_bits.o.d ${OBJECTDIR}/exceptions.o.d ${OBJECTDIR}/interrupts.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system.o.d ${OBJECTDIR}/user.o.d ${OBJECTDIR}/circularBuff.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/Drivers/Audio/BM64.o ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o ${OBJECTDIR}/Drivers/Display/font.o ${OBJECTDIR}/Drivers/Sensor/bme280.o ${OBJECTDIR}/configuration_bits.o ${OBJECTDIR}/exceptions.o ${OBJECTDIR}/interrupts.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system.o ${OBJECTDIR}/user.o ${OBJECTDIR}/circularBuff.o

# Source Files
SOURCEFILES=Drivers/Audio/BM64.c Drivers/Display/ILI9488_driver.c Drivers/Display/XPT2046_driver.c Drivers/Display/font.c Drivers/Sensor/bme280.c configuration_bits.c exceptions.c interrupts.c main.c system.c user.c circularBuff.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/Drivers/Audio/BM64.o: Drivers/Audio/BM64.c  .generated_files/flags/default/20a100241e042aae73ab4bcbab92b7de510cdbce .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Audio" 
	@${RM} ${OBJECTDIR}/Drivers/Audio/BM64.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Audio/BM64.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Audio/BM64.o.d" -o ${OBJECTDIR}/Drivers/Audio/BM64.o Drivers/Audio/BM64.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/ILI9488_driver.o: Drivers/Display/ILI9488_driver.c  .generated_files/flags/default/a209c45659a03e8711e540cd98684945d14d7298 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/ILI9488_driver.o.d" -o ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o Drivers/Display/ILI9488_driver.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/XPT2046_driver.o: Drivers/Display/XPT2046_driver.c  .generated_files/flags/default/4e2f9f9406811f329063a6fcd307f69c3bcb71f9 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/XPT2046_driver.o.d" -o ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o Drivers/Display/XPT2046_driver.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/font.o: Drivers/Display/font.c  .generated_files/flags/default/35fe5c9c13b7ce38067c9ccd5fb3dbfce1b0287f .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/font.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/font.o.d" -o ${OBJECTDIR}/Drivers/Display/font.o Drivers/Display/font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Sensor/bme280.o: Drivers/Sensor/bme280.c  .generated_files/flags/default/f056dd46a4c81d39d905d17a0d196700c99b416f .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Sensor" 
	@${RM} ${OBJECTDIR}/Drivers/Sensor/bme280.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Sensor/bme280.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Sensor/bme280.o.d" -o ${OBJECTDIR}/Drivers/Sensor/bme280.o Drivers/Sensor/bme280.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/configuration_bits.o: configuration_bits.c  .generated_files/flags/default/de62c3905446cdd81d34347f20f858edd7cb7acd .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/configuration_bits.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/configuration_bits.o.d" -o ${OBJECTDIR}/configuration_bits.o configuration_bits.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/exceptions.o: exceptions.c  .generated_files/flags/default/f29c5b5a7d7cc84d8be1cd4daf032870ebb45983 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/exceptions.o.d 
	@${RM} ${OBJECTDIR}/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/exceptions.o.d" -o ${OBJECTDIR}/exceptions.o exceptions.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/interrupts.o: interrupts.c  .generated_files/flags/default/44713792edfb87e085da16dff04b032e3f4a4991 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/interrupts.o.d" -o ${OBJECTDIR}/interrupts.o interrupts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/92e428030291b1984629535381c20be1b4d5d4b1 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/2ee647ca0ee2dd4ae176979410ee772a260b9c60 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/user.o: user.c  .generated_files/flags/default/5df5a86a6ab3968201adc0dcd88857a4f3aa3a45 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/user.o.d 
	@${RM} ${OBJECTDIR}/user.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/user.o.d" -o ${OBJECTDIR}/user.o user.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/circularBuff.o: circularBuff.c  .generated_files/flags/default/c65eb68f365d396a4a81c705601239e491041350 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/circularBuff.o.d 
	@${RM} ${OBJECTDIR}/circularBuff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/circularBuff.o.d" -o ${OBJECTDIR}/circularBuff.o circularBuff.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
else
${OBJECTDIR}/Drivers/Audio/BM64.o: Drivers/Audio/BM64.c  .generated_files/flags/default/b58d442ddb870866f1513a10c747355ffa61ffbf .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Audio" 
	@${RM} ${OBJECTDIR}/Drivers/Audio/BM64.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Audio/BM64.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Audio/BM64.o.d" -o ${OBJECTDIR}/Drivers/Audio/BM64.o Drivers/Audio/BM64.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/ILI9488_driver.o: Drivers/Display/ILI9488_driver.c  .generated_files/flags/default/fc7588587d76a2b16ed687a635cbf3f5d6216645 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/ILI9488_driver.o.d" -o ${OBJECTDIR}/Drivers/Display/ILI9488_driver.o Drivers/Display/ILI9488_driver.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/XPT2046_driver.o: Drivers/Display/XPT2046_driver.c  .generated_files/flags/default/b6d2e2be92d672e1b51d0b2b8bf3dc982c5f1c04 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/XPT2046_driver.o.d" -o ${OBJECTDIR}/Drivers/Display/XPT2046_driver.o Drivers/Display/XPT2046_driver.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Display/font.o: Drivers/Display/font.c  .generated_files/flags/default/c65f14db0f2c3fe09a96b5694561234708be131e .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Display" 
	@${RM} ${OBJECTDIR}/Drivers/Display/font.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Display/font.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Display/font.o.d" -o ${OBJECTDIR}/Drivers/Display/font.o Drivers/Display/font.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/Drivers/Sensor/bme280.o: Drivers/Sensor/bme280.c  .generated_files/flags/default/3c712a9fe415aa18853f735bda425948b91dc618 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}/Drivers/Sensor" 
	@${RM} ${OBJECTDIR}/Drivers/Sensor/bme280.o.d 
	@${RM} ${OBJECTDIR}/Drivers/Sensor/bme280.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/Drivers/Sensor/bme280.o.d" -o ${OBJECTDIR}/Drivers/Sensor/bme280.o Drivers/Sensor/bme280.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/configuration_bits.o: configuration_bits.c  .generated_files/flags/default/d3ceffcd6e75fac672d912f0202f3eee053d9686 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/configuration_bits.o.d 
	@${RM} ${OBJECTDIR}/configuration_bits.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/configuration_bits.o.d" -o ${OBJECTDIR}/configuration_bits.o configuration_bits.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/exceptions.o: exceptions.c  .generated_files/flags/default/c1fa25ee3957b65f1830f84a846fcbe808c09089 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/exceptions.o.d 
	@${RM} ${OBJECTDIR}/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/exceptions.o.d" -o ${OBJECTDIR}/exceptions.o exceptions.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/interrupts.o: interrupts.c  .generated_files/flags/default/5985f228bc899e8df37e8ab90c570280040248ab .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/interrupts.o.d 
	@${RM} ${OBJECTDIR}/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/interrupts.o.d" -o ${OBJECTDIR}/interrupts.o interrupts.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/4ac8eba5c475d8cfb370970c41125e082a088465 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/system.o: system.c  .generated_files/flags/default/ff3a420074fd0d9c41494afc67e6559ab2a744f0 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.o.d 
	@${RM} ${OBJECTDIR}/system.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/system.o.d" -o ${OBJECTDIR}/system.o system.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/user.o: user.c  .generated_files/flags/default/5427dc2dcdd3f5927d5bc3768e2ba12d4371d75b .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/user.o.d 
	@${RM} ${OBJECTDIR}/user.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/user.o.d" -o ${OBJECTDIR}/user.o user.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
${OBJECTDIR}/circularBuff.o: circularBuff.c  .generated_files/flags/default/a86700a39215c6984d395682f1a22fbab5117074 .generated_files/flags/default/87483c345429186c5999dcd95309ab66555f8ca8
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/circularBuff.o.d 
	@${RM} ${OBJECTDIR}/circularBuff.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/circularBuff.o.d" -o ${OBJECTDIR}/circularBuff.o circularBuff.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mreserve=prog@0x1D010F00:0x1D011F00 -mdfp="${DFP_DIR}"  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  -mreserve=prog@0x1D010F00:0x1D011F00 $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o ${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  -mreserve=prog@0x1D010F00:0x1D011F00 $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/AudioBoard3Dprint.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
