struct kore_json_item * party_struct_to_korejson(struct party *p);
struct kore_json_item * txn_struct_to_korejson(struct txn *t);
void party_list_to_korejson(struct list *parties, struct kore_json *json);
void txn_list_to_korejson(struct list *txn, struct kore_json *json);
int kore_apputil_extract_route_ids(const char *path, ...);



