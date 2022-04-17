#include<stdio.h>
#include<stdlib.h>

struct node //block information
{
	int id;
	int start;
	int end;
	struct node *link;
} *block_list = NULL;//list pointer for allocated blocks

typedef struct node block_type;

//global variables for the simulator
int phy_memory_size;
int hole_algo;
int remaining;
int best_of_all;

void intialize() 
{
	//Initialiser for memory size and free memory,also choose hole fitting, can be thought as a constructor
	printf ("Enter size of physical memory: ");
	scanf ("%d", &phy_memory_size);
	remaining = phy_memory_size;
	best_of_all = phy_memory_size;

	printf ("Enter hole-fitting algorithm (0=first fit, 1=best_fit): ");
	scanf ("%d", &hole_algo);
	return;
}

void printAllocationTable()
{
	block_type *next_block = block_list;
	// print table containing block id, starting address, ending address and actual memory address

	printf ("ID\tStart\tEnd(not inclusive)\n");
	printf ("______________________________\n");

	while (next_block != NULL)
    {
    	printf ("%d\t%d\t%d\t\n", next_block->id, next_block->start,next_block->end);
    	next_block = next_block->link;
    }
	return;
}


void allocate()
{

	int block_id, block_size;
	block_type *new_block, *current_block, *best_block;
	int hl_start, hl_end;
	int best_start, best_end;
	int large_enough_hole = 0;

	printf ("Enter block id: "); //basically serves as the variable name for pointer to this location, used for free()
	scanf ("%d", &block_id);
	printf ("Enter block size: ");
	scanf ("%d", &block_size);

	if (block_size > remaining)
    {
    	printf ("Cannot fit the block - not enough memory!");
    	return;
    }

	new_block = (block_type *) malloc (sizeof (block_type));
	new_block->id = block_id;

	if (block_list == NULL) //empty allocation list
	{
    	new_block->start = 0;
    	new_block->end = block_size;
    	new_block->link = NULL;
    	block_list = new_block;
		best_of_all -= block_size;	
    	remaining -= block_size;	
    	printAllocationTable ();
    	return;
    }
  //else traverse list until either appropriate hole is found or the end of the list is reached
	current_block = block_list;

	while (current_block != NULL)
    {
		if (block_id == current_block->id)
		{
			printf ("Duplicate id\n");
			return;
		}

      	hl_start = current_block->end;

      	if (current_block->link != NULL)
		{
			hl_end = current_block->link->start;
		}
		else
		{
	  		hl_end = phy_memory_size;
		}

    	if (block_size <= (hl_end - hl_start))
		{
	  		large_enough_hole = 1;
	  		if (hole_algo != 0)
	    	{
	    		//printf ("best so far: %d\n", best_of_all);
	      		if ((hl_end - hl_start) <= best_of_all)
				{	
		  			best_of_all = hl_end - hl_start;
		  			best_start = hl_start;
		  			best_end = hl_end;
		  			best_block = current_block;
				}
	      		
	    	}
	  		//best fit
	  		else
	    	{
	    		new_block->start = hl_start;
	      		new_block->end = hl_start + block_size;
	      		remaining -= block_size;
				new_block->link = current_block->link;
				current_block->link = new_block;
				printAllocationTable ();
				return;
	      		
	    	}
		}
    	current_block = current_block->link;
    }       

	if (large_enough_hole == 0)
    {
    	printf ("No hole large enough");
    	return;
    }

	new_block->start = best_start;
	new_block->end = best_start + block_size;
	new_block->link = best_block->link;
	best_block->link = new_block;
	remaining -= block_size;
	printAllocationTable ();
	return;
}

void f_block()//free block
{
	block_type *current_block = block_list;
	block_type *prev_block;
	int block_id;
	current_block = block_list;

	if (current_block == NULL)
    {
    	printf ("There are no blocks");
    	return;
    }

	printf ("Enter block id: ");
	scanf ("%d", &block_id);

	while ((current_block != NULL) && (block_id != current_block->id))//linear search till we find block id in allocation list
    {
		prev_block = current_block;
    	current_block = current_block->link;
    }
    //if block_id found, free it and add memory to free(this is where reducing  will be done)
  	if (current_block != NULL)
    {	prev_block->link = current_block->link;
      	remaining += current_block->end - current_block->start;
      	best_of_all += remaining;
      	free (current_block);
    }
  	else
    {
    	printf ("\nCould not find block ID\n");
    }//if end of linked list reached, print block id not found
  	printAllocationTable ();
  	return;
}

void reduce()
{
  	block_type *current_block;
  	int cr_size;//current size
  	int prev_end = 0;

  	current_block = block_list;
  	while (current_block != NULL)
    {
      	cr_size = current_block->end - current_block->start;	
      	current_block->start = prev_end;
      	current_block->end = current_block->start + cr_size;
      	prev_end = current_block->end;
      	current_block = current_block->link;
    }

  	printAllocationTable ();
  	return;
}

void End(block_type * node) //destructor
{

  	if (node != NULL)
    {
    	End(node->link);
      	free(node);
      	
    }
  	else
    {
      	return;
    }
  	return;
}

int main ()
{
  	int flag;
  	do
    {
      	printf ("\n Memory Allocation Selection\n");
      	printf ("________________________________\n");
      	printf ("1) Enter parameters\n");
      	printf ("2) Allocate memory in block\n");
      	printf ("3) Deallocate memory in block\n");
      	printf ("4) To reduce the memory\n");
      	printf ("5) Quit\n");

      	printf ("\nEnter Selection: ");
      	scanf ("%d", &flag);
      	switch (flag)
		{
		case 1:
	  		intialize();
	  		break;
		case 2:
	  		allocate();
		  	break;
		case 3:
	  		f_block();//free block
			break;
		case 4:
			reduce();
	  		break;
		}
	}while (flag != 5);
  	End(block_list);
  	printf ("Quit");
  	return 0;
}
