// pds_test.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "contact.h"

void test_create();

main()
{
	test_create();
	printf("Program terminated successfully\n");
}

void test_create()
{
	char *repo_name = "pds_demo.dat";
	char *ndx_name = "pds_demo.ndx";
	int status;

	status = pds_create_index( repo_name, ndx_name );
	if( status != PDS_SUCCESS ){
		fprintf(stderr, "pds_create failed: %d\n", status);
		exit(1);
	}
}

