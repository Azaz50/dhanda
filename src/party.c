#include <dhanda/dhanda.h>
#include <dhanda/party.h>


int party_add(dhanda *app, party *party)
{
    char local_sql[1024];
    char *err = NULL;
    int ret;

    char *cat = created_time(party->cat);
    char *uat = updated_time(party->uat);

    sprintf(local_sql, "INSERT INTO parties (fname, lname, phone, amount, created_at, updated_at) VALUES ('%s', '%s', '%s', '%d', '%s', '%s')",party->fname, party->lname, party->phone, party->amount, cat, uat);

    ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);


    if (ret != SQLITE_OK) {
        fprintf(stderr, "party_create(): sqlite3_exec error: %s\n", err);
        ret = -1;
    } else {
        ret = 0;
    }
    
    party->id = sqlite3_last_insert_rowid(app->db);


    free(err);

    return ret;
}



int party_delete(dhanda *app, party *party)        // return 1 for succes                                                 // return -1 for failure
{
    
    char local_sql[1024];
	char *err = NULL;
    int ret;
    struct list *result;
	party_filter filter = {};

	int offset = (filter.page - 1) * filter.items;
	
	sprintf(local_sql, "DELETE FROM parties WHERE id = %d", party->id);

	ret = sqlite3_exec(app->db, local_sql, cb_party_list, (void *)result, &err);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "party_delete(): sqlite3_exec error: %s\n", err);
        return -1;
    } 

    return 0;
}
		

int party_findbyid(dhanda *app, int id, party *result)   
{

	char sql[256];                                             
	int ret;
	char *err = NULL;
	party_filter filter = {};

	int offset = (filter.page - 1) * filter.items;

	sprintf(sql, "SELECT * FROM parties WHERE id = %d", id);

	result->id = 0;
	ret = sqlite3_exec(app->db, sql, cb_party_struct, (void *)result, &err);

	 if (ret != SQLITE_OK) {
        fprintf(stderr, "party_findbyid(): sqlite3_exec error: %s\n", err);
        return -1;
    } 

    if(result->id == 0){
    	return 0;
    }

    return 1;

}


int party_search(dhanda *app, char *query, struct list *result)
{
	int ret;
	char sql[1024];
	char *err = NULL;
	party_filter filter = {};

	int offset = (filter.page - 1) * filter.items;
	sprintf(sql, "SELECT * FROM parties WHERE phone LIKE '%%%s%%' OR fname LIKE '%%%s%%' OR fname LIKE '%%%s%%'", query, query, query);
    
    ret = sqlite3_exec(app->db, sql, cb_party_list, (void *)result, &err);
    if (ret != SQLITE_OK){
    	fprintf(stderr, "sqlite3_exec: %s\n", err);
    	return -1;
    }

    if (result->head == NULL){
    	return 0;
    }

	return 1;
	
}
			
/*
int party_get(dhanda *app, party_filter filter, struct list *result)
{
	char local_sql[1024];
    char *err = NULL;
    int ret;
    
	sprintf(local_sql, "SELECT * FROM parties");

	ret = sqlite3_exec(app->db, local_sql, cb_party_list, (void *)result, &err);

	 if (ret != SQLITE_OK) {
        fprintf(stderr, "party_get(): sqlite3_exec error: %s\n", err);
        return -1;
    }

    if(result->head == NULL){
    	return 0;
    }
    return 1;
}*/

int party_get(dhanda *app, party_filter filter, struct list *result)
{
 	int ret;
 	char *err = NULL;
 	char sql[1024], where_query[512];
	int offset;

    where_query[0] = '\0';
	if (filter.has_query) {
		sprintf(where_query,
				"WHERE fname LIKE '%%%s%%' OR "
				"lname LIKE '%%%s%%' OR "
				"phone LIKE '%%%s%%' ",
				filter.query, filter.query, filter.query);
	}

	offset = (filter.page - 1) * filter.items;
	sprintf(sql, "SELECT * FROM parties %s ORDER BY id DESC LIMIT %d OFFSET %d", where_query, filter.items, offset);
	printf("%s\n", where_query);
 
 	ret = sqlite3_exec(app->db, sql, cb_party_list, (void *) result, &err);
 	printf("%d\n", ret);
 	if (ret != SQLITE_OK){
 		 fprintf(stderr, "party_get(): sqlite3_exec error: %s\n", err);
         return -1;
 	}

 	if(result->head == NULL){
    	return 0;
    }
    
    return 1;		
}

int party_update(dhanda *app, party *old_party, struct party *new_party)
{
	party temp;

	int ret;
	char *err = NULL;
	char local_sql[1024];

	char *cat = created_time(new_party->cat);
	char *uat = updated_time(new_party->uat);
	party_filter filter = {};

	int offset = (filter.page - 1) * filter.items;

	sprintf(local_sql, "UPDATE parties SET "
		               "fname = '%s', lname = '%s',"
		               "phone = '%s', amount = %d,"
		               "created_at = '%s', updated_at = '%s' WHERE id = %d",
		               new_party->fname, new_party->lname, new_party->phone,
		               new_party->amount, cat, uat, new_party->id);

	ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

	if(ret != SQLITE_OK){
		
		printf("update error %s\n", err);
		exit(EXIT_FAILURE);
	}
	
	return 0;
}		 


void
party_insert_in_list(dhanda *app, party *p)
{
	Node *node;

	node = list_new_node(app->party_list, p);
	if (node)
		list_insert_end(app->party_list, node);
}

party *
party_first_in_list(dhanda *app)
{
	party *p = NULL;
	Node *head;

	head = app->party_list->head;
	if (head)
		p = (party *) head->data;

	return p;
}

party *
party_second_in_list(dhanda *app)
{
	party *p = NULL;
	Node *head;

	head = app->party_list->head;
	if (head && head->next)
		p = (party *) head->next->data;

	return p;
}
