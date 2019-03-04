#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
	// Holds the return value
	int return_val = 0;
	// Allocates memory for new group
	Group *new_gp = (Group*) malloc (sizeof(Group));
	// Checks if malloc properly
    if(new_gp == NULL){
    	perror("ERROR: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Allocates memory for name
    new_gp->name = (char*) malloc(strlen(group_name)+1);
    // Check if name was malloced properly
    if(new_gp->name == NULL){
    	perror("ERROR: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Copys name into the new group
    strcpy(new_gp->name, group_name);
    // Sets all the group properties
    new_gp->users = NULL;
    new_gp->xcts = NULL;
    new_gp->next = NULL;
    // Checks if the list has any groups
    if(*group_list_ptr == NULL){
    	*group_list_ptr = new_gp;
    	return_val = 0;
    } else {
    	// Loops through list of groups
    	// to add in the new group
    	Group *curr = *group_list_ptr;
    	while (curr != NULL){
    		// If the group already exist
    		if (strcmp(curr->name, group_name) == 0){
    			free(new_gp->name);
    			free(new_gp);
    			return_val = -1;
    		// Adding the new group in
    		} else if (curr->next == NULL){
    			curr->next = new_gp;
    			// Moves to next pointer
    			curr = curr->next;
    			// Success in adding new group
    			return_val = 0;
    		}
    		// Cotinues to loop through
    		curr = curr->next;
    	}
    }
    return(return_val);
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
	// Checks if the list is empty
	if(group_list == NULL){
		// Error catch
		perror("ERROR: Points to NULL");
		printf("NO GROUP(S) \n");
	} else {
		// Loops through list
		Group *curr = group_list;
		printf("Group or Groups: \n");
		while(curr != NULL){
			// Prints all groups in list
			printf("%s \n", curr->name);
			curr = curr->next;
		}
	}
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
	// Holds the return group
	Group *return_val = NULL;
	// Pointer to loop through list
    Group *curr = group_list;
    while(curr != NULL){
    	// Checks if the group does exist
    	if(strcmp(curr->name, group_name) == 0){
    		return_val = curr;
    	}
    	// Keeps checking next group
    	curr = curr->next;
    }
    // Returns group or null if not found
	return(return_val);
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
	// Holds the return value
	int return_val = 0;
	// Allocates memory for the new user
    User *new_us = (User*) malloc(sizeof(User));
    // Checks if it malloced properly
    if(new_us == NULL){
    	perror("ERROR: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Allocates space for the name
    new_us->name = (char*) malloc(strlen(user_name)+1);
    // Checks if it malloced properly
    if(new_us->name == NULL){
    	perror("ERROR: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Copys name into the new user
    strcpy(new_us->name, user_name);
    // Sets users properties to deafult
    new_us->balance = 0.0;
    new_us->next = NULL;
    // Finds the previous user to add new user
    User *previous_us = find_prev_user(group, user_name);
    // If previous user then adds user into list
    if(previous_us != NULL){
    	// The user already exist in list so frees memory
    	free(new_us->next);
    	free(new_us);
    	return_val = -1;
    } else {
    	// Holds the new user
    	previous_us = new_us;
    	// Saves the current user list
    	User *current = group->users;
    	// Sets the previous user as first
    	group->users = previous_us;
    	// Then adds the rest of the users to list
    	group->users->next = current;
    	return_val = 0;
    }
    // Return value of 0 for sucess or -1 is already in list
    return(return_val);
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
	// Holds the return value
	int return_val = 0;
	// Finds the previous user for the user to remove
	User *previous_us = find_prev_user(group, user_name);
	// If the user is in the list
	if(previous_us != NULL){
		// Checks first if previous is the one to remove
		if(strcmp(previous_us->name, user_name) == 0) {
			// Sets the list without user
			group->users = previous_us->next;
			// Free memory of User
			free(previous_us->name);
			free(previous_us);
		} else {
			// If the user the next node then removes that user
			User *curr = previous_us->next;
			// Sets the list without the user
			previous_us->next = previous_us->next->next;
			// Frees the memory of the user
			free(curr->name);
			free(curr);
		}
		// Return value for sucess
		return_val = 0;
	} else {
		// If no user existed error
		return_val = -1;
	}
	// return value 0 is sucess and -1 if no user
	return(return_val);
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
	// Checks if no users
	if(group->users == NULL){
		perror("Error: Points to NULL");
		printf("NO USERS \n");
	} else {
		// Holds the list of users
		User *curr = group->users;
		printf("User or Users: \n");
		// Loops through users and prints
		while(curr != NULL){
			printf("%s \n", curr->name);
			// Next user in list
			curr = curr->next;
		}
	}
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
	// Holds the return value
	int return_val = 0;
	// Finds if the user does exist
    User *previous_us = find_prev_user(group, user_name);
    // User is in the list
    if(previous_us != NULL){
    	// Heading of Balance
    	printf("Balance: \n");
    	// Checks if user is previous user
    	if(strcmp(previous_us->name, user_name)==0){
    		// Prints balance of user
    		printf("%.2f of %s \n", previous_us->balance, previous_us->name);
    		// Return value of success
    		return_val = 0;
    	} else if(strcmp(previous_us->next->name, user_name)==0){
    		// Checks if user if next from previous
    		previous_us = previous_us->next;
    		// Prinst the user balance
    		printf("%.2f of %s \n", previous_us->balance, previous_us->name);
    		// Return value of sucess
    		return_val = 0;
    	}
    } else{
    	// Return value if user does not exist
    	return_val = -1;
    }
    // Return value 0 is success and -1 if no user
    return(return_val);
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
	// Holds the return value
    int return_val;
    // if the group has users
	if(group->users != NULL){
		// Heading for users
    	printf("User or Users: \n");
    	// Holds the next user
    	User *curr = group->users->next;
    	// Prints the first user in the group
    	printf("%s \n", group->users->name);
    	// Loops through all users to see if balance match
    	while (curr->balance == group->users->balance){
    		// Prints the users with same balance
    		printf("%s \n", curr->name);
    		// Moves to each user
    		curr = curr->next;
    	}
    	// Return value for sucess
    	return_val = 0;
	} else {
		// Return value of no users
		return_val = -1;
	}
	// Returns 0 if success and -1 if no users
	return(return_val);
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    // Holds the previous user
	User *return_us = NULL;
	// Current list of users
	User *curr = group->users;
	// if user list is not empty
    if(curr != NULL){
    	// Loops through to find previous node from user
    	while ((curr != NULL) && (curr->next != NULL)){
    		// Checks the first in the list
    		if(strcmp(curr->name, user_name) == 0){
    		    // Return is the head
    		    return_us = curr;
    		}
    		// Compares the next nodes from curr
    		else if (strcmp(curr->next->name, user_name) == 0){
    			// Previous node has been found
    		    return_us = curr;
    		}
    		// Loops through every user
    		curr = curr->next;
    	}
    } else{
    	// If no users in list
    	return_us = NULL;
    }
    // Returns the previous or Null if user does not exist
    return(return_us);
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
	// Holds the return value
    int return_val;
    // Allocates the memory for the transaction
	Xct *new_trans = (Xct*) malloc(sizeof(Xct));
	// Checks for malloc error
    if(new_trans == NULL){
    	perror("Error: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Allocates the memory for transaction user
    new_trans->name = (char*) malloc(strlen(user_name)+1);
    // Checks for malloc error
    if (new_trans->name == NULL){
    	perror("Error: malloc failed");
    	// Terminates program
    	exit(0);
    }
    // Copys user name into transcation
    strcpy(new_trans->name, user_name);
    // Sets the transcation properties
    new_trans->amount = amount;
    new_trans->next = NULL;
    // Find if user is in the list of user
    User *previous_us = find_prev_user(group, user_name);
    // If user is in the list
    if(previous_us != NULL){
    	// Holds all transactions
    	Xct *curr = group->xcts;
    	// Adds new transaction
    	group->xcts = new_trans;
    	// Places all previous after the recent
    	group->xcts->next = curr;
    	// Compares if user is head
    	if(strcmp(previous_us->name, user_name) == 0){
    		// Assigns the new balance onto user
    		previous_us->balance = previous_us->balance + amount;
    		// Holds return value for success
    		return_val = 0;
    	} else{
    		// Assigns the new balance onto the next user node
    		previous_us->next->balance = previous_us->balance + amount;
    		// Holds return value for success
    		return_val = 0;
    	}
    } else{
    	// Frees the transaction as user does not exist
    	free(new_trans->name);
    	free(new_trans);
    	// Return value to hold no user exist
    	return_val = -1;
    }
    // Returns 0 if success and -1 if no users
    return(return_val);
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
	// Checks if group has transactions
	if(group->xcts != NULL){
		// Holds the transcation list
		Xct *curr = group->xcts;
		// Header
		printf("Recent Transaction: \n");
		// Holds count variable
		int x;
		// Loops prints for the number asked for
		for(x = 0; x < nu_xct; x++){
			if(curr != NULL){
				// Prints the first transaction and name
				printf("%s transaction: %.2f \n", curr->name, curr->amount);
			} else{
				// Breaks once no more transaction
				break;
			}
			// Next transaction
			curr = curr->next;
		}
	} else{
		// Error message since no transactions
		perror("Error: Point to NULL");
		printf("No transactions \n");
	}
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
	// DID NOT HAVE TIME TO FINISH!!!!!!!!
}

