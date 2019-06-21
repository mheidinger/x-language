###############################################################################
# Project:  COCKTAIL training
# Descr:    A simple pocket computer (scanner, parser, evaluator)
# Kind:     Makefile
# Author:   Prof. Dr. Juergen Vollmer <vollmer@dwbw-karlsruhe.de>
# $Id: Makefile,v 1.9 2018/12/07 10:44:52 vollmer Exp $
###############################################################################

# The executable to produce:
MAIN		= xlang-ast

# Source files
SRC_C		= main.c
SRC		= xlang.scan 	        \
		  xlang$(SOLUTION).pars	\
		  xlang$(SOLUTION).ast 	\

# Generated files
GEN_C		= xlang_scan.c xlang_scanSource.c xlang_pars.c Tree.c
GEN_H		= xlang_scan.h xlang_scanSource.h xlang_pars.h Tree.h yyTree.h
GEN		= xlang.rex xlang_scan.rpp xlang_pars.lrk      Tree.TS

###############################################################################

include ../config.mk

###############################################################################

# Create your parser
all: $(MAIN)

tar: clean
	cd .. && tar -czvf Heidinger-Riesinger.tar.gz Heidinger-Riesinger

# Run the test suite
test:
	./$(MAIN) test1
	./$(MAIN) test2
	- ./$(MAIN) test3
	- ./$(MAIN) test4
	- ./$(MAIN) test4

###############################################################################

# Create the Parser
xlang_pars.lrk xlang_scan.rpp: xlang$(SOLUTION).pars
	lpp -cxzj  xlang$(SOLUTION).pars

LARK_OPTS = -cdi -Dw -5  # -5 ==> create the graphic LR-browser
LARK_OPTS = -cdi -Dw     #    ==> see ../config.mk
xlang_pars.c xlang_pars.h: xlang_pars.lrk
	lark $(LARK_OPTS) xlang_pars.lrk

# Create the Scanner
xlang.rex: xlang.scan xlang_scan.rpp
	rpp < xlang.scan xlang_scan.rpp > xlang.rex

xlang_scan.c xlang_scan.h xlang_scanSource.c xlang_scanSource.h: xlang.rex
	rex -cdis -y xlang.rex
	rm -f xlang_scanDrv.c

# Create the Abstract Syntax Tree module
# AST options
# -a generate all except -ceh (default)
# -4 generate tree/graph description in file TREE.TS
# -c generate C    source code
# -d generate header file or definition module
# -i generate implementation part or module
# -w generate text graph writer    WriteTREE
# -R generate list reverser        ReverseTREE, ForallTREE
# -t generate top down  traversal  TraverseTREETD
# -b generate bottom up traversal  TraverseTREEBU
# -k generate graph type checker   CheckTREE
# -q generate text  graph browser  QueryTREE
# -e generate graphic browser      procedure DrawTREE
Tree.TS Tree.c Tree.h yyTree.h: xlang$(SOLUTION).ast
	ast -cdi -4wemRk xlang$(SOLUTION).ast

###############################################################################

include ../common.mk

###############################################################################
