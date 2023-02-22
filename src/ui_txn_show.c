#include <dhanda/dhanda.h>
#include <dhanda/txn.h>
#include <dhanda/ui.h>


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

