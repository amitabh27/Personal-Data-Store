// pds_test.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pds.h"
#include "contact.h"

void test_search();
void test_store();
void test_create();
void test_delete();

void print_contact(struct Contact *c );

int main(int argc, char *argv[])
{
	char repo_name[50];
	char ndx_name[50];
	char textline[50];
	char cmd[10], strval1[30], strval2[30], strval3[30];
	int intval1, intavl2, intval3;
	int status;
	FILE *cmdfptr;

	if( argc != 2 ){
		fprintf(stderr, "Usage: %s command-file\n", argv[0]);
		exit(1);
	}

	if( (cmdfptr = (FILE *) fopen(argv[1], "r")) == NULL ){
		fprintf(stderr, "Unable to open command file %s\n", argv[1]);
		exit(1);
	}

	while( fgets(textline, sizeof(textline), cmdfptr) != NULL ){
		if( strcmp(textline, "") == 0 )
			// Ignore empty lines
			continue;
		// Extract the command string
		sscanf(textline, "%s", cmd);
		if( strcmp(cmd, "CREATE") == 0 ){
			// Read the needed arguments
			sscanf(textline, "%s%s", cmd, strval1);
			strcpy(strval2, strval1);
			strcat(strval2, ".dat");
			strcpy(strval3, strval1);
			strcat(strval3, ".ndx");
			printf("Running %s", textline);
			status = pds_create_index( strval2, strval3 );
			if( status != PDS_SUCCESS ){
				printf("pds_create_index failed: %d\n", status);
				continue;
			}
			else
				printf("Command executed successfully\n");
		}
		else if( strcmp(cmd, "OPEN") == 0 ){
			// Read the needed arguments
			sscanf(textline, "%s%s", cmd, strval1);
			strcpy(strval2, strval1);
			strcat(strval2, ".dat");
			strcpy(strval3, strval1);
			strcat(strval3, ".ndx");
			printf("Running %s", textline);
			status = pds_open( strval2, strval3 );
			if( status != PDS_SUCCESS ){
				printf("pds_create_index failed: %d\n", status);
				continue;
			}
			else
				printf("Command executed successfully\n");
		}
		else if( strcmp(cmd, "STORE") == 0 ){
			struct Contact c;
			// Read the needed arguments
			sscanf(textline, "%s%d", cmd, &intval1);
			sprintf(strval1, "cname#%d", intval1);
			sprintf(strval2, "mphone#%d", intval1);
			sprintf(strval3, "email#%d", intval1);
			
			c.contact_id = intval1;
			strcpy(c.cname, strval1);
			strcpy(c.mphone, strval2);
			strcpy(c.email, strval3);

			printf("Running %s", textline);
			status = pds_store( &c );
			if( status != PDS_SUCCESS ){
				printf("pds_store failed for key %d: errorcode %d\n", c.contact_id, status);
			}
			else
				printf("Command executed successfully\n");
		}
		else if( strcmp(cmd, "SEARCH") == 0 ){
			struct Contact c;
			// Read the needed arguments
			sscanf(textline, "%s%d", cmd, &intval1);

			printf("Running %s", textline);
			status = pds_search_by_key( intval1, &c );
			if( status != PDS_SUCCESS ){
				printf("pds_search_by_key failed for key %d: errorcode %d\n", intval1, status);
			}
			else{
				print_contact(&c);
				printf("Command executed successfully\n");
			}
		}
		else if( strcmp(cmd, "DELETE") == 0 ){
			struct Contact c;
			// Read the needed arguments
			sscanf(textline, "%s%d", cmd, &intval1);

			printf("Running %s", textline);
			status = pds_delete_by_key( intval1 );
			if( status != PDS_SUCCESS ){
				printf("pds_delete_by_key failed for key %d: errorcode %d\n", c.contact_id, status);
			}
			else{
				printf("Command executed successfully\n");
			}
		}
		/*else if( strcmp(cmd, "NUMRECS") == 0 ){
			printf("Running %s", textline);
			status = pds_num_recs( &intval1 );
			if( status != PDS_SUCCESS ){		
				printf("pds_num_recs failed: errorcode %d\n", status);
			}
			else{
				printf("NUMRECS = %d\n", intval1);
				printf("Command executed successfully\n");
			}
		}*/
		else if( strcmp(cmd, "CLOSE") == 0 ){
			printf("Running %s", textline);
			status = pds_close();
			if( status != PDS_SUCCESS ){
				printf("pds_close failed: errorcode %d\n", status);
			}
			else{
				printf("Command executed successfully\n");
			}
		}
		printf("=====================\n");
	}
	printf("Testing terminated successfully\n");
}

void print_contact(struct Contact *c )
{
	printf("%d;%s;%s;%s\n", c->contact_id, c->cname, c->mphone, c->email);
}

