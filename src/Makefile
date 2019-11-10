# ------------------------------------------------------------------------------
# Copyright (c) 2019 Jose Moran <jmoran071996@gmail.com>
# ------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
.SHELL      = bash
.PWD        = $(shell pwd)
.COMPILER   = gcc 
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
compile: $(.FILES)

	@for file in $(.FILES); do \
		$(.COMPILER) -g $(addsuffix .c, $$file) -o $$file ;        \
	done 
	
	mv $(.FILES) $(.BFOLDER)/
	

#-------------------------------------------------------------------------------

default: compile

all: default

.PHONY: default compile all help



