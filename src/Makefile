# ------------------------------------------------------------------------------
# Copyright (c) 2019 Jose Moran <jmoran071996@gmail.com>
# ------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
.SHELL      = bash
.PWD        = $(shell pwd)
.COMPILER   = gcc -g
.LINKER     = gcc -c -g
.FILES      =  \
              ls \
              grep \
              chmod \
	      tesh \
						
.EXE 	    = tesh
    
.files_o    = $(addsuffix .o, $(.FILES))
.files_c    = $(addsuffix .c, $(.FILES))
.BFOLDER    = $(BIN_DIR)


# ------------------------------------------------------------------------------
# targets
# ------------------------------------------------------------------------------
# compile all .c files in folder
compile: $(.files_o)

	@for file in $(.FILES); do \
		$(.LINKER) $(addsuffix .c, $$file) -o $(addsuffix .o ,$$file) ;        \
	done 

	$(.COMPILER) $(addsuffix .o, $(.FILES)) -o $(.EXE) 
	
	mv *.o $(.EXE) $(.BFOLDER)/
	

#-------------------------------------------------------------------------------

default: compile

all: default

.PHONY: default compile all help



