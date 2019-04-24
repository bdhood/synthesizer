#ifndef __RESOURCE_H
#define __RESOURCE_H

typedef struct RESOURCE_TBL {
	char *hostfile;
} RESOURCE_TBL;

RESOURCE_TBL *load_resources();

#endif