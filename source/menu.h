#pragma once

//#include "gfx.h"
#include "scanner.h"
#include "descriptor.h"


#define ENTRY_PATHLENGTH (128)
#define ENTRY_NAMELENGTH (64)
#define ENTRY_DESCLENGTH (128)
#define ENTRY_AUTHORLENGTH (64)
#define ENTRY_ICON_WIDTH (48)
#define ENTRY_ICON_HEIGHT (48)
#define ENTRY_ICONSIZE (ENTRY_ICON_WIDTH*ENTRY_ICON_HEIGHT*4)
#define ENTRY_WIDTH (66)
#define ENTRY_WIDTH_SELECTED (72)
#define ENTRY_FWIDTH (63)
#define ENTRY_FWIDTH_SELECTED (63)
#define ENTRY_HEIGHT (294)
#define ENTRY_HEIGHT_SELECTED (298)

#define ENTRY_BGCOLOR (u8[]){246, 252, 255}
#define ENTRY_BGCOLOR_SHADOW (u8[]){34, 153, 183}

#define fptToInt(v) ((v)>>10)
#define intToFpt(v) ((v)<<10)

typedef struct menuEntry_s
{
	char executablePath[ENTRY_PATHLENGTH+1];
	char name[ENTRY_NAMELENGTH+1];
	char description[ENTRY_DESCLENGTH+1];
	char author[ENTRY_AUTHORLENGTH+1];
	u8 iconData[ENTRY_ICONSIZE];
	descriptor_s descriptor;
	struct menuEntry_s* next;
    int page;
    int row;
    int col;
    int iconX;
    int iconY;
    int iconW;
    int iconH;
    bool hidden;
    bool isRegionFreeEntry;
    bool isTitleEntry;
    u64 title_id;
}menuEntry_s;

typedef struct
{
	menuEntry_s* entries;
	u16 numEntries;
	u16 selectedEntry;
	s32 scrollTarget; //10 bit fixed point
	s32 scrollLocation; //10 bit fixed point
	s32 scrollVelocity; //10 bit fixed point
	s32 scrollBarSize;
	s32 currentScrollBarSize;
	s32 scrollBarPos;
	touchPosition previousTouch, firstTouch;
	u16 touchTimer;
	bool atEquilibrium;
}menu_s;

void initEmptyMenuEntry(menuEntry_s* me);
void initMenuEntry(menuEntry_s* me, char* execPath, char* name, char* description, char* author, u8* iconData);
void scanMenuEntry(menuEntry_s* me);
