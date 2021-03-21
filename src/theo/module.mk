LIB_THEO_HOME := src/theo
VPATH += $(LIB_THEO_HOME)

LIB_THEO_SRC  := $(notdir $(wildcard $(LIB_THEO_HOME)/*.c))
SRC           += $(wildcard $(LIB_THEO_HOME)/*.c)

LIB_THEO_OBJ_DEBUG    := $(addprefix $(DEBUG_OBJDIR)/,$(LIB_THEO_SRC:.c=.o))
LIB_THEO_OBJ_RELEASE  := $(addprefix $(RELEASE_OBJDIR)/,$(LIB_THEO_SRC:.c=.o))
LIB_THEO_OBJ_COVERAGE := $(addprefix $(COVERAGE_OBJDIR)/,$(LIB_THEO_SRC:.c=.o))
OBJ += $(LIB_THEO_OBJ_DEBUG) $(LIB_THEO_OBJ_RELEASE) $(LIB_THEO_OBJ_COVERAGE)

LIB_THEO_RELEASE  := $(RELEASE_LIBDIR)/libtheo.a
LIB_THEO_DEBUG    := $(DEBUG_LIBDIR)/libtheo.a
LIB_THEO_COVERAGE := $(COVERAGE_LIBDIR)/libtheo.a

LIB_THEO_RELEASE_SO  := $(RELEASE_LIBDIR)/libtheo.$(SO_SUFFIX)
LIB_THEO_DEBUG_SO    := $(DEBUG_LIBDIR)/libtheo.$(SO_SUFFIX)
LIB_THEO_COVERAGE_SO := $(COVERAGE_LIBDIR)/libtheo.$(SO_SUFFIX)

RELEASE_LIBS    += $(LIB_THEO_RELEASE) $(LIB_THEO_RELEASE_SO)
DEBUG_LIBS      += $(LIB_THEO_DEBUG) $(LIB_THEO_DEBUG_SO)
COVERAGE_LIBS   += $(LIB_THEO_COVERAGE) $(LIB_THEO_COVERAGE_SO)

###############################################################################
release: $(LIB_THEO_RELEASE) $(LIB_THEO_RELEASE_SO)
debug: $(LIB_THEO_DEBUG)  $(LIB_THEO_DEBUG_SO)
coverage: $(LIB_THEO_COVERAGE) $(LIB_THEO_COVERAGE_SO)

$(LIB_THEO_RELEASE): $(LIB_THEO_OBJ_RELEASE)
	echo "Making libtheo release"
	$(AR) ru $@ $(LIB_THEO_OBJ_RELEASE)
	$(RANLIB) $@

$(LIB_THEO_DEBUG): $(LIB_THEO_OBJ_DEBUG)
	echo "Making libtheo debug"
	$(AR) ru $@ $(LIB_THEO_OBJ_DEBUG)
	$(RANLIB) $@

$(LIB_THEO_COVERAGE): $(LIB_THEO_OBJ_COVERAGE)
	echo "Making libtheo coverage"
	$(AR) ru $@ $(LIB_THEO_OBJ_COVERAGE)
	$(RANLIB) $@

$(LIB_THEO_RELEASE_SO): $(LIB_THEO_OBJ_RELEASE)
	echo "Making libtheo shared release"
	$(CC) $(RELEASE_LDFLAGS) -shared $(LIB_THEO_OBJ_RELEASE) -o $@

$(LIB_THEO_DEBUG_SO): $(LIB_THEO_OBJ_DEBUG)
	echo "Making libtheo shared debug"
	$(CC) $(DEBUG_LDFLAGS) -shared $(LIB_THEO_OBJ_DEBUG) -o $@

$(LIB_THEO_COVERAGE_SO): $(LIB_THEO_OBJ_COVERAGE)
	echo "Making libtheo shared coverage"
	$(CC) $(COVERAGE_LDFLAGS) -shared $(LIB_THEO_OBJ_COVERAGE) -o $@
