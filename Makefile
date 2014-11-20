# These should be defined in .config
#GECKO_ROOT = /Volumes/fennec/gecko-desktop
#GECKO_OBJ = $(GECKO_ROOT)/obj-x86_64-apple-darwin12.5.0

include .config
PLATFORM:=$(shell uname)
include platforms/common.mk
include platforms/$(PLATFORM).mk

BUILD_DIR=./build-$(PLATFORM)

CFLAGS += -DMOZILLA_XPCOMRT_API

LIBS = \
$(GECKO_OBJ)/netwerk/mini/libneckomini.a.desc \
$(GECKO_OBJ)/xpcom/libxpcomrt/libxpcomrt.a.desc \
$(GECKO_OBJ)/mfbt/libmfbt.a.desc

LIB_ROLLUP = $(BUILD_DIR)/librollup.a

all: test

OBJS = \
$(BUILD_DIR)/main.o

test: $(OBJS) $(LIB_ROLLUP) iTest.h
	$(CXX) $(OBJS) $(LIB_ROLLUP) $(LFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp iTest.h
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(SDL_CFLAGS) $(INCLUDE) $< -c -o $@

%.h: %.idl
	PYTHONPATH=$(GECKO_OBJ)/dist/sdk/bin/ python $(GECKO_OBJ)/dist/sdk/bin/header.py -I$(GECKO_OBJ)/dist/idl $^ -o $@

$(LIB_ROLLUP): $(LIBS)
	@mkdir -p $(BUILD_DIR)
	$(AR) cr $@ `python ./tools/expand.py $(LIBS)`

clean:
	rm -f $(LIB_ROLLUP) $(BUILD_DIR)/*.o

clobber: clean
	rm -f test iTest.h
	rm -rf $(BUILD_DIR)
