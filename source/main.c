#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <3ds.h>
#include <sys/stat.h>

#include "menu.h"
#include "filesystem.h"
#include "boot.h"
#include "scanner.h"

#include <string.h>

u8 sdmcCurrent = 0;
u64 nextSdCheck = 0;

u32 wifiStatus = 0;
u8 batteryLevel = 5;
u8 charging = 0;
int rebootCounter;
titleBrowser_s titleBrowser;

// handled in main
// doing it in main is preferred because execution ends in launching another 3dsx
void __appInit()
{
}

// same
void __appExit()
{
}


int main()
{
    srvInit();
    aptInit();
    initFilesystem();
    openSDArchive();
    hidInit(NULL);
    irrstInit(NULL);
    acInit();
    ptmInit();
    titlesInit();
    
    
    // offset potential issues caused by homebrew that just ran
    aptOpenSession();
    APT_SetAppCpuTimeLimit(NULL, 0);
    aptCloseSession();
    
    FSUSER_IsSdmcDetected(NULL, &sdmcCurrent);
    
    nextSdCheck = osGetTime()+250;
    srand(svcGetSystemTick());
    rebootCounter=257;
    
    
    
    char * execPath;
    
    hidScanInput();
    
    if (hidKeysDown()&KEY_R || hidKeysHeld()&KEY_R) {
        execPath = "/emergency.3dsx";
    }
    else {
        execPath = "/boot1.3dsx";
    }
    
    
    int i, l=-1; for(i=0; execPath[i]; i++) if(execPath[i]=='/')l=i;
    static menuEntry_s me;
    initMenuEntry(&me, execPath, &execPath[l+1], execPath, "", NULL);
    
    scanMenuEntry(&me);
    
    // cleanup whatever we have to cleanup
    titlesExit();
    ptmExit();
    acExit();
    irrstExit();
    hidExit();
    exitFilesystem();
    closeSDArchive();
    aptExit();
    srvExit();
    
    
    return bootApp(me.executablePath, &me.descriptor.executableMetadata);
}
