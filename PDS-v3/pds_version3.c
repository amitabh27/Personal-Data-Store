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
	FILE *ndx=fopen(ndx_name,"r");

	if(fp!=NULL && ndx!=NULL)
	{

		pdsInfo.repo_fptr=fp;
		pdsInfo.ndx_fptr=ndx;
		pdsInfo.num_ndx=0;
		strcpy(pdsInfo.repo_name,repo_name);
		pdsInfo.repo_status=PDS_OPEN;

		//printf("File has been created\n");
		pds_close();
		return PDS_SUCCESS;
	}
	else
	return PDS_CREATE_ERROR;}


int pds_open( char *repo_name, char *ndx_name )
{
	FILE *fp,*ndx;
	root=NULL;
	if(pdsInfo.repo_status == PDS_OPEN)
	return PDS_REPO_ALREADYOPEN;
	else
	{
	
	fp=fopen(repo_name,"r+");
	ndx=fopen(ndx_name,"r");
	if(fp==NULL || ndx==NULL)
	return PDS_OPEN_ERROR;
	else
	{

	//File Pointers Initialization
	//printf("****************************File has been opened....**********************************\n");
	pdsInfo.repo_fptr=fp;
	pdsInfo.ndx_fptr=ndx;	
	pdsInfo.repo_status=PDS_OPEN;
	//BST Creation
		//printf("BST creation from file....\n");
		struct PdsNdxInfo temp[pdsInfo.num_ndx];
		fseek(pdsInfo.ndx_fptr,0,SEEK_SET);
		fread(temp,sizeof(struct PdsNdxInfo),pdsInfo.num_ndx,pdsInfo.ndx_fptr);
		
		int i=0;
		//printf("Number of initial records to be read....%d\n",pdsInfo.num_ndx);
			while(i < pdsInfo.num_ndx)
			{

		
			if(temp[i].is_deleted==0){
				pdsInfo.ndxEntries[i].key=temp[i].key;
				pdsInfo.ndxEntries[i].offset=temp[i].offset;
				pdsInfo.ndxEntries[i].is_deleted=temp[i].is_deleted;
				//printf("BST Insertion node%d   -----   %d  %d %u\n",i,temp[i].key,temp[i].offset,temp[i].is_deleted);
				if(root==NULL )
				{
				root =insert(root,temp[i].key,temp[i].offset,temp[i].is_deleted);
				}
				else
				{
				insert(root,temp[i].key,temp[i].offset,temp[i].is_deleted);
				}	
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
		struct node *temp =  (struct node *)malloc(sizeof(struct node));
		temp=search(root,c->contact_id);
		if(temp==NULL || temp->is_deleted==1)
		{
			fseek(pdsInfo.repo_fptr,0,SEEK_END);

			if(pdsInfo.num_ndx < MAX_RECS)
			{
				if(0==fwrite(c,sizeof(struct Contact),1,pdsInfo.repo_fptr))
				{
				//printf("Record writing in pds_repo failed....\n");	
				return PDS_STORE_ERROR;
				}

				//printf("Number of record is less than maximum size of repo allowed ....\n");
				key=c->contact_id;
				offset=(sizeof(struct Contact))*pdsInfo.num_ndx;
				//printf("KEY =%d		OFFSET =%d \n",key,offset);		

				pdsInfo.ndxEntries[pdsInfo.num_ndx].key=key;
				pdsInfo.ndxEntries[pdsInfo.num_ndx].offset=offset;
				pdsInfo.ndxEntries[pdsInfo.num_ndx].is_deleted=0;
				pdsInfo.num_ndx++;
		
				if(root==NULL)
				{
				root = insert(root,pdsInfo.ndxEntries[pdsInfo.num_ndx-1].key,pdsInfo.ndxEntries[pdsInfo.num_ndx-1].offset,pdsInfo.ndxEntries[pdsInfo.num_ndx].is_deleted);
				//printf("Its first node in the tree...");
				}
				else
				{
				//printf("Its just a secondary node in the tree...");
				insert(root,pdsInfo.ndxEntries[pdsInfo.num_ndx-1].key,pdsInfo.ndxEntries[pdsInfo.num_ndx-1].offset,pdsInfo.ndxEntries[pdsInfo.num_ndx].is_deleted);
				}
				//printf("Returning success from pds_store......\n");
				return PDS_SUCCESS;

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
		struct node *temp =  (struct node *)malloc(sizeof(struct node));
		temp=search(root,key);
		if(temp==NULL || temp->is_deleted==1)
		{
		return PDS_REC_NOT_FOUND;
		}
		else
		{
		fseek(pdsInfo.repo_fptr,temp->offset,SEEK_SET);
		fread(c,sizeof(struct Contact),1,pdsInfo.repo_fptr);
		return PDS_SUCCESS;
		}
	

}

}


int pds_close()
{

	fclose(pdsInfo.ndx_fptr);
	FILE *fp=fopen("pds_demo.ndx","w");
	fwrite(pdsInfo.ndxEntries,sizeof(struct PdsNdxInfo),pdsInfo.num_ndx,fp);
	pdsInfo.repo_status=PDS_CLOSED;
	fclose(pdsInfo.repo_fptr);
	fclose(fp);
}

int pds_delete_by_key( int key )
{

	int i=0;
	if(pdsInfo.repo_status == PDS_CLOSED)
	return PDS_REPO_NOTOPEN;
	else
	{
		struct node *temp =  (struct node *)malloc(sizeof(struct node));
		temp=search(root,key);
		if(temp==NULL || temp->is_deleted==1)
		{
		return PDS_REC_NOT_FOUND;
		}
		else
		{
		temp->is_deleted=1;
			for(i=0;i<pdsInfo.num_ndx;i++)
			{
			//printf("%d %d %d\n",pdsInfo.ndxEntries[i].key,pdsInfo.ndxEntries[i].offset,pdsInfo.ndxEntries[i].is_deleted);
			if(pdsInfo.ndxEntries[i].key==temp->key && pdsInfo.ndxEntries[i].is_deleted==0)
			{
				pdsInfo.ndxEntries[i].is_deleted=1;
				//printf("flag updated for deletion*************\n");
				Print(root);
				return PDS_SUCCESS;	
			}
			}
			
			
		}


	

	}

}



void Print(struct node *root)
{
   if ( NULL == root )
      return;
     Print(root->left);
     //printf("Node=%d %d %d\n", root->key,root->offset,root->is_deleted );
     Print(root->right);
}


int pds_num_recs( int *num_recs )
{
	return 0;
}

struct node *newNode(int k,int o,unsigned char a)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->key = k;temp->offset = o;temp->is_deleted=a;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* insert(struct node* root, int k,int o,unsigned char a)
{
    if (root == NULL) return newNode(k,o,a);
    if (k < root->key)
        root->left  = insert(root->left, k,o,a);
    else if (k >=root->key)
        root->right = insert(root->right, k,o,a);   

    return root;
}

struct node* search(struct node* root, int k)
{
    if(root==NULL)
    {
    return NULL;
    }
    if (root->key == k)
       {
	       if(root->is_deleted==0)
	       {
       	       return root;
		}
	       else
	       return search(root->right, k);
	}
    if (root->key <=k)
       return search(root->right, k);
    return search(root->left, k);


}
