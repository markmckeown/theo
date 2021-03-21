CGREEN_RUNNER := $(GENERATED)/bin/cgreen-runner
# Use the log file as trigger for re-build
CGREEN_LOG_FILE :=third-party/cgreen/theo.log

.PHONY: cgreen
cgreen: $(CGREEN_RUNNER)

$(CGREEN_RUNNER): $(CGREEN_LOG_FILE)
	(cd $(THIRD_PARTY)/cgreen && \
	cmake -DWITH_HTML:bool=OFF -DWITH_PDF:bool=Off  -DCMAKE_INSTALL_PREFIX=$(CWD)/$(GENERATED) && \
	make clean && \
	make install)

$(CGREEN_LOG_FILE): $(GENERATED_DIRS)


