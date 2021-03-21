# Makefile

CWD=$(shell pwd)
OS=$(shell uname)

ifeq "$(OS)" "Darwin"
    SO_SUFFIX     :=dylib
    CGREEN_LIB_DIR:=
    CC            :=/usr/local/bin/gcc-8
    GCOV          :=/usr/local/bin/gcov-8
    STAT          :=stat -x
    AR            :=ar
    RANLIB        :=ranlib
else
    SO_SUFFIX     :=so
    CGREEN_LIB_DIR:=x86_64-linux-gnu
    CC            :=gcc
    GCOV          :=gcov
    STAT          :=stat
    AR            :=gcc-ar 
    RANLIB        :=gcc-ranlib 
endif

GENERATED=generated
GENERATED_COVERAGE=generated/coverage
GENERATED_DEBUG=generated/debug
GENERATED_RELEASE=generated/release
GENERATED_INCLUDE=generated/include
GENERATED_LIB=generated/lib
GENERATED_BIN=generated/bin
GENERATED_DIRS=$(GENERATED) $(GENERATED_COVERAGE) $(GENERATED_DEBUG) $(GENERATED_RELEASE) \
$(GENERATED_INCLUDE) $(GENERATED_LIB) $(GENERATED_BIN)

THIRD_PARTY=third-party

###############################################################################
# The Target directories for debug objects, libraries and executables
###############################################################################
DEBUG_OBJDIR    := $(GENERATED)/debug/obj
DEBUG_BINDIR    := $(GENERATED)/debug/bin
DEBUG_LIBDIR    := $(GENERATED)/debug/lib
DEBUG_CGREENDIR := $(GENERATED)/debug/cgreen
DEBUG_DIR       := $(DEBUG_OBJDIR) \
        $(DEBUG_BINDIR) \
        $(DEBUG_LIBDIR) \
        $(DEBUG_CGREENDIR)

###############################################################################
# The Target directories for release objects, libraries and executables
###############################################################################
RELEASE_OBJDIR    := $(GENERATED)/release/obj
RELEASE_BINDIR    := $(GENERATED)/release/bin
RELEASE_LIBDIR    := $(GENERATED)/release/lib
RELEASE_CGREENDIR := $(GENERATED)/release/cgreen
RELEASE_DIR       := $(RELEASE_OBJDIR) \
        $(RELEASE_BINDIR) \
        $(RELEASE_LIBDIR) \
        $(RELEASE_CGREENDIR)

###############################################################################
# The Target directories for coverage objects, libraries and executables
###############################################################################
COVERAGE_OBJDIR    := $(GENERATED)/coverage/obj
COVERAGE_BINDIR    := $(GENERATED)/coverage/bin
COVERAGE_LIBDIR    := $(GENERATED)/coverage/lib
COVERAGE_CGREENDIR := $(GENERATED)/coverage/cgreen
COVERAGE_DIR       := $(COVERAGE_OBJDIR) \
        $(COVERAGE_BINDIR) \
        $(COVERAGE_LIBDIR) \
        $(COVERAGE_CGREENDIR)

###############################################################################
# Gobal variables that are built up by included modules
###############################################################################
VPATH         :=
OBJ           :=
SRC           :=
RELEASE_LIBS  :=
DEBUG_LIBS    :=
COVERAGE_LIBS :=


###############################################################################
# Setup Includes
###############################################################################
INCLUDES      := -Isrc
INCLUDES      += -I$(GENERATED)/include

###############################################################################
# Setup CFlags 
###############################################################################
CFLAGS               =
CFLAGS               += -Wall -Wextra -Werror -fPIC
DEBUG_CFLAGS         := -O0 -ggdb $(CFLAGS) $(INCLUDES)
RELEASE_CFLAGS       := -O3 -flto $(CFLAGS) $(INCLUDES)
COVERAGE_CFLAGS      := -O0 -fprofile-arcs -ftest-coverage $(CFLAGS) \
-DCOVERAGE $(INCLUDES)

###############################################################################
# Setup LD Flags and Library paths
###############################################################################
THIRD_PARTY_LIBDIR:=  $(GENERATED)/lib
# Not clear if flto needs O3 flag.
LDFLAGS           += -O3 -fPIC -flto
RELEASE_LIBPATH   := -L$(RELEASE_LIBDIR) -L$(THIRD_PARTY_LIBDIR)
DEBUG_LIBPATH     := -L$(DEBUG_LIBDIR) -L$(THIRD_PARTY_LIBDIR)
COVERAGE_LIBPATH  := -L$(COVERAGE_LIBDIR) -L$(THIRD_PARTY_LIBDIR)
DEBUG_LDFLAGS     :=  $(LDFLAGS) $(DEBUG_LIBPATH)
RELEASE_LDFLAGS   := $(LDFLAGS) $(RELEASE_LIBPATH)
COVERAGE_LDFLAGS  := --coverage $(LDFLAGS) $(COVERAGE_LIBPATH) -lgcov

###############################################################################
# Targets
###############################################################################
.PHONY: all
all: release 

.PHONY: release
release: $(RELEASE_DIR) third-party

.PHONY: debug
debug: $(DEBUG_DIR) third-party

.PHONY: coverage
coverage: $(COVERAGE_DIR) third-party


###############################################################################
# Include modules 
###############################################################################
include third-party/module.mk
include src/theo/module.mk
include test/module.mk

DEPENDENCIES := $(OBJ:.o=.dep)
# Don't build dependencies for these targets.
DEPENDENCIES_SKIP_TARGETS := "indent" "pmccabe" "clean" "clean-third-party" "cpd" "doc"
DEPENDENCIES_TARGETS := $(strip $(filter $(DEPENDENCIES_SKIP_TARGETS), "$(MAKECMDGOALS)"))

ifeq "$(DEPENDENCIES_TARGETS)" ""
include $(OBJ:.o=.dep)
endif


###############################################################################
# Generate C file dependencies.
###############################################################################
$(DEBUG_OBJDIR)/%.dep: %.c
	[ -d $(DEBUG_OBJDIR) ] || mkdir -p $(DEBUG_OBJDIR) 
	build/depend.sh $(DEBUG_OBJDIR) $(DEBUG_CFLAGS) $< > $@

$(RELEASE_OBJDIR)/%.dep: %.c
	[ -d $(RELEASE_OBJDIR) ] ||  mkdir -p $(RELEASE_OBJDIR) 
	build/depend.sh $(RELEASE_OBJDIR) $(RELEASE_CFLAGS) $< > $@

$(COVERAGE_OBJDIR)/%.dep: %.c
	[ -d $(COVERAGE_OBJDIR) ] || mkdir -p $(COVERAGE_OBJDIR) 
	build/depend.sh $(COVERAGE_OBJDIR) $(COVERAGE_CFLAGS) $< > $@

###############################################################################
# Rules for building object files - debug, release or coverage
###############################################################################
$(DEBUG_OBJDIR)/%.o : %.c
	$(CC) $(DEBUG_CFLAGS) -c $< -o $@

$(RELEASE_OBJDIR)/%.o : %.c
	$(CC) $(RELEASE_CFLAGS) -c $< -o $@

$(COVERAGE_OBJDIR)/%.o : %.c
	$(CC) $(COVERAGE_CFLAGS) -c $< -o $@

###############################################################################
# Targets for building directories
###############################################################################
$(COVERAGE_DIR):
	mkdir -p $(COVERAGE_DIR) 

$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)  

$(DEBUG_DIR):
	mkdir -p $(DEBUG_DIR) 

.PHONY: clean
clean:
	rm -rf $(GENERATED_COVERAGE) $(GENERATED_DEBUG) $(GENERATED_RELEASE)

.PHONY: clean-third-party
clean-third-party: clean
	rm -rf $(GENERATED_INCLUDE) $(GENERATED_LIB) $(GENERATED)


###############################################################################
# Run commit checks.
###############################################################################
.PHONY: commit
commit: cpd pmccabe cppcheck valgrind coverage indent

###############################################################################
# Run pmccabe - check code complexity.
###############################################################################
COMPLEXITY_DIR   := generated/complexity
COMPLEXITY_LIMIT := 10
PMCCABE_EXE      := pmccabe

.PHONY: pmccabe
pmccabe: 
	@printf "Make pmccabe...\n"
	@if [ -d $(COMPLEXITY_DIR) ]; then rm -rf $(COMPLEXITY_DIR); fi 
	@mkdir -p $(COMPLEXITY_DIR)
	@$(PMCCABE_EXE) -vt $(SRC) | tee $(COMPLEXITY_DIR)/overall.txt
	@$(PMCCABE_EXE) -vt $(SRC) | head -7 | tee $(COMPLEXITY_DIR)/topten.txt 
	@$(PMCCABE_EXE) $(SRC) | sort -nr | head -10 | tee -a $(COMPLEXITY_DIR)/topten.txt 
	@$(PMCCABE_EXE) $(SRC) | awk '$$1 > $(COMPLEXITY_LIMIT) ' > $(COMPLEXITY_DIR)/error.txt 
	@if [ -s $(COMPLEXITY_DIR)/error.txt ]; then \
		printf "pmccabe error - limit of $(COMPLEXITY_LIMIT) exceeded\n"; \
		cat $(COMPLEXITY_DIR)/error.txt; \
		exit 1; \
	fi 

###############################################################################
# Run doxygen - create doxygen output.
###############################################################################
GENERATED_DOCS := generated/docs

.PHONY: doc
doc: $(GENERATED_DOCS)
	@printf "Make doc...\n"
	@doxygen build/Doxyfile

$(GENERATED_DOCS):
	@mkdir -p $(GENERATED_DOCS)

###############################################################################
# Indent the code to Linux Kernel Std.
#
# We run make indent twice. If we do not then indent can flip bits of
# the code around on alternate runs.
###############################################################################
INDENT_SRC := $(SRC) $(CGREEN_SRC_FILES)

.PHONY: indent
indent:
	@printf "Make indent...\n"
	@for file in $(INDENT_SRC); do \
		./build/Lindent $$file; \
	done
	@for file in $(INDENT_SRC); do \
		./build/Lindent $$file; \
	done
	@find src -name "*.c~" | xargs rm
	@find test -name "*.c~" | xargs rm

###############################################################################
# Duplictes check
###############################################################################
GENERATED_CPD := generated/cpd
CPD_REPORT    := $(GENERATED_CPD)/report.txt

# Minimum number of repeated tokens
CPD_MINIMUM_TOKENS :=50
# Recursively check src directory, test has lots of c'n'p between tests.
CPD_DIRS      := src

.PHONY: cpd
cpd: $(GENERATED_CPD)
	@printf "Make cpd...\n"
	@(export MINIMUM_TOKENS=$(CPD_MINIMUM_TOKENS) && \
	  export LANGUAGE=c && \
	build/pmd-4.2.5/bin/cpd.sh $(CPD_DIRS) > $(CPD_REPORT))	
	@# if file size is greater than 1 then cpd failed.
	@if [ $$($(STAT) "$(CPD_REPORT)" | grep Size | awk '{print $$2}') -gt 1 ]; then \
                cat $(CPD_REPORT); \
                exit 1; \
        fi

$(GENERATED_CPD):
	@mkdir -p $(GENERATED_CPD)

###############################################################################
# cppcheck
###############################################################################
.PHONY: cppcheck
cppcheck: 
	@printf "Make cppcheck...\n"
	@cppcheck  --enable=warning,performance --inconclusive test
	@cppcheck  --enable=warning,performance --inconclusive src

###############################################################################
# Make the generated directories
###############################################################################
$(GENERATED_DIRS):
	mkdir -p $(GENERATED_DIRS)


