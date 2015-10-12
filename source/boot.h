#pragma once
#include <3ds.h>

typedef struct
{
	u8 mediatype;
	u64 title_id;
}titleInfo_s;

extern int targetProcessId;
extern titleInfo_s target_title;

bool isNinjhax2(void);
int bootApp(char* executablePath);
