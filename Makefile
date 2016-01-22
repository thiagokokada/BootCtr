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
VERSION		:=	$(shell git describe --tags || echo "UNKNOWN")
TARGET		:=	boot
BUILD		:=	build
SOURCES		:=	source inih CakeBrah/source CakeBrah/source/libkhax
INCLUDES	:=	include inih CakeBrah/include
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
			-DARM11 -D_3DS
CFLAGS		:=	$(COMMONFLAGS) -std=gnu11
CXXFLAGS	:=	$(COMMONFLAGS) -fno-rtti -fno-exceptions -std=gnu++11 
ASFLAGS		:=	$(ARCH)
LDFLAGS		:=	-specs=3dsx.specs $(ARCH) -Wl,-Map,$(notdir $*.map) \
			-Wl,--gc-sections
LIBS		:= 	-lctru
LIBDIRS		:= 	$(PORTLIBS) $(CTRULIB)

#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------
export OUTPUT	:=	$(CURDIR)/$(TARGET)
export TOPDIR	:=	$(CURDIR)
export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir \
			$(filter-out CakeBrah/source/main.c, \
			$(filter-out CakeBrah/source/hid.c, \
			$(wildcard $(dir)/*.c)))))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CC)
else
	export LD	:=	$(CXX)
endif

#---------------------------------------------------------------------------------
export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
			$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
			$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
			-I$(CURDIR)/$(BUILD)
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: $(BUILD) clean all

#---------------------------------------------------------------------------------
# make targets
#---------------------------------------------------------------------------------
all: $(BUILD)

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@make --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile
release: $(BUILD)
	@rm -f $(APP_TITLE)-$(VERSION).zip
	@zip -9 -r $(APP_TITLE)-$(VERSION).zip * --exclude="*build*" \
		--exclude="*.git*" --exclude="*.elf" --exclude="*.zip"
clean:
	rm -rf $(BUILD) $(TARGET).{3dsx,elf} $(APP_TITLE)-*.zip

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
else
DEPENDS		:=	$(OFILES:.o=.d)
$(OUTPUT).3dsx	:	$(OUTPUT).elf
$(OUTPUT).elf	:	$(OFILES)
endif
