#include "config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
	configuration *pconfig = (configuration *) user;
	// unless user forces this setting, it should always default to AUTO
	pconfig->payload = -1;
	if (MATCH(pconfig->key, "path")) {
		pconfig->path = strdup(value);
	} else if (MATCH(pconfig->key, "delay")) {
		pconfig->delay = strtoull(value, NULL, 10);
	} else if (MATCH(pconfig->key, "payload")) {
		pconfig->payload = strtol(value, NULL, 10);
	} else if (MATCH(pconfig->key, "offset")) {
		pconfig->offset = strtoul(value, NULL, 0);
	} else if (MATCH(pconfig->key, "cfw_fix")) {
		pconfig->cfw_fix = strtol(value, NULL, 10) ? true : false;
	} else if (MATCH(pconfig->key, "debug")) {
		pconfig->debug = strtol(value, NULL, 10) ? true : false;
	}
	return 1;
}
