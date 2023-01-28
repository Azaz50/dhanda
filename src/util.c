#include <dhanda/dhanda.h>
#include <dhanda/util.h>
#include <regex.h>
#include <dhanda/party.h>

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

void title_case(char *str){
	str[0] = toupper(str[0]);
	for(int i = 1; str[i] != '\0'; ++i){
		if(str[i] == ' '){
			str[i+1] = toupper(str[i+1]);

		}
	}
}

/*void get_time(time_t cat){
	
    struct tm *t = localtime(&cat);
    char iso_time[100];
    strftime(iso_time, sizeof iso_time, "%Y-%m-%d", t);
    printf(" CREATED AT : %s\n", iso_time);

}
*/

void created_at(time_t t) 
{
	struct tm *tm;
	char timestr[100];


	tm = localtime(&t);

	strftime(timestr, sizeof(timestr), "%Y-%m-%d %T", tm);

	printf("%s\n", timestr);

}

void updated_at(time_t t)
{
	struct tm *tm;
	char timestr[100];


	tm = localtime(&t);

	strftime(timestr, sizeof(timestr), "%Y-%m-%d %T", tm);

	printf("%s\n", timestr);

}


void input_pid(dhanda *app, int *pid, int (*validator)(char *)){
	char pd[10];
	struct party result;

	while(1){
		printf("> ");
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



