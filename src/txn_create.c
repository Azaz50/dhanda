#include<dhanda/dhanda.h>
#include<dhanda/txn.h>


int txn_add(dhanda *app, txn *txn)
{

    char local_sql[1024];
    char *err = 0;
    int ret;


	char *cat = created_time(txn->cat);

	sprintf(local_sql, "INSERT INTO transactios(id, amount, created_at, type, desc, party_id) VALUES
		('%d', '%d', '%s', '%s', '%s', '%d'), txn->id, txn->amount, cat, txn->type, txn->desc, txn->party_id");

	ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

	 if (ret != SQLITE_OK) {
        fprintf(stderr, "sqlite3_exec: %s\n", err);
        return -1;
    } else {
        return 0;
    }

}
