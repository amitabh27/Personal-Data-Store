// pds_test.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "contact.h"

void test_search();
void test_store();
void test_create();

void printContact(struct Contact *c );

main()
{
	char *repo_name = "pds_demo.dat";
	char *ndx_name = "pds_demo.ndx";
	int status;

	status = pds_open( repo_name, ndx_name );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_open failed: %d\n", status);
		exit(1);
	}
	test_store();
	test_search();

	// Close and reopen to check if the index got saved properly
	pds_close();
	status = pds_open( repo_name, ndx_name );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_open failed: %d\n", status);
		exit(1);
	}
	test_search();

	pds_close();
	printf("Program terminated successfully\n");
}

void test_store()
{
	int status, key;
	struct Contact c1, c2;
	
	// Testing pds_store
	c1.contact_id = 101;
	strcpy(c1.cname, "Contact #1");
	strcpy(c1.mphone, "Phone #1");
	strcpy(c1.email, "Email #1");

	c2.contact_id = 102;
	strcpy(c2.cname, "Contact #2");
	strcpy(c2.mphone, "Phone #2");
	strcpy(c2.email, "Email #2");

	status = pds_store( &c1 );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_store failed for key %d: errorcode %d\n", c1.contact_id, status);
	}
	status = pds_store( &c2 );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_store failed for key %d: errorcode %d\n", c2.contact_id, status);
	}
	status = pds_store( &c2 );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_store failed for key %d: errorcode %d\n", c2.contact_id, status);
	}

}

void test_search()
{
	int status;
	struct Contact c3;
	int key;

	key = 102;
	status = pds_search_by_key(key, &c3);
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_search_by_key failed for key %d: errorcode %d\n", key, status);
	}
	else{
		printf("Test success ......key found in search\n");
		printContact(&c3);
	}

	key = 101;
	status = pds_search_by_key(key, &c3);
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_search_by_key failed for key %d: errorcode %d\n", key, status);
	}
	else{
		printf("Test success ......key found in search\n");
		printContact(&c3);
	}
	key = 1020000;
	status = pds_search_by_key(key, &c3);
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_search_by_key failed for key %d: errorcode %d\n", key, status);
	}
	else{
		printContact(&c3);
	}

}

void printContact(struct Contact *c )
{
	printf("%d;%s;%s;%s\n", c->contact_id, c->cname, c->mphone, c->email);
}

