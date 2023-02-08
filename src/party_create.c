#include<dhanda/dhanda.h>
#include<dhanda/party.h>

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
