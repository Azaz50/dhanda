#include <dhanda/dhanda.h>
#include <dhanda/party.h>

int 
cb_party_list(void *list, int ncols, char **values, char **fields);
int
cb_party_struct(void *ptr, int ncols, char **values, char **fields);


int party_findbyid(dhanda *app, int id, party *result)   
{

	char sql[256];                                             
	int ret;
	char *err = NULL;


	sprintf(sql, "SELECT * FROM parties WHERE id = %d", id);

	result->id = 0;
	ret = sqlite3_exec(app->db, sql, cb_party_struct, (void *)result, &err);

	 if (ret != SQLITE_OK) {
        fprintf(stderr, "party_findbyid(): sqlite3_exec error: %s\n", err);
        ret = -1;
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

	sprintf(sql, "SELECT * FROM parties WHERE phone LIKE '%%%s%%' OR fname LIKE '%%%s%%'", query, query);
    
    ret = sqlite3_exec(app->db, sql, cb_party_struct, (void *)result, &err);
    if (ret != SQLITE_OK){
    	fprintf(stderr, "sqlite3_exec: %s\n", err);
    	return -1;
    }

    if (result->head == NULL){
    	return -1;
    }

	return 0;
	
}
			

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
    	return -1;
    }

    return 1;

}
int
cb_party_list(void *list, int ncols, char **values, char **fields){
	party p = {};
	party *ptr;
	Node *node;
	
	char *cat = created_time(ptr->cat);
    char *uat = updated_time(ptr->uat);

	p.id = atoi(values[0]);
	strcpy(p.fname, values[1]);
	strcpy(p.lname, values[2]);
	strcpy(p.phone, values[3]);
	p.amount = atoi(values[4]);
	p.cat = unix_time(values[5]);
	p.uat = unix_time(values[6]);

	node = list_new_node((struct list *) list, (void *) &p);
	list_insert_end((struct list *) list, node);

	return SQLITE_OK;

}	

int
cb_party_struct(void *ptr, int ncols, char **values, char **fields){
	party *p = (party *) ptr;

	p->id = (int)atoi(values[0]);
	strcpy(p->fname, values[1]);
	strcpy(p->lname, values[2]);
	strcpy(p->phone, values[3]);
	p->amount = atoi(values[4]);
	p->cat = unix_time(values[5]);
	p->uat = unix_time(values[6]);

	return SQLITE_OK;


}	




	
