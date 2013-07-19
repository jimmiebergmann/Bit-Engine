################################################
#
#	Bit Engine Makefile
#
################################################

MAKE 				= make -f
MAKEFILE_PATH		= build/makefile
MAKEFILE_AUDIO		= Makefile-bit-audio
MAKEFILE_GRAPHICS		= Makefile-bit-graphics
MAKEFILE_NETWORK		= Makefile-bit-network
MAKEFILE_SYSTEM		= Makefile-bit-system
MAKEFILE_WINDOW		= Makefile-bit-window


all:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO)
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS)
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK)
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM)
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW)

# DEBUG targets
# Static DEBUG target
debug_static:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@


# Dynamic DEBUG target
debug_dynamic:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@

# RELEASE targets
# Static RELEASE target
release_static:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@

# Dynamic RELEASE target
release_dynamic:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@




# Clean
.PHONY: clean cleanall

clean:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@

cleanall:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@

cleandebug_static:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@

cleanrelease_static:
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_AUDIO) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_GRAPHICS) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_NETWORK) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_SYSTEM) $@
	@cd ../../; $(MAKE) $(MAKEFILE_PATH)/$(MAKEFILE_WINDOW) $@
