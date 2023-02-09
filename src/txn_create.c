#include<dhanda/dhanda.h>
#include<dhanda/txn.h>


int txn_add(dhanda *app, txn *txn)
{

    char local_sql[1024];
    char *err = 0;
    int ret;


	char *cat = created_time(txn->cat);

    sprintf(local_sql, "INSERT INTO transactios(amount, created_at, type, desc, party_id) VALUES ('%d', '%s', '%d', '%s', '%d')",txn->amount, cat, txn->type, txn->desc, txn->party_id);

	ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

	 if (ret != SQLITE_OK) {
        fprintf(stderr, "sqlite3_exec: %s\n", err);
        ret = -1;
    }else{
        ret = 0;
    }

     txn->id = sqlite3_last_insert_rowid(app->db);
     free(err);

    return ret;

}
