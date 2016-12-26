/**************************************************************************** 
 
                                                        Kyle Sit 
                                                        13 October 2015 
                                Stack Manipulation 
 
File Name:      stack.c 
Description:    This program contains methods for manipulating stacks and are
		used in the driver.c file.  
****************************************************************************/  

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of last used space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "Num_elements check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                        "Attempt to write to a non-existent stack!!!\n";

/* Debug messages. HEX messages used for negative numbers on the stack. */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}
 
/*-------------------------------------------------------------------------- 
Function Name:         delete_Stack 
Purpose:               This functions deallocates memory all memory associated
		       with the stack and sets pointer to null.
Description:           The main component of this method is using the free 
		       function to deallocate memory returned by malloc.
Input:                 @param spp is a pointer to a pointer pointing to a stack 
Result:                At the end of the method we should have freed memory and
		       decremented the stack counter to an appropriate value.
		       Nothing is returned.
--------------------------------------------------------------------------*/ 
void delete_Stack (Stack ** spp) {
	
	/* Check if pointer to pointer is null */
	if( !spp ) {
		
		/* If so print error message and return */
		if( debug ) {
			fprintf(stderr, DELETE_NONEXIST);
		}
		return;
	
	}
	
	/* Check if pointer to stack is null */
	if( !*spp ) {
		
		/* If so print error message and return */
		if( debug ) {
			fprintf(stderr, DELETE_NONEXIST);
		}
		return;
	
	}

	/* Free the memory, set pointer to null, and decrement stack counter */
	free(*spp - STACK_OFFSET);
	*spp = NULL;
	stack_counter--;

}

/*-------------------------------------------------------------------------- 
Function Name:         empty_Stack 
Purpose:               This method empties the stack for the purposes of our
		       program since we don't actually empty each index.
Description:           We reset the stack pointer to the initial value when
		       a stack is created (-1) making it empty.
Input:                 @param this_Stack is the stack pointer to the stack to 
		       to be emptied
Result:                If the stack is not null we will have "emptied" it.
		       No return value.
--------------------------------------------------------------------------*/ 
void empty_Stack (Stack * this_Stack) {
	
	/* Check for null stack passed in */
	if( this_Stack ) {
		/* If the stack exists we reset Stack Pointer to empty value */
		this_Stack[STACK_POINTER_INDEX] = -1;
		return;
	}
	/* If the stack is null we print error message and return */
	else {
		if( debug ) {
			fprintf(stderr, EMPTY_NONEXIST);
		}
		return;
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         isempty_Stack 
Purpose:               This method is used to check if a stack is empty. 
Description:           We verify a stack is empty by checking the stack pointer
		       and if that pointer points to -1, it means we have not
		       touched indeces holding stack values and thus the stack
		       is empty.
Input:                 @param this_Stack is the pointer to the stack to be 
		       checked.
Result:                If the stack is not null then we return true or false
		       depending on whether or not the stack is empty.
                       Return 1 for true and 0 for false.
--------------------------------------------------------------------------*/ 
long isempty_Stack (Stack * this_Stack) {
	
	/* Check for null stack passed in */
	if( this_Stack ) {
		
		/* Check if Stack Pointer points to initial index */
		if( this_Stack[STACK_POINTER_INDEX] == -1 ) {
			/* Return true if it is on initial index */
			return 1;
		}
		else {
			/* Return false indicating stack is not empty */
			return 0;
		}	
	
	}
	
	/* If the stack is null we print error message and return false */
	else {
		if( debug ) {
			fprintf(stderr, ISEMPTY_NONEXIST);
		}
		return 0;
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         isfull_Stack 
Purpose:               This method is used to check if a stack is full. 
Description:           We use stack pointer to see if the last index accessed
		       is the last spot in the stack, if so the stack is full.
Input:                 @param this_Stack is the pointer to the stack to be 
		       checked.
Result:                If the stack is not null then we return true or false
		       depending on whether or not the stack is full.
                       Return 1 for true and 0 for false.
--------------------------------------------------------------------------*/ 
long isfull_Stack (Stack * this_Stack) {
	
	/* Check for null stack passed in */
	if( this_Stack ) {
		
		/* Check if Stack Pointer points to last array index */
		if( this_Stack[STACK_POINTER_INDEX] == 
			(this_Stack[STACK_SIZE_INDEX] - 1) ) {
			/* Return true if it is pointing to last index */
			return 1;
		}
		else {
			/* Return false indicating stack is not full */
			return 0;
		}
	
	}
	
	/* If the stack is null we print error message and return false */
	else {
		if( debug ) {
			fprintf(stderr, ISFULL_NONEXIST);
		}
		return 0;
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         new_Stack
Purpose:               This function allocates a space in memory for the new
		       stack and creates a pointer to that stack.
Description:           We first allocate memory using malloc and then cast that 
                       memory to be assigned to a stack pointer. Using offset
		       we create the first 3 indeces holding stack count, size,
		       and pointer.
Input:                 @param stacksize is the size of the stack wanted. 
Result:                We should have created a new empty stack in memory. 
                       Return a pointer to the new stack.
--------------------------------------------------------------------------*/ 
Stack * new_Stack (unsigned long stacksize) {
	
	/* Allocating memory for a stack as big as stacksize plus 3 initial 
	 * spots for stack count, size, and pointer */
	void* memory = malloc((STACK_OFFSET + stacksize)*sizeof(long));
	
	/* Creating a stack pointer to the stack in memory casted as a stack
	 * pointer.  Adding offset so our pointer starts at 0 index. */
	Stack* this_Stack = (Stack*)memory + STACK_OFFSET;

	/* Instantiating first three values in stack to approriate values */
	this_Stack[STACK_SIZE_INDEX] = stacksize;
	this_Stack[STACK_POINTER_INDEX] = -1;
	stack_counter++; /* increment counter here so stack count is correct */
	this_Stack[STACK_COUNT_INDEX] = stack_counter;
	
	/* Appropriate print and return statements */
	if( debug ) {
		fprintf(stderr, ALLOCATED,(long)stack_counter);
	}
	return this_Stack;

}

/*-------------------------------------------------------------------------- 
Function Name:         num_elements 
Purpose:               Returns the number of elements in the stack 
Description:           We find the last index accessed and add 1 to show number
		       of elements.
Input:                 @param this_Stack is a pointer to the stack we want to 
		       check.
Result:                If the stack is not null we should return the number of 
		       elements in the stack.
--------------------------------------------------------------------------*/ 
long num_elements (Stack * this_Stack) {
	
	/* Check if stack is null */
	if( this_Stack ) {

		/* Return the stack pointer plus 1 to indicate number of 
		 * elements because index is one below */
		return this_Stack[STACK_POINTER_INDEX] + 1;
	
	}
	/* If null we print error message and return false */
	else {
		
		if( debug ) {
			fprintf(stderr, NUM_NONEXIST);
		}
		return 0;
	
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         pop 
Purpose:               We use this method to pop the top value of the stack. 
Description:           We use a passed in long pointer to return the item 
		       on the top of the stack and then decrement our stack
		       pointer so that our access is one below.
Input:                 @param this_Stack is the pointer to the stack we want 
		       to pop.
		       @param item is the long pointer that will point to the 
		       long we want to return.
Result:                We will return the long from the top of the stack
		       and essentially move one position down.
--------------------------------------------------------------------------*/ 
long pop (Stack * this_Stack, long * item) {
	
	/* Check for null stack */
	if( !this_Stack ) {
		
		/* If null print error message and return false */
		if( debug ) {
			fprintf(stderr, POP_NONEXIST);
		}
		return 0;
	
	}
	
	/* Check for null item */
	if( !item ) {
		
		/* If null print error message and return false */
		if( debug ) {
			fprintf(stderr, INCOMING_NONEXIST);
		}
		return 0;
	
	}

	/* Check if stack is empty */
	if( isempty_Stack(this_Stack) ) {
		
		/* If empty we print error message and return false */
		if( debug ) {
			fprintf(stderr, POP_EMPTY);
		}
		return 0;
	
	}

	/* If no errors check true */
	else {
		
		/* Dereference item pointer and decrement stack pointer */
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]];
		this_Stack[STACK_POINTER_INDEX]--;
		
		/* Print debug statement and return casted item */
		if( debug ) {
			fprintf(stderr, POP, (long)stack_counter, *item);
		}
		return (long)item;
	
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         push
Purpose:               We use this method to push items onto the stack. 
Description:           We take the long passed in and place it in the next 
		       index of the stack and increment our stack pointer
		       appropriately.
Input:                 @param this_Stack is a pointer to the stack we want to 
		       push onto.
		       @param item is the long we want to push onto the stack.
Result:                If the stack is not null we should end up with a stack
		       containing the item at its last index accessed.
--------------------------------------------------------------------------*/ 
long push (Stack * this_Stack, long item) {
	
	/* Check for null stack */
	if( this_Stack ) {
		
		/* Check if the stack is full */
		if( !isfull_Stack(this_Stack) ) {
			
			/* Initialize long variable to hold the index we want 
			 * to push on to. */
			long sp = this_Stack[STACK_POINTER_INDEX] + 1;
			
			/* Push item onto stack and increment stack pointer. */
			this_Stack[sp] = item;
			this_Stack[STACK_POINTER_INDEX]++;
			
			/* Print debug statement and return true. */
			if( debug ) {
				fprintf(stderr,PUSH,(long)stack_counter,item);
			}
			return 1;
		}
		
		/* If the stack is full then we cannot push onto it and we 
		 * print an error message then return false. */
		else {
			if( debug ) {
				fprintf(stderr, PUSH_FULL);
			}
			return 0;
		}
	
	}
	
	/* If stack is null we print error and return false */
	else {
		if( debug ) {
			fprintf(stderr, PUSH_NONEXIST);
		}
		return 0;
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         top 
Purpose:               This method is used to find out what long is at the
		       top of the stack.
Description:           We find the last index accessed and return the long at
		       that index to the user.
Input:                 @param this_Stack is a pointer to the stack to be 
		       checked.
		       @param item is a pointer we use to point to the long
		       at the top of the stack.
Result:                If no parameters are null we should return the long
		       at the top of the stack.
--------------------------------------------------------------------------*/ 
long top (Stack * this_Stack, long * item) {
	
	/* Check if stack is null */
	if( !this_Stack ) {
		
		/* Stack is null so we print error statement and return false */
		if( debug ) {
			fprintf(stderr, TOP_NONEXIST);
		}
		return 0;
	
	}

	/* Check if item is null */
	if( !item ) {

		/* Item is null so we print error statement and return false */
		if( debug ) {
			fprintf(stderr, INCOMING_NONEXIST);
		}
		return 0;

	}
	
	/* Check if stack is empty */
	if( isempty_Stack(this_Stack) ) {
		
		/*Stack is empty so we print error statement and return false*/
		if( debug ) {
			fprintf(stderr, TOP_EMPTY);
		}
		return 0;
	
	}

	/* If all errors check out */
	else {
		
		/* Dereference pointer to top spot in stack */
		*item = this_Stack[this_Stack[STACK_POINTER_INDEX]];
		
		/* Print debugger and return the long at the top of the stack */
		if( debug ) {
			fprintf(stderr, TOP, (long)stack_counter, *item);
		}
		return *item;
	
	}

}

/*-------------------------------------------------------------------------- 
Function Name:         write_Stack
Purpose:               This method writes out each item in the stack. 
Description:           Using a for loop we access each long at the indeces from
		       beginning to end and print them to the stream.
Input:                 @param this_Stack is a pointer to the stack we want to 
		       print out.
		       @param stream is a pointer to the stream we want to print
		       on.
Result:                If no parameters are null we should end up with a row
		       of the elements printed to the stream.
--------------------------------------------------------------------------*/ 
FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        /* Check for a null stack */
	if (this_Stack == NULL) {
                
		/* Print error message and return stream */
		if( debug ) {
			fprintf (stderr, WRITE_NONEXIST_STACK);
                }
		return stream;
        
	}

	/* Check if the stream is null */
        if (stream == NULL) {
               
	        /* Print error messagea and return stream */
		if( debug ) {
			fprintf (stderr, WRITE_NONEXIST_FILE);
                }
		return stream;
        
	}
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        num_elements (this_Stack));

        /* For loop used to print each element of the stack */
	for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < num_elements (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
