<<<<<<< HEAD
# if "template" is in the make command, do not include static.lib files
ifneq (,$(findstring template,$(MAKECMDGOALS)))
ASSET_FILES=$(wildcard static/*)
else
ASSET_FILES=$(wildcard static/*) $(wildcard static.lib/*)
endif

TEMPLATE_FILES+=$(wildcard static/*) $(wildcard firmware/hot-cold-asset.mk)

ASSET_OBJ=$(addprefix $(BINDIR)/, $(addsuffix .o, $(ASSET_FILES)) )

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1)) $(ASSET_OBJ)

.SECONDEXPANSION:
$(ASSET_OBJ): $$(patsubst bin/%,%,$$(basename $$@))
	$(VV)mkdir -p $(BINDIR)/static
	$(VV)mkdir -p $(BINDIR)/static.lib
	@echo "ASSET $@"
=======
# if "template" is in the make command, do not include static.lib files
ifneq (,$(findstring template,$(MAKECMDGOALS)))
ASSET_FILES=$(wildcard static/*)
else
ASSET_FILES=$(wildcard static/*) $(wildcard static.lib/*)
endif

TEMPLATE_FILES+=$(wildcard static/*) $(wildcard firmware/hot-cold-asset.mk)

ASSET_OBJ=$(addprefix $(BINDIR)/, $(addsuffix .o, $(ASSET_FILES)) )

GETALLOBJ=$(sort $(call ASMOBJ,$1) $(call COBJ,$1) $(call CXXOBJ,$1)) $(ASSET_OBJ)

.SECONDEXPANSION:
$(ASSET_OBJ): $$(patsubst bin/%,%,$$(basename $$@))
	$(VV)mkdir -p $(BINDIR)/static
	$(VV)mkdir -p $(BINDIR)/static.lib
	@echo "ASSET $@"
>>>>>>> 1d9f8e89a47b9fdcfcd5046e5cc23ef5a95901bd
	$(VV)$(OBJCOPY) -I binary -O elf32-littlearm -B arm $^ $@