#include "config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
	configuration *pconfig = (configuration *) user;
	if (MATCH(KEY_PRESSED, "path")) {
		pconfig->path = strdup(value);
	} else if (MATCH(KEY_PRESSED, "delay")) {
		pconfig->delay = atoi(value);
	} else {
		return 0;
	}
	return 1;
}
