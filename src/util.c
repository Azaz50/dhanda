#include <dhanda/dhanda.h>
#include <dhanda/util.h>
#include <regex.h>
#include <dhanda/party.h>
#include <dhanda/txn.h>


int get_line(char line[], int size)
{
	int i;
	char ch;

	i = 0;
	do
	{
		ch = getchar();
		line[i] = ch;
		++i;
		


	} while (ch != EOF && ch != '\n' && i < size - 1);

	if (ch == EOF)
		line[--i] = '\0';
	else
		line[i] = '\0';

	return i;
}

int
get_string(char line[], int size)
{
	int len;

	len = get_line(line, size);
	line[len - 1] = '\0';

	return len - 1;
}

void input_valid_string(char *input, size_t size, int (*validator)(char *)){
	while(1){
		 printf("> ");
		 get_string(input, size);

		 if(validator(input) == 0)
		 	break;

		 printf("Invalid Input\n");
	}
}

int validate_name(char *str){
	//Regular Expression
	char *pattern = "^[a-zA-Z ]{2,31}$";
	char buff[1024];
	int err;
	regex_t rgx;
	if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
		return -1;
	}
	if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
		return -1;
	}
	return 0;
}

int validate_desc(char *str){
	char *pattern = "^.{1,256}$";
	char buff[1024];
	int err;
	regex_t rgx;
	if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
		return -1;
	}
	if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
		return -1;
	}
	return 0;

}



void input_phone(char *input, int (*validator)(char *)){
	char phone[10];

	while(1){
		printf("> ");
		get_string(phone, 10);

		if(validator(phone) == 0){
			long ret = strtol(phone, NULL, 10);
			*input = (int) ret;
			break;
		}
		printf("Invalid input\n");
	}
}

int validate_phone(char *str){
	//Regular expression
	char *pattern = "^[1-9][0-9]{9}$";
	char buff[1024];
	int err;

	regex_t rgx;

	if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
		return -1;
	}
	if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
		return -1;
	}

	return 0;
}

void input_amount(int *input, int (*validator)(char *)){
	char amount[10];

	while(1){
		printf("> ");
		get_string(amount, 10);

		if(validator(amount) == 0){
			long ret = strtol(amount, NULL, 10);
			*input = (int) ret;
			break;
		}
		printf("Invalid input\n");
	}
}

int validate_amount(char *str){
	//Regular expression
	char *pattern = "^[0-9]{1,10}$";
	char buff[1024];
	int err;
	regex_t rgx;

	if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
		regerror(err, &rgx, buff, sizeof(buff));
		printf("%s\n", buff);
		return -1;
	}

	if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
		return -1;
	}
	return 0;

}
int validate_amount_kore(int amount){
    //Regular expression
    char *pattern = "^[0-9]{1,10}$";
    char buff[1024];
    int err;
    regex_t rgx;

    // Convert integer to string
    char str[11];
    sprintf(str, "%d", amount);

    if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
        regerror(err, &rgx, buff, sizeof(buff));
        printf("%s\n", buff);
        return -1;
    }

    if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
        return -1;
    }
    return 0;
}


void title_case(char *str){
	str[0] = toupper(str[0]);
	for(int i = 1; str[i] != '\0'; ++i){
		if(str[i] == ' '){
			str[i+1] = toupper(str[i+1]);

		}
	}
}


void created_at(time_t t) 
{
	struct tm *tm;
	char timestr[100];

	tm = localtime(&t);
	//char *timestr = malloc(sizeof(char)*128);
	
	strftime(timestr, sizeof(timestr), "%Y-%m-%d %T", tm);

	printf("%s",timestr);

}

void updated_at(time_t t)
{
	struct tm *tm;
	char timestr[100];
	//char *timestr = malloc(sizeof(char)*128);

	tm = localtime(&t);

	strftime(timestr, sizeof(timestr), "%Y-%m-%d %T", tm);

	printf("%s", timestr);

}

void *created_time(time_t t)
{
	struct tm *tm;
	char *timestr = malloc(sizeof(char) * 128);

	tm = localtime(&t);

	strftime(timestr, 128, "%Y-%m-%d %T", tm);
	return timestr;

}


void *updated_time(time_t t)
{
	struct tm *tm;
	
	char *timestr = malloc(sizeof(char) * 128);

	tm = localtime(&t);

	strftime(timestr, 128, "%Y-%m-%d %T", tm);
	
	return timestr;

}

time_t unix_time(char *timestr){
	struct tm tm = {};
	
	strptime(timestr, "%Y-%m-%d %T", &tm);

	time_t t = mktime(&tm);

	return t;
}


void input_pid(dhanda *app, int *pid, int (*validator)(char *)){
	char pd[10];
	struct party result;

	while(1){
		printf("> ");
		get_string(pd, sizeof(pd));
		if(validator(pd) == 0){
			long ret = strtol(pd, NULL, 10);
			*pid = (int)ret;
			if(party_findbyid(app, *pid, &result) == 1){
				break;
			}
			printf("Invalid input\n");
		}
	}
}

int validate_pid(char *str){

	char *pattern = "^[0-9]{1,10}$";
	char buff[1024];
	int err;
	regex_t rgx;

	if((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0){
		regerror(err, &rgx, buff, sizeof(buff));
		printf("%s\n", buff);
		return -1;
	}
	if(regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH){
		return -1;
	}
	return 0;
}

void unispace_separate(char *str) 
	{
		int i = 0;

		char last = 'a';
		char res[strlen(str)];

		res[0] = '\0';

		while (str[i] == ' ') {
			i++;
		}

		while (str[i] != '\0') {
			char ch = str[i];
			if (ch != ' ') {
				strncat(res, &ch, 1);
			} else if(last != ' ') {
					strncat(res, &ch, 1);
			}

			last = ch;
			i++;
		}

		strcpy(str, res);

}


void input_txn_type(char *type, int (*validator) (char *))
	{
		char t[10];

		while(1) {
			printf("> ");
			get_string(t, 10);
			
			if(validator(t) == 0) {
				long ret = strtol(t, NULL, 10);
				*type = (int) ret;
				break;
			}
		
			printf("Invalid input\n");
		}
	}

int validate_type(char *str)
	{
		//regular expression
		char *pattern = "^[0-1]{1}$";
		char buf[1024];
		int err;

		regex_t rgx;

		if ((err = regcomp(&rgx, pattern, REG_EXTENDED)) != 0) {
			 regerror(err, &rgx, buf, sizeof(buf));
			 printf("%s\n", buf);
			 return -1;
		}

		if (regexec(&rgx, str, 0, NULL, 0) == REG_NOMATCH) {
			return -1;
		}	

		return 0;
}

int
cb_party_list(void *list, int ncols, char **values, char **fields){
	party p = {};
	party *ptr = (party *) list;
	Node *node;
	
	char *cat = created_time(ptr->cat);
  char *uat = updated_time(ptr->uat);

	p.id = atoi(values[0]);
	strcpy(p.fname, values[1]);
	strcpy(p.lname, values[2]);
	strcpy(p.phone, values[3]);
	p.amount = atoi(values[4]);
	p.cat = unix_time(values[5]);
	p.uat = unix_time(values[6]);

	node = list_new_node((struct list *) list, (void *) &p);
	list_insert_end((struct list *) list, node);

	return SQLITE_OK;

}	

int
cb_party_struct(void *ptr, int ncols, char **values, char **fields){
	party *p = (party *) ptr;

	p->id = (int)atoi(values[0]);
	strcpy(p->fname, values[1]);
	strcpy(p->lname, values[2]);
	strcpy(p->phone, values[3]);
	p->amount = atoi(values[4]);
	p->cat = unix_time(values[5]);
	p->uat = unix_time(values[6]);

	return SQLITE_OK;


}	



int put_in_txn_struct(void *ptr, int ncols, char **values, char **fields){

	txn *temp = (txn *) ptr;

	temp->id = (int) atoi(values[0]);
	temp->amount = (int) atoi(values[1]);
	temp->cat = unix_time(values[2]);
	temp->type = (int) atoi(values[3]);
	strcpy(temp->desc, values[4]);
	temp->party_id = (int) atoi(values[5]);

	return SQLITE_OK;

}

int put_in_txn_list(void *ptr, int ncols, char **values, char **fields){
	
	Node *node;
	txn temp = {};

	temp.id = (int) atoi(values[0]);
	temp.amount = (int) atoi(values[1]);
	temp.cat = unix_time(values[2]);
	temp.type = (int) atoi(values[3]);
	strcpy(temp.desc, values[4]);
	temp.party_id = (int) atoi(values[5]);

	node = list_new_node((struct list *) ptr, (void *) &temp);
	assert(node != NULL);
	list_insert_end((struct list *) ptr, node);

	return SQLITE_OK;
}






//COLORS FUNCTIONS
void sky()
{
	printf("\x1b[30;106m");
}
void red () {
  printf("\x1b[1;31m");
}
void yellow() {
  printf("\x1b[1;33m");
}
void grey(){
	printf("\x1b[30;47m");
}
void reset () {
  printf("\x1b[0m");
}
void bold(){
	printf("\x1b[1m");
}
void green(){
	printf("\x1b[30;42m");
}
void error() {
	printf("\x1b[37;41m");
	printf("ERROR:");
}
void success(){
	printf("\x1b[37;42m");
	printf("SUCCESS:");
}
void warning(){
	yellow();
	printf("WARNING:");
}


//CENTRE PRINTING FUNCTION

void centreprint(char *s){

	int len = strlen(s);
	int x = 136;

	int sp = x - len;

	for (int i = 0; i < sp / 2; ++i)
	{

		printf(" ");
	}
	printf("%s", s);
	for (int i = 0; i < sp / 2; ++i)
	{
		printf(" ");
	}

}

void print_success(char *s){

	printf("\x1b[1;32m");
	printf("%s", s);
}

void print_error(char *s){
	printf("\x1b[1;31m");
	printf("%s", s);
}

void print_warning(char *s){
	printf("\x1b[1;93m");
	printf("%s", s);
}



