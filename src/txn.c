#include <dhanda/dhanda.h>
#include <dhanda/txn.h>
#include <dhanda/sqlite3.h>



int txn_add(dhanda *app, txn *txn)
{

    char local_sql[1024];
    char *err = NULL;
    int ret;


	char *cat = created_time(txn->cat);

    sprintf(local_sql, "INSERT INTO transactions(amount, created_at, type, desc, party_id) VALUES ('%d', '%s', '%d', '%s', '%d')",txn->amount, cat, txn->type, txn->desc, txn->party_id);

	ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

	 if (ret != SQLITE_OK) {
        // fprintf(stderr, "sqlite3_exec: %s\n", err);
        // ret = -1;
        printf("Txn error %s\n", err);
        exit(EXIT_FAILURE);
    }else{
        ret = 1;
    }

     txn->id = sqlite3_last_insert_rowid(app->db);
     free(err);

    return ret;

}

int txn_delete(struct dhanda *app, int pid){
	int ret;
	char *err = NULL;
	char sql[1024];

	sprintf(sql, "DELETE FROM transactions WHERE party_id = %d", pid);

	ret = sqlite3_exec(app->db, sql, NULL, NULL, &err);
	if (ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec error : %s\n", err);
		return -1;
	}

	return 0;
}


int txn_findbyid(dhanda *app, int id, txn *result)
{
	int ret;
	char *err = NULL;
	char sql[1024];

	sprintf(sql, "SELECT * FROM transactions WHERE id = %d", id);
	result->id = 0;

	ret = sqlite3_exec(app->db, sql, put_in_txn_struct, (void *) result, &err);

	if (ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec error : %s\n", err);
		return -1;
	}
	 if(result->id == 0){
    	return 0;
    }
	return 1;
}


	
int txn_search(dhanda *app, char *query, struct list *result)
{
	int ret;
	char *err;
	char sql[1024];

	int pid = atoi(query);

	sprintf(sql, "SELECT * FROM transactions WHERE party_id = %d", pid);

	ret = sqlite3_exec(app->db, sql, put_in_txn_list, (void *) result, &err);

	if (ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec error : %s\n", err);
		return -1;
	}

	if (result->head == NULL){
		return 0;
	}

	return 1;
}

int txn_findbytype(dhanda *app, int type, struct list *result)
{	
	int ret;
	char *err = NULL;
	char sql[1024];

	sprintf(sql, "SELECT * FROM transactions WHERE type = %d", type);

	ret = sqlite3_exec(app->db, sql,put_in_txn_list, (void *) result, &err);
	if (ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec error : %s\n", err);
		return -1;
	}

	if (result->head == NULL){
		return 0;
	}

	return 1;
}

int txn_get(dhanda *app, txn_filter filter, struct list *result)
{
	int ret;
	char *err = NULL;
 	char sql[1024], where_query[512];
	int offset;

	where_query[0] = '\0';
	if (filter.has_query) {
		sprintf(where_query,
				"WHERE id LIKE '%%%d%%' OR "
				"party_id LIKE '%%%d%%' OR "
				"type LIKE '%%%d%%' ",
				filter.query, filter.query, filter.query);
	}

	offset = (filter.page - 1) * filter.items;

	/*sprintf(sql, "SELECT * FROM transactions ORDER BY id DESC LIMIT %d OFFSET %d",
			filter.items, offset);*/
	sprintf(sql, "SELECT * FROM transactions %s ORDER BY id DESC LIMIT %d OFFSET %d", where_query, filter.items, offset);


	ret = sqlite3_exec(app->db, sql, put_in_txn_list, (void *)result, &err);
	if(ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec error : %s\n", err);
		return -1;
	}

	if(result->head == NULL){
		return 0;
	}

	return 1;

}



void
txn_insert_in_list(dhanda *app, txn *t)
{
	Node *node;
	debug_print("");

	node = list_new_node(app->txn_list, t);
	if (node)
		list_insert_end(app->txn_list, node);
}

txn *
txn_first_in_list(dhanda *app)
{
	txn *t = NULL;
	Node *head;
	debug_print("");

	head = app->txn_list->head;
	if (head)
		t = (txn *) head->data;

	return t;
}

txn *
txn_second_in_list(dhanda *app)
{
	txn *t = NULL;
	Node *head;
	debug_print("");

	head = app->txn_list->head;
	if (head && head->next)
		t = (txn *) head->next->data;

	return t;
}

