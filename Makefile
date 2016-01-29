#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITARM)/3ds_rules

#---------------------------------------------------------------------------------
# homebrew metadata
#---------------------------------------------------------------------------------
VERSION		:=	$(shell git describe --abbrev=4 --always --dirty --tags || echo "UNKNOWN")
TARGET		:=	boot
BUILD		:=	build
SOURCES		:=	source libs/inih libs/CakeBrah/source libs/CakeBrah/source/libkhax
INCLUDES	:=	include libs/inih libs/CakeBrah/include
NO_SMDH		:=	1
APP_TITLE	:=	BootCtr
APP_DESCRIPTION	:=	A simple boot manager for 3DS
APP_AUTHOR	:=	m45t3r

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH		:=	-march=armv6k -mtune=mpcore -mfloat-abi=hard
COMMONFLAGS	:=	$(ARCH) $(INCLUDE) -Wall -Wextra -Os -mword-relocations \
			-fomit-frame-pointer -ffunction-sections -ffast-math \
			-DARM11 -D_3DS -DVERSION_STRING="\"$(VERSION)\""
CFLAGS		:=	$(COMMONFLAGS) -std=gnu11
CXXFLAGS	:=	$(COMMONFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS		:=	$(ARCH)
LDFLAGS		=	-specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS		:= 	-lctru

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:= $(CTRULIB)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir \
			$(filter-out libs/CakeBrah/source/main.c, \
			$(filter-out libs/CakeBrah/source/hid.c, \
			$(wildcard $(dir)/*.c)))))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(PNGFILES:.png=.bgr.o) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o) \

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) $(INCLUDES) clean all

#---------------------------------------------------------------------------------
all: $(BUILD)

#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
release: $(BUILD)
	@rm -f $(APP_TITLE)-$(VERSION).zip
	@zip -9 -r $(APP_TITLE)-$(VERSION).zip boot{.3dsx,_config.ini} README.md splash/

#---------------------------------------------------------------------------------
clean:
	rm -rf $(BUILD) $(TARGET).{3dsx,elf} $(APP_TITLE)-*.zip

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
ifeq ($(strip $(NO_SMDH)),)
.PHONY: all
all	:	$(OUTPUT).3dsx $(OUTPUT).smdh
endif
$(OUTPUT).3dsx	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)
-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
