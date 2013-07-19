MAKEFILE_PATH		= build/makefile
SYSTEM_MAKEFILE		= Makefile-bit-system


all:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE)

# DEBUG targets
# Static DEBUG target
debug_static:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@


# Dynamic DEBUG target
debug_dynamic:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@

# RELEASE targets
# Static RELEASE target
release_static:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@

# Dynamic RELEASE target
release_dynamic:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@




# Clean
.PHONY: clean cleanall

clean:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@

cleanall:
	@cd ../../; make -f $(MAKEFILE_PATH)/$(SYSTEM_MAKEFILE) $@


