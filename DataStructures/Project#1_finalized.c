#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/*
String int values
*/
#define MAX_STR 		4096	//maximum string buffer size
#define STR_OK 			+1		//string was processed successfully
#define STR_ERR_main	-8		//string was not processed successfully return to main menu
#define STR_ERR_in		-1		//string was not processed successfully return to input menu
#define STR_RET 		+8		//no string input return (b)

/**********/
/*
data structures
*/

typedef enum { false, true }bool;
typedef struct node * pntr;
struct node
{
	double coefficient;
	int power; //Using unsigned caused trouble when comparing with int !
	pntr next;
};
typedef struct node Node;
typedef pntr List;
typedef char string[MAX_STR]; //TODO work on that later <- <- //for now it ends with '\0'


/**********/
/*
Polynomials Handling
*/

//TODO for some arbitrary reason it doesn't consider 2.000 - 2.000 in the polynomials (probabely for others subtractions as well) to equal zero and thus fails when comapring it to 0 and doesn't remove the node; it registers a very small number != 0

/*void remove_zeroes(const List l)*/
/*{*/
/*	pntr tmp, prev;*/
/*	tmp = l->next;*/
/*	prev = l;*/
/*	*/
/*	while(tmp!=NULL)*/
/*	{*/
/*		if(tmp->coefficient == 0)*/
/*		{*/
/*			prev->next = tmp->next;*/
/*			free(tmp);*/
/*			tmp = prev->next;*/
/*		}*/
/*		*/
/*		prev = tmp;*/
/*		tmp = tmp->next;*/
/*	}*/
/*}*/

//sorts a List in decending order based on the Node->power for each element
//Also removes any Node with a coefficient = 0
//And merges Nodes with the same power
void sort_list(const List l)
{
	List lcp = l; // Hold the List l to bes used and modified, as an indicator for the main loop

	pntr tmp; // Holds temporary Node as it's compared to the current Maximum
	pntr prev; // Holds the Node previous to the one currently comparing

	pntr max_power; // Holds the Node with the Maximum power to compare it to the other Nodes
	pntr pre_max; // Holds the Node before the Maximum Node

	int power = -1; // Used to store the maximum power while it's being compared

	while(lcp->next != NULL) // FIXME doesn't check the last node for if it's coefficient is zero *_* /* maybe use lcp != */
	{
		tmp = lcp->next;
		prev = lcp;
		power = -1;


		while(tmp != NULL)
		{
			if(tmp->coefficient < 0.001 && tmp->coefficient > -0.001) // If the temporary Node has a coefficient of zero it gets removed // in a range between 0.001 & -0.001 since it's uses double values so it may have a small approximation error when the number is very small
			{
				prev->next = tmp->next;
				free(tmp);

				tmp = prev->next;

				continue;
			}

			if(tmp->power > power) // if the temporary Node has a larger power it becomes the Max
			{
				max_power = tmp;
				pre_max = prev;

				power = max_power->power;
			}else if(tmp->power == power) // if it has an equal power to the max it gets merged with it, coefficients added, and tmp freed
			{
				max_power->coefficient += tmp->coefficient;

				prev->next = tmp->next;
				free(tmp);
				tmp = prev->next;

				if(max_power->coefficient < 0.001 && max_power->coefficient > -0.001) // checks if the sum of the coefficients is still not 0, else it removes the node and restarts the loop
				{
					pre_max->next = max_power->next;
					free(max_power);

					tmp = lcp->next;
					prev = lcp;
					power = -1;

					continue;
				}

				continue;
			}

			prev = tmp;
			tmp = tmp->next;
		}

		//replaces the Max Node to the beginning of the list and advances forward
		pre_max->next = max_power->next;
		tmp = lcp->next;
		max_power->next = tmp;
		lcp->next = max_power;

		lcp = lcp->next;
	}

	// remove_zeroes(l);
}

//Adds two lists, f1, f2, and stores the result in result
//It does so by making a long List in result that holds a copy of all the elements in f1 & f2 and then sends them to the sort_list function
void add(const List result, const List f1, const List f2) //TODO double check here
{
	pntr tmp, tmp_res;

	tmp = f1->next;
	tmp_res = result;
	while(tmp != NULL)
	{
		tmp_res->next = (pntr)malloc(sizeof(Node));
		tmp_res = tmp_res->next;

		tmp_res->coefficient = tmp->coefficient;
		tmp_res->power = tmp->power;
		tmp_res->next = NULL;

		tmp = tmp->next;
	}

	tmp = f2->next;
	while(tmp != NULL)
	{
		tmp_res->next = (pntr)malloc(sizeof(Node));
		tmp_res = tmp_res->next;

		tmp_res->coefficient = tmp->coefficient;
		tmp_res->power = tmp->power;
		tmp_res->next = NULL;

		tmp = tmp->next;
	}

	sort_list(result);
}

//subtracts two lists f & g and stores the result in result
//result = f - g = f + (-g)
//it does so by multiplying the coefficients of g by -1 sending it to the add function with f and result, then multiplying it back with -1 before ending the function
void sub(const List result, const List f, const List g)
{
	pntr tmp;

	tmp = g->next;
	while(tmp != NULL)
	{
		(tmp->coefficient) *= (-1);
		tmp = tmp->next;
	}

	add(result, f, g);

	tmp = g->next;
	while(tmp != NULL)
	{
		(tmp->coefficient) *= (-1);
		tmp = tmp->next;
	}
}

//multiplies two Lists f1 & f2 and stores the result in result
//creates a List in result that holds the result of multiplying each element int f1 with f2 (adding powers, multiplying coefficients)
//then calls the sort_list function
void multiply(const List result, const List f1, const List f2)
{
	pntr tmp1, tmp2, tmp_res;

	tmp1 = f1->next;
	tmp2 = f2->next;

	tmp_res = result;

	while(tmp1 != NULL)
	{
		tmp2 = f2->next;

		while(tmp2 != NULL)
		{
			tmp_res->next = (pntr)malloc(sizeof(Node));
			tmp_res = tmp_res->next;

			tmp_res->coefficient = (tmp1->coefficient)*(tmp2->coefficient);
			tmp_res->power = (tmp1->power)+(tmp2->power);
			tmp_res->next = NULL;

			tmp2 = tmp2->next;
		}

		tmp1 = tmp1->next;
	}

	sort_list(result);

}

//TODO is behaves weirdly here for some reason ?? enters with the power and what not even though it has nothing to do with the code written ?!
//Frees a List back to the memory
void nullify(const List l)
{
	pntr tmp = l->next;

	while(l->next != NULL)
	{
		l->next = tmp->next;
		free(tmp);
		tmp = l;
	}
}
//checks if the List is empty
bool is_empty(const List l)
{
	return (l->next == NULL)?(true):(false);
}

double pow(double x, int y)
{
	double res = 1;

	for(int i=0; i<y; i++)
		res*=x;

	return res;
}

//TODO make a function for power or keep using math.h ??
//Used to evaluate the result of inputing X in the polynomial
double evaluate(const List f, const double X)
{
	double result = 0;
	pntr tmp;

	tmp = f->next;
	while(tmp != NULL)
	{
		result += ( (tmp->coefficient) * pow( X, (tmp->power) ) );

		tmp = tmp->next;
	}

	return result;
}


/**********/
/*
Strings Handling
*/

//Used to remove any character at i, then moving all the elements in the string str one index backwards
//The resulting strings is held temporarily by tmp
//At the end str is freed back to the memory and the pointed at the value at tmp
void remove_char(char* *str, int i, int n)
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

//Used to insert any character at i, then moving all the elements in the string str one index forwards
//The resulting strings is held temporarily by tmp
//At the end str is freed back to the memory and the pointed at the value at tmp
void insert_char(char* *str, char x, int i, int n)
{
	char *tmp = (char*)calloc(n+1, sizeof(char));

	for(int j=0; j<n+1; j++)
	{
		if(j<i)
		{
			tmp[j] = (*str)[j];
		}else if(j==i)
		{
			tmp[j] = x;
		}else if(j>i)//TODO for some reasong when this part happens it doesn't add anyting to tmp
		{
			tmp[j] = (*str)[j-1];
		}
	}

	free(*str);
	*str = tmp;
}

//Transforms a term from string to a coefficient part and power part to be used to creat a Node of it
//Input MUST be in the form "dXi" <-where d (coefficient) in a double value, positive or negative, i (the power) in an integer value, and only positive
void to_numbers(char* in, double* coef, int* power) // //TODO try doing it without strtok and see if the whole works ?
{
	int size = strlen(in); //size of the string input
	int p; //holds the first index of X after X after finding it the first time
	char tmp[size];//temporarily holds part of the string, either the coefficient or the power
	int i=0;

	for(; i<size; i++)
	{
		if(in[i] == 'x')
		{
			i++;
			p = i;
			break;
		}
		tmp[i] = in[i];
	}
	sscanf(tmp, "%lf", coef);//scans the coefficient value

	for(; i<size+p; i++)
	{
		if(size - i > 0)
		{
			tmp[i-p] = in[i];
		}else
		{
			tmp[i-p] = '\0'; //adds an NULL character to the string to clear any values left from calculating the power
		}
	}
	sscanf(tmp, "%i", power);//scans the power value

}

//transforms an input string into a polynomial (List F)
//returns int status = { 0 : string error but retry, -1 : string error, return to input menu, 1 : success, -8 : string error, return to main menu, +8, go back to poly menu no input }
int str_to_poly(const char const* str_in, const List F)
{
	if(str_in[0] == 'b' || str_in[0] == 'B')
	{
		return STR_RET;
	}

	char *tmp; //Holds a copy of the input string to be manipulated
	pntr tmp_list = F; //POints at the List header

	int i=0; //Index counter

	//flags to manipulate the string based on all possibilities
	bool sign_flag = true; //string input MUST START with a '+' or '-'
	bool num_flag = false;
	bool power_flag = false;
	bool decimal_flag = false;

	int N = strlen(str_in); //size of string with the NULL character included

	//copy the input string to tmp
	tmp = (char*)calloc(N, sizeof(char));
	for(i=0; i<N; i++)
	{
		tmp[i] = str_in[i];//TODO could add the remove_char of ' 's here
	}

	//removes the white spaces from the string and brackets from the string
	i=0;
	while(tmp[i] != '\0')
	{
		if(tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == '(' || tmp[i] == ')')
		{
			remove_char(&tmp, i--, N--);
		}
		++i;
	}

	//adds '+' to the beginning if the input string didn't have '+' or '-' at the beginning
	if(tmp[0] != '+' && tmp[0] != '-')
	{
		insert_char(&tmp, '+', 0, N++);
	}

	//starts scanning and manipulating the string
	i=1;
	while(tmp[i] != '\0')
	{
		if(sign_flag)
		{
			if((tmp[i] == '-' || tmp[i] == '+') && !num_flag)//removes extra '+' and '-' signs from the bnegiining of each term
			{
				if(tmp[i] == '-')
				{
					if(tmp[i-1] == '+')
					{
						tmp[i-1]='-';
					}else if(tmp[i-1] == '-')
					{
						tmp[i-1]='+';
					}
				}

				remove_char(&tmp, i--, N--);
			}else if(tmp[i] >= '0' && tmp[i] <='9')//if it encounters a number then it changes the flags and goes to the numbers cases
			{
				sign_flag = false;
				num_flag = true;
			}else if(tmp[i] == 'x' || tmp[i] == 'X')//if it finds 'x' this means that the coefficient is 1 so it inserts it and changes the flags
			{
				insert_char(&tmp, '1', i--, N++);
				sign_flag = false;
				num_flag = true;
			}
		}else if(num_flag && !power_flag)//the coefficient numbers
		{
			if(tmp[i] >= '0' && tmp[i] <='9')
			{ //do nothing
			}else if(tmp[i] == '.' && !decimal_flag)//if it finds a decimal point '.' it activiates its flag
			{
				decimal_flag = true;
			}else if(decimal_flag && tmp[i] == '.')//it removes any extra decimal points
			{
				remove_char(&tmp, i--, N--); //TODO or give an input error
			}else if(tmp[i] == '*')//removes the '*' sign
			{
				remove_char(&tmp, i--, N--);
			}else if(tmp[i] == 'x' || tmp[i] == 'X')
			{
				tmp[i] = 'x'; //changes 'x' || 'X' to small letter
				num_flag = false; //the coefficient is over
				power_flag = true; //the power part is next
				decimal_flag = false; //the decimal point is initialized back to false
			}else if((tmp[i] == '-' || tmp[i] == '+')) 	/*if the term only has a number the '+' or '-' this means the string has an x raised to the power zero, so it inserts these characters and a space indicating the end of this term */
			{
				insert_char(&tmp, 'x', i++, N++);
				insert_char(&tmp, '0', i++, N++);
				insert_char(&tmp, ' ', i++, N++);

				//reinitializes the flags
				num_flag = false;
				power_flag = false;
				decimal_flag = false;
				sign_flag = true;
			}
		}else if(power_flag)
		{
			if(tmp[i] == '^' && !num_flag)//removes all '^' signs as long as it didn't reach a number (the powe)
			{
				remove_char(&tmp, i--, N--);
				num_flag = true;
			}else if(tmp[i] >= '0' && tmp[i] <='9')
			{
				num_flag =true;
			}else if(tmp[i] == '-' || tmp[i] == '+') // when it reaches a '+' or '-' sign it adds a space to indicate the end of this term and reinitializes the flags
			{
				if(!num_flag)
				{
					insert_char(&tmp, '1', i++, N++);
				}
				insert_char(&tmp, ' ', i++, N++);

				num_flag = false;
				power_flag = false;
				//decimal_flag = false;
				sign_flag = true;
			}
		}else
		{
			free(tmp);
			char ret;

			printf("\n** The string you entered was invalid, the program couldn't handle it.\n");
			printf("Return to :\n\
			A) Main Menu.\n\
			B) Input Menu.[default]\n\
			C) to retry\
			\n\n> ");

			scanf(" %c", &ret);
			if(ret == 'A' || ret == 'a')
			{
				return STR_ERR_main;
			}else if(ret == 'C' || ret == 'c')
			{
				return str_to_poly(str_in, F);
			}else //defaults to return to polynomial sub menu
			{
				return STR_ERR_in;
			}
		}

		++i;
	}

	//checks the end of the string
	if( (tmp[N-1] >= '0' && tmp[N-1] <= '9') )
	{
		if(num_flag && !power_flag) //it it ends with a number with no "...x..."
		{
			insert_char(&tmp, 'x', N-1, N++);
			insert_char(&tmp, '0', N-1, N++);
		}
	}else if(tmp[N-1] == 'x') //if it ends with x^1 written as "...x"
	{
		insert_char(&tmp, '1', N-1, N++);
	}

/*
Creating the polynomial as a List
*/
	char* term_token = strtok(tmp, " ");

	//temporary values for the coefficient and the power
	double coef;
	int power;

	pntr temp_node = NULL;
	while(term_token)
	{
		to_numbers(term_token, &coef, &power); //calls to_numbers to stroe the coefficient and power in coef and power

		temp_node = (pntr)malloc(sizeof(Node)); //creates a new Node

		//stores the coefficient and power in it
		temp_node->coefficient = coef;
		temp_node->power = power;
		temp_node->next = NULL;

		//connects it to the List
		tmp_list->next = temp_node;

		//moves the List pointer to it
        tmp_list = tmp_list->next;

        temp_node = NULL;

        term_token = strtok(NULL, " ");
	}

	//sorts the list and organises it
	sort_list(F);

	return STR_OK; //str_to_poly was successful
}


/**********/
/*
Messages and menus
*/
void hint_msg()
{
	printf("Welcome;\n");
	printf("This is a basic program for adding, subtracting, and multiplying polynomials.\n");

	printf("A polynomial is a function, that takes the general form of:\n\
\t->\ta(n)*x^(n) + a(n-1)*x^(n-1) + a(n-2)*x^(n-2) + ... + a(0)\n\
Where:\ta(i) is an arbitrary constant, positive or negative, with or without a fraction, representing the coefficient of the unknown X.\n\tn is a positive integer, representing the power of the unknown X.\n\n");

	printf("\t*Notes on input*\n\
-> Order of the terms does not need to be decending or ascending based on it's power.\n\
-> Presence of the brackets { ( and ) } is not neccessary.\n\
-> Presence of '*' (multiplied by) sign between the coefficient and the unknow X is not neccessary.\n\
-> Presence of '^' (raised to the power of) sign is required. Otherwise the input would be concidered incorrect.\n\n");

	printf("\t\t****\n");
}
void main_menu()
{
	printf("Hello :)\n\n");

	printf("\t>Please select the option you want:\n");
	printf("1) Input polynomials to be stored.\n\
2) Add stored polynomials.\n\
3) Subtract stored polynomials.\n\
4) Multiply stored polynomials.\n\
5) Print stored polynomials.\n\
6) Evaluate a polynomial to a value of X.\n\n\
\th) to get info about the program.\n\
\tq) To quit the program.\n");
}
//Prints the polynomials stored
void poly_sub_menu(const List p1, const List p2)
{
	printf("\t>Please select which polynomial you want to store the input at.\n\n");

	printf("1->\tP1 = ");
		print_poly(p1);
		printf("\n");
	printf("2->\tP2 = ");
		print_poly(p2);

	printf("\n\tb-> to return to previous menu.");
		printf("\n\n");
	printf(">>Note that if a polynomial is already stored, and you chose to store at it, its original value would be lost.\n\n");
}

//Prints the polynomials and gives the user the choice to subtract p1 from p2, or p2 from p1
//return int depending on what the user wants
int subtract_menu(const List p1, const List p2)
{
	char c;
	printf("Would you like to find:\n\
	a) Res = P1 - P2\n\
	b) Res = P2 - P1\n");

	printf("P1 = ");
		print_poly(p1);

	printf("\nP2 = ");
		print_poly(p2);

	printf("\n\n\t*default is P1 - P2\
	\n> ");

	scanf(" %c",&c);

	if(c == 'b' || c == 'B')
	{
		return -1;
	}else
	{
		return 1;
	}
}
//asks the user to choose which polynomial to evaluate it's value
//returns int depending on if the user choses p1, p2, or the result
int eval_menu(const List p1, const List p2, const List res)
{
	char c;

	printf("Please select which of the stored polynomials you want to evaluate:\n");

	if(!is_empty(p1))
	{
		printf("1) P1 = ");
			print_poly(p1);
			printf("\n");
	}

	if(!is_empty(p2))
	{
		printf("2) P2 = ");
			print_poly(p2);
			printf("\n");
	}

	if(!is_empty(res))
	{
		printf("R) Res = ");
			print_poly(res);
			printf("\n");
	}

	printf("\nb) To return to previous menu.");

	printf("> ");
	scanf(" %c", &c);

	if(c == 'b' || c == 'B')
	{
		return -1;
	}
	else if(!is_empty(p1) && c == '1') // P1
	{
		return 1;
	}else if(!is_empty(p2) && c == '2') // P2
	{
		return 2;
	}else if(!is_empty(res) && (c == 'R' || c == 'r') ) // Res
	{
		return 3;
	}else
	{
		printf("Invalid input! Input [R/r] try again. Input [B/b] to return to previous menu.\n\n\n");
			scanf(" %c ", &c);
			if(c != 'b' && c != 'B')
			{
				clear_screen();
				return eval_menu(p1, p2, res);
			}else
			{
				return -1;
			}
	}
}

//printing the polynomials to screen
void print_poly(const List f)
{
	pntr tmp = f->next;
	bool first = true;

	while(tmp != NULL)
	{
		if(tmp->coefficient == 1 || tmp->coefficient == -1)
		{
			if(first)
			{
				if(tmp->power > 0)
				{
					if(tmp->coefficient == 1)
					{}else if(tmp->coefficient == -1)
					{
						printf("-");
					}
				}else
				{
					printf("%.3lf", tmp->coefficient);
				}

				first = false;
			}else
			{
				if(tmp->power > 0)
				{
					if(tmp->coefficient == 1)
					{
						printf(" +");
					}else if(tmp->coefficient == -1)
					{
						printf(" -");
					}
				}else
				{
					if(tmp->coefficient == 1)
					{
						printf(" +%.3lf", tmp->coefficient);
					}else if(tmp ->coefficient == -1)
					{
						printf(" %.3lf", tmp->coefficient);
					}
				}
			}
		}else if(tmp->coefficient > 0 || tmp->coefficient < 0)
		{
			if(first)
			{
				printf("%.3lf", tmp->coefficient);
				first = false;
			}else
			{
				if(tmp->coefficient > 0)
				{
					printf(" +%.3lf", tmp->coefficient);
				}else
				{
					printf(" %.3lf", tmp->coefficient);
				}
			}

			if(tmp->power > 0)
			{
				printf("*");
			}
		}

		if(tmp->power == 0)
		{}else
		{
			printf("x");

			if(tmp->power != 1)
			{
				printf("^%i", tmp->power);
			}
		}

		tmp = tmp->next;
	}
}

bool end_msg()
{
	char c;
	clear_screen();
	printf("Are you sure you want to exit ?[Y/N]\n>");

	scanf(" %c", &c);

	return (c=='n' || c=='N')?(false):(true);//default is yes
}

//clears the screen
//checks if the system is Windows uses system command "cls"
//else assumes it uses POSIX standards and uses system command "clear"
void clear_screen()
{

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
		system("cls");
	#else
		system("clear");
	#endif

}


/**********/
/*
Main program
*/
main()
{
	const List p1	= (pntr) malloc(sizeof(Node));
		p1->next  = NULL;
	const List p2	= (pntr) malloc(sizeof(Node));
		p2->next  = NULL;
	const List res	= (pntr) malloc(sizeof(Node));
		res->next = NULL;

	string str;
	char e;
	double x;

	char flag;
	int str_status;

	bool end_prog	= false;
	bool is_res		= false;

	clear_screen();
	hint_msg();
		printf("\n");
	printf("Press enter to continue: ");
		scanf("%c", &e); //inputting any value wouldn't make a difference
		clear_screen();

	main_menu();
		printf("\n>");
			scanf(" %c", &e);
	while(!end_prog)
	{
		switch(e)
		{
			case '1': //Input polynomials to be stored
				clear_screen();
				poly_sub_menu(p1, p2); //calls polynomials choice sub menu
					printf("\n>");
					scanf(" %c", &flag);
				printf("\n>Please insert your input here: ");

				if(flag == '1')
				{
					scanf(" %[^\n]", str);
					// str[strlen(str)-1] = '\0';
					str_status = str_to_poly(str, p1);

					switch(str_status)
					{
						case STR_OK:
						case STR_ERR_in:
						case STR_RET:
							clear_screen();
							poly_sub_menu(p1, p2);
							e = '1';
							break;
						case STR_ERR_main:
							clear_screen();
							main_menu();
							printf("\n>");
							scanf(" %c ", &e);
							break;
					}
				}else if(flag == '2')
				{
					scanf(" %[^\n]", str);
					str_status = str_to_poly(str, p2);

					switch(str_status)
					{
						case STR_OK:
						case STR_ERR_in:
						case STR_RET:
							clear_screen();
							poly_sub_menu(p1, p2);
							e = '1';
							break;

						case STR_ERR_main:
							clear_screen();
							main_menu();
							printf("\n>");
							scanf(" %c", &e);
							break;
					}
				}else if(flag == 'b' || flag == 'B')
				{
					clear_screen();
					main_menu();
					printf("\n>");
					scanf(" %c", &e);
					break;
				}else
				{
					printf("Invalid."); //Write something TODO
				}
				break;
			case '2': // Add stored polynomials
				add(res, p1, p2);
				clear_screen();
				printf("The sum of the stored polynomials is\n\t>Res = ");
					print_poly(res);
					printf("\n");
				is_res = true;

				printf("\nPress enter to continue:");
					scanf("%2[\n]", &e); //waits the user interaction, any input won't make a difference
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c",&e);
				break;
			case '3': // Subtract stored polynomials
				clear_screen();
				flag = subtract_menu(p1, p2);
				if(flag == 1)
				{
					sub(res, p1, p2);
				}else if(flag == 2)
				{
					sub(res, p2, p1);
				}
				clear_screen();
				printf("The subtraction of the stored polynomials is\n\t>Res = ");
					print_poly(res);
					printf("\n");
				is_res = true;

				printf("\nPress enter to continue:");
					scanf("%2[\n]", &e); //waits the user interaction
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c",&e);
				break;
			case '4': // Multiply stored polynomials
				multiply(res, p1, p2);
				clear_screen();
				printf("The multiplication of the stored polynomials is\n\t>Res = ");
					print_poly(res);
					printf("\n");
				is_res = true;

				printf("\nPress enter to continue:");
					scanf("%2[\n]", &e); //waits the user interaction
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c",&e);
				break;
			case '5': //Print stored polynomials
				clear_screen();
				printf("> P1  =  ");
					print_poly(p1);
					printf("\n");
				printf("> P2  =  ");
					print_poly(p2);
					printf("\n");
				if(is_res)
				{
					printf("\n\t* The result of the last operation is :\n");
					printf("> Res = ");
					print_poly(res);
					printf("\n");
				}
				printf("\nPress enter to continue:");
					scanf("%2[\n]", &e); //waits the user interaction
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c",&e);
				break;
			case '6': // Evaluate a polynomial to a value of X
				clear_screen();
				flag = eval_menu(p1, p2, res);
				if(flag != -1)
				{
					printf("\nInsert value for X to evaluate the polynomial to: ");
						scanf("%lf", &x);
				}
				switch(flag)
				{
					case 1 :
						x = evaluate(p1, x);
						break;
					case 2 :
						x = evaluate(p2, x);
						break;
					case 3 :
						x = evaluate(res, x);
						break;
				}
				printf("The result is : %lf\n\n", x);
				printf("\nPress enter to continue:");
					scanf("%2[\n]", &e); //waits the user interaction
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c", &e);
				break;

			// Info about the program
			case 'H':
			case 'h':
				hint_msg();
					scanf(" %c", &e); //waits the user interaction
				clear_screen();
				main_menu();
					printf("\n>");
					scanf(" %c",&e);
				break;
			// Quitting the program
			case 'Q':
			case 'q':
				end_prog = end_msg();
				if(end_prog)
				{
					clear_screen();
				}else
				{
					clear_screen();
					main_menu();
						printf("\n>");
						scanf(" %c",&e);
				}
				break;

			default:
				printf("\nInvalid choice, please insert your choice again from the available options.\n");
					printf("\n>");
					scanf("%c",&e); // this works correctly with and without fflush, and with and without a s[pace before the '%c' !! // TODO
		}
	}

	//free used memory
	nullify(p1);
		free(p1);
	nullify(p2);
		free(p2);
	nullify(res);
		free(res);

}
