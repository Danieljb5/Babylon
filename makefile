# ----- leave blank ----- #
FLAGS = 
INCLUDES = 
LINKS = 
LINKDIRS = 
RUN_CMD = 
CMDS = 
# ----------------------- #

# ----- don't touch ----- #
SILENT = @
CXX = g++
.PHONY: deps cmds
# ----------------------- #

# ---- generic flags ---- #
OBJDIR = obj
SOURCES = src/main.cpp
OBJECTS = $(OBJDIR)/**.o
DEPS = 
LIBGEN = 0 # set to 1 to generate a .so/.dll
STATIC_LIB = 0 # if generating libraries, use .a/.lib
TARGET = smol
# ---- generic flags ---- #

# platform specific flags #
COMMON_FLAGS = -std=c++17
COMMON_INCLUDES = -I . -I vendor -I include -I src
COMMON_LINKS = -pthread
COMMON_LINKDIRS = 
COMMON_CMDS = cp assets bin/$(BUILD_DIR)/ -r


LINUX_FLAGS = 
LINUX_INCLUDES = 
LINUX_LINKS = lib/*
LINUX_LINKDIRS = 
LINUX_CMDS = cp lib/*.so* bin/$(BUILD_DIR)/ ; mkdir -p bin/$(BUILD_DIR)/tools/dialogue ; cp tools/dist-linux-x64/* bin/$(BUILD_DIR)/tools/dialogue/

WINDOWS_FLAGS = 
WINDOWS_INCLUDES = 
WINDOWS_LINKS = windows-lib/* -lws2_32 -lwsock32
WINDOWS_LINKDIRS = 
WINDOWS_CMDS = cp windows-lib/*.dll* bin/$(BUILD_DIR)/ ; cp windows-lib-copy-only/*.dll* bin/$(BUILD_DIR)/ ; mkdir -p bin/$(BUILD_DIR)/tools/dialogue ; cp tools/dist-windows-x64/* bin/$(BUILD_DIR)/tools/dialogue/
# ----------------------- #

# -- command line vars -- #
ifndef platform
platform = linux
endif

ifndef mode
mode = debug
endif

ifdef verbose
SILENT = 
endif
# ----------------------- #

BUILD_DIR = $(mode)-$(platform)-x64

ifeq ($(strip $(platform)), linux)
CXX = g++
FLAGS = $(LINUX_FLAGS) -DLINUX
INCLUDES = $(LINUX_INCLUDES)
LINKS = $(LINUX_LINKS)
LINKDIRS = $(LINUX_LINKDIRS)
RUN_CMD = cd bin/$(BUILD_DIR) && ./$(TARGET)
CMDS = $(LINUX_CMDS)
ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATIC_LIB)), 1)
TARGET := lib$(TARGET).a
else
TARGET := lib$(TARGET).so
endif
else
TARGET := $(TARGET)
endif
endif

ifeq ($(strip $(platform)), windows)
CXX = x86_64-w64-mingw32-g++-posix
FLAGS = $(WINDOWS_FLAGS) -DWINDOWS
INCLUDES = $(WINDOWS_INCLUDES)
LINKS = $(WINDOWS_LINKS)
LINKDIRS = $(WINDOWS_LINKDIRS)
RUN_CMD = cd bin/$(BUILD_DIR) && wine $(TARGET)
CMDS = $(WINDOWS_CMDS)
ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATIC_LIB)), 1)
TARGET := lib$(TARGET).lib
else
TARGET := lib$(TARGET).dll
endif
else
TARGET := $(TARGET).exe
endif
endif

ifeq ($(strip $(mode)), debug)
FLAGS := $(FLAGS) -Og -g -DDEBUG
endif
ifeq ($(strip $(mode)), release)
FLAGS := $(FLAGS) -O3 -fopenmp -D_GLIBCXX_PARALLEL -DRELEASE
endif
ifeq ($(strip $(mode)), dist)
FLAGS := $(FLAGS) -O3 -fopenmp -D_GLIBCXX_PARALLEL -DDIST
endif

ifeq ($(strip $(CMDS)), )
CMDS = $(COMMON_CMDS)
else
CMDS := $(CMDS); $(COMMON_CMDS)
endif
OBJDIR := $(OBJDIR)/$(BUILD_DIR)
FLAGS := $(FLAGS) $(COMMON_FLAGS)
INCLUDES := $(INCLUDES) $(COMMON_INCLUDES)
LINKS := $(LINKS) $(COMMON_LINKS)
LINKDIRS := $(LINKDIRS) $(COMMON_LINKDIRS)

COMPILER = 
LINKER = 

ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATIC_LIB)), 1)
COMPILER = $(CXX) -c $(SOURCES) $(FLAGS) $(INCLUDES)
LINKER = ar rcs bin/$(BUILD_DIR)/$(TARGET) $(OBJECTS) $(LINKS)
else
COMPILER = $(CXX) -c $(SOURCES) $(FLAGS) $(INCLUDES) -fPIC
LINKER = $(CXX) -shared $(OBJECTS) -o bin/$(BUILD_DIR)/$(TARGET) $(FLAGS)
endif
else
COMPILER = $(CXX) -c $(SOURCES) $(FLAGS) $(INCLUDES)
LINKER = $(CXX) $(OBJECTS) $(LINKDIRS) $(LINKS) $(INCLUDES) -o bin/$(BUILD_DIR)/$(TARGET) $(FLAGS)
endif

all: bin/$(BUILD_DIR)/$(TARGET)


bin/$(BUILD_DIR):
	$(SILENT) mkdir -p bin/$(BUILD_DIR)

$(OBJECTS):
	$(SILENT) mkdir -p $(OBJDIR)
	$(SILENT) $(COMPILER)
	$(SILENT) mv *.o $(OBJDIR)
	
deps:
	$(foreach dep,$(DEPS),$(SILENT) cd $(dep) && $(MAKE);)
	
depsclean:
	$(foreach dep,$(DEPS),$(SILENT) cd $(dep) && $(MAKE) clean;)
	
cmds:
	$(CMDS)

bin/$(BUILD_DIR)/$(TARGET): bin/$(BUILD_DIR) $(OBJECTS) deps cmds
	$(SILENT) $(LINKER)

clean: depsclean
	$(SILENT) rm bin/* -r || true
	$(SILENT) rm obj/* -r || true
	
run: bin/$(BUILD_DIR)/$(TARGET)
	$(SILENT) $(RUN_CMD)
