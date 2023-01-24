
#include <dhanda/dhanda.h>
#include <dhanda/txn.h>
#include <dhanda/ui.h>


void ui_txn_create(dhanda *app)
{
	txn t;
	char line[256], ch;
	int len;
	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > CREATE");
	printf("\n\n");
	reset();
	printf("   PARTY ID : ");
	//scanf("%d", &t.party_id);
	input_pid(app, &t.party_id, validate_pid);
	printf("\n");
	printf("   AMOUNT   : ");
	//scanf("%d", &t.amount);
	input_amount(&t.amount, validate_amount);
	printf("\n");
	printf("[O FOR CRED, 1 FOR DEBT]\n");
	printf("   TYPE     : ");
	scanf("%d", &t.type);
	scanf("%c", &ch);
	printf("\n");
	printf("   DESC     : ");
	get_string(t.desc, 256);
	printf("\n");
	/*if(len > 0)
		strcpy(t.desc, line);*/
	//scanf("%s", t.desc);

	time(&t.cat);
	txn_insert_in_list(app, &t);
	puts("");
}
