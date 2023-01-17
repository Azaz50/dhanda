#include <dhanda/dhanda.h>
#include <dhanda/party.h>
#include <dhanda/ui.h>


void ui_party_create(struct dhanda *app)
{
	
	party p;
	int len;
	long ret;
	char s[100];
	char *ptr;


	sky();
	bold();
	centreprint("DHANDA > PARTY > CREATE");
	printf("\n\n");
	reset();
	grey();
	
	
	printf("   FIRST NAME :  ");
	//get_string(p.fname, sizeof(p.fname));
	input_valid_string(p.fname, sizeof(p.fname), validate_name);
	title_case(p.fname);
	
	
	printf("   LAST NAME  :  ");
	//get_string(p.lname, sizeof(p.lname));
	input_valid_string(p.lname, sizeof(p.lname), validate_name);
	title_case(p.lname);
	
	printf("   PHONE      :  ");
	//get_string(p.phone, sizeof(p.phone));
	input_valid_string(p.phone, sizeof(p.phone), validate_phone);
	

	printf("  AMOUNT     :  ");
        //scanf("%d",&p.amount);
        input_amount(&p.amount, validate_amount);
        
       
        
    printf("\n");
	time(&p.cat);
	reset(); 
	
	party_insert_in_list(app, &p);

	puts("");
}

