#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "1Dpoi_type.h"
#include "1Dpoi_def.h"
#include "1Dpoi_keywords.h"


void push(double);
double pop(void);
#define MAXVAL 100	/* maximum depth of value stack */

int sp = 0;			/* next free stack position */
int spop = 0;			/* next free op stack position */
double val[MAXVAL];	/* value stack */

/* Sructure for operators with operator name and precedence. */
struct op_type {
	char op_name[40];	/* operator name */
	int op_pres;		/* operator precedence */
};
struct op_type op[MAXVAL];	/* op stack */
struct op_type pop_op(void); /* pop op stack */
void push_op(struct op_type); /* push op stack */
static int iskeyword(char field[]);  /* determine field is a keyword*/
int lin_grade(char expression[]); /* determine if the field represents a linear grading */

/*******************************************************/
int fieldtrim(char in[], int *begin, char field[], char subfield[], int keepCase) {
/*******************************************************/

/*	This routine strips off the first field after start and returns
 	it in field.  Fields are delimited by spaces or tabs.  The variable start is
 	updated so that a subsequent call will find the next field. */

	int i,j,length,first,beginfield,endfield,type,sign_possible,beginlast,begin2ndlast, test;
	char tmpfield[MAX_EXPRESS],lastop[MAX_EXPRESS];

	/*  set the initial conditions */

	strcpy(field,"");
	strcpy(subfield,"");
	length=0;
	beginfield=-1;
	endfield=-1;
	first = true;	/* set the first time flag */
	sign_possible = true;
	i=*begin;
	beginlast = *begin;
	j=0;

/* first look for the first non-blank character in the input string, store its position
   in beginfield, and reset the first flag.  Then look for the next blank, tab, comma,
   or = and store its position in endfield. */

	if (in[*begin] == '\0') {
		field[0] = '\0';
		return 0;
	}
	type = getop(field,in,begin,&sign_possible);
	if (type == '\0' || type == '\n') return 0; /* kick out if no field is found*/
	beginlast = *begin;
    // Check for = sig
	type = getop(tmpfield,in,begin,&sign_possible);  // Get what should be an = sign
    if (tmpfield[0] != '=') {
        *begin = beginlast; //set back to before search
        /*convert any alphabetic characters to lower case */
        if (!keepCase) lowercase(field);
        return -1; //if not = sign kick out with -1 which signals error if '=' was expected
    }

	type = getop(tmpfield,in,begin,&sign_possible);
	while(type != '=' && type != '\0' && type != '\n' && tmpfield[0] != 'v'
          && ((iskeyword(tmpfield) && (tmpfield[0] == 'y')) || !iskeyword(tmpfield))) { // loop until end of line or = sign (means new keyword)
		strcat(subfield,tmpfield);  //if the found field is not a keyword, append it to subfield
        strcpy(lastop, tmpfield); // save the last op in case we need to subract it
		begin2ndlast = beginlast; // save second to last starting point, in case we need to back out next keyword
        beginlast = *begin;
		type = getop(tmpfield,in,begin,&sign_possible);
	}
	*begin = beginlast;
    if (type == '=') { //this means we have copied the next keyword to subfield, and we must remove it
        j=strlen(subfield)-strlen(lastop);
        strncpy(tmpfield,subfield, j);
		tmpfield[j] = '\0';
        strcpy(subfield,tmpfield);
        *begin = begin2ndlast; //back up begin to before the next keyword.
    }

	length = strlen(field);

    //convert any alphabetic characters to lower case
	if (!keepCase) {
		lowercase(field);
		lowercase(subfield);
	}

	return length;
}
/***********************************************/
int getInputNew(char FileInput[], int *pointer, char **line) {
/***********************************************/

    // Fuction to get a line of data from a file

	int length,c,i,start;
	char messg[MAX_MESSAGE],buf[5];

	if (line[0] == '\0' && (*pointer) > 0)
		return EOF;


    if ((*line) != NULL) {
        free((*line));  // If line is in use, free the space
        *line = NULL;
    }

    start = *pointer;
    c = FileInput[*pointer];
    if (c=='\0') {
        ;
    }
    i=0;
	while (c != '\0' && c != '\n' && c!= '\r') {  //increment pointer to find end of line
		(*pointer)++;
        i++;
        c = FileInput[*pointer];
    }

    length = i;
    (*line) = (char *) malloc((length+1)*sizeof(char)); //allocate space for line
    for (i=0; i<length;i++) {
        (*line)[i] = FileInput[start+i];
    }
    (*line)[i] = '\0'; // append termination
    (*pointer)++; //increment file pointer to skip end of line
    while (FileInput[*pointer] == '\n' || FileInput[*pointer]== '\r') (*pointer)++; //increment again if there is extra linefeeds or CRs

	if (c == '\0' && i == 0)
		length = EOF;
	/*else
		length = i;*/

	return length;
}


/*****************************************************/
int getop(char s[], char input[], int *j, int *sign_possible)
/*****************************************************/
/* getop: get next operator or numeric operand */
{
	/* sign_possible is a rather strange flag.  It is set "true" if there is a possiblity of a leading "-" being
	the sign of a number and not an operator, and if found will insert a chs operator.  If set "false" then "-"
	signs are treated as operators. */

	int i, c, chs,string;

	chs = false;
	string = false;

	while ((s[0] = c = input[(*j)++]) == ' ' || c == '\t')  // skip leading spaces, and tabs
		;
	s[1] = '\0';

	if (c == '"') {
		/* collect string */
		i=1;
		s[0] = input[(*j)++];
		while (input[*j] != '"')
			s[i++] = input[(*j)++];
		s[i] = '\0';
		(*j)++;
		return '"';	// not a number
	}

	if (!isdigit(c) && c != '.' && (((c == '-' || c == '+') && !*sign_possible) || c=='*' || c=='/' || c=='^' || c=='(' || c==')' || c=='{' || c=='}' || c=='\0' || c == '=')) {
		*sign_possible = false;
		return c;	/* not a number. Single charater operator or paren*/
	}
	i=0;
	if (!isdigit(c) && c != '.' && c != '-' && c != '+') {
		/* collect operator or variable name */
		while ((s[++i] = c = input[(*j)++]) != ' ' && c != '.' && c != '-' && c != '+' && c != '*' && c != '/'
				&& c != '^' && c != '(' && c != '{' && c != '\t' && c != ')' && c != '}' && c != '\0' && c != '\n'&& c != '=')
				;
		*sign_possible = false;
		(*j)--;
		s[i] = '\0';
		c = s[i-1];
		return c;	/* not a number */
	}
	if (isdigit(c) || c == '-' || c== '+') { /* collect integer part */
		if (c == '-') chs = true;
		if (c == '-' || c== '+') s[0] = input[(*j)++];
		while (isdigit(s[++i] = c = input[(*j)++]))
			;
	}
	if (c == '.')	/* collect fraction part */
		while (isdigit(s[++i] = c = input[(*j)++]))
			;
	if (c == 'e'|| c == 'E')
	{
		s[i] = 'e';
		if (input[*j] == '+' || input[*j] == '-')
			s[++i] = input[(*j)++];
		while (isdigit(s[++i] = c = input[(*j)++])) /* collect exponent */
			;
	}
	(*j)--;
	s[i] = '\0';
	if (chs) strcat(s," chs");
	*sign_possible = false;
	return NUMBER;
}



/*****************************************************/
int getSingField(char s[], char input[], int *j) {
/*****************************************************/
/* getSingField: get a single operator */
	/* This routine returns the contents of a single field which was delimited by spaces tabs, or = signs */

	int i, c;

	while ((c=input[(*j)]) == ' ' || c == '\t' || c == '=')
		(*j)++;
	/*s[1] = '\0';*/
	s[0] = '\0';
	if (input[(*j)] == '\0')
		return 0;	// kick out if the end of the string

	i=0;
	// collect operator or variable name
	while ((s[i++] = c = input[(*j)++]) != ' ' && c != '\t' && c != '\0' && c != '\n' && c != '=')
				;
	(*j)--;
	s[i-1] = '\0';
	if (isdigit(s[0]) || s[0]=='.' || (s[0]=='-' && (isdigit(s[1]) || s[1]=='.')))
		return NUMBER;
	else
		return s[0];	// not a number
}


/*************************************/
static int iskeyword(char value[]) {
/*************************************/

	//This binary search determines if the field contains a keyword
	int position;
	int begin = 0;
	int end;
	int cond = 0;
	char value2[MAX_EXPRESS];

	strcpy(value2,value);
	lowercase(value2);
	if (value2[0] == 'v') {
		if (isdigit(value2[1])) return 1; //special case for voltage sources 
	}

	end = (sizeof(keywords)/sizeof(keywords[0])) - 1;
	while(begin <= end) {
		position = (begin + end) / 2;
		if((cond = strcmp(keywords[position], value2)) == 0)
   			return 1; /* return 1 if keyword is found*/
		else if(cond < 0)
			begin = position + 1;
		else
			end = position - 1;
	}

	return 0;
}



/*************************************/
double numconv(char number[]) {
/*************************************/

/* Convert string to number, converting to Ang or cm-3 */

	char *pchar;
	double convfac,dnum;

	dnum = strtod(number,&pchar);

	if (*pchar == 'n' && *(pchar+1) == 'm') //thickness is given in nm
		convfac = 10.0e0;
	else if (*pchar == 'u' && *(pchar+1) == 'm') //thickness is given in microns
		convfac = 10000.0e0;
	else if (*pchar == 'm' && *(pchar+1) == 'm') //thickness is given in millimeters
		convfac = 1.0e7;
	else if (*pchar == 'c' && *(pchar+1) == 'm') //thickness is given in millimeters
		convfac = 1.0e8;
	else if (*pchar == 'm' && *(pchar+1) == '-' && *(pchar+2) == '3') //doping concentration given in m^3
		convfac = 1.0e6;
	else if (*pchar == 'm' && *(pchar+1) == '-' && *(pchar+2) == '2') //doping concentration given in m^2
		convfac = 1.0e4;
	else
		convfac = 1.0e0; //thickness was given in angstroms or doping in cm-3

	dnum = convfac*dnum;	// convert to Ang, cm-2, or cm-3

	return dnum;
}


/*****************************************************/
	struct mesh_type *ymesh(double position, struct global_type *pGlobals) {
/*****************************************************/

/* Given a real space coordinate, position, ymesh returns the index of the nearest mesh point */

	int meshfound;
	struct mesh_type *pMesh,*ypos;

	meshfound = false;
	pMesh = pGlobals->pTopMesh;

	do {
		if (pMesh->pDownMesh != NULL) {
			if ((position >= pMesh->meshcoord) && (position <= pMesh->pDownMesh->meshcoord)) {
				if ((pMesh->pDownMesh->meshcoord-position) < (position-pMesh->meshcoord))
					ypos = pMesh->pDownMesh;
				else
					ypos = pMesh;

				meshfound = true;
			}
		}
	} while((pMesh=pMesh->pDownMesh) != NULL);

	if (!meshfound)
		ypos = pGlobals->pBottomMesh;	/* allow for overflow */
	if (position < 0.0e0)
		ypos = pGlobals->pTopMesh;	/* allow for underflow */

	return ypos;
}

/*****************************************************/

void lowercase(char input[]) {

/*****************************************************/

/*   This routine changes all letters of an input string to lower case */

	int i;

	i=0;
	while (input[i] != '\0') {
		input[i] = tolower(input[i]);	/*convert character to lower case*/
		i++;
	}
}


/*****************************************************/

double d_sign(double *a, double *b) {

/*****************************************************/

/*  This function mimics the fortran sign function */

	if (*b <= 0.0)
		return -fabs(*a);
	else
		return fabs(*a);
}


/*****************************************************/
double evaluate(char expression[], struct global_type *pGlobals, struct flag_type *pFlags)
/*****************************************************/
{
	int type,j,sign_possible;
	double op2, result;
	char s[MAXOP], message[MAX_MESSAGE];

	j=0;
	sign_possible = false;

	while ((type = getSingField(s,expression,&j)) != '\0') {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '^':
			op2 = pop();
			push(pow(pop(),op2));
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
			{
				strcpy(message,"error: zero divisor\n");
				alertbox(message);
			}
			break;
		default:
			if (!strcmp(s,"chs")) push(-1.0 * pop());
			else if (!strcmp(s,"y")) push(pGlobals->y_loc);
			else if (!strcmp(s,"xval")) push(pGlobals->xval);
			else if (!strcmp(s,"yval")) push(pGlobals->yval);
			else if (!strcmp(s,"wval")) push(pGlobals->wval);
			else if (!strcmp(s,"zval")) push(pGlobals->zval);
			else if (!strcmp(s,"temp")) push(pGlobals->temperature);
			else if (!strcmp(s,"yabs")) push(pGlobals->y);
			else if (!strcmp(s,"exp")) push(exp(pop()));
			else if (!strcmp(s,"log")) push(log(pop()));
			else if (!strcmp(s,"sin")) push(sin(pop()));
			else if (!strcmp(s,"asin")) push(asin(pop()));
			else if (!strcmp(s,"cos")) push(cos(pop()));
			else if (!strcmp(s,"acos")) push(acos(pop()));
			else if (!strcmp(s,"tan")) push(tan(pop()));
			else if (!strcmp(s,"sinh")) push(sinh(pop()));
			else if (!strcmp(s,"cosh")) push(cosh(pop()));
			else if (!strcmp(s,"tanh")) push(tanh(pop()));
			else if (!strcmp(s,"abs")) push(abs(pop()));
			else if (!strcmp(s,"log10")) push(log10(pop()));
			else if (!strcmp(s,"sqrt")) push(sqrt(pop()));
			else
			{
				strcpy(message,"error: unknown command \n");
				alertbox(message);
                pFlags->badversion = true;
			}
			break;
		}
	}
    result = pop();
    if (sp !=0) {
        strcpy(message,"error: syntax error \n");
        strcat(message, expression);
        alertbox(message);
        pFlags->badversion = true;

    }
	return result;
}


/* push f onto value stack */
void push(double f)
{
	char message[MAX_MESSAGE];
	if (sp < MAXVAL)
		val[sp++] = f;
	else {
		strcpy(message,"error: stack full, can't push new value\n");
		alertbox(message);
	}
}


/* pop: pop and return top value from stack */
double pop(void)
{
	char message[MAX_MESSAGE];
	if (sp > 0)
		return val[--sp];
	else {
		strcpy(message,"error: stack empty\n");
		alertbox(message);
		return 0.0;
	}
}

/* push operator onto operator stack */
void push_op(struct op_type op_loc)
{
	char message[MAX_MESSAGE];
	if (spop < MAXVAL)
		op[spop++] = op_loc;
	else {
		strcpy(message,"error: stack full, can't push new operator\n");
		alertbox(message);
	}
}


/* pop: pop and return top value from op stack */
struct op_type pop_op(void)
{
	char message[MAX_MESSAGE];
	if (spop > 0)
		return op[--spop];
	else {
		strcpy(message,"error: stack empty\n");
		alertbox(message);
		return op[0];
	}
}
/*****************************************************/
int lin_grade(char expression[]) {
/*****************************************************/

/*  This routine seperates out the fields '-' delimiter,
returning a 1 if a call for grading is found  */

	char gradstart[MAX_EXPRESS],s[MAX_EXPRESS],gradstop[MAX_EXPRESS];
	double a,b;
	int type,j,sign_possible;
	struct global_type *pDummyGlobals;
	struct flag_type *pDummyFlags;


	if (expression[0] == '\0') return 0; /* empty string */

	j=0;
	sign_possible = true;
	type = getop(s,expression,&j,&sign_possible);
	if (type != NUMBER) return 0;
	strcpy(gradstart,s);
	sign_possible = false;
	type = getop(s,expression,&j,&sign_possible);
	if (s[0] != '-') return 0;
	sign_possible = true;
	type = getop(s,expression,&j,&sign_possible);
	if (type != NUMBER) return 0;
	strcpy(gradstop,s);
	type = getop(s,expression,&j,&sign_possible);
	if (type != '\0') return 0;

/* At this point it has been determined that this is a request for linear grading.
produce the post-fix expression for the grading, and return it in expression */

	a = evaluate(gradstart,pDummyGlobals,pDummyFlags);
	b = evaluate(gradstop,pDummyGlobals,pDummyFlags);
	sprintf(gradstart, "%11.4e",a);
	strcpy(expression,gradstart);
	strcat(expression," ");
	sprintf(s, "%11.4e", fabs(b-a));
	strcat(expression,s);
	strcat(expression," y *");
	if ((b-a) >= 0.0)
		strcat(expression," +");
	else
		strcat(expression," -");
	return 1;
}

/*****************************************************/
int infix_postfix(char expression[]) {
/*****************************************************/

	int sign_possible,type,j,precedence;
	int operat(char s[]);
	int op_prec(char s[]);
	struct op_type op_loc;
	char s[MAX_KEYWORD], dummy[MAX_EXPRESS], message[MAX_MESSAGE];

	if(lin_grade(expression)) return 0;	// check for linear grading, return linear grading postfix expression

	sign_possible = true;
	j=0;
	dummy[0] = '\0';
	while ((type = getop(s,expression,&j,&sign_possible)) != '\0') {

		if ((type == NUMBER || !operat(s)) && type != ')' && type != '}' && type != '(' && type != '{') {
			strcat(dummy,s);
			strcat(dummy," ");
		}

		else if (!strcmp(s,"(") || !strcmp(s,"{")) {
			strcpy(op_loc.op_name,"(");
			op_loc.op_pres = 9;
			push_op(op_loc);
			sign_possible = true;
		}
		else if (!strcmp(s,")") || !strcmp(s,"}")) {
			op_loc = pop_op();
			while (strcmp(op_loc.op_name,"(")) {
				strcat(dummy,op_loc.op_name);
				strcat(dummy," ");
				op_loc = pop_op();
			}
		}

		else {
			precedence = op_prec(s);
			if (spop>0) {
				op_loc = pop_op();
				if (precedence >= op_loc.op_pres) {
					strcat(dummy,op_loc.op_name); // if operator is of greater or equal precedence, append to result
					strcat(dummy," ");
					while (spop > 0 && precedence >= (op_loc=pop_op()).op_pres ) {
						strcat(dummy,op_loc.op_name);
						strcat(dummy," ");
					}
					if (precedence < op_loc.op_pres)
						push_op(op_loc); // if operator was not appended to string, push back on stack
				}
				else
					push_op(op_loc);
			}
			strcpy(op_loc.op_name,s);
			op_loc.op_pres = precedence;
			push_op(op_loc);
		}
	}

	while (spop > 0) {
		op_loc = pop_op();
		strcat(dummy,op_loc.op_name);
		strcat(dummy," ");
	}

    if (sp !=0 || spop != 0) {
        strcpy(message,"error: syntax error \n");
        strcat(message, expression);
        alertbox(message);
        return 1;
    }

	strcpy(expression, dummy); // copy the converted post-fix expression to the char var expression

	return 0;
}


/*****************************************************/
int op_prec(char s[]) {
/*****************************************************/

	if (s[0]=='^') return 1;

	if (s[0]=='*' || s[0]=='/') return 2;

	if (s[0] == '-' || s[0] == '+') return 3;

	return 0;
}


/*****************************************************/
int operat(char s[]) {
/*****************************************************/

	if (s[0] == '-' || s[0] == '+' || s[0]=='*' || s[0]=='/' || s[0]=='^') return 1;

	if(!strcmp(s,"exp") || !strcmp(s,"log") || !strcmp(s,"sin") || !strcmp(s,"asin") || !strcmp(s,"cos") || !strcmp(s,"acos") || !strcmp(s,"tan")
			|| !strcmp(s,"atan") || !strcmp(s,"sinh") || !strcmp(s,"cosh") || !strcmp(s,"tanh") || !strcmp(s,"abs") || !strcmp(s,"sqrt")
			|| !strcmp(s,"log10"))
		return 1;
	else
		return 0;
}
