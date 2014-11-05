INCLUDE = \
-I$(GECKO_ROOT)/xpcom/base \
-I$(GECKO_ROOT)/xpcom/libxpcomrt \
-I$(GECKO_OBJ)/dist/include/nspr \
-I$(GECKO_OBJ)/dist/include \
-I$(GECKO_OBJ)/dist/include/nss

CFLAGS = \
-DMOZILLA_INTERNAL_API \
-DMOZILLA_XPCOMRT_API
