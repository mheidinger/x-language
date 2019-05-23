###############################################################################
# Project:  XLang Compiler
# Descr:    Compiler for the X language
# Author:   Max Heidinger <mail@max-heidinger.de>, 
#           Pascal Riesinger <mail@pascal-riesinger.de>
###############################################################################

# The executable to produce:
MAIN		= xlang

# Source files
SRC_C		= main.c
SRC_H		=
SRC		= xlang.scan xlang$(SOLUTION).pars

# Generated files
GEN_C		= xlang_scan.c xlang_scanSource.c xlang_pars.c
GEN_H		= xlang_scan.h xlang_scanSource.h xlang_pars.h
GEN		= xlang.rex xlang_scan.rpp xlang_pars.lrk

###############################################################################

include ../config.mk

###############################################################################

# Create your parser
all: $(MAIN)

# Run the test suite
test:
	./$(MAIN) test1
	./$(MAIN) test2
	- ./$(MAIN) test3
	- ./$(MAIN) test4
	- ./$(MAIN) test4

tar: clean
	cd .. && tar -czvf Heidinger-Riesinger.tar.gz Heidinger-Riesinger

###############################################################################

# Create the Parser
xlang_pars.lrk xlang_scan.rpp: xlang$(SOLUTION).pars
	lpp -cxzj  xlang$(SOLUTION).pars

LARK_OPTS = -cdi -Dw -5  # -5 ==> create the graphic LR-browser
LARK_OPTS = -cdi -Dw     #    ==> see ../config.mk
xlang_pars.c xlang_pars.h: xlang_pars.lrk
	lark $(LARK_OPTS) xlang_pars.lrk

# Create the Scanner
xlang.rex:	xlang.scan xlang_scan.rpp
	rpp < xlang.scan xlang_scan.rpp > xlang.rex

xlang_scan.c xlang_scan.h xlang_scanSource.c xlang_scanSource.h: xlang.rex
	rex -cdis -y xlang.rex
	rm -f xlang_scanDrv.c

###############################################################################

include ../common.mk

###############################################################################
## Internal stuff !!!!
###############################################################################
xlang.scan:
	cp -f ../xlang/xlang.scan .

###############################################################################
