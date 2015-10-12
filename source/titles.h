#ifndef TITLES_H
#define TITLES_H

#include <3ds.h>

#include "smdh.h"
#include "menu.h"

typedef struct
{
	u8 mediatype;
	u64 title_id;
	smdh_s* icon;
}titleInfo_s;

typedef bool (*titleFilter_callback)(u64 tid);

typedef struct
{
	u8 mediatype;
	u32 num;
	titleInfo_s* titles;
	titleFilter_callback filter;
}titleList_s;

typedef struct
{
	titleList_s lists[3];
	u32 total;
	u64 nextCheck;
	int selectedId;
	titleInfo_s* selected;
	menuEntry_s selectedEntry;
}titleBrowser_s;

void titlesInit();
void titlesExit();

#endif
