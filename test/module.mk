CGREEN_HOME     := test
VPATH           += $(CGREEN_HOME)
CGREEN_SRC      := $(notdir $(wildcard $(CGREEN_HOME)/*.c))
CGREEN_SRC_FILES:= $(wildcard $(CGREEN_HOME)/*.c)

CGREEN_RELEASE  := $(addprefix $(RELEASE_CGREENDIR)/,$(CGREEN_SRC:.c=.exe))
CGREEN_DEBUG    := $(addprefix $(DEBUG_CGREENDIR)/,$(CGREEN_SRC:.c=.exe))
CGREEN_COVERAGE := $(addprefix $(COVERAGE_CGREENDIR)/,$(CGREEN_SRC:.c=.exe))

# Coverage
TEST_COVERAGE_LIMIT         := 95
GENERATED_COVERAGE_RESULTS  := generated/coverage-results

# Valgrind
GENERATED_VALGRIND  := generated/valgrind
GENERATED_CACHEGRIND  := generated/cachegrind
VALGRIND_OPTIONS    := --quiet \
                       --leak-check=full \
                       --log-file=$(GENERATED_VALGRIND)/valgrind.txt \
		       --suppressions=build/valgrind.supp \
                       --track-origins=yes  --gen-suppressions=all
HELGRIND_OPTIONS    :=  --quiet \
			--tool=helgrind \
			--trace-children=yes \
			--gen-suppressions=all \
			--history-level=approx \
			--log-file=$(GENERATED_VALGRIND)/valgrind.txt
CACHEGRIND_OPTIONS    := --tool=cachegrind \
			--trace-children=yes
VALGRIND_LD_LIBRARY   :=
# Use debug version to test valgrind with to get symbols
VALGRIND_TEST_EXE     := $(CGREEN_DEBUG)

CACHEGRIND_TEST_EXE   := $(RELEASE_CGREENDIR)/test_chunk_streamer_perf.exe

CGREEN_LIBS     := -Wl,-Bdynamic -ltheo -lcgreen -lpthread -lisal_crypto
ifeq "$(OS)" "Linux"
GCREEN_LINK_DIR        := -L$(GENERATED_LIB) -L$(GENERATED_LIB)/$(CGREEN_LIB_DIR)
VALGRIND_LD_LIBRARY    := $(GENERATED_LIB):$(GENERATED_LIB)/x86_64-linux-gnu:$(DEBUG_LIBDIR)
else
GCREEN_LINK_DIR        := -L$(GENERATED_LIB) 
VALGRIND_LD_LIBRARY    := $(GENERATED_LIB):$(DEBUG_LIBDIR)
endif


###############################################################################
# Targets to run tests.
###############################################################################
.PHONY: test-all
test-all: test test-coverage test-valgrind


.PHONY: test
test: cgreen-unit
test: TEST_EXE :=  $(CGREEN_RELEASE)
test: TEST_LD_LIBPATH := $(GENERATED_LIB)/x86_64-linux-gnu:$(GENERATED_LIB):$(RELEASE_LIBDIR)
test: run-unit-test

.PHONY:test-debug
test-debug: cgreen-debug
test-debug: TEST_EXE := $(CGREEN_DEBUG)
test-debug: TEST_LD_LIBPATH := $(GENERATED_LIB)/x86_64-linux-gnu:$(GENERATED_LIB):$(DEBUG_LIBDIR)
test-debug: run-unit-test

.PHONY: coverage
coverage: test-coverage
.PHONY: test-coverage
unit-test-coverage: cgreen-coverage
unit-test-coverage: TEST_EXE :=  $(CGREEN_COVERAGE)
unit-test-coverage: TEST_LD_LIBPATH := $(GENERATED_LIB)/x86_64-linux-gnu:$(GENERATED_LIB):$(COVERAGE_LIBDIR)
unit-test-coverage: run-unit-test-coverage

.PHONY: run-unit-test-coverage
run-unit-test-coverage: coverage-clean $(TEST_EXE)
	./build/run_unit_tests --ld-library-path $(LD_LIBRARY_PATH):$(TEST_LD_LIBPATH) --exe $(exe) --suite $(suite) --tests $(TEST_EXE)

.PHONY: run-unit-test
run-unit-test: $(TEST_EXE)
	./build/run_unit_tests --ld-library-path $(LD_LIBRARY_PATH):$(TEST_LD_LIBPATH)  --exe $(exe) --suite $(suite) --tests $(TEST_EXE)


###############################################################################
# Targets to run coverage.
###############################################################################
.PHONY: test-coverage
test-coverage: coverage-process-results

.PHONY: coverage-process-results
coverage-process-results: unit-test-coverage $(GENERATED_COVERAGE_RESULTS)
	printf "Make coverage...\n"
	lcov --capture \
	     --gcov-tool $(GCOV) \
             --base-directory . \
	     --rc lcov_branch_coverage=1 \
             --directory $(COVERAGE_OBJDIR) \
             --output-file $(GENERATED_COVERAGE_RESULTS)/coverage.info
	lcov --gcov-tool $(GCOV) \
	    --rc lcov_branch_coverage=1 \
	     --remove $(GENERATED_COVERAGE_RESULTS)/coverage.info \
             /usr/include/*  \
             --output-file $(GENERATED_COVERAGE_RESULTS)/coverage.filtered
	genhtml --show-details \
		--rc lcov_branch_coverage=1 \
                --legend \
                --output-directory $(GENERATED_COVERAGE_RESULTS) \
                $(GENERATED_COVERAGE_RESULTS)/coverage.filtered | tee $(GENERATED_COVERAGE_RESULTS)/summary.txt
	build/limit_coverage $(GENERATED_COVERAGE_RESULTS)/summary.txt $(TEST_COVERAGE_LIMIT)

$(GENERATED_COVERAGE_RESULTS):
	mkdir -p $(GENERATED_COVERAGE_RESULTS)

###############################################################################
# Valgrind.
###############################################################################
.PHONY: valgrind
valgrind: test-valgrind

.PHONY: test-valgrind
test-valgrind: $(DEBUG_DIR)
test-valgrind: run-test-valgrind

.PHONY: helgrind
helgrind: test-helgrind

.PHONY: test-helgrind
test-helgrind: $(DEBUG_DIR)
test-helgrind: VALGRIND_OPTIONS       := $(HELGRIND_OPTIONS)
test-helgrind: run-test-valgrind

.PHONY: cachegrind
cachegrind: test-cachegrind

.PHONY: test-cachegrind
test-cachegrind: TEST_LD_LIBPATH := $(GENERATED_LIB)/x86_64-linux-gnu:$(GENERATED_LIB):$(RELEASE_LIBDIR)
test-cachegrind: run-test-cachegrind

.PHONY: run-test-cachegrind
run-test-cachegrind: $(CACHEGRIND_TEST_EXE) $(GENERATED_CACHEGRIND)
	echo "Make cachegrind...."
	for testexe in $(CACHEGRIND_TEST_EXE); do \
		echo "Running $$testexe under cachegrind..." ; \
		LD_LIBRARY_PATH=$(TEST_LD_LIBPATH) valgrind $(CACHEGRIND_OPTIONS) \
		--cachegrind-out-file=$(GENERATED_CACHEGRIND)/cachegrind.out.%p.`basename $$testexe` ./$$testexe; \
	done 



.PHONY: run-test-valgrind
run-test-valgrind:$(VALGRIND_TEST_EXE) $(GENERATED_VALGRIND)
	echo "Make valgrind..."
	rm -f $(GENERATED_VALGRIND)/report.txt; \
	for testexe in $(VALGRIND_TEST_EXE); do \
		echo "Running $$testexe under valgrind..." ; \
		LD_LIBRARY_PATH=$(VALGRIND_LD_LIBRARY) valgrind $(VALGRIND_OPTIONS) ./$$testexe; \
		if [ $$? -ne 0 ]; then \
			printf "\nTest failed, aborting valgrind\n\n"; exit 1;\
		fi; \
		line_count=$$(wc -l $(GENERATED_VALGRIND)/valgrind.txt | awk '{print $$1}'); \
		if [ $$line_count -gt 1 ]; then \
			printf "\nVagrind errors.\n"; \
			cat $(GENERATED_VALGRIND)/valgrind.txt; exit 1;\
		fi; \
	done 
	

$(GENERATED_VALGRIND):
	mkdir -p $(GENERATED_VALGRIND)

$(GENERATED_CACHEGRIND):
	mkdir -p $(GENERATED_CACHEGRIND)
###############################################################################
# Targets to compile tests.
###############################################################################
.PHONY: cgreen-unit
cgreen-unit: release 
cgreen-unit: $(CGREEN_RELEASE)

.PHONY: cgreen-debug
cgreen-debug: debug 
cgreen-debug: $(CGREEN_DEBUG)

.PHONY: cgreen-coverage
cgreen-coverage: coverage
cgreen-coverage: $(CGREEN_COVERAGE)

$(CGREEN_RELEASE): $(RELEASE_LIBS)
$(CGREEN_DEBUG): $(DEBUG_LIBS)
$(CGREEN_COVERAGE): $(COVERAGE_LIBS)

$(RELEASE_CGREENDIR)/%.exe: $(CGREEN_HOME)/%.c
	$(CC) $(RELEASE_CFLAGS)  -Wl,--no-as-needed $(RELEASE_LIBPATH) $(GCREEN_LINK_DIR) -o $@ $< $(CGREEN_LIBS)

$(DEBUG_CGREENDIR)/%.exe: $(CGREEN_HOME)/%.c
	$(CC) $(DEBUG_CFLAGS)  -Wl,--no-as-needed $(DEBUG_LIBPATH) $(GCREEN_LINK_DIR) -o $@ $< $(CGREEN_LIBS)

$(COVERAGE_CGREENDIR)/%.exe: $(CGREEN_HOME)/%.c
	$(CC) $(DEBUG_CFLAGS)  -Wl,--no-as-needed $(COVERAGE_LIBPATH) $(GCREEN_LINK_DIR) -o $@ $< $(CGREEN_LIBS)


###############################################################################
# Clean targets
###############################################################################
.PHONY: coverage-clean
coverage-clean:
	printf "Make coverage-clean...\n"
	find $(COVERAGE_OBJDIR) -name *.gcda -delete 

.PHONY: cgreen-clean
cgreen-clean:
	rm -rf $(CGREEN_RELEASE) $(CGREEN_DEBUG) $(CGREEN_COVERAGE)
