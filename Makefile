# These should be defined in .config
#GECKO_ROOT = /Volumes/fennec/gecko-desktop
#GECKO_OBJ = $(GECKO_ROOT)/obj-x86_64-apple-darwin12.5.0

include .config
PLATFORM:=$(shell uname)
include platforms/common.mk
include platforms/$(PLATFORM).mk

BUILD_DIR=./build-$(PLATFORM)

XPCOM_LIBS = \
$(GECKO_OBJ)/xpcom/base/libxpcom_base.a.desc \
$(GECKO_OBJ)/xpcom/build/libxpcom_build.a.desc \
$(GECKO_OBJ)/xpcom/string/libxpcom_string.a.desc \
$(GECKO_OBJ)/xpcom/ds/libxpcom_ds.a.desc \
$(GECKO_OBJ)/xpcom/io/libxpcom_io.a.desc \
$(GECKO_OBJ)/xpcom/components/libxpcom_components.a.desc \
$(GECKO_OBJ)/xpcom/glue/libxpcomglue_s.a.desc \
$(GECKO_OBJ)/xpcom/threads/libxpcom_threads.a.desc

# ./glue/nomozalloc/libxpcomglue_s_nomozalloc.a.desc
# ./glue/standalone/libxpcomglue.a.desc
# ./libxpcomrt/libxpcomrt.a.desc
# ./reflect/xptcall/libxpcom_reflect_xptcall.a.desc
# ./reflect/xptcall/md/unix/libxpcom_reflect_xptcall_md_unix.a.desc
# ./reflect/xptinfo/libxpcom_reflect_xptinfo.a.desc

LIBS = \
$(GECKO_OBJ)/mfbt/libmfbt.a.desc \
$(GECKO_OBJ)/xpcom/libxpcomrt/libxpcomrt.a.desc \
$(GECKO_OBJ)/ipc/chromium/libipc_chromium.a.desc \
$(XPCOM_LIBS) \
$(GECKO_OBJ)/ipc/glue/libipc_glue.a.desc

LIB_ROLLUP = $(BUILD_DIR)/librollup.a

all: test

OBJS = \
$(BUILD_DIR)/main.o

test: $(OBJS) $(LIB_ROLLUP)
	$(CXX) $(OBJS) $(LIB_ROLLUP) $(LFLAGS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(SDL_CFLAGS) $(INCLUDE) $^ -c -o $@

$(LIB_ROLLUP): $(LIBS)
	@mkdir -p $(BUILD_DIR)
	$(AR) cr $@ `python ./tools/expand.py $(LIBS)`

clean:
	rm -f $(LIB_ROLLUP) $(BUILD_DIR)/*.o

clobber: clean
	rm -f test
	rm -rf $(BUILD_DIR)
