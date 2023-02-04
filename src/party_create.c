#include<dhanda/dhanda.h>
#include<dhanda/party.h>

int party_add(dhanda *app, party *party)
{
     struct party p;
     char local_sql[1024];
     char *err = 0;
     int ret;
      
    sprintf(local_sql, "INSERT INTO parties(fname, lname, phone, amount) VALUES('%s', '%s', '%ld', '%ld')",
                                                   "party->fname, party->lname, party->phone, party->amount)";
                           
    ret = sqlite3_exec(db, local_sql, NULL, NULL, &err);

     if (ret != SQLITE_OK) {
          fprintf(stderr, "sqlite3_exec: %s\n", err);
          return -1;
     } else {
          return 0;
     }   
   
   char *sql =  "INSERT INTO 'parties' ADD COLUMN
                (
                   'created_at' DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
                   'updated_at' DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
    
                )
                 AFTER 'amount';"

    sprintf(local_sql, sql, party->cat, party->uat);

     if (ret != SQLITE_OK) {
          fprintf(stderr, "sqlite3_exec: %s\n", err);
          return -1;
     } else {
          return 0;
     }   

}

int party_add(dhanda *app, party *party)
{
    char local_sql[1024];
    char *err = 0;
    int ret;

    char *cat = created_time(party->cat);
    char *uat = updated_time(party->uat);

    sprintf(local_sql, "INSERT INTO parties (fname, lname, phone, amount, created_at, updated_at) VALUES ('%s', '%s', '%ld', '%ld', '%s', '%s')",
            party->fname, party->lname, party->phone, party->amount, cat, uat);

    ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "sqlite3_exec: %s\n", err);
        return -1;
    } else {
        return 0;
    }
}
