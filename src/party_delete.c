#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <unistd.h>




int party_delete(dhanda *app, party *party)        // return 1 for succes
                                                  // return -1 for failure
{
    
    char local_sql[1024];
	char *err = NULL;
    int ret;


	
	sprintf(local_sql, "DELETE FROM parties WHERE id = %d", party->id);

	ret = sqlite3_exec(app->db, local_sql, NULL, NULL, &err);

    if (ret != SQLITE_OK) {
        fprintf(stderr, "party_delete(): sqlite3_exec error: %s\n", err);
        return -1;
    } 

    return 0;
}
			
	
	
	
	
	
	
	
	
	
	
	
