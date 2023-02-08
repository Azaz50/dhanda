#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <dhanda/ui.h>


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
  


      
	reset(); 
	
	party_insert_in_list(app, &p);
	puts("");
}

