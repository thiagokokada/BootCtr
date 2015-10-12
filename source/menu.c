#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <math.h>

#include "filesystem.h"
#include "menu.h"
#include "smdh.h"

void initEmptyMenuEntry(menuEntry_s* me)
{
	if(!me)return;

	me->name[0]=0x00;
	me->description[0]=0x00;
	me->executablePath[0]=0x00;

	initDescriptor(&me->descriptor);

	me->next=NULL;
}

void initMenuEntry(menuEntry_s* me, char* execPath, char* name, char* description, char* author, u8* iconData)
{
	if(!me)return;

	initEmptyMenuEntry(me);

	strncpy(me->executablePath, execPath, ENTRY_PATHLENGTH);
	strncpy(me->name, name, ENTRY_NAMELENGTH);
	strncpy(me->description, description, ENTRY_DESCLENGTH);
	strncpy(me->author, author, ENTRY_AUTHORLENGTH);
	if(iconData)memcpy(me->iconData, iconData, ENTRY_ICONSIZE);
    
	initDescriptor(&me->descriptor);
    
    me->title_id = 0;
}