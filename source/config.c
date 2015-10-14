#include "config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
	configuration *pconfig = (configuration *) user;
	if (MATCH(pconfig->key, "path")) {
		pconfig->path = strdup(value);
	} else if (MATCH(pconfig->key, "delay")) {
		pconfig->delay = atoll(value);
	} else {
		return 0;
	}
	return 1;
}
