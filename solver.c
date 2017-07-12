/*
 * calc.cpp
 *
 * the math expression solver.
 *
 * Author : Mohamad zbib
 * 
 * THIS SOFTWARE UNDER THE BSD LICENSE.
 * COPYRIGHT 2017.
 * 
 * 
 *
 * when you use this libary you should check from your program that
 * the expression is true without any syntax error.
 *
 *
 *
 * NOTE:
 * 		When we find bracket used for parameter of a function we will convert it to tilts and dots
 * 		exmple:
 * 				cos(2*12)  <<===>>   cos~.24.~
 * 		this will make it easy to solve and less latency.
 *
 * this library built first time to be a part of private project,
 * then it was an open source for all.
 *
 *
 * project started on july 2017.
 *
 */
#include "solver.h"



int main (){
	//this main function just for test our libary

	expression=(char *)malloc(256);//allocate the array
	temp=(char *)malloc(256);//allocate the array
	do{
		printf("write an expression:\n");
		scanf("%s",expression);

		solve(expression);	//we have the same global varaible but when we remove the main,
							//we will have a different.
		printf("ans=%s\n",expression);
	}while(strcmp(expression,"q")!=0);
	free(expression);//after the end destroy the location
	free(temp);//after the end destroy the location

	return 0;
}

void solve(const char*data){

		//use the global variables
		strcpy(expression,data);//with this main we have the same variable


		//start first with high priority first

		//level one
		do{
		//while we still found new operation.
			opLen=strlen(expression);
			bracket(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;//reset the mission variable.
		
	
		do{
			Sign_Dot(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;
		
		//level two
		do
		{
			opLen=strlen(expression);
			functions(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;//reset the mission variable.


	/*
	 * we should skip the Sign_Dot here to show if we will include the sign under the power
	 */
		
		//level three
		do
		{
			opLen=strlen(expression);
			power(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;

		do{
			Sign_Dot(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;
		
		//level four
		do
		{
			opLen=strlen(expression);
			Mul_Div_Mod(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;


		do{
			Sign_Dot(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;		
		//level five
		do
		{
			opLen=strlen(expression);
			Add_Sub(0,opLen-1);
		}while(missionDone==UPDATE);
		missionDone=RESET;
}

void bracket(int start, int end){
	int i;
	int brOp,brCl;//brOp for bracket open && brCl for bracket closed
	double tmpNum;//to store the result number temporary.

	again_bracket:
	//reset the variable.
	brOp=0;
	brCl=0;

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
		missionDone=DONE;
		return;
	}
	else{
		//the loop to end solving the bracket.
		while(1){

			if(checkSimple(brOp+1,brCl-1)==1)
			{
				//clone the simple number to variable.
				tmpNum=CloneNumber(brOp+1,brCl-1);

				//update the expression string and break the loop.
				//check if is function parameter.
				if(FuncFlag(brOp+1)!=0)
					update_expression(tmpNum,brOp,brCl,BR_TO_FX);//update and change to tilts for function.
				else
					update_expression(tmpNum,brOp,brCl,NULL_F);
				missionDone=UPDATE;//we still update the expression
				return;
			}
			//the test of what we found!!
			//we shold check it simple later
			
		
			Sign_Dot(0,opLen-1);
			if(missionDone=UPDATE){
				missionDone=RESET;
				goto again_bracket;
			}
			missionDone=RESET;	
			functions(brOp+1,brCl-1);//try the next step.
			if(missionDone==UPDATE)
			{
				missionDone=RESET;//reset the mission variable.
				goto again_bracket;
			}

			power(brOp+1,brCl-1);
			if(missionDone==UPDATE){
				missionDone=RESET;//reset the mission variable.
				goto again_bracket;
			}
			Mul_Div_Mod(brOp+1, brCl-1);
			if(missionDone==UPDATE){

				missionDone=RESET;//reset the mission variable.
				goto again_bracket;
			}

			Add_Sub(brOp+1, brCl-1);
			if(missionDone==UPDATE){

				missionDone=RESET;//reset the mission variable.
				goto again_bracket;
			}
		}
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

		missionDone=DONE;//send a signal to end the function
		return;
	}
	else{

		if(checkSimple(brOp,brCl)==1){
			CNumber=CloneNumber(brOp,brCl);//get the parameter of the function.
			FName=FuncFlag(brOp);
			Calc_and_put_result(FName,CNumber,brOp,brCl);//calculate the function result and edit the main expression.
			missionDone=UPDATE;//the mission here achived

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

void power(int f,int l){
	int i;
	int pr=-1;//the index of the power operation symbol.
	int opS=-1,opE=-1;//define opreration start and end.
	double base=0,powerN=0;
	double answer=0;

	for(i=f;i<=l;i++)
	{
		if(expression[i]=='^')
		{
			pr=i;
			break;
		}
	}

	if(pr==-1){
		missionDone=DONE;
		return;
	}
	else{

		//find the base number.
		for(i=pr-1;i>=0;i--){

			if(ifNum(expression[i])==1)
				opS=i;
			else
				break;
		}
		//check if we have a sign for a number 
		if(expression[opS-1]=='-' || expression[opS-1]=='+')
			if(expression[opS-2]=='-' || expression[opS-2]=='+')
				opS--;//include the sign with the number.
		
		base=CloneNumber(opS,pr-1);

		//find the the power number.
		for(i=pr+1;i<=l;i++){
			if(ifNum(expression[i])==1)
				opE=i;
			else
				break;
		}
		powerN=CloneNumber(pr+1,opE);

		//get the answer now.
		answer=pow(base,powerN);
	
		//update the expression.
		update_expression(answer,opS,opE,NULL_F);
		missionDone=UPDATE;
	}
}

void Mul_Div_Mod(int f, int l){
	int i,pos=-1,Flag=0;//index , position of operation and flag to specify the operation.
	double num1=0,num2=0,result=0;//for calculation
	int opS=-1,opE=-1;//the start and the end of the equation.
	int len;//the length of the expression.
	
	for(i=f;i<=l;i++)
	{
		switch(expression[i])
		{
			case '*':
				pos=i;
				Flag=MUL;
				break;
			case '/':
				pos=i;
				Flag=DIV;
				break;
			case '%':
				pos=i;
				Flag=MOD;
				break;
		}
		if(Flag!=0)
			break;//if we break from the stwitch we should break from the loop
		
	}

	if(Flag==0){
		missionDone=DONE;
		return;
	}

	//find the first number
	for(i=pos-1;i>=0;i--){
		if(ifNum(expression[i])==1)
			opS=i;
		else
			break;
	}

	num1=CloneNumber(opS,pos-1);

	//find the second number
	for(i=pos+1;i<=l;i++)
	{
		if(ifNum(expression[i])==1)
			opE=i;
		else
			break;
	}
	num2=CloneNumber(pos+1,opE);

	//get the result
	switch(Flag){
		case MUL:
			result=num1*num2;
			break;
		case DIV:
			result=num1/num2;
			break;
		case MOD:
			result=(int)num1% (int)num2;
			break;
	}

	update_expression(result,opS,opE,NULL_F);
	missionDone=UPDATE;
}

void Add_Sub(int f, int l){
	int i,pos=-1,Flag=0;//index , position of operation and flag to specify the operation.
	double num1=0,num2=0,result=0;//for calculation
	int opS=-1,opE=-1;//the start and the end of the equation.
	int len;//the length of the expression.
	
	
	for(i=f+1;i<=l;i++)
	{
		//if we start i=f so we test the sign of the number 
		//and this will crach the program.
		switch(expression[i])
		{
			case '+':
				pos=i;
				Flag=ADD;
				break;
			case '-':
				pos=i;
				Flag=SUB;
				break;
		}
		
		if(Flag!=0)
			break;//if we break from the stwitch we should break from the loop
	}

	if(Flag==0){
		missionDone=DONE;
		return;
	}

	//find the first number
	for(i=pos-1;i>=0;i--){
		if(ifNum(expression[i])==1)
			opS=i;
		else
			break;
	}

	num1=CloneNumber(opS,pos-1);

	//find the second number
	for(i=pos+1;i<=l;i++)
	{
		if(ifNum(expression[i])==1)
			opE=i;
		else
			break;
	}
	num2=CloneNumber(pos+1,opE);

	//get the result
	switch(Flag){
		case ADD:
			result=num1+num2;
			break;
		case SUB:
			result=num1-num2;
			break;
		
	}

	update_expression(result,opS,opE,NULL_F);
	missionDone=UPDATE;
}

int checkSimple(int f, int l){
	int i;
	if(expression[f]=='-'  || expression[f]=='+')
		f++;//if first if a sign character skip it.

	for(i=f;i<=l;i++)
		if((expression[i]<'0' || expression[i]>'9') && expression[i]!='.' )//if not belong numbers
			return 0;

	return 1;

}

int ifNum(char n){
	if(n>='0' && n<='9')
		return 1;
	if(n=='.')
		return 1;

	return 0;

}

double CloneNumber(int f, int l){
	int i,dotIndex=-1;
	int afterDot=0;
	double CNum=0;
	char sign='+';
	//check if found a sign for the number
	if(expression[f]=='-'){
		f++;
		sign='-';
	}else if(expression[f]=='+'){
		f++;
		sign='+';
	}

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

	if(sign=='-')
		CNum*=-1;//change the sign if it is negative.

	return CNum / pow(10,afterDot);
}

int FuncFlag(int f){
	//check first the kind of the brackets.
	if(expression[f-1]=='.' &&expression[f-2]=='~')
		f-=3;
	else if(expression[f-1]=='(')
		f-=2;
	//if cosine
	if(expression[f-2]=='c' && expression[f-1]=='o' && expression[f]=='s')
		return COS_F;
	//if sine
	if(expression[f-2]=='s' && expression[f-1]=='i' && expression[f]=='n')
		return SIN_F;
	//if log
	if(expression[f-2]=='l' && expression[f-1]=='o' && expression[f]=='g')
		return LOG_F;

	//if log10
	if(expression[f-4]=='l' && expression[f-3]=='o' && expression[f-2]=='g' && expression[f-1]=='1' && expression[f]=='0')
		return LOG10_F;

	//will be continue if we need more..
	return NULL_F;//if we dont find any function.
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

	update_expression(val,start,end,NULL_F);//update the expression.
}

void update_expression(double num , int start, int end,int flag){

	int lenOfStr;
	int intPart=(int)num;
	char strN[32],strN2[32];
	if(num<0){

		strcpy(strN2,"-");
		num*=-1;//avoid problem of negative just.
	}
	else
		strcpy(strN2,"");
	if(num<1)
	{
		strcat(strN2,"0");//if should be 0 or 0.xx or -0.xx
	}
	intToStr(intPart,strN);//convert the integer to string.

	strcat(strN2,strN);// here we complete the conversion of the integer part.

	//now will work on float part.
	num-=intPart;
	lenOfStr=strlen(strN2);

	if(num!=0)
	{
		strN2[lenOfStr++]='.';
		while(num!=0 && lenOfStr<=10){

			num=num*10;
			intPart=(int)num;
			strN2[lenOfStr++]=intPart+'0';
			num-=intPart;
		}
	}
		strN2[lenOfStr]='\0';//put the null termenator at the end of the string.

		//now we have the string then we update.

		strncpy(temp,expression,start);
		temp[start]='\0';//to ensure the null.
			//check the flag .
		if(flag==BR_TO_FX)
			strcat(temp,"~.\0");//if function insert the bracket of the function.

		strcat(temp,strN2);//insert the number
			//again the flag to end the bracket of the function.
		if(flag==BR_TO_FX)
			strcat(temp,".~\0");

		strcat(temp,expression+end+1);
		strcpy(expression,temp);
}

void intToStr(int n ,char *str){
	int i=0;
	int len;
	char tmp[32];//to store the data temporary in reversed order

	while(n!=0){

		tmp[i++]=(n%10)+'0';//put the last digit directly at the beginning of the string.
		n/=10;
	}
	tmp[i]='\0';//put the null tremenitor at the end of the string.

	//second part reverse the string to give the correct answer.
	len=strlen(tmp);

	for(i=0;i<len;i++)
		str[i]=tmp[len-1-i];
	str[i]='\0';//Null termenator.

}

void Sign_Dot(int f,int l){
	
	char result[2];//to use it in strcat as a string
	int i,pos;
	result[0]='\0';
	result[1]='\0';
	for(i=f;i<l;i++)
	{
		if(expression[i]=='+' || expression[i]=='-')
			if(expression[i+1]=='+' || expression[i+1]=='-'){//then we have two sign to solve it
				//now getting the result
				if(expression[i]==expression[i+1])
					result[0]='+';
				else
					result[0]='-';
				pos=i;
			}
	}
		if(result[0]=='\0'){
			missionDone=DONE;
			return;
		}
		strncpy(temp,expression,pos);
		temp[pos]=result[0];
		temp[pos+1]='\0';//make sure you put the NULL terminator.
		strcat(temp,expression+pos+2);
		strcpy(expression,temp);
		missionDone=UPDATE;
}

void print(int f, int l){
	int i;
	for(i=f;i<=l;i++)
		printf("%c",expression[i]);
	printf("\n");
}
