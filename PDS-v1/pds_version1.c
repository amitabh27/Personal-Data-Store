#include "pds.h"
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PdsInfo pdsInfo;

int pds_create( char *repo_name )
{

FILE *fp=fopen(repo_name,"w");

if(fp!=NULL)
{


pdsInfo.repo_fptr=fp;
strcpy(pdsInfo.repo_name,repo_name);
pdsInfo.repo_status=PDS_OPEN;

//printf("File has been created\n");
pds_close();
return PDS_SUCCESS;
}
else
return PDS_CREATE_ERROR;



}


int pds_open( char *repo_name)
{

FILE *fp;

if(pdsInfo.repo_status == PDS_OPEN)
return PDS_REPO_ALREADYOPEN;
else
{
	
	fp=fopen(repo_name,"r+");
	if(fp==NULL)
	return PDS_OPEN_ERROR;
	else
	{
	pdsInfo.repo_fptr=fp;
	strcpy(pdsInfo.repo_name,repo_name);
	pdsInfo.repo_status=PDS_OPEN;
	//printf("In PDS_OPEN --->%s\n",pdsInfo.repo_name);
	return PDS_SUCCESS;
	}
}

}




int pds_store( struct Contact *c )
{

if(pdsInfo.repo_status == PDS_CLOSED)
return PDS_REPO_NOTOPEN;
else
{

	fseek(pdsInfo.repo_fptr,0,SEEK_END);

	if(0==fwrite(c,sizeof(struct Contact),1,pdsInfo.repo_fptr))
	return PDS_STORE_ERROR;
	else
	{
	//printf("In PDS_STORE ....");
	return PDS_SUCCESS;
	}


}

}




int pds_search_by_key( int key, struct Contact *c )
{

if(pdsInfo.repo_status == PDS_CLOSED)
return PDS_REPO_NOTOPEN;
else
{		
		fseek(pdsInfo.repo_fptr,0,SEEK_SET);
		while(fread(c,sizeof(struct Contact),1,pdsInfo.repo_fptr))
		{
		//printf("***------>%d\n",c->contact_id);
			if(c->contact_id==key)
			{
			return PDS_SUCCESS;
			}
		}	
		return PDS_REC_NOT_FOUND;
	

}

}


int pds_close()
{

pdsInfo.repo_status=PDS_CLOSED;
fclose(pdsInfo.repo_fptr);

}
