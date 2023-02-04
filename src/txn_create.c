#include<dhanda/dhanda.h>
#include<dhanda/txn.h>


int txn_add(dhanda *app, txn *txn)
{

	char local_sql[1024];
    char *err = 0;
    int ret;

	/*struct txn temp;
	int init_posn, fin_posn, txn_id, ret;
	debug_print("");

	fseek(app->txn_fp, 0, SEEK_END);
	if (ftell(app->txn_fp)) {
		fseek(app->txn_fp, -sizeof(*txn), SEEK_END);
		ret = fread(&temp, sizeof(*txn), 1, app->txn_fp);
		if (ret != 1) {
			return -1;
		}
		txn_id = temp.id;
		txn_id++;
	} else {
		txn_id = 1;
	}

	txn->id = txn_id;

	init_posn = ftell(app->txn_fp);
	ret = fwrite(txn, sizeof(*txn), 1, app->txn_fp);
	if (ret != 1) {
		return -1;
	}

	return 0;*/


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