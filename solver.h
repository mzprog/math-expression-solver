#ifndef _SOLVER_H_
#define _SOLVER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//define some flags for functions
#define NULL_F	0
#define COS_F	1
#define SIN_F	2
#define TAN_F	3
#define LOG_F	4
#define LOG10_F	5

//define flags for missionDone varialbe
#define RESET	0
#define DONE	1
#define UPDATE	2

//define operation flags
#define MUL	201
#define DIV	202
#define MOD	203
#define ADD	204
#define SUB	205

//define operation signal
#define BR_TO_FX 301 //when change the bracket to tilts and dots


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
void power(int f,int l);//solve the power int the expression.
void Mul_Div_Mod(int f, int l);// solve the multiplication and division and the modules.
void Add_Sub(int f, int l);//solve the addition and subtriction.
void Sign_Dot(int f,int l);//to show if we have to sign beside each other solve it.

int checkSimple(int f, int l);//check if we have unique simple number in parameter.
int ifNum(char n);//check if this character is number or dot.
double CloneNumber(int f, int l);//clone the number from the string.
int FuncFlag(int f);//find what is the function will use and one parameter to the first open bracket ~.
void Calc_and_put_result(int Flag,double Number,int brOp,int brCl);//calculate the result of the funtion then update the expression.
void update_expression(double num , int start, int end,int flag);//only for update the expression

void intToStr(int n ,char *str);//convert integer to string.

void print(int f, int l);//temporary used function.

#endif
