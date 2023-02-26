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

	http_response(req, 201, buf.data, buf.offset);

cleanup:
	kore_json_cleanup(&json);
	kore_json_item_free(res_json);
	return KORE_RESULT_OK;

}

int
api_party_get(struct http_request *req)
{
	struct list *result;
	party_filter filter = {};
	struct kore_json json = {};
	struct kore_buf buf;

	http_populate_get(req);

	result = list_create(sizeof(struct party));
	party_get(&app, filter, result);
	party_list_to_korejson(result, &json);

	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);

	http_response(req, 200, buf.data, buf.offset);


cleanup:
	if (result) list_delete_all(result);
	kore_json_item_free(json.root);

	return KORE_RESULT_OK;
}

int
api_txn_get(struct http_request *req)
{
	struct list *result;
	txn_filter filter = {};
	struct kore_json json = {};
	struct kore_buf buf;

	printf("OK\n");

	http_populate_get(req);

	result = list_create(sizeof(struct txn));
	txn_get(&app, filter, result);
	txn_list_to_korejson(result, &json);

	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);

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
		http_response(req, 400, NULL, 0);
		goto cleanup;
	}

	if (kore_apputil_extract_route_ids(req->path, &id) != 1){
        http_response(req, 400, NULL, 0);
        return KORE_RESULT_ERROR;
    }

	if (party_findbyid(&app, id, &result) != 1){
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

	http_response(req, 201, buf.data, buf.offset);

cleanup:
	kore_json_cleanup(&json);
	kore_json_item_free(res_json);
	return KORE_RESULT_OK;
}

int api_party_delete(struct http_request *req)
{
    int id;
    int result;

    if (kore_apputil_extract_route_ids(req->path, &id) != 1) {
        http_response(req, 400, NULL, 0);
        return KORE_RESULT_ERROR;
    }

    result = party_delete(&app, id);
    if (result == -1) {
        http_response(req, 404, NULL, 0);
        return KORE_RESULT_ERROR;
    }

    http_response(req, 204, NULL, 0);
    return KORE_RESULT_OK;
}

int api_txn_delete(struct http_request *req)
{
    int id;
    int result;

    if (kore_apputil_extract_route_ids(req->path, &id) != 1) {
        http_response(req, 400, NULL, 0);
        return KORE_RESULT_ERROR;
    }

    result = txn_delete(&app, id);
    if (result == -1) {
        http_response(req, 404, NULL, 0);
        return KORE_RESULT_ERROR;
    }

    http_response(req, 204, NULL, 0);
    return KORE_RESULT_OK;
}


int
api_party_details(struct http_request *req)
{
	struct list *result;
	party_filter filter = {};
	txn_filter tfilter = {};
	struct kore_json json = {};
	struct kore_buf buf;
	int id;

	http_populate_get(req);

	result = list_create(sizeof(struct party));

	if (kore_apputil_extract_route_ids(req->path, &id) != 1){
        http_response(req, 400, NULL, 0);
        return KORE_RESULT_ERROR;
    }

	if (party_findbyid(&app, id, &result) != 1){
		http_response(req, 400, NULL, 0);
		return KORE_RESULT_ERROR;
	}

	party_get(&app, filter, result);
	party_struct_to_korejson(result);

	if (txn_findbyid(&app, id, &result) != 1){
		http_response(req, 400, NULL, 0);
		return KORE_RESULT_ERROR;
	}

    txn_get(&app, tfilter, result);
	txn_list_to_korejson(result, &json);

	kore_buf_init(&buf, 1024);
	kore_json_item_tobuf(json.root, &buf);

	http_response(req, 200, buf.data, buf.offset);

cleanup:
	if (result) list_delete_all(result);
	kore_json_item_free(json.root);

	return KORE_RESULT_OK;
}

#endif
