#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STR 2048

/**********/
/*
data structures
*/

typedef enum {false, true} bool;
typedef struct node* pntr;
struct node
{
	char* element;
	pntr next;
};
typedef struct node node;


/**********/
/*
Stack
*/
typedef pntr Stack;
bool push(Stack S, char* element)
{
	pntr tmp = (pntr) malloc(sizeof(node));
	
	if(tmp != NULL)
	{
		int n = strlen(element);
		tmp->element = (char*)calloc(n, sizeof(char));
		strcpy(tmp->element, element);
		
		tmp->next = S->next;
		S->next = tmp;
		
		return true;
	}else
	{
		return false; // out of memory
	}
}
pntr top(Stack S)
{
	return S->next;
}
pntr pop(Stack S)
{
	pntr ret = top(S);
	
	if(ret != NULL/*!is_empy(S)*/)
	{
		S->next = ret->next;
		ret->next = NULL;
	}

	return ret;
}
bool is_empty(Stack S)
{
	return (S->next == NULL)?(true):(false);
}

/**********/
/*
List
*/
struct list_ref
{
	pntr head;
	pntr tail;
};
typedef struct list_ref list_ref;
typedef struct list_ref* List;
bool append_list_node(List l, char* element)
{
	pntr tmp = (pntr) malloc(sizeof(node));
	
	if(tmp != NULL)
	{
		tmp->element = (char*)calloc(strlen(element), sizeof(char));
		strcpy(tmp->element, element);
		
		tmp->next = NULL;
		l->tail->next = tmp;
		l->tail = tmp;
		
		return true;
	}else
	{
		return false; // out of memory
	}
}
bool is_empty_list(List l)
{
	return (l->head == l->tail)?(true):(false);
}

/**********/
/*
Prints the inputs and the requested operation on them.
*/
void print_to_consol(List inputs, List l)
{
	pntr tmp1, tmp2;
	tmp1 = inputs->head->next;
	tmp2 = l->head->next;
	
	while(tmp1 != NULL)
	{
		printf("%s >> %s", tmp1->element, tmp2->element);
			tmp1 = tmp1->next;
			tmp2 = tmp2->next;
		printf("\n");
	}
}

/**********/
/*
Takes a header node and frees all following nodes from the memory
*/
void clear_nodes(pntr head)
{
	pntr tmp;
	while(head->next != NULL)
	{
		tmp = head->next;
		head->next = tmp->next;
		free(tmp->element);
		free(tmp);
	}
}

/**********/

//Used to remove any character at i, then moving all the elements in the string str one index backwards
//The resulting strings is held temporarily by tmp
//At the end str is freed back to the memory and the pointed at the value at tmp
void remove_char(char **str, int i, int n)
{
	char *tmp = (char*)calloc(n-1, sizeof(char));

	for(int j=0; j<n; j++)
	{
		if(j<i)
		{
			tmp[j] = (*str)[j];
		}else if(j==i)
		{}else if(j>i)
		{
			tmp[j-1] = (*str)[j];
		}
	}

	free(*str);
	*str = tmp;
}

/**********/
/*
Checks whether the given equation is valid or not. Also modifies it removing white spaces from it.
*/
bool check_valid(char **line)
{
	int bracket_count = 0, plus_minus_count = 0, mult_div_count = 0, deci_count = 0;
	char prev = '\0';
	
	int line_len = strlen(*line);
	
	// if the lines scanned ended with '\n' removes it and replaces it by '\0'
	if((*line)[line_len-1] == '\n')
	{
		(*line)[(line_len--)-1] = '\0';
	}
	
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
	#else
		if((*line)[line_len-1] == '\r')
		{
			(*line)[(line_len--)-1] = '\0';
		}
	#endif
	
	// remove white spaces before the doing anything
	for(int i=0; i<line_len; i++)
	{
		if((*line)[i] == ' ' || (*line)[i] == '\t')
		{
			remove_char(line, i--, line_len--); // TODO check if this is a correct use
		}
	}
	
	/* if the line begins with a '+' || '-' as a sign for the following number it's allowed (explicitly stating +num or -num) */
	/* if it begins with two consecutive + || - it's false */
	if( ((*line)[0] == '+' || (*line)[0] == '-') && ((*line)[1] == '+' || (*line)[1] == '-') )
	{
		return false;
	}
	
	/* if it begins with a '*' || '/' with not operator */
	if( (*line)[0] == '*' || (*line)[0] == '/' )
	{
		return false;
	}
	
	/* If the line ends with an operation */
	if((*line)[line_len-1] == '+' || (*line)[line_len-1] == '-' || (*line)[line_len-1] == '*' || (*line)[line_len-1] == '/' )
	{
		return false;
	}
	
	for(int i=0; i<line_len; i++)
	{
		switch((*line)[i])
		{
			case '(' :
				if(prev == ')') /* if the bracket is closed with nothing in it */
				{
					return false;
				}else
				{
					if( (*line)[i+1] == '*' || (*line)[i+1] == '/' )
					{
						return false;
					}
					
					if( ((*line)[i+1] == '+' || (*line)[i+1] == '-') && ((*line)[i+2] == '+' || (*line)[i+2] == '-') )
					{
						return false;
					}
					
					
					bracket_count++;
					
					plus_minus_count	= 0;
					mult_div_count		= 0;
					deci_count = 0;
				}
				break;
			case ')' :
				if(prev == '(') /* if no operation separates between a close and open bracket */
				{
					return false;
				}else
				{
					if(mult_div_count != 0 || plus_minus_count != 0 || deci_count != 0) /* if bracket is closed before an operand for the last operation is present */
					{
						return false;
					}
					bracket_count--;
				}
				break;
			case '+' :
			case '-' :
				if( (plus_minus_count > 2) || (mult_div_count != 0 && plus_minus_count != 0) )
				{
					return false;
				}else
				{
					plus_minus_count++;
					deci_count = 0;
				}
				break;
				
			case '*' :
			case '/' :
				if( (mult_div_count != 0) || (plus_minus_count != 0) )
				{
					return false;
				}else
				{
					mult_div_count++;
					plus_minus_count = 0;
					deci_count = 0;
				}
				break;
			
			default  :
				if( ( (*line)[i] >= '0' && (*line)[i] <= '9' ) || (*line)[i] == '.' /*decimal point*/ )
				{
					plus_minus_count	= 0;
					mult_div_count		= 0;
					
					if( (*line)[i] == '.' )
					{
						if(deci_count != 0)
						{
							return false;
						}else
						{
							deci_count++;
						}
					}else
					{
						deci_count = 0;
					}
				}else
				{ /* if something isn't a number or an operation or a decimal point */
					return false;
				}
		}
		
		prev = (*line)[i];
	}
	
	/* if any brackets are left open */
	if(bracket_count != 0)
	{
		return false;
	}
	
	
	return true;
}


/**********/
/*
Decides whether op1 has a higher or equal priority to op2.
*/
bool prec(const char op1, const char op2)
{
/**/
/*
(op1 >= op2)?(true):(false);
*/
/**/
	switch(op1)
	{
		case '+' :
		case '-' :
			switch(op2)
			{
				case '+' :
				case '-' :
					return true;
					break;
				case '*' :
				case '/' :
					return false;
					break;
				
				case '(' :
					return false;
					break;
				case ')' :
					return true;
					break;
			}
			break;
		case '*' :
		case '/' :
			switch(op2)
			{
				case '+' :
				case '-' :
					return true;
					break;
				case '*' :
				case '/' :
					return true;
					break;
				
				case '(' :
					return false;
					break;
				case ')' :
					return true;
					break;
			}
			break;
		
		case '(' :
			return false;
			break;
	}
}


/**********/
/*
Takes a postfix equation and uses stack to evaluate it.
*/
double evaluate_wStack(char * postfix)
{
	Stack S = (pntr)malloc(sizeof(node));
		S->next = NULL;
	
	pntr tmp;
	
	char* tkn, tmp_str;
	double num1, num2;
	
	double res = 0;
	char res_str[100];
	
	tkn = strtok(postfix, " ");
	while(tkn != NULL)
	{
		if( (tkn[0] >= '0' && tkn[0] <= '9') || ( (tkn[0] == '-' || tkn[0] == '.') && tkn[1] >= '0' && tkn[1] <= '9' ) ) /*include the case of a fraction not beginning with zero ?? .4 .5 ??*/
		{
			push(S, tkn);
		}else
		{
			tmp = pop(S);
			sscanf(tmp->element, "%lf", &num2);
			free(tmp->element);
			free(tmp);
			
			tmp = pop(S);
			sscanf(tmp->element, "%lf", &num1);
			free(tmp->element);
			free(tmp);
			
			switch(tkn[0])
			{
				case '+' :
					res = num1 + num2;
					break;
				case '-' :
					res = num1 - num2;
					break;
				case '*' :
					res = num1 * num2;
					break;
				case '/' :
					if(num2 == 0)
					{
						// divide by zero; do something here
					}
					res = num1 / num2;
					break;
			}
			
			sprintf(res_str, "%.4lf\0", res); // is the '\0' needed ??
			push(S, res_str);
		}
		
		tkn = strtok(NULL, " ");
	}
	
	tmp = pop(S);
	sscanf(tmp->element, "%lf", &res);
	free(tmp->element);
	free(tmp);
	
	return res;
}
/*
Calls the evaluate_wStack function and changes the result to string before returning it.
*/
char* evaluate_asString(char* postfix)
{
	double x = evaluate_wStack(postfix);
	
	char* str = (char*)calloc(MAX_STR, sizeof(char)); //or use static or non if it's possible to have it used once then discarded ??
	sprintf(str, "%.4lf", x); // add '\0' to it's end maybe ??
	
	return str; //TODO double check if it would return a value or not ??
}


/**********/
/*
Changes valid infix equations to postfix
*/
char* to_postfix(char* line)
{
	Stack S = (pntr)malloc(sizeof(node));
		S->next = NULL;
	
	char* postfix;
	int line_len = strlen(line);
	
	pntr tmp;
	char* op;
	
	postfix = (char*)calloc(2*line_len, sizeof(char));
	
	int i = 0, j =0;
	
	if(line[i] == '+' || line[i] == '-')
	{
		if(line[i] == '-')
		{
			postfix[j] = '-';
			j++;
			i++;
		}else
		{
			i++;
		}
	}
	
	for(; i<line_len; i++)
	{
		if( (line[i] >= '0' && line[i] <= '9') || line[i] == '.' )
		{
			postfix[j++] = line[i];
			if(!((line[i+1] >= '0' && line[i+1] <= '9') || line[i+1] == '.'))
			{
				postfix[j++] = ' ';
			}
		}else
		{
			if( (line[i-1] == '*' || line[i-1] == '/' || line[i-1] == '+' || line[i-1] == '-') &&  (line[i] == '-' || line[i] == '+') ) // if operation and an explicit sign for the following number
			{
				if(line[i] == '-')
				{
					postfix[j++] = '-';
					continue;
				}else
				{
					continue; //skip it if it's + sign 
				}
			}
			
			/*checking stack and either pushing operand or popping it*/
			while(true)
			{
				/*TODO dealing with brackets*/
				if(!is_empty(S))
				{
					op = top(S)->element;
					if(prec(op[0], line[i])) /*TODO prec('(', ')') = false*/ /*prec(op,')') = true*/
					{
						//pop
						postfix[j++] = op[0];
						postfix[j++] = ' ';
						
						tmp = pop(S);
						free(tmp->element);
						free(tmp);
					}else if(op[0] == '(' && line[i] == ')')
					{
						tmp = pop(S);
						free(tmp->element);
						free(tmp);
						break;
					}else
					{
						//push
						push(S, &line[i]);
						break;
					}
				}else
				{
					push(S, &line[i]);
					break;
				}
			}
		}
	}
	
	// emptying the stack after the line ended
	while(!is_empty(S))
	{
		postfix[j++] = ' ';
		tmp = pop(S);
		postfix[j++] = (tmp->element)[0];
		
		free(tmp->element);
		free(tmp);
	}
	
	//is this needed ?? // TODO
	line_len = strlen(postfix);
	if(postfix[line_len - 1] == ' ')
	{
		postfix[line_len - 1] = '\0';
	}
	
	free(S);
	
	return postfix;
}


/**********/
/*
File manipulation.
*/

/* Takes input file and applies the necessary operations on it, returns an array of Lists of Validity, Postfix, Evaluation and Input */
void* read_file(FILE* input)
{
	static List outputs[4];
		
	List valid_list = (List)malloc(sizeof(list_ref));
		valid_list->head = (pntr)malloc(sizeof(node)); //head node
		valid_list->tail = valid_list->head;
		valid_list->head->next = NULL;
	
	List postfix_list = (List)malloc(sizeof(list_ref));
		postfix_list->head = (pntr)malloc(sizeof(node)); //head node
		postfix_list->tail = postfix_list->head;
		postfix_list->head->next = NULL;
	
	List eval_list = (List)malloc(sizeof(list_ref));
		eval_list->head = (pntr)malloc(sizeof(node)); //head node
		eval_list->tail = eval_list->head;
		eval_list->head->next = NULL;
	
	List inputs_list = (List)malloc(sizeof(list_ref));
		inputs_list->head = (pntr)malloc(sizeof(node));
		inputs_list->tail = inputs_list->head;
		inputs_list->head->next = NULL;
	
	outputs[0] = valid_list;
	outputs[1] = postfix_list;
	outputs[2] = eval_list;
	outputs[3] = inputs_list;
	
	char* line = (char*)calloc(MAX_STR, sizeof(char)); // char line[MAX_STR];
	char* postfix;
	
	fgets(line, MAX_STR, input);
	
	do
	{
		if(check_valid(&line))
		{
			append_list_node(valid_list, "Valid.");
			
			postfix = to_postfix(line);
			append_list_node(postfix_list, postfix);
			
			append_list_node(eval_list, evaluate_asString(postfix));
			
		}else /*equation not valid*/
		{
			append_list_node(valid_list, "Not valid."); //TODO maybe add reason later on ?
			append_list_node(postfix_list, "Not valid.");
			append_list_node(eval_list, "Not valid.");
		}
		
		append_list_node(inputs_list, line);
	}while(fgets(line, MAX_STR, input) != NULL);
	
	free(line); // just in case to free the memory ?? // TODO is it needed or not ??
	
	return outputs;
}
/* Save the outputs to a file.*/
void save_file(FILE* output, const List input, const List valid, const List postfix, const List evaluations)
{
	pntr val 	= valid->head->next;
	pntr post 	= postfix->head->next;
	pntr eval 	= evaluations->head->next;
	pntr inp 	= input->head->next;
	
	fprintf(output, "Input:\t||\tValidity:\t||\tPostfix:\t||Evaluation:\n\n");
	
	while(inp/*->next*/ != NULL)
	{
		fprintf(output, "%s\t||\t%s\t||\t%s\t||\t%s\n", inp->element, val->element, post->element, eval->element);
		
			val 	= val->next;
			post 	= post->next;
			eval 	= eval->next;
			inp 	= inp->next;
	}
}


/**********/
/*
Messages and menus
*/

void main_menu()
{
	printf("Hello :)\n\n");
	
	printf("Please select one of the following options:\n\
1) Change/view input file.\n\
2) Check the validity ofthe equations in the file.\n\
3) Transform valid equations to Postfix format.\n\
4) Calculate the result of valid equations (This is done through postfix format and using stacks).\n\
5) Save results of the program to another file.\n\
\n\
\tq) To quit the program.\n");
}
bool end_msg()
{
	char c;
	clear_screen();
	printf("Are you sure you want to exit ?[Y/N]\n>");
	
	scanf(" %c", &c);
	
	return (c=='n' || c=='N')?(false):(true);//default is yes
}


/**********/
//clears the screen
//checks if the system is Windows uses system command "cls"
//else assumes it uses POSIX standards and uses system command "clear"
void clear_screen()
{
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		system("cls");
	#else
		system("clear"); // TODO or use write command to avoid overhead in POSIX based
	#endif
}


/**********/
/*
Main program
*/
main()
{
	FILE *input, *save;
	List *output;
	
	List val, post, eval, inp;
	
	char* input_path 	= (char*)calloc(MAX_STR, sizeof(char));
	char* output_path 	= (char*)calloc(MAX_STR, sizeof(char)); 
	
	char e;
	
	bool end_prog 		= false;
	bool is_file 		= false;
	
	while(!end_prog)
	{
		clear_screen();
		main_menu();
			printf(">");
			scanf(" %c", &e);
		switch(e)
		{
			case '1' : // viewing current file, changing it
				if(is_file)
				{
					clear_screen();
					printf("The current used file is:\n\t%s", input_path);
					printf("\nDo you want to select another input file ?[Y/n]");
						scanf(" %c", &e);
					
					if(e == 'Y' || e == 'y')
					{
						clear_nodes(val->head);
							free(val->head);
							
						clear_nodes(post->head);
							free(post->head);
							
						clear_nodes(eval->head);
							free(eval->head);
							
						clear_nodes(inp->head);
							free(inp->head);
							
						fclose(input);
					
					inputprocessed:
						printf("Please insert file path to open: ");
							scanf("%s", input_path);
							// process_path(&input_path);
						
						printf("\n**The input file is : %s", input_path);
						printf("\nEnter [Y/y] to continue, [N/n] to change it.\n>");
							scanf(" %c", &e); // or use "%[YyNn]"
						
						if(e == 'N' || e == 'n')
						{
							clear_screen();
							goto inputprocessed;
						}// default approve input
						
						input = fopen(input_path, "r");
						if(input != NULL)
						{
							output = (List*)read_file(input);
								val		= *(output+0);
								post	= *(output+1);
								eval	= *(output+2);
								inp		= *(output+3);
						}else
						{
							printf("An error occurred while opening file!");
							is_file = false;
						}
						
						printf("\n\n\tPress enter to continue:"); // wait for user interaction
							scanf("%2[\n]", &e);
					}
				}else
				{
				inputfile:
					clear_screen();
					printf("Please insert file path to open: ");
						scanf("%s", input_path);
					
					printf("\n**The input file is : %s", input_path);
					printf("\nEnter [Y/y] to continue, [N/n] to change it.\n>");
						scanf(" %c", &e);
					
					if(e == 'N' || e == 'n')
					{
						clear_screen();
						goto inputfile;
					}// default approve input
					
					input = fopen(input_path, "r");
					if(input != NULL)
					{
						output = (List*)read_file(input);
							val		= *(output+0);
							post	= *(output+1);
							eval	= *(output+2);
							inp		= *(output+3);
						
						is_file = true;
					}else
					{
						printf("An error occurred while opening file!");
						printf("\n\n\tPress enter to continue:"); // wait for user interaction
							scanf("%2[\n]", &e);
					}
				}
				break;
			case '2' : // validity check
				if(is_file)
				{
					clear_screen();
					print_to_consol(inp, val);
					
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}else
				{
					clear_screen();
					printf("* You have no input files, please chose an input file first!");
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}
				break;
			case '3' : // postfix
				if(is_file)
				{
					clear_screen();
					print_to_consol(inp, post);
					
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}else
				{
					clear_screen();
					printf("* You have no input files, please chose an input file first!");
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}
				break;
			case '4' : // calculating results
				if(is_file)
				{
					clear_screen();
					print_to_consol(inp, eval);
					
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}else
				{
					clear_screen();
					printf("* You have no input files, please chose an input file first!");
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}
				break;
			case '5' : // saving to file
				if(is_file)
				{
					printf("Please insert file path to save to: ");
						scanf("%s", output_path);
					
					save = fopen(output_path, "w");
					if(save != NULL)
					{
						clear_screen();
						save_file(save, inp, val, post, eval);
						printf("Saving finished, file data and results saved as :\n\t> %s", output_path);
						
						printf("\n\n\tPress enter to continue:");
							scanf("%2[\n]", &e);
					}else
					{
						printf("An error occurred while opening file!");
						printf("\n\n\tPress enter to continue:"); // wait for user interaction
							scanf("%2[\n]", &e);
					}
				}else
				{
					clear_screen();
					printf("* You have no input files, please chose an input file first!");
					printf("\n\n\tPress enter to continue:");
						scanf("%2[\n]", &e);
				}
				break;
			
			case 'Q' :
			case 'q' :
				end_prog = end_msg();
				break;
			
			default :
				printf("\nInvalid input. Press enter to return to previous menu:");
					scanf("%2[\n]", &e);
		}
	}
	
	
	/*
	freeing nodes from memory
	*/
	clear_nodes(val->head);
		free(val->head);
		free(val);
	clear_nodes(post->head);
		free(post->head);
		free(post);
	clear_nodes(eval->head);
		free(eval->head);
		free(eval);
	clear_nodes(inp->head);
		free(inp->head);
		free(inp);
		
}
