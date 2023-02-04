#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <dhanda/ui.h>


void ui_party_show(struct dhanda *app)
{
	int id;
	party *p;

	//char local_sql[1024];
    char *err = 0;
    int ret;

	debug_print("");
	p = party_first_in_list(app);

	sky();
	bold();
	centreprint("DHANDA > PARTY > SHOW");

	printf("\n\n");
	reset();
	grey();

	/*printf("   ID NO      : %d               \n\n", p->id);
    title_case(p->fname);
	printf("   FIRST NAME : %s               ", p->fname);
	
	
	printf("\n\n");

    title_case(p->lname);
	printf("   LAST NAME  : %s               ", p->lname);
	printf("\n\n");

	printf("   PHONE NO   : %s               \n\n", p->phone);
	printf("   AMOUNT     : %d                \n\n", p->amount);

	printf("   CREATED AT : ");
	created_at(p->cat);

	printf("\n");

	printf("   UPDATED AT : ");
	updated_at(p->uat);
*/
	
	char *user_select_sql = "SELECT * FROM parties";
	ret = sqlite3_exec(app->db, user_select_sql, print_user, NULL, &err);

	if(ret != SQLITE_OK){
		fprintf(stderr, "sqlite3_exec: %s\n", err);
		goto cleanup;
	}

	cleanup:
	sqlite3_close(db);
	if(err)
		sqlite3_free(err);

	return 0;

	reset();
}
