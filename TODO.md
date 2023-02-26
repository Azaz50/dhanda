TODO
------------------

- Generate ID for party/txn
- show party
- search party
- edit party
- list txn
- Remove ID input from txn/create
- Improve txn type input
  [C]redit / [D]ebit :
- show txn

- Fix the HEADING FORMAT of every screen
  DHANDA > PARTY > LIST

- Error handling
  - Show error message
  - Check for error in file operation
  - Handle return value of every function (database layer)


TASK
------------------

### PARTY
- add created at and updated at
- validate add/edit input
	- First/Last name should only contain alpha/num/space
	- Amount should be integer (handle it in input), don't use scanf
	- Phone should be 10 digit number
- store sanitized input 
	- First/Last name should be in title case
- show date/time in list/show screen in ISO format

### TXN
- validate party ID (party must exist)
- store sanitized input 
	- First/Last name should be in title case
- show date/time in list/show screen in ISO format
- change the UI of show screen
- change the order of the fields


- Edit
	- on blank input take old value
	- show hint about what happens when user inputs blank

- Add new function to push error message from db layer to app struct
- Show that error message in UI rendering

### UI
- Add bg color in header and footer
- make space for status bar (show stats on the status bar)
- Need to show the correct statistics on first screen.
-Remove Assertion from everywhere. Handle it.
-Party Edit is not working.
-Delete operation should ask for confirmation first before deleting the record.
-Deleting a party should delete all transactions of that party.
-Regular expression error (regcomp) should be printed using debug_print() function.
-Add pagination command (next, prev, last, first, mid).
-Show Party name instead of Party ID in transaction list screen.
-Show date and time (both) of creation of record in show screen.
-Show last 10 transaction below Party detail in Party Show screen.

