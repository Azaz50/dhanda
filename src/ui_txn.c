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
	input_pid(app, &t.party_id, validate_pid);
	printf("\n");

	printf("   AMOUNT   : ");
	input_amount(&t.amount, validate_amount);
	printf("\n");
	
	printf("[O FOR CRED, 1 FOR DEBT]\n");
	printf("   TYPE     : ");
	input_txn_type(&t.type, validate_type);
	printf("\n");
	
	printf("   DESC     : ");
	get_string(t.desc, 256);
	printf("\n");

	time(&t.cat);
	txn_insert_in_list(app, &t);
	puts("");
}
void ui_txn_list(struct dhanda *app)
{
	Node *temp;
	txn *t;
	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > LIST");
	printf("\n\n");
	reset();
	printf("   ID         PARTY_ID            AMOUNT          DEBIT/CREDIT(D/C)\n");
	printf("\n\n");

	temp = app->txn_list->head;
	while(temp != NULL)
	{	
		t = (txn *)temp->data;
         	printf("   %-10d %-19d %-15d %-15d\n\n",
			       	t->id,
				    t->party_id,
			       	t->amount,
			       	t->type);

		temp = temp->next;
	}

}

void ui_txn_show(struct dhanda *app)
{
	Node *ptr;
	ptr = app->txn_list->head;
	txn *t;
	debug_print("");

	t = (txn *) ptr->data;

	sky();
	bold();
	centreprint("DHANDA > TRANSACTION > SHOW");
	printf("\n\n");
	reset();

	int n = strlen(t->desc);
  printf("%-10s %-10s %-10s %-10s %-*s %s", "ID", 
                                          "AMOUNT ",
                                          "TYPE",
                                          "PARTY_ID",
                                          n, "DESC",
                                          "   CAT");
  printf("\n\n");
  printf("%-10d %-10d %-10d %-10d %-*s", t->id, 
                                      t->amount,
                                      t->type,
                                      t->party_id,
                                      n, t->desc);

  printf("   ");
	created_at(t->cat);
	printf("\n\n");

	
}

