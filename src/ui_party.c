#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <sys/types.h>
#include <dhanda/ui.h>
#include <regex.h>

void ui_party_create(dhanda *app)
{
	
	party p;
	int len;
	long ret;
	char s[100];
	char *ptr;
	char local_sql[1024];

	sky();
	bold();
	centreprint("DHANDA > PARTY > CREATE");
	printf("\n\n");
	reset();
	grey();
	
	printf("   FIRST NAME :  ");
	input_valid_string(p.fname, sizeof(p.fname), validate_name);
	title_case(p.fname);
	
	
	printf("   LAST NAME  :  ");
	input_valid_string(p.lname, sizeof(p.lname), validate_name);
	title_case(p.lname);
	
	printf("   PHONE      :  ");
	input_valid_string(p.phone, sizeof(p.phone), validate_phone);

	

	printf("  AMOUNT     :  ");
        input_amount(&p.amount, validate_amount);
  

        time(&p.cat);
        time(&p.uat);

      
	reset(); 
	
	party_insert_in_list(app, &p);
	puts("");
}


void ui_party_delete(struct dhanda *app)
{
	Node *ptr;
	party *p;

	debug_print("");


	sky();
	bold();
	centreprint("DHANDA > PARTY > DELETE");
	printf("\n\n");
	reset();

	
	ptr = app->party_list->head;
	while(!ptr){
		p = (party *) ptr->data;
	}

}



void ui_party_list(struct dhanda *app)
{
	Node *ptr;
	party *p;
	ptr = app->party_list->head;
	
	sky();
	bold();
	centreprint("DHANDA > PARTY > LIST");
	printf("\n\n");
	reset();
	grey();

	printf("   %-13s %-20s %-20s %-15s %-10s\n\n\n", "ID", "FIRST NAME", "LAST NAME", "AMOUNT", "PHONE");
	reset();
	while(ptr){

		p = (party *) ptr->data;

		printf("   %-13d %-20s %-20s %-15d %-10s\n\n", p->id, p->fname, p->lname, p->amount, p->phone);
		
		ptr = ptr->next;
	}

}

void ui_party_show(struct dhanda *app)
{
	int id;
	party *p;

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

	printf("   ID NO      : %d               \n\n", p->id);
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
	printf("\n\n");

	printf("   UPDATED AT : ");
	updated_at(p->uat);
	printf("\n\n");

	reset();

	
	return;

}


void ui_party_edit(struct dhanda *app)
{
	long int phone, amount;
	char fname[64], lname[64];
	party *old_party, *new_party;
	int len;
	char *ptr;
	long int ret;

	old_party = party_first_in_list(app);
	new_party = party_second_in_list(app);

	debug_print("");

	sky();
	bold();
	centreprint("DHANDA > PARTY > EDIT");
	printf("\n\n");
	reset();

	printf("FIRST NAME :  %s\n\n", old_party->fname);
	printf("LAST NAME  :  %s\n\n", old_party->lname);
	printf("PHONE      :  %s\n\n", old_party->phone);
	printf("AMOUNT     :  %d\n\n", old_party->amount);
	puts("");


	printf("FIRST NAME :  ");
	input_valid_string(new_party->fname, sizeof(new_party->fname), validate_name);
	title_case(new_party->fname);
	

	printf("LAST NAME  :  ");
	input_valid_string(new_party->lname, sizeof(new_party->lname), validate_name);
	title_case(new_party->lname);

	printf("PHONE      :  ");
	input_valid_string(new_party->phone, sizeof(new_party->phone), validate_phone);
	
	printf("AMOUNT     :  ");
	input_amount(&new_party->amount, validate_amount);

	time(&new_party->uat);

	party_insert_in_list(app, new_party);


	printf("\n");

	
}

