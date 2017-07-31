#include "pds.h"
#include "contact.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PdsInfo pdsInfo;
struct node *root;

int pds_create_index( char *repo_name, char *ndx_name )
{

FILE *fp=fopen(repo_name,"w");
FILE *ndx=fopen(ndx_name,"w");

if(fp!=NULL && ndx!=NULL)
{


pdsInfo.repo_fptr=fp;
pdsInfo.ndx_fptr=ndx;
pdsInfo.num_recs=0;
strcpy(pdsInfo.repo_name,repo_name);
pdsInfo.repo_status=PDS_OPEN;

////printf("File has been created\n");
pds_close();
return PDS_SUCCESS;
}
else
return PDS_CREATE_ERROR;}


int pds_open( char *repo_name, char *ndx_name )
{
FILE *fp,*ndx;

if(pdsInfo.repo_status == PDS_OPEN)
return PDS_REPO_ALREADYOPEN;
else
{
	
	fp=fopen(repo_name,"r+");
	ndx=fopen(ndx_name,"r+");
	if(fp==NULL || ndx==NULL)
	return PDS_OPEN_ERROR;
	else
	{

	//File Pointers Initialization
	//printf("File has been opened....\n");
	pdsInfo.repo_fptr=fp;
	pdsInfo.ndx_fptr=ndx;	
	pdsInfo.repo_status=PDS_OPEN;

	//BST Creation
		//printf("BST creation from file....");
		struct PdsNdxInfo temp[pdsInfo.num_recs];
		fseek(pdsInfo.ndx_fptr,0,SEEK_SET);
		fread(temp,sizeof(struct PdsNdxInfo),pdsInfo.num_recs,pdsInfo.ndx_fptr);
		
		
		int i=0;
		//printf("Number of initial records to be read....%d",pdsInfo.num_recs);
		while(i < pdsInfo.num_recs)
		{

		//printf("BST Insertion node%d   -----   %d  %d",i,temp[i].key,temp[i].offset);
			if(root==NULL)
			{
			root =insert(root,temp[i].key,temp[i].offset);
			}
			else
			{
			insert(root,temp[i].key,temp[i].offset);
			}
		i++;	
		}

	//printf("returning success.....\n");
	return PDS_SUCCESS;
	}
}

}




int pds_store( struct Contact *c )
{

int key=-1;
int offset=-1;

if(pdsInfo.repo_status == PDS_CLOSED)
{
//printf("REPO CLOSED.....\n");
return PDS_REPO_NOTOPEN;
}
else
{

	int t=search(root,c->contact_id);
	if(t==-1)
	{
		fseek(pdsInfo.repo_fptr,0,SEEK_END);
		fseek(pdsInfo.ndx_fptr,0,SEEK_END);


		if(0==fwrite(c,sizeof(struct Contact),1,pdsInfo.repo_fptr))
		{
		//printf("Record writing in pds_repo failed....\n");	
		return PDS_STORE_ERROR;
		}
		else if(pdsInfo.num_recs < MAX_RECS)
		{
		
		//printf("Number of record is less than maximum size of repo allowed ....\n");
		key=c->contact_id;
		offset=(sizeof(struct Contact))*pdsInfo.num_recs;
		//printf("KEY =%d		OFFSET =%d \n",key,offset);		

		pdsInfo.ndxEntries[pdsInfo.num_recs].key=key;
		pdsInfo.ndxEntries[pdsInfo.num_recs].offset=offset;
		pdsInfo.num_recs++;
		
		
			if(0==fwrite(&pdsInfo.ndxEntries[pdsInfo.num_recs-1],sizeof(struct PdsNdxInfo),1,pdsInfo.ndx_fptr))
			{
			//printf("Record writing in ndx_repo failed....\n");
			return PDS_STORE_ERROR;
			}
			else
			{
				if(root==NULL)
				{
				root = insert(root,pdsInfo.ndxEntries[pdsInfo.num_recs-1].key,pdsInfo.ndxEntries[pdsInfo.num_recs-1].offset);
				//printf("Its first node in the tree...");
				}
				else
				{
				//printf("Its just a secondary node in the tree...");
				insert(root,pdsInfo.ndxEntries[pdsInfo.num_recs-1].key,pdsInfo.ndxEntries[pdsInfo.num_recs-1].offset);
				}
			//printf("Returning success from pds_store......\n");
			return PDS_SUCCESS;
			}
		}
		else
		return PDS_REPOSITORY_FULL;
	}
	else
	{
	//printf("Duplicate record insertion detected....REJECTION\n");
	return PDS_DUPLICATE_RECORD;
	}

}

}




int pds_search_by_key( int key, struct Contact *c )
{

if(pdsInfo.repo_status == PDS_CLOSED)
return PDS_REPO_NOTOPEN;
else
{
		
		int t=search(root,key);
		if(t==-1)
		return PDS_REC_NOT_FOUND;
		else
		{
		fseek(pdsInfo.repo_fptr,t,SEEK_SET);
		fread(c,sizeof(struct Contact),1,pdsInfo.repo_fptr);
		return PDS_SUCCESS;
		}
	

}

}


int pds_close()
{
pdsInfo.repo_status=PDS_CLOSED;
fclose(pdsInfo.repo_fptr);
fclose(pdsInfo.ndx_fptr);
}



struct node *newNode(int k,int o)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->key = k;temp->offset = o;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* insert(struct node* root, int k,int o)
{
    if (root == NULL) return newNode(k,o);
    if (k < root->key)
        root->left  = insert(root->left, k,o);
    else if (k > root->key)
        root->right = insert(root->right, k,o);   

    return root;
}

int search(struct node* root, int k)
{

    if(root==NULL)
    return -1;
    if (root->key == k)
       return root->offset;
    if (root->key < k)
       return search(root->right, k);
    return search(root->left, k);


}
