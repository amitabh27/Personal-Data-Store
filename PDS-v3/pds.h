// pds.h

#ifndef PDS_H
#define PDS_H

#include "contact.h"

// Limits
#define MAX_RECS 1000

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
#define PDS_REPOSITORY_FULL 17

struct PdsNdxInfo{
	int key;
	int offset;
	unsigned char is_deleted;
};

struct node
{
    int key;int offset;unsigned char is_deleted;
    struct node *left, *right;
};
struct node* search(struct node* root, int k);
struct node* insert(struct node* root, int k,int o,unsigned char a);
struct node *newNode(int k,int o,unsigned char a);
void Print(struct node *root);

struct PdsInfo{
	FILE *repo_fptr;
	FILE *ndx_fptr;
	char repo_name[MAX_NAME_LEN];
	int repo_status; // 0 - CLOSED; 1 - OPEN 
	int num_ndx;
	struct PdsNdxInfo ndxEntries[MAX_RECS];
};


extern struct PdsInfo pdsInfo;

int pds_create_index( char *repo_name, char *ndx_name );
int pds_open( char *repo_name, char *ndx_name );
int pds_store( struct Contact *c );
int pds_search_by_key( int key, struct Contact *c );
int pds_delete_by_key( int key );
int pds_close();
int pds_num_recs( int *num_recs );

// Indexing functions
int pds_create_index( char *repo_name, char *ndx_name );

#endif
