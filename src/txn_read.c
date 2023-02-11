#include <dhanda/dhanda.h>
#include <dhanda/txn.h>

int put_in_txn_struct(void *ptr, int ncols, char **values, char **fields);
int put_in_txn_list(void *ptr, int ncols, char **values, char **fields);

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
	char sql[1024];

	sprintf(sql, "SELECT * FROM transactions");

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


int put_in_txn_struct(void *ptr, int ncols, char **values, char **fields){

	txn *temp = (txn *) ptr;

	temp->id = (int) atoi(values[0]);
	temp->amount = (int) atoi(values[1]);
	temp->cat = unix_time(values[2]);
	temp->type = (int) atoi(values[3]);
	strcpy(temp->desc, values[4]);
	temp->party_id = (int) atoi(values[5]);

	return SQLITE_OK;

}

int put_in_txn_list(void *ptr, int ncols, char **values, char **fields){
	
	Node *node;
	txn temp = {};

	temp.id = (int) atoi(values[0]);
	temp.amount = (int) atoi(values[1]);
	temp.cat = unix_time(values[2]);
	temp.type = (int) atoi(values[3]);
	strcpy(temp.desc, values[4]);
	temp.party_id = (int) atoi(values[5]);

	node = list_new_node((struct list *) ptr, (void *) &temp);
	assert(node != NULL);
	list_insert_end((struct list *) ptr, node);

	return SQLITE_OK;
}







