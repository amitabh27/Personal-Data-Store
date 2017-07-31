# Personal-Data-Store

Repository for storing,deleting and searching personal contacts.

The C codes have been implemented using following constructs:
=============================================================================================================================

1. .dat files are used for stroring the contact details
2. .ndx files are used for stroring the KEY-Address(or Offset locations) of contacts in .dat file
3. Binary Search Tree has been used for maintaining the contacts for current session of interaction with the data store 
    while the files serve as the offline storage once the session ends.
    
VERSIONS
===========================================================================================================================

V1 - This version implements basic store and search operations of a repository using files.
V2 - This version introduces an efficient data structure BST to handle the above operations.
V3 - This version introduces additional features like deletion,avoiding duplicate insertion etc.




