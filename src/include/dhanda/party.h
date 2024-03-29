#ifndef _DHANDA_PARTY_H_
#define _DHANDA_PARTY_H_

#include <dhanda/dhanda.h>

typedef struct party {
	int id;
	char fname[128];
	char lname[128];
	char phone[12];
	int amount;
	time_t cat;
	time_t uat;
} party;

typedef struct party_filter {
	int page;
	int items;
	char *query;
	int has_query;
	int type;
	int is_found;
} party_filter;


int party_init(dhanda *app);
int party_add(dhanda *app, party *party);
int party_update(dhanda *app, party *old_party, struct party *new_party);
int party_delete(dhanda *app, party *party);
int party_findbyid(dhanda *app, int id, party *result);
int party_search(dhanda *app, char *query, struct list *result);
int party_get(dhanda *app, party_filter filter, struct list *result);


/* Handy functions for passing data from Data layer to UI layer, since
 * UI functions only takes one argument and are called using callback
 * there is no way to call a callback with different type/number of
 * argument at run time. */
void party_insert_in_list(dhanda *app, party *p);
party *party_first_in_list(dhanda *app);
party *party_second_in_list(dhanda *app);

#endif
