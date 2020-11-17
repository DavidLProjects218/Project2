#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include <string>
#include<fstream>
#include<sstream>
#define MAX 50
#define SIZE 50
using namespace std;

/* inside the structure are 2 variables declared. "value " is hold the character in the expression
num is to hold the int or float value if the scanned variable is a number */

/*This structure is created for the nodes in the stack*/
struct Stack_Node {

	char value;
	float num;

	Stack_Node *next;

}; typedef struct Stack_Node Stack_Node;

/*This structure is created for the nodes in the Queue*/
struct Queue_Node {

	char value;
	float num;

	Queue_Node * next;

}; typedef struct Queue_Node Queue_Node;

/* All the functions used in the program */
void print_Stack(Stack_Node *node);
void push_Node(float data_in);
void pop_Node();
void reset_Stack();

void printQueue(Queue_Node *n);
void enQueue(int data);
void deQueue();

void infix_To_Postfix(int i, string infix[]);
void Evaluate_Postfix();
void Store_Postfix(char postfix[][SIZE], int expr);
void Display(string infix[], char postfix[][SIZE], float result[], int total);

int PRE(char op);

/* These are global variables.  These variables constantly change,
they must be declared in the outer scope to make it easier .*/

Stack_Node *Top = NULL;
Queue_Node *front = NULL;
Queue_Node *rear = NULL;

int count_stack = 0;
char postfix[MAX][SIZE];
float pos_result[20];

int main()
{
	string infix[100];
	int count = 0;
	string filename = "a2.txt";

	cout << "\nRead in Expressions from File:" << endl;
	ifstream myfile(filename.c_str());

	if (myfile.is_open() == true)
	{
		while (getline(myfile, infix[count]))
		{
			cout << "\n" << infix[count];
			count++;
		}
		myfile.close();
	}
	cout << endl;

	int i = 0;

	for (int expr = 0; expr < count; expr++)
	{
		infix_To_Postfix(expr, infix);
		Store_Postfix(postfix, expr);	
		Evaluate_Postfix();

		if (Top != NULL)
		{
			pos_result[expr] = Top->num;
		}	
	    i = 0;
		reset_Stack();
	}

	Display(infix, postfix, pos_result, count);

	system("PAUSE");

	return 0;
}

void Display(string infix[], char postfix[][SIZE], float result[], int total)
{
	int i = 0;
	float sum = 0;
	cout << "\nExpressions: " << endl;
	for (int expr = 0; expr < total; expr++)
	{
		cout << "\nInfix: " << infix[expr];
		cout << "\nPostfix: ";
		
		while (postfix[expr][i] != '\0')
		{
			cout <<postfix[expr][i] << " ";
			i++;
		}
		i = 0;
		cout << "\nEvaluated Result: " << result[expr];
		cout << endl;
	}
	cout << "\nEvaluated Expressions Sum: " << endl;
	for (int i = 0; i < total; i++)
	{
		sum = sum + result[i];
		cout << "\n" << result[i];
	}
	cout << "\n________";
	cout << "\n" << sum <<endl;
	
}
void reset_Stack()
{
	while (Top != NULL)
	{
		Top = NULL;
	}
}

void Store_Postfix(char postfix[][SIZE], int expr)
{
	int count_ch = 0;
	Queue_Node *temp;

	for (temp = front; temp != NULL; temp = temp->next)
	{
		postfix[expr][count_ch] = temp->value;
		count_ch++;
	}
	
	postfix[expr][count_ch] = '\0';
	
	count_ch = 0;


}
void infix_To_Postfix(int i, string infix[] )
{
	for (int j = 0; j < infix[i].length(); j++)
	{
		if (infix[i][j] >= 48 && infix[i][j] <= 57)
		{
			enQueue(infix[i][j]);
		}
		else if (infix[i][j] == '(')
		{
			push_Node(infix[i][j]);
		}
		else if (infix[i][j] >= 42 && infix[i][j] <= 47 || infix[i][j]==94)
		{
			while (Top != NULL && PRE(infix[i][j]) <= PRE(Top->value))
			{
				enQueue(Top->value);
				pop_Node();
			}
			push_Node(infix[i][j]);
		}
		else if (infix[i][j] == ')')
		{
			while (Top != NULL && Top->value != '(')
			{
				enQueue(Top->value);
				pop_Node();  // pop value from Stack and into Queue
			}
			pop_Node();
		}
	}
	while (Top != NULL)
	{
		enQueue(Top->value);
		pop_Node();
	}
	
}

void Evaluate_Postfix()
{
	float temp;

	while (front != NULL)
	{
		if (front->value>= 48 && front->value<= 57)
		{
			push_Node(front->num);
			deQueue();
		}
		else if (PRE(front->value) != 0)  // checks if the element in Queue is an operator 
		{
			switch (PRE(front->value))
			{
			case 1:
				if (front->value == '-')
				{

					if (count_stack > 1)
					{
						temp = Top->next->num -Top->num;
						pop_Node();
						pop_Node();
						push_Node(temp);
						deQueue();
					}
				}
				else if (front->value == '+')
				{
					if (count_stack > 1)
					{
						temp = Top->next->num + Top->num;
						pop_Node();
						pop_Node();
						push_Node(temp);
						deQueue();
					}
				}
				break;

			case 2:
				if (front->value == '/')
				{

					if (count_stack > 1)
					{
						temp = Top->next->num / Top->num;// 
						pop_Node();
						pop_Node();
						push_Node(temp);
						deQueue();

					}

				}
				else if (front->value == '*')
				{
					if (count_stack > 1)
					{
						temp = Top->next->num *Top->num;
						pop_Node();
						pop_Node();
						push_Node(temp);
						deQueue();
					}
				}
				break;

			case 3:
				if (front->value == '^')
				{
					if (count_stack > 1)
					{
						temp = pow(Top->next->num, Top->num);
						pop_Node();
						pop_Node();
						push_Node(temp);
						deQueue();
					}
				}
				break;
			}
		}
		
		printQueue(front);
		print_Stack(Top);
	}


}
int PRE(char op )
{
	
	if (op == '+' || op == '-') return 1;
	
	 if (op == '*' || op == '/') return 2;

	if (op == '^') return 3;

	return 0;
}
void push_Node(float data_in)
{
	count_stack++;

	Stack_Node *new_add = new Stack_Node;
	new_add->value = data_in; //declares the variables
	
	if (data_in >= 48 && data_in <= 57 )
	{
		new_add->num = data_in - 48;
	}
	else
	{
		new_add->num = data_in;
	}
	new_add->next = Top;
	
	Top = new_add;

}

void pop_Node()
{
	Stack_Node *temp;

	count_stack--;

	if (Top == NULL)
	{
		cout << "\nThe stack is empty" << endl << endl;

	}
	temp = Top;
	Top = Top->next;

	delete(temp);
}


void print_Stack(Stack_Node *node) // helper function to loop through entire stack
{
	if (Top == NULL)
	{
		cout << "\nSTACK IS EMPTY!!!" << endl << endl;
	}
	else if (Top != NULL) {

		Stack_Node *temp;
		cout << "\nCurrent Stack: \n";
	
		for (temp = node; temp != NULL; temp = temp->next)
		{
			if (temp->value >= 48 && temp->value <= 57 || temp ->num != 0 || temp->num ==0)
			{
				cout << temp->num << "\n";
			}
			else
			{
				cout << temp->value << "\n";
			}	
		}
		
	}
}

// Queue functions 
void enQueue(int data)
{
	Queue_Node *temp;

	temp = new Queue_Node;
	temp->value = data;
	
	if (data >= 48 && data <= 57)
	{
		temp->num = data - 48;		
	}

	if (front == NULL)
	{
		front = rear = temp;
		rear->next = NULL;
	}
	else
	{
		temp->next = NULL;  // the new rear to point to NULL 
		rear->next = temp;  // point the old rear to the new rear
		rear = temp;  // update rear
	}
}

void deQueue()
{
	Queue_Node *temp;

	if (front == NULL)
	{
		cout << "\nQUEUE IS EMPTY!!!!" << endl << endl;
	}
	else
	{
		temp = front;
		front = front->next;
		delete(temp);
	}

}

void printQueue(Queue_Node *n) // Helper function to loop through entire Queue 
{
	Queue_Node *temp;

	if (front == NULL)
	{
		cout << "\nQUEUE IS EMPTY!!!!!!" << endl << endl;
	}

	else if (front != NULL)
	{
		cout << "\nMy Queue: " << endl;
		
		for (temp = n; temp != NULL; temp = temp->next)
		{
			cout << temp->value << " ";
		}
		cout << endl;
	}	
}