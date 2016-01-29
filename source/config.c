#include "config.h"

int handler(void *user, const char *section, const char *name, const char *value)
{
	configuration *pconfig = (configuration *) user;
	if (MATCH(pconfig->section, "path")) {
		pconfig->path = strdup(value);
	} else if (MATCH(pconfig->section, "delay")) {
		pconfig->delay = strtoull(value, NULL, 10);
	} else if (MATCH(pconfig->section, "payload")) {
		pconfig->payload = strtol(value, NULL, 10);
	} else if (MATCH(pconfig->section, "offset")) {
		pconfig->offset = strtoul(value, NULL, 0);
	} else if (MATCH(pconfig->section, "splash")) {
		pconfig->splash = strtol(value, NULL, 10);
	} else if (MATCH(pconfig->section, "splash_image")) {
		pconfig->splash_image = strdup(value);
	}
	return 1;
}
