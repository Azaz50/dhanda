int
kore_apputil_extract_route_ids(const char *path, ...);

int
util_http_json_error_response(struct http_request *req, struct kore_json_item *item, const char *field, int (*validator)(char *));

int
util_http_response_msg(struct http_request *req, char *str);

