struct kore_json_item * party_struct_to_korejson(struct party *p);
struct kore_json_item * txn_struct_to_korejson(struct txn *t);
struct kore_json_item * party_list_to_korejson(struct list *parties);
struct kore_json_item * party_list_to_korejson(struct list *parties);
struct kore_json_item * txn_list_to_korejson(struct list *transaction);
int kore_apputil_extract_route_ids(const char *path, ...);



