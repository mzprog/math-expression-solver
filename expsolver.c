/*
 * calc.cpp
 *
 * the math expression solver.
 *
 * Author : Mohamad zbib
 *
 *
 * when you use this libary you should check from your program that
 * the expression is true without any syntax error.
 *
 * NOTE:
 * 		When we find bracket used for parameter of a function we will convert it to tilts and dots
 * 		exmple:
 * 				cos(2*12)  <<===>>cos~.24.~
 * 		this will make it easy to solve and less latency.
 *
 * this library built first time to be a part of private project,
 * then it was an open source for all.
 *
 *
 * project started on july 2017.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//define some flags
#define COS_F	1
#define SIN_F	2
#define TAN_F	3
#define LOG_F	4
#define LOG10_F	5


//Global variables
char * expression;//hold the math expression
unsigned int opLen;//hold the length of expression
char * temp;//used temporary for substitude the calculated value

unsigned char missionDone=0;//the boolean value of test of operation

//defines of functions
void solve(const char *data);	//we should change later from void to double;
								//now we will use just the printf because have many answers from each piece of data

void bracket(int start, int end);//the bracket solver.
void functions(int f,int l);//check the built-in function as log and trig.
							//this function should be developed later.

int checkSimple(int f, int l);//check if we have unique simple number in parameter.
double CloneNumber(int f, int l);//clone the number from the string.
int FuncFlag(int f);//find what is the function will use and one parameter to the first open bracket ~.
void Calc_and_put_result(int Flag,double Number,int brOp,int brCl);//calculate the result of the funtion then update the expression.
void update_expression(double num , int start, int end);//only for update the expression



void print(int f, int l);//temporary used function.





int main (){
	//this main function just for test our libary

	expression=(char *)malloc(256);//allocate the array
	temp=(char *)malloc(256);//allocate the array
	scanf("%s",expression);

	solve(expression);	//we have the same global varaible but when we remove the main,
						//we will have a different.

	free(expression);//after the end destroy the location
	free(temp);//after the end destroy the location

	return 0;
}

void solve(const char*data){

		//use the global variables
		strcpy(expression,data);//with this main we have the same variable
		opLen=strlen(data);

		//high priority first
		bracket(0,opLen-1);

		/*
		//////////////////
		TO BE CONTINUE....
		//////////////////
		*/

}

void bracket(int start, int end){
	int i;
	int brOp=0,brCl=0;//brOp for bracket open && brCl for bracket closed

	for(i=start;i<=end;i++)
	{
		if(expression[i]=='('){
			brOp=i;//define new bracket
		}
		else if(expression[i]==')'){
			brCl=i;
			break;
		}

	}

	if(brCl==0){
		printf("No bracket found\n");
		//this printf should be replaced of missiondone boolean var
		//when we make the calculation of the numbers to make the loops.
	}else{
		//the test of what we found!!
		print(brOp,brCl);

		functions(brOp+1,brCl-1);//try the next step.
	}
}

void functions(int f,int l){
	int i ;
	int brOp=0,brCl=0;//open and closed bracket for  functions.
	double CNumber;//the number the we take from CloneNumber function.
	int FName;//a flag to specify what this function will used.
	//NOTE: Read the note above in description.

	for(i=f;i<=l;i++){
		if(expression[i]=='~' && expression[i+1]=='.'){
			brOp=i+2;
			i++;//the increment of the var should be 2 but the loop make the anather
		}
		if(expression[i]=='.' && expression[i+1]=='~'){
			brCl=i-1;
			break;
		}
	}

	if(brCl==0){
		printf("No function found\n");
		//this function should be replaced with missiondone later.
	}else{
		print(brOp-2,brCl+2);
		if(checkSimple(brOp,brCl)==1){
			CNumber=CloneNumber(brOp,brCl);//get the parameter of the function.
			FName=FuncFlag(brOp);
			Calc_and_put_result(FName,CNumber,brOp,brCl);//calculate the function result and edit the main expression.

			/*
			 * check which function was declared and calculate the result.
			 */

		}else
		{
			printf("will do it later!!\n");
			/*
			 * we should calculate the number to be simple.
			 * will gonna done later.
			 */
		}
	}

}

int checkSimple(int f, int l){
	int i;
	for(i=f;i<=l;i++)
		if((expression[i]<'0' || expression[i]>'9') && expression[i]!='.')//if not belong numbers
			return 0;

	return 1;

}

double CloneNumber(int f, int l){
	int i,dotIndex=-1;
	int afterDot=0;
	double CNum=0;
	//we will do it in best way to avoid double problems
	for(i=f;i<=l;i++){
		if(expression[i]>='0' && expression[i]<='9')
		{
			CNum*=10;
			CNum+= (expression[i]-'0');
		}
		if(expression[i]=='.')
			dotIndex=i;
		if(expression[i]!='.' && dotIndex!=-1)//we have found a number after dot
			afterDot++;
	}

	return CNum / pow(10,afterDot);
}

int FuncFlag(int f){
	f-=3;
	//if cosine
	if(expression[f-2]=='c' && expression[f-1]=='o' && expression[f]=='s')
		return COS_F;
	//if sine
	if(expression[f-2]=='s' && expression[f-1]=='i' && expression[f]=='n')
		return SIN_F;
	//if log
	if(expression[f-2]=='l' && expression[f-1]=='o' && expression[f]=='o')
		return LOG_F;

	//if log10
	if(expression[f-4]=='l' && expression[f-3]=='o' && expression[f-2]=='o' && expression[f-1]=='1' && expression[f]=='0')
		return LOG10_F;

	//will be continue if we need more..

}

void Calc_and_put_result(int Flag,double Number,int brOp,int brCl){

	int start,end=brCl+2;
	double val;
	switch(Flag){
		case COS_F:
			start=brOp-5;
			val=cos(Number);
			break;
		case SIN_F:
			start=brOp-5;
			val=sin(Number);
			break;
		case TAN_F:
			start=brOp-5;
			val=tan(Number);
			break;
		case LOG_F:
			start=brOp-5;
			val=log(Number);
			break;
		case LOG10_F:
			start=brOp-7;
			val=log10(Number);
			break;

			//to be continue...
	}

	update_expression(val,start,end);//update the expression.
}

void update_expression(double num , int start, int end){

	int lenOfStr;
	int intPart=(int)num;
	char strN[32],strN2[32];
	if(num<0)
		strcpy(strN2,"-");
	else
		strcpy(strN2,"");
	itoa(intPart,strN,10);
	strcat(strN2,strN);// here we complete the conversion of the integer part.

	//now will work on float part.
	num-=intPart;
	if(num<0)
		num*=-1;//avoid problem of negative just.

	if(num!=0)
	{
		lenOfStr=strlen(strN2);
		strN2[lenOfStr++]='.';
		while(num!=0){
			num=num*10;
			intPart=(int)num;
			strN2[lenOfStr++]=intPart+'0';
			num-=intPart;
		}

		strN2[lenOfStr]='\0';//put the null termenator at the end of the string.

		//now we have the string then we update.
		strncpy(temp,expression,start);
		strcat(temp,strN2);
		strcat(temp,expression+end);
		printf("after update the new value is:\n%s.\n",temp);
		strcpy(expression,temp);
	}


}


void print(int f, int l){
	int i;
	printf("first pirnt=%d\nlastprint=%d\n",f,l);
	for(i=f;i<=l;i++)
		printf("%c",expression[i]);
	printf("\n");
}
