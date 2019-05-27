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
							tesh
    
.files_o    = $(addsuffix .o, $(.FILES))
.files_c    = $(addsuffix .c, $(.FILES))
.BFOLDER 		= $(BUILD_DIR)


# ------------------------------------------------------------------------------
# targets
# ------------------------------------------------------------------------------
# compile all .c files in folder
compile: $(.files_o)

	@for file in $(.FILES); do \
		$(.COMPILER) $(addsuffix .c, $$file) -o $(addsuffix .o, $$file);        \
	done 

	mv *.o $(.BFOLDER)/

#-------------------------------------------------------------------------------

default: compile

all: default

.PHONY: default compile all help



