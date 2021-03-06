/*
@author:伍丹梅
学号：2016301500017
班级：计科1班
*/

#include "pch.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <vector>
#include <map>
#define MAX 20
using namespace std;

char ocamltxt[10000];
char token[20] = { 0 };  
int res = 1;             //token类型码
int scannext = 0;        //扫描下标
int col = 1, row = 1;    //列号，行号
bool strkind = false;        //String型常量
bool charkind = false;       //Char型常量

typedef struct {
	string FuncName;   //函数名
	vector<string> OcamlList;    //参数列表
}Function;      //函数声明

typedef struct AstTree {
	int type;
	//树结点类型 0树根 1函数声明 2函数名及参数列表 3表达式 4变量声明 5语句组合 6IF-ELSE结构 7函数调用
	string Vars[MAX];           //存储当前结点下的局部变量
	int countV = 0;

	Function Fun[MAX];          //存储程序中定义过的函数的声明
	int countF = 0;

	//Ocaml | Statements
	struct AstTree* nodes[MAX];
	int childNumber;             //孩子结点个数
	int Lnumber;                 //跳转标号

	//Declaration
	struct AstTree* Dlchild;    //左孩子结点
	struct AstTree* Drchild;    //右孩子结点

	//Prototype
	string FuncName;               //函数名
	vector<string> OcamlList;    //参数列表

	//Expr 
	string op;                    //运算符
	struct AstTree* Elchild;      //左表达式孩子
	struct AstTree* Erchild;      //右表达式孩子
	string name;                  //变量名              
	double value;

	int Tentry, Fentry;           //if-else两个跳转

}AstTree;

enum Token {
	tok_bound= -1,
	tok_keyword = -2,
	tok_operator = -3,
	tok_constant = -4,
	tok_identifier = -5
};

static char keyword[56][20] = {
	"and",         "as",          "assert",     "asr" ,     "begin",       "class",
	"constraint",  "do",          "done" ,      "downto",     "else",        "end",
	"exception",   "external",    "false",       "for",         "fun",         "function",
	"functor",     "if",          "in",          "include",     "inherit",     "initializer",
	"land",        "lazy",        "let",         "lor",         "lsl",         "lsr",
	"lxor",        "match",       "method",      "mod",         "module",      "mutable",
	"new",         "nonrec",      "object",     "of",          "open",         "or",
	"private",     "rec",         "sig",         "struct",      "then",       "to",
	"true",        "try",         "type",        "val",         "virtual",     "when",
	"while",       "with"
};

//界符表
static char bound[11][20] = {
	";", ";;", "\"", "[", "]", "(", ")", "|","'",
	"[<",">]"
};

//运算符表
static char opert[33][20] = {
	"+", "-", "*", "/", "+.", "-.", "*.", "/.", "%","**","=/","*/","-/","+/"
	"->", ">", "<", ">=", "<=", ":=", "||", "&&", "==", "!=",".","!"
	,"@","^",",","::","~","`","#","?"
};


int searchKeyword(char keyword[][20], char s[]){
	for (int i = 0; i < 56; i++)
		if (strcmp(keyword[i], s) == 0){
			return i;
		}

	return -1;
}

bool searchBound(char bound[][20], char ch, char next){
	for (int i = 0; i < 11; i++){
		if (ch == '|' && next != '|')
			return true;
		else if (ch == '>' && next == ']')
			return true;
		else if (ch != '|' && ch != '>' && ch == bound[i][0])
			return true;
	}

	return false;
}

int scanner(char ocamltxt[], char token[], int &scannext){
	int i, count = 0;
	char ch; 
	ch = ocamltxt[scannext];

	//跳过空格
	while (ch == ' '){
		scannext++;
		col++;
		ch = ocamltxt[scannext];
	}

	for (i = 0; i < 20; i++)
		token[i] = '\0';

	//结束符
	if (ch == '\0'){
		return 0;
	}

	else if (ch == '\n'){
		row += 1;
		col = 1;
		scannext++;
		token[count++] = ch;
		return 2;  
	}
	//注释部分
	else if (ch == '(' && ocamltxt[scannext + 1] == '*'){
		scannext = scannext + 2;
		col += 2;
		while (ocamltxt[scannext] != '*' || ocamltxt[scannext + 1] != ')'){
			scannext++;
			col++;

			if (ocamltxt[scannext] == '\n'){
				row += 1;
				col = 1;
			}

			if (ocamltxt[scannext] == '\0'){
				cout << "注释出错，没有找到*), 分析结束！\n";
				exit(0);
			}
		}
		scannext += 2; 
		col += 2;
		return 2;   
	}

	else if (ocamltxt[scannext - 1] == '\'' && charkind){
		token[count++] = ch;
		scannext++;
		col++;
		token[count] = '\0';

		return tok_constant;
	}
	else if (ocamltxt[scannext - 1] == '"' && strkind){
		do {
			token[count++] = ch;
			scannext++;
			col++;
			ch = ocamltxt[scannext];
		} while (ch != '"');

		token[count] = '\0';

		return tok_constant;  
	}
	//开头为字母或者下划线
	else if (isalpha(ch) || ch == '_'){
		do {
			token[count++] = ch;
			scannext++;
			col++;
			ch = ocamltxt[scannext];
		} while (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '\'');
		token[count] = '\0';

		//判断该token是关键字还是用户自定义标识符
		int res = searchKeyword(keyword, token);

		if (res != -1)  
			return tok_keyword;
		else             
			return tok_identifier;
	}

	else if (isdigit(ch)){
		do { 
			token[count++] = ch;
			scannext++;
			col++;
			ch = ocamltxt[scannext];
		} while (isdigit(ch) || ch == '.');

		token[count] = '\0';

		return tok_constant;    
	}

	else if (searchBound(bound, ch, ocamltxt[scannext + 1])){
		if (ch == '"')
			strkind = !strkind;
		else if (ch == '\'')
			charkind = !charkind;

		token[count++] = ch;
		scannext++;
		col++;


		if ((ch == ';'&& ocamltxt[scannext] == ';') || (ch == '[' && ocamltxt[scannext] == '<') || (ch == '>' && ocamltxt[scannext])){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}

		token[count] = '\0';

		return tok_bound;
	}
	//运算符 +,-,*,/,+.,-.,*.,/.,->,**,+/,-/,*/
	else if (ch == '+' || ch == '-' || ch == '*' || ch == '/'){
		token[count++] = ch;
		scannext++;
		col++;

		if (ocamltxt[scannext] == '.' || ocamltxt[scannext] == '/'){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}
		else if (ch == '-'){
			if (ocamltxt[scannext] == '>')
			{
				token[count++] = '>';
				scannext++;
				col++;
			}
		}
		else if (ch == '*'){
			if (ocamltxt[scannext] == '*'){
				token[count++] = '*';
				scannext++;
				col++;
			}
		}

		token[count] = '\0';

		return tok_operator;
	}
	//运算符 >,>=,<,<=,:=,==,!=,!,::,=/
	else if (ch == '>' || ch == '<' || ch == ':' || ch == '=' || ch == '!'){
		token[count++] = ch;
		scannext++;
		col++;

		if (ocamltxt[scannext] == '='){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}
		else if (ch == ':' && ocamltxt[scannext] == ':'){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}
		else if (ch == '=' && ocamltxt[scannext] == '/'){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}

		token[count] = '\0';

		return tok_operator;
	}
	//运算符||,&&,%,.,^,@，~,`,#
	else if (ch == '|' || ch == '&' || ch == '%' || ch == '.' || ch == '^' || ch == '@' || ch == ',' || ch == '~' || ch == '`' || ch == '#' || ch == '?'){
		token[count++] = ch;
		scannext++;
		col++;

		if ((ch == '|' && ocamltxt[scannext] == '|') || (ch == '&' && ocamltxt[scannext] == '&')){
			token[count++] = ocamltxt[scannext];
			scannext++;
			col++;
		}

		token[count] = '\0';

		return tok_operator;
	}
	else
	{
		cout << "error at " << row << "," << col << endl;
		exit(0);
	}
}

static int CurTok;
static int getNextToken() { return CurTok = scanner(ocamltxt, token, scannext); }

AstTree* Ocaml = new AstTree;      

bool Varexist(string Vars[], int countV, string name){
	if (name == "")    
		return true;

	for (int i = 0; i < countV; i++){
		if (Vars[i] == name)
			return true;
	}
	return false;
}

int Funexist(string FuncName){
	for (int i = 0; i < Ocaml->countF; i++){
		if (Ocaml->Fun[i].FuncName == FuncName)
			return i;
	}
	return -1;
}

static std::map<string, int> Pre;

static int GetTokPrecedence() {
	if (!isascii(token[0]))
		return -1;

	int TokPrec = Pre[token];
	if (TokPrec <= 0)
		return -1;
	return TokPrec;
}

AstTree* ParseNumberExpr() {
	AstTree* Expr = new AstTree;
	Expr->value = strtod(token, nullptr);
	Expr->type = 3;
	Expr->name = "";
	Expr->Elchild = Expr->Erchild = NULL;

	getNextToken(); 
	return Expr;
}

AstTree* ParseExpression(AstTree* Upperlevel);
AstTree* ParseParenExpr(AstTree* Upperlevel) {
	getNextToken(); 
	AstTree* V = ParseExpression(Upperlevel);
	if (!V)
		return nullptr;

	getNextToken(); 
	return V;
}

AstTree* ParseIdentifierExpr() {
	AstTree* Expr = new AstTree;

	Expr->name = token;
	Expr->type = 3;
	Expr->Elchild = Expr->Erchild = NULL;

	getNextToken(); 

	return Expr;
}

AstTree* ParsePrimary(AstTree* Upperlevel) {
	switch (CurTok) {
	case tok_identifier:
		return ParseIdentifierExpr();
	case tok_constant:
		return ParseNumberExpr();
	default:
		if (token[0] == '(')
			return ParseParenExpr(Upperlevel);
	}
}

AstTree* ParseBinOpRHS(int ExprPrec, AstTree* LHS, AstTree* Upperlevel) {

	while (true) {
		int TokPrec = GetTokPrecedence();

		if (TokPrec < ExprPrec)
			return LHS;

		string BinOp = token;
		getNextToken(); 

		AstTree* RHS = ParsePrimary(Upperlevel);

		if (!Varexist(Upperlevel->Vars, Upperlevel->countV, RHS->name)){
			cout << "error at " << row << "," << col << ": " << RHS->name << " is not defined\n";
			exit(0);
		}

		if (!RHS)
			return nullptr;

		int NextPrec = GetTokPrecedence();
		if (TokPrec < NextPrec) {
			RHS = ParseBinOpRHS(TokPrec + 1, RHS, Upperlevel);
			if (!RHS)
				return nullptr;
		}

		AstTree* temp = new AstTree;
		temp->type = 3;
		temp->Elchild = LHS;
		temp->Erchild = RHS;
		temp->op = BinOp;

		LHS = temp;
	}
}

AstTree* ParseExpression(AstTree* Upperlevel) {
	AstTree* LHS = ParsePrimary(Upperlevel);

	if (CurTok == -3) {
		if (!Varexist(Upperlevel->Vars, Upperlevel->countV, LHS->name)){
			cout << "error at " << row << "," << col << ": " << LHS->name << " is not defined\n";
			exit(0);
		}

		if (!LHS)
			return nullptr;

		return ParseBinOpRHS(0, LHS, Upperlevel);
	}
	else{
		AstTree* FC = new AstTree;
		FC->type = 7;
		FC->FuncName = LHS->name;

		int Findex = Funexist(LHS->name);
		if (Findex == -1){
			cout << "error at " << row << "," << col << ": " << LHS->name << " is not defined\n";
			exit(0);
		}

		int ArgumentNumber = 0;
		vector<string> Arguments;
		while (CurTok != 2 && CurTok != 0){
			ArgumentNumber++;
			Arguments.push_back(token);
			getNextToken();
		}
		if (ArgumentNumber != Ocaml->Fun[Findex].OcamlList.size()){
			cout << "error at " << row << "," << col << ": Number of needed arguments is wrong\n";
			exit(0);
		}
		FC->OcamlList = Arguments;
		return FC;
	}

}

AstTree* ParseStatements(AstTree* Upperlevel);

AstTree* ParseIF(AstTree* Upperlevel){
	AstTree* IFnode = new AstTree;
	IFnode->type = 6;
	IFnode->childNumber = 0;

	getNextToken();   
	IFnode->nodes[IFnode->childNumber] = ParseExpression(Upperlevel);
	IFnode->childNumber++;

	if (strcmp(token, "then") != 0)
		cout << "error: at" << row << "," << col << " IF-ELSE structure is not complete.";

	getNextToken();  
	IFnode->nodes[IFnode->childNumber] = ParseStatements(Upperlevel);
	IFnode->childNumber++;

	if (strcmp(token, "else") != 0)
		return IFnode;

	getNextToken(); 
	IFnode->nodes[IFnode->childNumber] = ParseStatements(Upperlevel);
	IFnode->childNumber++;

	return IFnode;

}

AstTree* ParseDefinition(AstTree* Upperlevel);		 
AstTree* ParseStatements(AstTree* Upperlevel) {
	AstTree* Statements = new AstTree;
	Statements->childNumber = 0;
	Statements->type = 5;

	while (strcmp(token, ";;") != 0 && strcmp(token, "else") != 0){
		if (strcmp(token, "\n") == 0 || strcmp(token, "in") == 0)
			getNextToken();
		else if (strcmp(token, "let") == 0){
			Statements->nodes[Statements->childNumber] = ParseDefinition(Upperlevel);
			Statements->childNumber++;
		}
		else if (strcmp(token, "if") == 0){
			Statements->nodes[Statements->childNumber] = ParseIF(Upperlevel);
			Statements->childNumber++;
		}
		else{
			Statements->nodes[Statements->childNumber] = ParseExpression(Upperlevel);
			Statements->childNumber++;
		}
	}
	return Statements;
}

AstTree* ParsePrototype(AstTree* FD) {
	AstTree* prototype = new AstTree;
	prototype->type = 2;
	prototype->FuncName = token; 

	while (getNextToken() == tok_identifier){
		prototype->OcamlList.push_back(token);
		FD->Vars[FD->countV] = token;
		FD->countV++;
	}

	getNextToken();
	if (CurTok == 2)
		getNextToken();

	return prototype;
}

AstTree* ParseDefinition(AstTree* Upperlevel) {
	getNextToken();
	int tempscannext = scannext;
	string tempToken = token;
	int tempRes = res;
	int tempCurTok = CurTok;
	int tempCol = col;
	int tempRow = row;
	bool tempstrkind = strkind;
	bool tempcharkind = charkind;

	int flag = 0;    

	while (strcmp(token, ";;") != 0){
		if (strcmp(token, "let") == 0){
			flag = 0;
			break;
		}
		else if (strcmp(token, "in") == 0)
		{
			flag = 1;
			break;
		}
		getNextToken();
	}

	strcpy_s(token, tempToken.c_str());
	res = tempRes;
	scannext = tempscannext;
	col = tempCol;
	row = tempRow;
	strkind = tempstrkind;
	charkind = tempcharkind;
	CurTok = tempCurTok;


	if (flag == 1){
		AstTree* VDecla = new AstTree;
		VDecla->name = token;
		VDecla->type = 4;

		Upperlevel->Vars[Upperlevel->countV] = token;
		Upperlevel->countV++;

		return VDecla;
	}

	if (strcmp(token, "rec") == 0)
		getNextToken(); 

	AstTree* FDecla = new AstTree;
	FDecla->type = 1;

	AstTree* Proto = ParsePrototype(FDecla);
	if (!Proto)
		return nullptr;

	Ocaml->Fun[Ocaml->countF].FuncName = Proto->FuncName;
	Ocaml->Fun[Ocaml->countF].OcamlList = Proto->OcamlList;
	Ocaml->countF++;

	if (AstTree* S = ParseStatements(FDecla)){
		FDecla->Dlchild = Proto;
		FDecla->Drchild = S;

		return FDecla;
	}
	return nullptr;
}

int Lnumber = 0;  
int tnumber = 0;

void printExp(AstTree* node){
	if (node->op == "="){
		if (node->Erchild->name == "")
			cout << node->Erchild->value;
		else
			cout << node->Erchild->name;

		cout << " " + node->Elchild->name << endl;
		return;
	}
	if (node->Elchild->name == "")
		cout << node->Elchild->value;
	else
		cout << node->Elchild->name;
	cout << " ";
	if (node->Erchild->name == "")
		cout << node->Erchild->value;
	else
		cout << node->Erchild->name;
	cout << " t" << tnumber << endl;

	node->name = "t" + to_string(tnumber);
	tnumber++;
}

void gen(AstTree* node){
	if (node == NULL || node->type == 2 || node->type == 3 || node->type == 4 || node->type == 7)
		return;
	else{
		if (node->type == 0)   {
			for (int i = 0; i < node->childNumber; i++)
				gen(node->nodes[i]);
		}
		else if (node->type == 1)   {
			gen(node->Dlchild);
			gen(node->Drchild);
		}
		else if (node->type == 5)  {
			node->Lnumber = Lnumber;
			Lnumber++;
			for (int i = 0; i < node->childNumber; i++)
				gen(node->nodes[i]);
		}
		else if (node->type == 6) {
			node->nodes[0]->Tentry = Lnumber;
			node->nodes[0]->Fentry = Lnumber + 1;
			for (int i = 1; i < node->childNumber; i++)
				gen(node->nodes[i]);
		}
	}

}

void IR(AstTree* node){
	if (node == NULL)
		return;
	else{
		if (node->type == 0){
			for (int i = 0; i < node->childNumber; i++)
				IR(node->nodes[i]);
		}
		else if (node->type == 1){
			IR(node->Dlchild);
			IR(node->Drchild);
			cout << "}" << endl << endl << endl;
		}
		else if (node->type == 2){
			string para = "";
			for (int i = 0; i < node->OcamlList.size(); i++)
				if (i + 1 != node->OcamlList.size())
					para = para + node->OcamlList[i] + ",";
				else
					para = para + node->OcamlList[i];

			cout <<  node->FuncName + "(" + para + "){" << endl;
		}
		else if (node->type == 3){
			if (node->Elchild != NULL && node->Erchild != NULL) {

				if (node->op == ">")
					cout << "jbe " << node->Elchild->name << " " << node->Erchild->value << " L" << node->Fentry << endl;
				else if (node->op == ">=")
					cout << "jb " << node->Elchild->name << " " << node->Erchild->value << " L" << node->Fentry << endl;
				else if (node->op == "<")
					cout << "jge " << node->Elchild->name << " " << node->Erchild->value << " L" << node->Fentry << endl;
				else if (node->op == "<=")
					cout << "jg " << node->Elchild->name << " " << node->Erchild->value << " L" << node->Fentry << endl;
				else{
					IR(node->Elchild);
					IR(node->Erchild);

					if (node->op == "+")
						cout << "add ";
					else if (node->op == "+.")
						cout << "fadd ";
					else if (node->op == "-")
						cout << "sub ";
					else if (node->op == "-.")
						cout << "fsub ";
					else if (node->op == "*")
						cout << "mul ";
					else if (node->op == "*.")
						cout << "fmul ";
					else if (node->op == "/")
						cout << "div ";
					else if (node->op == "/.")
						cout << "fdiv ";
					else if (node->op == "=")
						cout << "mov ";
					printExp(node);
				}
			}
		}
		else if (node->type == 4) {
			cout << "alloc " + node->name << endl;
		}
		else if (node->type == 5){
			cout << "L" << node->Lnumber << ":" << endl;
			for (int i = 0; i < node->childNumber; i++)
				IR(node->nodes[i]);
		}
		else if (node->type == 6) {
			for (int i = 0; i < node->childNumber; i++)
				IR(node->nodes[i]);
		}
		else if (node->type == 7){
			string para = "";
			for (int i = 0; i < node->OcamlList.size(); i++)
				if (i + 1 != node->OcamlList.size())
					para = para + node->OcamlList[i] + ",";
				else
					para = para + node->OcamlList[i];

			cout << "call " << node->FuncName << "(" + para + ")" << endl;
		}
	}
}

int main(){
	Pre["="] = 5;
	Pre["<"] = 10;
	Pre[">"] = 10;
	Pre["<="] = 10;
	Pre[">="] = 10;
	Pre["+"] = 20;
	Pre["-"] = 20;
	Pre["*"] = 40;
	Pre["/"] = 40;
	Pre["+."] = 20;
	Pre["-."] = 20;
	Pre["*."] = 40;
	Pre["/."] = 40;

	FILE * fp;
	if ((fp = fopen("E:\\done\\编译原理\\task3\\ocaml.txt", "r")) == NULL){
		cout << "不能打开文件.\n";
		exit(0);
	}

	char cc; 
	while ((cc = fgetc(fp)) != EOF)
		ocamltxt[scannext++] = cc;

	ocamltxt[scannext] = '\0';
	fclose(fp);

	scannext = 0;

	Ocaml->type = 0;
	Ocaml->childNumber = 0;

	getNextToken();
	while (CurTok != 0){
		if (strcmp(token, "\n") == 0 || strcmp(token, "in") == 0 || strcmp(token, ";;") == 0)
			getNextToken();
		else if (strcmp(token, "let") == 0){
			Ocaml->nodes[Ocaml->childNumber] = ParseDefinition(Ocaml);
			Ocaml->childNumber++;
		}
		else if (strcmp(token, "if") == 0){
			Ocaml->nodes[Ocaml->childNumber] = ParseIF(Ocaml);
			Ocaml->childNumber++;
		}
		else{
			Ocaml->nodes[Ocaml->childNumber] = ParseExpression(Ocaml);
			Ocaml->childNumber++;
		}
	}
	

	//前序遍历
	gen(Ocaml);

	//生成三地址码
	IR(Ocaml);

	return 0;
}