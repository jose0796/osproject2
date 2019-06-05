.PROJECTNAME = tesh
# ------------------------------------------------------------------------------
.SUBMAKE	= $(MAKE) --no-print-directory
.PWD		= $(shell pwd)
.SRC		= src
.BFOLDER	= $(.PWD)/bin
.TESTSF		= tests
.TBEXE		= $(.BFOLDER)/$(.PROJECTNAME).o

# ------------------------------------------------------------------------------
# targets
# ------------------------------------------------------------------------------
help:
	@echo -e "--------------------------------------------------------------------------------"
	@echo -e "Please, choose one target:"
	@echo -e "- compile: Compile TeSh main program."
	@echo -e "- run:	 Run TeSh main program"
	@echo -e "--------------------------------------------------------------------------------"

# ------------------------------------------------------------------------------
# build executable
compile:
	@printf "%b" "Building TeSh\n"
	@mkdir -p $(.BFOLDER)
	+@$(.SUBMAKE) -C $(.SRC) compile BIN_DIR=$(.BFOLDER)

# ------------------------------------------------------------------------------
# run TeSh
run-interactive: compile 
	$(.BFOLDER)/$(.PROJECTNAME)

# ------------------------------------------------------------------------------
# run script with TeSh

run-script: compile 
	$(.BFOLDER)/$(.PROJECTNAME) $(SCRIPT)

# clean
# ------------------------------------------------------------------------------
clean:
	@rm -rf $(.BFOLDER)


.PHONY: all compile run clean
