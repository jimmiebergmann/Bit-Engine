CC		= g++
CFLAGS		= -W -Wall
CCPATHS		= include
AR		= ar rcs
RM		= rm -f
MKDIR		= mkdir -p
OBJ_PATH        = obj/Linux/32
LIB_PATH        = lib/Linux/32
EXTLIBS_PATH	= extlibs


# Libraries
LIB_PREFIX	= lib
LIB_BIT_PREFIX	= bit-
LIB_AUDIO	= $(LIB_BIT_PREFIX)audio
LIB_GRAPHICS	= $(LIB_BIT_PREFIX)graphics
LIB_NETWORK	= $(LIB_BIT_PREFIX)network
LIB_SYSTEM	= $(LIB_BIT_PREFIX)system
LIB_WINDOW	= $(LIB_BIT_PREFIX)window


# All target
all: debug

Debug: debug


# Debug target
debug: $(LIB_PATH)/$(LIB_PREFIX)$(LIB_SYSTEM).a
	$(info -----------------------------------) 
	$(info Building DEBUG profile )
	$(info -----------------------------------)


# System target
SYSTEM_CPP = $(shell find source/Bit/System -type f -name "*.cpp" )
SYSTEM_CPP += source/Bit/System.cpp
SYSTEM_CPP_FILE = $(notdir $(SYSTEM_CPP))
SYSTEM_CPP_PATH = $(dir $(SYSTEM_CPP))
SYSTEM_FILE = $(patsubst %.cpp, %, $(SYSTEM_CPP_FILE))
SYSTEM_OBJ = $(addprefix $(OBJ_PATH)/, $(patsubst %.cpp, %.o, $(SYSTEM_CPP)))

$(LIB_PATH)/$(LIB_PREFIX)$(LIB_SYSTEM).a: $(SYSTEM_OBJ)
	@echo
	@echo target: $@
	@echo
	@echo cpp:  $(SYSTEM_CPP)
	@echo
	@echo cpp file: $(SYSTEM_CPP_FILE)
	@echo
	@echo cpp path: $(SYSTEM_CPP_PATH)	
	@echo
	@echo file: $(SYSTEM_FILE)
	@echo
	@echo obj: $(SYSTEM_OBJ)
	@echo

# System objects
$(OBJ_PATH)/%.o: %.cpp
	$(MKDIR) $(dir $@)
	$(CC) -std=gnu++0x -I$(CCPATHS) -c $< -o $@
	@echo
	@echo 1: $@
	@echo 2: $<
	@echo 3: %


# Clean target
.PHONY: clean cleanall

clean:
	$(RM) -rf obj

cleanall: clean
	$(RM) -rf lib
