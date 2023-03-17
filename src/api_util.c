#ifndef TERMINAL_APP

#include <dhanda/party.h>
#include <dhanda/txn.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/seccomp.h>
#include "dhanda/api_util.h"



struct kore_json_item * 
party_struct_to_korejson(struct party *p)
{
	struct kore_json_item *result, *item;


	result = kore_json_create_object(NULL, NULL);

	item = kore_json_create_integer(result, "id", p->id);
	item = kore_json_create_string(result, "first_name", p->fname);
	item = kore_json_create_string(result, "last_name", p->lname);
	item = kore_json_create_string(result, "phone", p->phone);
	item = kore_json_create_integer(result, "amount", p->amount);
	item = kore_json_create_integer_u64(result, "created_at", p->cat);
	item = kore_json_create_integer_u64(result, "updated_at", p->uat);

	return result;
}

struct kore_json_item *
txn_struct_to_korejson(struct txn *t)
{
	struct kore_json_item *result, *item;

	result = kore_json_create_object(NULL, NULL);

	item = kore_json_create_integer(result, "id", t->id);
	item = kore_json_create_integer(result, "amount", t->amount);
	item = kore_json_create_integer(result, "type", t->type);
	item = kore_json_create_string(result, "desc", t->desc);
	item = kore_json_create_integer_u64(result, "created_at", t->cat);
	item = kore_json_create_integer(result, "party_id", t->party_id);

	return result;
}

struct kore_json_item * 
party_list_to_korejson(struct list *parties)
{
	Node *ptr;
	struct party *party_ptr;
	struct kore_json_item *root, *item, *array;
 
    root = kore_json_create_object(NULL, NULL);

 	array = kore_json_create_array(root, "parties");	

	ptr = parties->head;

	while (ptr) {
		party_ptr = (struct party *) ptr->data;
		item = party_struct_to_korejson(party_ptr);
		kore_json_item_attach(array, item);

		ptr = ptr->next;
	}
	return root;
}

struct kore_json_item * 
txn_list_to_korejson(struct list *transaction)
{
	Node *ptr;
	struct txn *txn_ptr;
	struct kore_json_item *root, *item, *array;
 
 	root = kore_json_create_object(NULL, NULL);

 	array = kore_json_create_array(root, "transactions");	

	ptr = transaction->head;

	while (ptr) {
		txn_ptr = (struct txn *) ptr->data;
		item = txn_struct_to_korejson(txn_ptr);
		kore_json_item_attach(array, item);

		ptr = ptr->next;
	}
	return root;
}


int kore_apputil_extract_route_ids(const char *path, ...)
{
	va_list arg;
	size_t *id;
	int ret, err;
	char *parts[16];
	char *route = kore_strdup(path);
	char *part;

	ret = 0;
	va_start(arg, path);
	kore_split_string(route, "/", parts, sizeof(parts) / sizeof(parts[0]));
	for (int i = 0; parts[i] != NULL; ++i) {
		part = parts[i];
		while (*part) {
			if (!isdigit(*part)) break;
			++part;
		}
		/*If part has left something then it's not a number, skip it */
		if (*part) continue;
		id = va_arg(arg, size_t *);
		*id = kore_strtonum64(parts[i], 0, &err);
		if (err == KORE_RESULT_ERROR) {
			ret = -1;
			break;
		}
	}

	va_end(arg);
	kore_free(route);

	return ret;
}



#endif