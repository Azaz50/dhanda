#ifndef TERMINAL_APP

#include <dhanda/party.h>
#include <dhanda/txn.h>
#include <kore/kore.h>
#include <kore/http.h>
#include <kore/seccomp.h>



struct kore_json_item * 
party_struct_to_korejson(struct party *p)
{
	struct kore_json_item *item;
	struct kore_json_item *result;

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
	struct kore_json_item *item;
	struct kore_json_item *result;

	result = kore_json_create_object(NULL, NULL);

	item = kore_json_create_integer(result, "id", t->id);
	item = kore_json_create_string(result, "amount", t->amount);
	item = kore_json_create_string(result, "type", t->type);
	item = kore_json_create_string(result, "desc", t->desc);
	item = kore_json_create_integer(result, "amount", t->amount);
	item = kore_json_create_integer_u64(result, "created_at", t->cat);
	item = kore_json_create_integer(result, "party_id", t->party_id);

	return result;
}

#endif