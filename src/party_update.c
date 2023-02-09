#include<dhanda/dhanda.h>
#include<dhanda/party.h>

int party_update(dhanda *app, party *old_party, struct party *new_party)
{
	party temp;

	int ret;
	char *err = NULL;
	char local_sql[1024];

	char *cat = created_time(new_party->cat);
	char *uat = updated_time(new_party->uat);

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
