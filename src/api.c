#ifndef TERMINAL_APP

#include <kore/kore.h>
#include <kore/http.h>
#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <dhanda/txn.h>
#include <dhanda/api.h>
#include <dhanda/api_util.h>
#include <kore/seccomp.h>

KORE_SECCOMP_FILTER("app", 
	KORE_SYSCALL_ALLOW(fsync),
	KORE_SYSCALL_ALLOW(newfstatat),
)

static dhanda app = {};

void init_app(void)
{
	dhanda_init_app(&app);
}

int
api_party_add(struct http_request *req)
{
	struct kore_json json;
	struct kore_json_item *item;
	struct party p = {};

	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		http_response(req, 400, NULL, 0);
		goto cleanup;
	}

	item = kore_json_find_string(json.root, "first_name");
	fprintf(stderr, "First Name: %s\n", item->data.string);
	strcpy(p.fname, item->data.string);

	item = kore_json_find_string(json.root, "last_name");
	fprintf(stderr, "Last Name: %s\n", item->data.string);
	strcpy(p.lname, item->data.string);

	item = kore_json_find_string(json.root, "phone");
	fprintf(stderr, "Phone: %s\n", item->data.string);
	strcpy(p.phone, item->data.string);

	item = kore_json_find_integer(json.root, "amount");
	fprintf(stderr, "Amount: %d\n", (int) item->data.u64);

	p.amount = (int) item->data.u64;
	p.cat = p.uat = time(NULL);
	party_add(&app, &p);

	struct kore_json_item *res_json;
	struct kore_buf buf;

	res_json = party_struct_to_korejson(&p);

	kore_buf_init(&buf, 512);
	kore_json_item_tobuf(res_json, &buf);

    http_response_header(req, "content-type", "application/json");
	http_response(req, 201, buf.data, buf.offset);

cleanup:
	kore_json_cleanup(&json);
	kore_json_item_free(res_json);
	return KORE_RESULT_OK;
}


int api_txn_add(struct http_request *req){

	struct kore_json json;
	struct kore_json_item *item;
	struct txn t = {};

	kore_json_init(&json, req->http_body->data, req->http_body->length);

	if (!kore_json_parse(&json)) {
		http_response(req, 400, NULL, 0);
		goto cleanup;
	}

	item = kore_json_find_integer(json.root, "party_id");
	fprintf(stderr, "party_id: %d\n", (int) item->data.u64);
	t.party_id = (int) item->data.u64;

	item = kore_json_find_integer(json.root, "amount");
	fprintf(stderr, "amount: %d\n", (int) item->data.u64);
	t.amount = (int) item->data.u64;

	t.cat = time(NULL);

	item = kore_json_find_integer(json.root, "type");
	fprintf(stderr, "type: %d\n", (int) item->data.u64);
    t.type = (int) item->data.u64;

	item = kore_json_find_string(json.root, "desc");
	fprintf(stderr, "desc: %s\n", item->data.string);
	strcpy(t.desc, item->data.string);

	txn_add(&app, &t);

	struct kore_json *res_json;
	struct kore_buf buf;
	res_json = txn_struct_to_korejson(&t);

	kore_buf_init(&buf, 512);
	kore_json_item_tobuf(res_json, &buf);

    http_response_header(req, "content-type", "application/json");
	http_response(req, 201, buf.data, buf.offset);

cleanup:
	kore_json_cleanup(&json);
	kore_json_item_free(res_json);
	return KORE_RESULT_OK;

}

/*int
api_party_get(struct http_request *req)
{
	struct list *result;
	party_filter filter = {};
	struct kore_json json = {};
	struct kore_buf buf;

	http_populate_get(req);

	result = list_create(sizeof(struct party));
	party_get(&app, filter, result);

	json.root = party_list_to_korejson(result);

	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);

	http_response(req, 200, buf.data, buf.offset);


cleanup:
	if (result) list_delete_all(result);
	kore_json_item_free(json.root);

	return KORE_RESULT_OK;
}*/

int
api_party_get(struct http_request *req)
{
	int ret, page, items;
	struct list *result;
	party_filter filter = {};
	struct kore_json json = {};
	struct kore_buf buf;
	char *ptr;
	int err = 0;

	http_populate_get(req);

	ret = http_argument_get_uint32(req, "page", &page);
	if (!ret) page = 1;

	ret = http_argument_get_uint32(req, "items", &items);
	if (!ret) items = 50;

	ret = http_argument_get_string(req, "query", &ptr);
	if (ret) {
		filter.query = ptr;
		filter.has_query = 1;
	}

	filter.page = page;
	filter.items = items;

	result = list_create(sizeof(struct party));
	party_get(&app, filter, result);

	json.root = party_list_to_korejson(result);

	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);

    http_response_header(req, "content-type", "application/json");
	http_response(req, 200, buf.data, buf.offset);


cleanup:
	if (result) list_delete_all(result);
	kore_json_item_free(json.root);

	return KORE_RESULT_OK;
}

int
api_txn_get(struct http_request *req)
{
	int ret, page, items;
	struct list *result;
	txn_filter filter = {};
	struct kore_json json = {};
	struct kore_buf buf;
	int err = 0;
	char *ptr;
	
	http_populate_get(req);

	ret = http_argument_get_uint32(req, "page", &page);
	if (!ret) page = 1;

	ret = http_argument_get_uint32(req, "items", &items);
	if (!ret) items = 50;

	ret = http_argument_get_uint32(req, "party_id", &ptr);
	if (ret) {
		filter.query = ptr;
		filter.has_query = 1;
	}

	filter.page = page;
	filter.items = items;

	result = list_create(sizeof(struct txn));
	txn_get(&app, filter, result);

	json.root = txn_list_to_korejson(result);
	
	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);
 
    http_response_header(req, "content-type", "application/json");
	http_response(req, 200, buf.data, buf.offset);
	


cleanup:
	if (result) list_delete_all(result);
	kore_json_item_free(json.root);

	return KORE_RESULT_OK;
}


int api_party_update(struct http_request *req){

	struct kore_json json;
	struct kore_json_item *item;
	struct party result = {};
	struct party p = {};
	int id;

	kore_json_init(&json, req->http_body->data, req->http_body->length);
	if (!kore_json_parse(&json)){
        http_response_header(req, "content-type", "application/json");
		http_response(req, 400, NULL, 0);
		goto cleanup;
	}

	kore_apputil_extract_route_ids(req->path, &id);

	if (party_findbyid(&app, id, &result) != 1){
        http_response_header(req, "content-type", "application/json");
		http_response(req, 400, NULL, 0);
		return KORE_RESULT_ERROR;
	}

	p.id = id;
	item = kore_json_find_string(json.root, "first_name");
	strcpy(p.fname, item->data.string);
	item = kore_json_find_string(json.root, "last_name");
	strcpy(p.lname, item->data.string);
	item = kore_json_find_string(json.root, "phone");
	strcpy(p.phone, item->data.string);
	item = kore_json_find_integer(json.root, "amount");
	p.amount = (int) item->data.u64;

	p.cat = result.cat;
	time(&p.uat);
	party_update(&app, NULL, &p);

	struct kore_json_item *res_json;
	struct kore_buf buf;
	res_json = party_struct_to_korejson(&p);
	
	kore_buf_init(&buf, 512);
	kore_json_item_tobuf(res_json, &buf);
    
    http_response_header(req, "content-type", "application/json");
	http_response(req, 201, buf.data, buf.offset);

cleanup:
	kore_json_cleanup(&json);
	kore_json_item_free(res_json);
	return KORE_RESULT_OK;
}

int api_party_delete(struct http_request *req)
{
    int id;
    struct party result = {};
    struct kore_json json;
	struct kore_json_item *item;

    http_populate_get(req);

    kore_apputil_extract_route_ids(req->path, &id);

    if(party_findbyid(&app, id, &result) != 1){
        http_response_header(req, "content-type", "application/json");
		http_response(req, 400, NULL, 0);
		return KORE_RESULT_ERROR;
	}

    //txn_delete(&app, result.id);
    printf("OK\n");
    party_delete(&app, &result);

    http_response_header(req, "content-type", "application/json");
    http_response(req, 200, NULL, 0);
    return KORE_RESULT_OK;
}


int
api_party_show(struct http_request *req)
{
	struct kore_json json;
	struct kore_json_item *item;
	struct party result = {};
	struct list *txn_result;
	txn_filter filter = {};
	int id, ret;
	int page, items;
	char *ptr = NULL;

	http_populate_get(req);

	kore_apputil_extract_route_ids(req->path, &id);
	
	if (party_findbyid(&app, id, &result) != 1) {
		http_response(req, 400, NULL, 0);
		return KORE_RESULT_ERROR;
	}

	ret = http_argument_get_string(req, "includes", &ptr);

	ret = http_argument_get_uint32(req, "page", &page);
	if (!ret) page = 1;

	ret = http_argument_get_uint32(req, "items", &items);
	if (!ret) items = 50;

	filter.page = page;
	filter.items = items;

	txn_result = list_create(sizeof(struct txn));
	if (ptr) {
		ret = txn_findby_pid(&app, filter, id, txn_result);
	}


	struct kore_json_item *res_json1, *res_json2;
	struct kore_buf buf;
	res_json1 = party_struct_to_korejson(&result);
	res_json2 = txn_list_to_korejson(txn_result);
	struct kore_json_item *temp = kore_json_find_array(res_json2, "transactions");
	kore_json_item_attach(res_json1, temp);


	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(res_json1, &buf);

	http_response(req, 200, buf.data, buf.offset);


	return KORE_RESULT_OK;
}

#endif

