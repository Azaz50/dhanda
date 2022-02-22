#include <dhanda/dhanda.h>
#include <dhanda/ui.h>
#include <dhanda/party.h>
#include <dhanda/txn.h>
#include <dhanda/cursor.h>

/* Theses functions are like controller */
static void dhanda_command_party_home(dhanda *app);
static void dhanda_command_txn_home(dhanda *app);
static void dhanda_command_exit(dhanda *app);
static void dhanda_command_add(dhanda *app);
static void dhanda_command_list(dhanda *app);
static void dhanda_command_show(dhanda *app);
static void dhanda_command_edit(dhanda *app);
static void dhanda_command_delete(dhanda *app);
static void dhanda_command_back(dhanda *app);
static void dhanda_command_search(dhanda *app);

static void dhanda_resolve_add_renderer(dhanda *app);
static void dhanda_resolve_list_renderer(dhanda *app);
static void dhanda_resolve_show_renderer(dhanda *app);
static void dhanda_resolve_search_renderer(dhanda *app);
static void dhanda_resolve_edit_renderer(dhanda *app);
static void dhanda_resolve_delete_renderer(dhanda *app);


static struct {
	char *cmd;
	void (*handle)(dhanda *);
	void (*renderer)(dhanda *);
} commands[] = {
	{ "p", 				dhanda_command_party_home, 	ui_party_list },
	{ "party", 			dhanda_command_party_home, 	ui_party_list },
	{ "t", 				dhanda_command_txn_home, 		ui_txn_list },
	{ "txn", 			dhanda_command_txn_home, 		ui_txn_list },
	{ "back", 			dhanda_command_back },

	/* Theses commands are contextual based (meaning theses commands are
	 * same for other screens), so it will inspect which function to call
	 * based on context inside which it is run */
	{ "add", 			dhanda_command_add, 		dhanda_resolve_add_renderer },
	{ "list", 			dhanda_command_list, 		dhanda_resolve_list_renderer },
	{ "show", 			dhanda_command_show, 		dhanda_resolve_show_renderer },
	{ "search", 		dhanda_command_search, 		dhanda_resolve_search_renderer },
	{ "edit", 			dhanda_command_edit, 		dhanda_resolve_edit_renderer },
	{ "delete", 		dhanda_command_delete, 		dhanda_resolve_delete_renderer },


	{ "exit", 				dhanda_command_exit },

	{ NULL, NULL }
};

static int quit;

int main(int argc, char *argv[])
{
	int len;
	struct dhanda app = {};
	char *line;

	line = app.cmd.cmdline;
	dhanda_init_app(&app);

	ui_home(&app);
	while (!quit) {
		/* 1. Show command line help for the current screen */
		dhanda_app_print_helpline(&app);

		/* 2. Read command line input */
		len = get_line(line, MAXLINE);
		app.cmd.cmdline_len = len;
		if (len >= MAXLINE) {
			fprintf(stderr, "Error: command line is unexpectedly long.\n");
			continue;
		}
		dhanda_parse_cmd_line(&app.cmd);

		/* 3. Handle the command (Update app logic) */
		dhanda_app_cmd_handle(&app);

		/* 4. Render the screen */
		if (!quit) {
			dhanda_app_render(&app);
			/* @TODO Clean up the app state, so that renderer can render correct data on screen */
		}
	}
	//ui_party_list(&app);
	//ui_party_show(&app);
	//ui_party_create(&app);
	//ui_party_edit(&app);

	return 0;
}

void
dhanda_init_app(struct dhanda *app)
{
	int ret;
	struct stat buf;
	struct passwd *pw;
	char base_path[256], party_filepath[256], txn_filepath[256];

	/* Setup data storage directory (in user's home dir) */
	if ((pw = getpwuid(getuid())) == NULL) {
		perror("getpwuid error");
		exit(EXIT_FAILURE);
	}
	/* @DANGEROUS BUFFER OVERFLOW upto below */
	strcpy(base_path, pw->pw_dir);
	strcat(base_path, "/");
	strcat(base_path, DHANDA_DATA_STORAGE);
	if ((ret = stat(base_path, &buf)) == -1 && errno == ENOENT) {
		if (mkdir(base_path, 0700) == -1) {
			perror("mkdir error");
			exit(EXIT_FAILURE);
		}
	} else if (ret == -1) {
		perror("Storage create error");
		exit(EXIT_FAILURE);
	}

	app->context = SCREEN_HOME;

	strcpy(party_filepath, base_path);
	strcat(party_filepath, "/");
	strcat(party_filepath, DHANDA_PARTY_DB_FILE);

	strcpy(txn_filepath, base_path);
	strcat(txn_filepath, "/");
	strcat(txn_filepath, DHANDA_TXN_DB_FILE);

	app->party_fp = fopen(party_filepath, "r+b");
	app->txn_fp   = fopen(txn_filepath, "r+b");

	if (app->party_fp == NULL && errno == ENOENT)
		app->party_fp = fopen(party_filepath, "w+b");
	if (app->txn_fp == NULL && errno == ENOENT)
		app->txn_fp = fopen(txn_filepath, "w+b");

	if (app->party_fp == NULL) {
		fprintf(stderr, "Error in file '%s': %s\n", party_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (app->txn_fp == NULL) {
		fprintf(stderr, "Error in file '%s': %s\n", txn_filepath, strerror(errno));
		exit(EXIT_FAILURE);
	}

	app->party_list = list_create(sizeof(party));
	app->txn_list 	= list_create(sizeof(txn));

	if (app->party_list == NULL) {
		perror("Failed to create party list");
		exit(EXIT_FAILURE);
	}
	if (app->txn_list == NULL) {
		perror("Failed to create txn list");
		exit(EXIT_FAILURE);
	}
}

void
dhanda_app_print_helpline(dhanda *app)
{
	/* @TODO According to screen context print correct command help line */
	printf("CMD: ");
	printf("p[arty]    t[xn]");
	puts("");
	printf("> ");
}

void
dhanda_parse_cmd_line(cmdline *cmd)
{
	char *p, *q;
	int i;

	cmd->cmdline[cmd->cmdline_len - 1] = '\0';
	p = cmd->cmdline;
	i = 0;
	while (i < MAXCMDARG - 1 && (q = strchr(p, ' '))) {
		cmd->argv[i++] = p;
		*q = '\0';
		p = q + 1;
	}
	cmd->argv[i++] = p;
	cmd->argc = i;
	cmd->cmd = cmd->argv[0];
	cmd->argv[cmd->argc] = NULL;
}

void
dhanda_app_cmd_handle(dhanda *app)
{
	for (int i = 0; commands[i].cmd; ++i) {
		if (strcmp(app->cmd.argv[0], commands[i].cmd) == 0) {
			app->renderer = commands[i].renderer;
			commands[i].handle(app);
			return;
		}
	}

	fprintf(stderr, "Error: command '%s' not found\n", app->cmd.argv[0]);
}

void
dhanda_app_render(dhanda *app)
{
	/* Clear the screen */
	clear();
	if (app->renderer) {
		app->renderer(app);
	}
}

static void
dhanda_command_party_home(dhanda *app)
{
	party_filter filter = {
		.page=1,
		.items=10,
	};
	party_get(app, filter, app->party_list);
	/* Set party context (since user entered in party screen) to perform
	 * operations on party */
	app->context = SCREEN_PARTY;
}

static void
dhanda_command_txn_home(dhanda *app)
{
	txn_filter filter = {
		.page=1,
		.items=10,
	};
	txn_get(app, filter, app->txn_list);
	app->context = SCREEN_TXN;
}

static void
dhanda_command_back(dhanda *app)
{
}

static void
dhanda_command_exit(dhanda *app)
{
	/* @TODO Clean app */
	quit = 1;
}

static void
dhanda_command_add(dhanda *app)
{
	party p;
	txn t;

	switch (app->context) {
		case SCREEN_PARTY:
			ui_party_create(app);

			party_add(app, &p);
			/* @TODO Add this party in app->party_list */
			break;

		case SCREEN_TXN:
			ui_txn_create(app);
			txn_add(app, &t);
			break;
	}
}

static void
dhanda_command_list(dhanda *app)
{
}

static void
dhanda_command_show(dhanda *app)
{
}

static void
dhanda_command_edit(dhanda *app)
{
}

static void
dhanda_command_delete(dhanda *app)
{
}

static void
dhanda_command_search(dhanda *app)
{
}

static void
dhanda_resolve_add_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_create; break;
		case SCREEN_TXN: 	app->renderer = ui_txn_create; break;
		default: app->renderer = NULL;
	}
}

static void
dhanda_resolve_list_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_list; break;
		case SCREEN_TXN: 	app->renderer = ui_txn_list; break;
		default: app->renderer = NULL;
	}
}

static void
dhanda_resolve_show_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_show; break;
		case SCREEN_TXN: 	app->renderer = ui_txn_show; break;
		default: app->renderer = NULL;
	}
}

static void
dhanda_resolve_search_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_list; break;
		case SCREEN_TXN: 	app->renderer = ui_txn_list; break;
		default: app->renderer = NULL;
	}
}

static void
dhanda_resolve_edit_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_edit; break;
		default: app->renderer = NULL;
	}
}

static void
dhanda_resolve_delete_renderer(dhanda *app)
{
	switch (app->context) {
		case SCREEN_PARTY: 	app->renderer = ui_party_delete; break;
		default: app->renderer = NULL;
	}
}


