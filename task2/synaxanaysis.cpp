/*
@author:伍丹梅
学号：2016301500017
班级：计科1班 
*/
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string.h>
#include <sstream>
using namespace std;

#define KEY 300
#define ID 301
#define OPERA 302
#define CONST 303
#define BOUND 304
#define NUMOFKEY 60
#define NUMOFOP 26
#define NUMOFBOUND 8
// ---------------- 关键词表 ----------------------
char *keyword_table[NUMOFKEY] = {"and","as","assert","asr","begin","bool","char","class","constraint","do","done","downto","else","end","exception",
					"external","false","float","for","fun","function","functor","if","in","int","include","inherit","initializer","land","lazy",
					"let","lor","lsl","lsr","lxor","match","method","mod","module","mutable","new","object","of","open","or",
					"private","rec","sig","string","struct","then","to","true","type","unit","val","virtual","when","while","with"};

// ---------------- 运算符表 ----------------------
static const char *operator_table[NUMOFOP] = {"+", "-" ,"*", "/" , "+.","-.","*.","/.","**","=","<>","==","!=","<","<=",">",">=","&&","||","&",":=","@","^","->","::","."};
    
// ---------------- 限界符表 ----------------------
static const char *boundword_table[NUMOFBOUND] = {"(",")",";;","[","]",";",",",":"};

//关键词种别码1-60
#define KEY_DESC  "KEY"

//常量种别码200-206
#define DEC_VAL     200   //十进制常量
#define OCT_VAL     201  //八进制常量
#define HEX_VAL     202  //十六进制常量
#define BIN_VAL     203   //二进制常量 
#define CHAR_VAL    204   //字符常量
#define FLOAT_VAL   205   //浮点数常量
#define STRING_VAL  206   //字符串常量
#define CONSTANT_DESC "CONST"

//算术运算符150-158，逻辑运算符159-175
#define ADD 150  //+
#define SUB 151   //-
#define MUL 152   //*
#define DIV 153   ///
#define ADDDOT 154 //+.
#define SUBDOT 155 //-.
#define MULDOT 156//*.
#define DIVDOT 157///.
#define MULMUL 158//**

#define EQUAL 159//=
#define NOTEQUALLR 160//<>
#define EQUALEQUAL 161//==
#define NOTEQUAL   162//!=
#define LESS_THAN    163  //<
#define LESSEQUAL_THAN 164 //<=
#define GREATER_THAN 165  //>
#define GREATEREQUAL_THAN 166//>=
#define ANDAND 167//&&
#define OROR 168 //||
#define AND 169 //&
#define EQUAL2 170//:=
#define AT 171//@
#define CONNECT 172//^
#define POINT 173//->
#define COLCOL 174//::
#define DOT 175//.
#define OPERATOR_DESC "OPERA" 

//限界符101-108
#define LEFT_PAR  101 //(
#define RIGHT_PAR 102 //)
#define SEMSEM 103 //;;
#define LEFT_CO 104//[
#define RIGHT_CO 105//]
#define SEM 106  //;
#define COM 107  //,
#define COL 108 //:
#define BOUNTWROD_DESC "BOUND"

//标识符种别码0
#define IDENTIFER 0
#define IDENTIFER_DESC "ID"

typedef struct Token {
    int id;
    int type;
    int line;
    char content[50];
}Token;

typedef struct Node {
    char *desc;
    vector<Node*> children;
    Node(const char* str) {
        desc=new char[30];
        strcpy(desc,str);
    }
} Node;

vector<Token>* tokens;
int Token_n=0;

Node* Program();
Node* OcamlList();
Node* OcamlMore();
Node* Ocaml();
Node* StartOcaml();
Node* ConditionalOcaml();
Node* LoopOcaml();
Node* Loop2Ocaml();
Node* FunctionOcaml();
Node* Function2Ocaml();
Node* MatchOcaml();
Node* TryOcaml();
Node* LetOcaml();
Node* AssertOcaml();
Node* LazyOcaml();
Node* Exp();
Node* Term();
Node* OtherFactor();
Node* OtherTerm();
Node* Factor();
Node* Variable();
Node* VariMore();
Node* FieldVar();
Node* FieldVarMore();
Node* Parse();
Node* CmpOp();
Node* AddOp();
Node* MultOp();

int add(vector<Node* >& children,Node* node) {
    if(node!=NULL) {
        children.push_back(node);
    }
    return 0;
}

vector<Token>*  getTokenlist(const char* filename) {
    tokens->clear();
    FILE *fp;
    if((fp = fopen(filename, "r")) == NULL){
        printf("文件不存在");
    }
    char ch;
    char tmp[30];
    char *word;
    int count=0;
    Token temp;
    while(ch!=-1){
    	ch = fgetc(fp);
    	if(ch==-1){
    		break;
		}
    	if(ch=='K'){
    		temp.id=KEY;
    	}
    	if(ch=='I'){
    		temp.id=ID;
		}
		if(ch=='O'){
			temp.id=OPERA;
		}
		if(ch=='C'){
			temp.id=CONST;
		}
		if(ch=='B'){
			temp.id=BOUND;
		}
    	while(ch!=' '){
    		ch=fgetc(fp);
		}
		if(ch==' '){
			count=0;
			ch=fgetc(fp);
		}
		while(ch!=' '){
			tmp[count++]=ch;
			ch=fgetc(fp);
		}
    	word = (char *)malloc(sizeof(char)*count);
        memcpy(word, tmp, count);
        word[count] = '\0';	
		stringstream ss;
    	ss << word;
    	ss >> temp.type;
		if(ch==' '){
			count=0;
			ch=fgetc(fp);
		}
		while(ch!=' '){
			tmp[count++]=ch;
			ch=fgetc(fp);
		}
		word = (char *)malloc(sizeof(char)*count);
        memcpy(word, tmp, count);
        word[count] = '\0';
        stringstream ss2;
    	ss2 << word;
    	ss2 >> temp.line;
    	if(ch==' '){
    		count=0;
    		ch=fgetc(fp);
		}
    	while(ch!='\n'){
    		tmp[count++]=ch;
			ch=fgetc(fp);
			if(ch==-1){
				break;
			}
		}
		word = (char *)malloc(sizeof(char)*count);
        memcpy(word, tmp, count);
        word[count] = '\0';
        strcpy(temp.content,word);
		//printf("%d %d %d %s\n",temp.line,temp.id,temp.type,temp.content);
		tokens->push_back(temp);
	}
    fclose(fp);
    return tokens;
}

void show_token(Token t) {
    if(t.id==KEY) {
        printf("<%d %s %s> \n",t.line,"KEYWORD",keyword_table[t.type-1]);
        return ;
    }
    if(t.id==ID) {
        printf("<%d %s %d> \n",t.line,"ID",t.type);
        return ;
    }
    if(t.id==CONST) {
        printf("<%d %s %d>\n",t.line,"CONST",t.type);
        return ;
    }
    if(t.id==BOUND) {
        printf("<%d %s %s> \n",t.line,"BOUNDWORD",boundword_table[t.type-101]);
        return ;
    }
    if(t.id==OPERA) {
        printf("<%d %s %s> \n",t.line,"OPERATOR",operator_table[t.type-150]);
        return ;
    }
    return ;
}

void syntaxError(const char* message) {
    if(strcmp(message,"outofrange")==0) {
        printf("出现越界错误");
        printf("   Token_n:%d    tokens->size:%d\n",Token_n,tokens->size());
        exit(-1);
    }
    printf("行号：%d    %s",tokens->at(Token_n).line,message);
    printf("当前token：");
    show_token(tokens->at(Token_n));
    exit(-1);
}

Node* MatchCONST() {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id;
    char content[30];
	strcpy(content,tokens->at(Token_n).content);
	char consts[50]="CONST:";
	strcat(consts,content);
    if(id==CONST) {
        Node* t= new Node(consts);
        Token_n++;
        return t;
    }
    syntaxError("match const error\n");
    return NULL;
}

Node* MatchID() {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id;
    char content[30];
	strcpy(content,tokens->at(Token_n).content);
	char ids[50]="ID:";
	strcat(ids,content);
    if(id==ID) {
        Node* t =new Node(ids);
        Token_n++;
        return t;
    }
    syntaxError("match id error\n");
    return NULL;
}

Node* MatchKEY(const char* expected) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id;
    int type=tokens->at(Token_n).type;
    if(id==KEY&&strcmp(expected,keyword_table[type-1])==0) {
        Node*t =new Node(expected);
        Token_n++;
        return t;
    }
    char* message=new char[100];
    strcpy(message,"match keyword error.\nexpect: ");
    strcat(message,expected);
    strcat(message,"\n");
    syntaxError(message);
    delete message;
    return NULL;
}

Node* MatchOP(const int expected) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id,type=tokens->at(Token_n).type;
    if(id==OPERA&&type==expected) {
        Node* t =new Node(operator_table[type-150]);
        Token_n++;
        return t;
    }
    char* message=new char[100];
    strcpy(message,"match operator error.\nexpect: ");
    strcat(message,operator_table[expected-150]);
    strcat(message,"\n");
    syntaxError(message);
    delete message;
    return NULL;
}

Node* MatchBOUND(const int expected) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id,type=tokens->at(Token_n).type;
    if(id==BOUND&&type==expected) {
        Node* t =new Node(boundword_table[type-101]);
        Token_n++;
        return t;
    }
    char* message=new char[100];
    strcpy(message,"match boundword error.\nexpect: ");
    strcat(message,boundword_table[expected-101]);
    strcat(message,"\n");
    syntaxError(message);
    delete message;
    return NULL;
}

bool isKEY(const char* key) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id,type=tokens->at(Token_n).type;
    if(id==KEY&&strcmp(keyword_table[type-1],key)==0)return true;
    return false;
}

bool isOP(const int opera) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id,type=tokens->at(Token_n).type;
    if(id=OPERA&&type==opera)return true;
    return false;
}

bool isID() {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id;
    if(id==ID)return true;
    return false;
}

bool isCONST() {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id;
    if(id==CONST)return true;
    return false;
}

bool isBOUND(const int bound) {
    if(Token_n>=(int)tokens->size())syntaxError("outofrange");
    int id=tokens->at(Token_n).id,type=tokens->at(Token_n).type;
    if(id=BOUND&&type==bound)return true;
    return false;
}

Node* Program() {
    Node* t=new Node("OcamlTest");
    add(t->children,OcamlList());
    add(t->children,MatchBOUND(SEMSEM)); 
    return t;
}

Node* OcamlList() {
    Node* t=new Node("OcamlList");
    add(t->children,Ocaml());
    add(t->children,OcamlMore());
    return t;
}

Node* OcamlMore() {
    Node*t=NULL;
    if(isBOUND(SEM)) {
        t=new Node("OcamlMore");
        add(t->children,MatchBOUND(SEM));
        add(t->children,OcamlList());
    }
    return t;
}

Node* Ocaml() {
    Node* t=new Node("Ocaml");
    if(isKEY("begin")) {
        add(t->children,StartOcaml());
        return t;
    }
    if(isKEY("if")) {
        add(t->children,ConditionalOcaml());
        return t;
    }
    if(isKEY("while")) {
        add(t->children,LoopOcaml());
        return t;
    }
    if(isKEY("for")) {
        add(t->children,Loop2Ocaml());
        return t;
    }
    if(isKEY("match")) {
        add(t->children,MatchOcaml());
        return t;
    }
    if(isKEY("function")) {
        add(t->children,FunctionOcaml());
        return t;
    }
    if(isKEY("fun")) {
        add(t->children,Function2Ocaml());
        return t;
    }
    if(isKEY("let")) {
        add(t->children,LetOcaml());
        return t;
    }
    if(isKEY("assert")) {
        add(t->children,AssertOcaml());
        return t;
    }
    if(isKEY("lazy")) {
        add(t->children,LazyOcaml());
        return t;
    }
    if(isKEY("try")) {
        add(t->children,TryOcaml());
        return t;
    }
    if(isCONST()){
    	add(t->children,Exp());
    	return t;
	}
	if(isID()) {
        add(t->children,Exp());
        return t;
    }
    delete t;
    return NULL;
}

int IWFOcaml(Node *t){
	if(isOP(NOTEQUALLR))add(t->children,MatchOP(NOTEQUALLR));
    else if(isOP(EQUALEQUAL)) add(t->children,MatchOP(EQUALEQUAL));
    else if(isOP(EQUAL)) add(t->children,MatchOP(EQUAL));
    else if(isOP(NOTEQUAL)) add(t->children,MatchOP(EQUALEQUAL));
    else if(isOP(LESS_THAN)) add(t->children,MatchOP(LESS_THAN));
    else if(isOP(LESSEQUAL_THAN)) add(t->children,MatchOP(LESSEQUAL_THAN));
    else if(isOP(GREATER_THAN)) add(t->children,MatchOP(GREATER_THAN));
    else if(isOP(GREATEREQUAL_THAN)) add(t->children,MatchOP(GREATEREQUAL_THAN));
	else if(isOP(ANDAND)) add(t->children,MatchOP(ANDAND));
	else if(isOP(AND)) add(t->children,MatchOP(AND));
	else if(isOP(OROR)) add(t->children,MatchOP(OROR));
	else return 0;
	return 1;
}

Node* StartOcaml(){
	Node* t=new Node("StartOcaml");
	add(t->children,MatchKEY("begin"));
	add(t->children,OcamlList());
	add(t->children,MatchKEY("end"));
	return t;
}

Node* ConditionalOcaml() {
    Node* t=new Node("ConditionalOcaml");
    add(t->children,MatchKEY("if"));
    add(t->children,OcamlList());
    if(IWFOcaml(t)){}
    else syntaxError("condition error\n");
    add(t->children,OcamlList());
    add(t->children,MatchKEY("then"));
    add(t->children,OcamlList());
    if(isKEY("else")){
    	add(t->children,MatchKEY("else"));
    	add(t->children,OcamlList());	
	}
    return t;
}

Node* LoopOcaml() {
    Node* t=new Node("LoopOcaml");
    add(t->children,MatchKEY("while"));
    add(t->children,OcamlList());
   	if(IWFOcaml(t)){}
    else syntaxError("condition error\n");
    add(t->children,OcamlList());
    add(t->children,MatchKEY("do"));
    add(t->children,OcamlList());
    add(t->children,MatchKEY("done"));
    return t;
}

Node* Loop2Ocaml() {
    Node* t=new Node("Loop2Ocaml");
    add(t->children,MatchKEY("for"));
    add(t->children,MatchID());
   	if(isOP(EQUAL))add(t->children,MatchOP(EQUAL));
    else syntaxError("condition error\n");
    add(t->children,OcamlList());
    if(isKEY("to"))
    	add(t->children,MatchKEY("to"));
    else if(isKEY("downto"))
    	add(t->children,MatchKEY("downto"));
    add(t->children,OcamlList());
    add(t->children,MatchKEY("do"));
    add(t->children,OcamlList());
    add(t->children,MatchKEY("done"));
    return t;
}

Node* MatchOcaml(){
	Node* t=new Node("MatchOcaml");
	add(t->children,MatchKEY("match"));
	add(t->children,OcamlList());
	add(t->children,MatchKEY("with"));
	add(t->children,OcamlList());
	return t;
}

Node* FunctionOcaml(){
	Node* t=new Node("FunctionOcaml");
	add(t->children,MatchKEY("function"));
	add(t->children,OcamlList());
	return t;
}

Node* Function2Ocaml(){
	Node* t=new Node("Function2Ocaml");
	add(t->children,MatchKEY("fun"));
	add(t->children,OcamlList());
	return t;
}

Node* LetOcaml(){
	Node* t=new Node("LetOcaml");
	add(t->children,MatchKEY("let"));
	if(isKEY("rec")){
		add(t->children,MatchKEY("rec"));
		add(t->children,OcamlList());
		if(isKEY("and")){
			add(t->children,OcamlList());		
		}
		if(isKEY("in")){
			add(t->children,OcamlList());	
		}
	}	
	else if(isKEY("open")){
		add(t->children,MatchKEY("open"));
		add(t->children,OcamlList());
	}
	else if(isKEY("module")){
		add(t->children,MatchKEY("module"));
		add(t->children,OcamlList());
	}		
	else{
		add(t->children,OcamlList());	
		if(isKEY("and")){
			add(t->children,OcamlList());		
		}
		if(isKEY("in")){
			add(t->children,OcamlList());	
		}
	}
	return t;
}

Node* AssertOcaml(){
	Node* t=new Node("AssertOcaml");
	add(t->children,MatchKEY("assert"));
	add(t->children,OcamlList());	
	return t;
}

Node* LazyOcaml(){
	Node* t=new Node("LazyOcaml");
	add(t->children,MatchKEY("lazy"));
	add(t->children,OcamlList());	
	return t;
}

Node* TryOcaml(){
	Node* t=new Node("TryOcaml");
	add(t->children,MatchKEY("try"));
	add(t->children,OcamlList());	
	add(t->children,MatchKEY("with"));
	add(t->children,OcamlList());
	return t;
}

Node* Exp() {
    Node* t=new Node("Exp");
    add(t->children,Term());
    add(t->children,OtherTerm());
    return t;
}

Node* OtherTerm() {
    Node*t=NULL;
    if(isOP(ADD)||isOP(SUB)||isOP(ADDDOT)||isOP(SUBDOT)){
        t=new Node("OtherTerm");
        add(t->children,AddOp());
        add(t->children,Exp());
    }
    return t;
}

Node* Term() {
    Node* t=new Node("Term");
    add(t->children,Factor());
    add(t->children,OtherFactor());
    return t;
}

Node* OtherFactor() {
    Node* t=NULL;
    if(isOP(MUL)||isOP(DIV)||isOP(MULDOT)||isOP(DIVDOT)){
        t=new Node("OtherFactor");
        add(t->children,MultOp());
        add(t->children,Term());
    }
    if(isOP(EQUAL)){
        t=new Node("OtherFactor");
        add(t->children,CmpOp());
        add(t->children,OcamlList());
    } 
    return t;
}

Node* Factor(){
    Node* t=new Node("Factor");
    if(isCONST()){
        add(t->children,MatchCONST());
        return t;
    }
    if(isBOUND(LEFT_PAR)){
        add(t->children,MatchBOUND(LEFT_PAR));
        add(t->children,Exp());
        add(t->children,MatchBOUND(RIGHT_PAR));
        return t;
    }
    if(isID()) {
        add(t->children,Variable());
        return t;
    }
    delete t;
    syntaxError("factor error\n");
    return NULL;
}

Node* Variable() {
    Node* t=new Node("Variable");
    add(t->children,MatchID());
    add(t->children,VariMore());
    return t;
}

Node* VariMore() {
    Node*t=NULL;
    if(isBOUND(LEFT_CO)) {
        t=new Node("VariMore");
        add(t->children,MatchBOUND(LEFT_CO));
        add(t->children,Exp());
        add(t->children,MatchBOUND(RIGHT_CO));
    }
    if(isOP(POINT)){
        t=new Node("VariMore");
        add(t->children,MatchOP(POINT));
        add(t->children,FieldVar());
    }
    if(isID()){
    	t=new Node("VariMore");
        add(t->children,MatchID());
	}
	if(isBOUND(LEFT_PAR)) {
        t=new Node("VariMore");
        add(t->children,MatchBOUND(LEFT_PAR));
        add(t->children,Exp());
        add(t->children,MatchBOUND(RIGHT_PAR));
    }
    return t;
}

Node* FieldVar() {
    Node* t=new Node("FieldVar");
    add(t->children,MatchID());
    add(t->children,FieldVarMore());
    return t;
}

Node* FieldVarMore() {
    Node* t=NULL;
    if(isBOUND(LEFT_CO)) {
        t=new Node("FieldVarMore");
        add(t->children,MatchBOUND(LEFT_CO));
        add(t->children,Exp());
        add(t->children,MatchBOUND(RIGHT_CO));
    }
    return t;
}

Node* CmpOp() {
    Node* t=NULL;
    if(isOP(LESS_THAN )) {
        t=new Node("CmpOp");
        add(t->children,MatchOP(LESS_THAN ));
        return t;
    }
    if(isOP(EQUAL)){
        t=new Node("CmpOp");
        add(t->children,MatchOP(EQUAL));
        return t;
    }
    if(isOP(GREATER_THAN)){
    	t=new Node("CmpOp");
        add(t->children,MatchOP(GREATER_THAN));
        return t;
	}
	if(isOP(NOTEQUALLR)){
		t=new Node("CmpOp");
		add(t->children,MatchOP(NOTEQUALLR));
		return t;
	}
	if(isOP(EQUALEQUAL)){
		t=new Node("CmpOp");
		add(t->children,MatchOP(EQUALEQUAL));
		return t;
	}
	if(isOP(NOTEQUAL)){
		t=new Node("CmpOp");
		add(t->children,MatchOP(NOTEQUAL));
		return t;	
	}
    if(isOP(LESSEQUAL_THAN)){
    	t=new Node("CmpOp");
		add(t->children,MatchOP(LESSEQUAL_THAN));
		return t;	
	}
    if(isOP(GREATEREQUAL_THAN)){
    	t=new Node("CmpOp");
		add(t->children,MatchOP(GREATEREQUAL_THAN));
		return t;
	}
    syntaxError("cmpop error\n");
    return t;
}

Node* AddOp() {
    Node* t=NULL;
    if(isOP(ADD)) {
        t=new Node("AddOp");
        add(t->children,MatchOP(ADD));
        return t;
    }
    if(isOP(ADDDOT)) {
        t=new Node("AddOp");
        add(t->children,MatchOP(ADDDOT));
        return t;
    }
    if(isOP(SUB)) {
        t=new Node("AddOp");
        add(t->children,MatchOP(SUB));
        return t;
    }
    if(isOP(SUBDOT)) {
        t=new Node("AddOp");
        add(t->children,MatchOP(SUBDOT));
        return t;
    }
    syntaxError("addop error\n");
    return t;
}

Node* MultOp() {
    Node* t=NULL;
    if(isOP(MUL)) {
        t=new Node("MultOp");
        add(t->children,MatchOP(MUL));
        return t;
    }
    if(isOP(MULDOT)) {
        t=new Node("MultOp");
        add(t->children,MatchOP(MULDOT));
        return t;
    }
    if(isOP(DIV)) {
        t=new Node("MultOp");
        add(t->children,MatchOP(DIV));
        return t;
    }
    if(isOP(DIVDOT)) {
        t=new Node("MultOp");
        add(t->children,MatchOP(DIVDOT));
        return t;
    }
    syntaxError("mulop error\n");
    return t;
}

Node* Parse() {
    Node* root=Program();
    return root;
}

void show_tree(Node* root,int depth,vector<int>* v,bool islast) {
    if(root==NULL)return ;
    printf("\n");
    for(int i=0; i<depth; i++){
        if(v->at(i)==1)printf(" | ");
        else printf("  ");
    }
    if(islast){
        printf(" ┗-%s",root->desc);
        v->at(depth)=0;
    }
	else 
		printf(" |-%s",root->desc);
    if(depth+1==(int)v->size())
		v->push_back(1);
    v->at(depth+1)=1;
    int len=root->children.size();
    for(int i=0; i<len; i++){
        if(i==len-1)show_tree(root->children.at(i),depth+1,v,true);
        else show_tree(root->children.at(i),depth+1,v,false);
    }
}

int main(){
	tokens=new vector<Token>();
    getTokenlist("token.txt");
    printf("AST:\n");
    vector<int>* v=new vector<int>();
    v->push_back(0);
    show_tree(Parse(),0,v,true);
    return 0;
} 


