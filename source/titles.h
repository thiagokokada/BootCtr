#ifndef TITLES_H
#define TITLES_H

#include <3ds.h>

typedef struct
{
	u8 mediatype;
	u64 title_id;
}titleInfo_s;

typedef bool (*titleFilter_callback)(u64 tid);

#endif
