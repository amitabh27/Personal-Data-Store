// pds.h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PDS_H
#define PDS_H

#include "contact.h"

// Status codes
#define PDS_OPEN 1
#define PDS_CLOSED 0

// Error codes
#define PDS_SUCCESS 0
#define PDS_CREATE_ERROR 10
#define PDS_OPEN_ERROR 11
#define PDS_REPO_NOTOPEN 12
#define PDS_REPO_ALREADYOPEN 13
#define PDS_STORE_ERROR 14
#define PDS_REC_NOT_FOUND 15
#define PDS_DUPLICATE_RECORD 16

struct PdsInfo{
	FILE *repo_fptr;
	char repo_name[MAX_NAME_LEN];
	int repo_status; // 0 - CLOSED; 1 - OPEN 
};

struct PdsNdxInfo{
	int key;
	int offset;
};

extern struct PdsInfo pdsInfo;

int pds_create( char *repo_name );
int pds_open( char *repo_name );
int pds_store( struct Contact *c );
int pds_search_by_key( int key, struct Contact *c );
int pds_close();

// Indexing functions
int pds_create_index( char *repo_name, char *ndx_name );

#endif
