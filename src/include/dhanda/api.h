void init_app(void);
int api_party_add(struct http_request *req);
int api_txn_add(struct http_request *req);
int api_party_get(struct http_request *req);
int api_txn_get(struct http_request *req);
int api_party_update(struct http_request *req);
int api_party_delete(struct http_request *req);
int api_party_show(struct http_request *req);
int api_txn_show(struct http_request *req);
struct kore_json_item * party_struct_to_korejson(struct party *p);
struct kore_json_item * party_list_to_korejson(struct list *parties);
struct kore_json_item * txn_list_to_korejson(struct list *transactions);
struct kore_json_item * txn_struct_to_korejson(struct txn *t);

static int util_http_json_error_response(struct http_request *req,
		struct kore_json_item *item,
		const char *field,
		int (*validator)(char *));


