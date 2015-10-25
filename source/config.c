#include "config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
	configuration *pconfig = (configuration *) user;
	if (MATCH(pconfig->key, "path")) {
		pconfig->path = strdup(value);
	} else if (MATCH(pconfig->key, "delay")) {
		// convert to ms here, since ns is too fast to be useful
		pconfig->delay = strtoull(value, NULL, 10) * MS_TO_NS;
	} else if (MATCH(pconfig->key, "payload")) {
		pconfig->payload = strtoul(value, NULL, 10);
	} else if (MATCH(pconfig->key, "offset")) {
		pconfig->offset = strtoul(value, NULL, 0);
	}
	return 1;
}
