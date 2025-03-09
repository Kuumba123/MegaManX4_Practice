ifeq ($(BUILD_ID),561)
LDFLAGS += symbol.ld
else
LDFLAGS += symbol_jp.ld
endif
CPPFLAGS += -fpermissive