// Contact.h

#ifndef CONTACT_H
#define CONTACT_H



#define MAX_NAME_LEN 50
#define MAX_EMAIL_LEN 50
#define MAX_PHONE_LEN 11

struct Contact{
	int contact_id;
	char cname[MAX_NAME_LEN];
	char mphone[MAX_PHONE_LEN];
	char email[MAX_EMAIL_LEN];
};

#endif
