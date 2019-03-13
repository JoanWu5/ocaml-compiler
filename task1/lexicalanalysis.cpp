/*
@author:伍丹梅
学号：2016301500017
班级：计科1班 
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxContent 50
#define MaxLine 30

// ---------------- 关键词表 ----------------------
const char *keyword[] = {"and","as","assert","asr","begin","bool","char","class","constraint","do","done","downto","else","end","exception",
					"external","false","float","for","fun","function","functor","if","in","int","include","inherit","initializer","land","lazy",
					"let","lor","lsl","lsr","lxor","match","method","mod","module","mutable","new","object","of","open","or",
					"private","rec","sig","string","struct","then","to","true","type","unit","val","virtual","when","while","with"};

// ---------------- 算术运算符表 ----------------------
char *digitaloperator[] = {"+", "-" ,"*", "/" , "+.","-.","*.","/.","**"};

//-----------------逻辑运算符表------------------------
char *logicaloperator[]={"=","<>","==","!=","<","<=",">",">=","&&","||","&",":=","@","^","->","::","."};
    
// ---------------- 限界符表 ----------------------
char *boundword[] = {"(",")",";;","[","]",";",",",":"};

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

#define NOTE 100 //(*注释*) 
#define NOTE_DESC "NOTE"

#define MAX_CHAR_NUMBER   100000
char charSET[MAX_CHAR_NUMBER];

typedef struct NormalNode{
    char content[MaxContent];
    char description[MaxLine];
    int  type;
    int  address;
    int  line;
    struct NormalNode *next;
}NormalNode;

typedef struct IdentiferNode{
    char content[MaxContent];
    char description[MaxLine];
    int  type;
    int  address;
    int  line;
    struct IdentiferNode *next;
}IdentiferNode;

NormalNode *normalHead;
IdentiferNode *identiferHead;

void initNode(){
    normalHead = (NormalNode *)malloc(sizeof(NormalNode));
    strcpy(normalHead->content,"");
    strcpy(normalHead->description,"");
    normalHead->type = -1;
    normalHead->address = -1;
    normalHead->line = -1;
    normalHead->next = NULL;
    
    identiferHead = (IdentiferNode *)malloc(sizeof(IdentiferNode));
    strcpy(identiferHead->content,"");
    strcpy(identiferHead->description,"");
    identiferHead->type = -1;
    identiferHead->address = -1;
    identiferHead->line = -1;
    identiferHead->next = NULL;
}

void createNewNode(char *content,char *description,int type,int address,int line){
    NormalNode *p = normalHead;
    NormalNode *temp = (NormalNode *)malloc(sizeof(NormalNode));
    
    while ( p->next != NULL) {
        p = p->next;
    }
    
    strcpy(temp->content,content);
    strcpy(temp->description,description);
    temp->type = type;
    temp->address = address;
    temp->line = line;
    temp->next = NULL;
    
    p->next = temp;
}

int createIdeNode(char *content,char *description,int type,int address,int line){
    IdentiferNode *p = identiferHead;
    IdentiferNode *temp = (IdentiferNode *)malloc(sizeof(IdentiferNode));
    
    int address_count = 1;
    
    while (p->next != NULL){
        p = p -> next;
        address_count++;
        if(!strcmp(p->content, content))          //标识符已经存在
            return p->address;
    }
    strcpy(temp->content,content);
    strcpy(temp->description,description);
    temp->type = type;
    temp->address = address_count;
    temp->line = line;
    temp->next = NULL;
    
    p->next= temp;
    
    return temp->address;
}

void displaynormalNode(){
    NormalNode *p = normalHead;

    printf("\t\t内容\t\t描述\t种别码\t内存地址\t行号\n");
    
    while ( (p = p -> next)) {
        printf("%20s",p->content);
        printf("%15s",p->description);
        printf("%10d",p->type);
        if (p->address!=-1) {
            printf("%11d",p->address);
        }else{
            printf("\t\t");
        }
        printf("%10d",p->line);
        printf("\n");
    }
}

int matchKeyword(char *word){
    for (int i = 0; i <60 ; i++) {
        if(strcmp(word, keyword[i]) == 0){
            return i+1;
        }
    }
    return IDENTIFER;
}

void write_token(FILE *file,char *description,int type,int line,char *content){
	fprintf(file,"%s %d %d %s\n",description,type,line,content);	
}

int main(){  
    initNode();
    FILE *fp;
    char ch;
    int count=0;
    int line=1;
    int state=1;
    int state2=1;
    
    char temp[30];
    char *word; 
    
    if((fp = fopen("ocaml.txt", "r")) == NULL){
        printf("文件不存在");
    }
	
	FILE *fw=fopen("token.txt","w");
	
	
    while(ch!= EOF){
        ch = fgetc(fp);
          
		//处理注释(*...*)            
        if (ch == '(') {
        	int isDeclare=0;
        	while(true){		
            switch(state){
            	case 1:
					state=2;
					ch = fgetc(fp);
					break;
            	case 2:
					if(ch=='*'){
            			state=3;
						ch = fgetc(fp);
					}
					else{
						state=1;
					}
					break;
            	case 3:
            		if(ch=='*'){
            			state=4;
            			ch = fgetc(fp);
					}
					else{
						ch = fgetc(fp);
					}
					break;
				case 4:
					if(ch==')'){
						state=5;
						ch = fgetc(fp);
					}
					else if(ch=='*'){
						ch = fgetc(fp);
					}
					else{
						state=3;
						ch = fgetc(fp);
					}
					break;
				case 5:
					if(ch=='\n'||ch==EOF){
						isDeclare=1;
						ch = fgetc(fp);
					}
					else{
						isDeclare=-1;
						ch = fgetc(fp);
					}	
					break;
			}
			if(state==1){
				createNewNode("(", BOUNTWROD_DESC,LEFT_PAR, -1, line);
				write_token(fw,BOUNTWROD_DESC,LEFT_PAR,line,"(");	
				break;
			}
			if(state==5&&isDeclare==1){
				state=1;
				createNewNode("空", NOTE_DESC, NOTE, -1, line);
				break;
			}
			if(isDeclare==-1){
				break;
			}
			}
        }
        
        //处理关键字和标识符
        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'){
            count = 0;
            while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch<= '9')||(ch=='\'')){
                temp[count++] = ch;
                ch = fgetc(fp);
            }
            word = (char *)malloc(sizeof(char)*count);
            memcpy(word, temp, count);
            word[count] = '\0';
            int id = matchKeyword(word);
            if (id == IDENTIFER ) {
                int address_id = createIdeNode(word, IDENTIFER_DESC, IDENTIFER, -1, line);
                createNewNode(word, IDENTIFER_DESC, IDENTIFER, address_id, line);
                write_token(fw,IDENTIFER_DESC,IDENTIFER,line,word);
            }else{
                createNewNode(word, KEY_DESC, id, -1, line);
                write_token(fw,KEY_DESC,id,line,word);
            }
            fseek(fp,-1L,SEEK_CUR);//向后回退一位
        }
              
        //处理数字常量
        else if((ch >= '0' && ch <= '9')||ch=='-'){
        	char tempch;
        	count = 0;
        	int sub=0;
        	int number=0;
        	int subdot=0;
            int point=0;
        	if(ch=='-'){
        		sub=1;
        		temp[count++]=ch;
				ch=fgetc(fp);
				if(ch>='0'&&ch<='9'){
					number=1;
				}
			}
            int isFloat = 0;
            int isHex=0;
            int isOct=0;
            int isBin=0;
            
            if(ch=='0'){
            	number=1;
            	temp[count++]=ch;
				ch=fgetc(fp);
				if(ch=='x'||ch=='X'){
					isHex=1;
					temp[count++]=ch;
					ch=fgetc(fp);
					if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'||ch<='Z')){
						while ((ch >= '0' && ch <= '9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
                			temp[count++] = ch;
                			ch = fgetc(fp);
            			}
					}
					else{
						isHex=-1;
					}
				}
				else if(ch=='O'||ch=='o'){
					isOct=1;
					temp[count++]=ch;
					ch=fgetc(fp);
					if((ch>='0'&&ch<='7')){
						while((ch>='0'&&ch<='7')&&isOct==1){
							temp[count++]=ch;
							ch=fgetc(fp);
						}
					}
					else{
						isOct=-1;
					}
				}
				else if(ch=='B'||ch=='b'){
					isBin=1;
					temp[count++]=ch;
					ch=fgetc(fp);
					if(ch=='0'||ch=='1'){
						while((ch=='0'||ch=='1')&&isBin==1){
							temp[count++]=ch;
							ch=fgetc(fp);
						}
					}
					else{
						isBin=-1;
					}
				}
			}			
           	
			while((ch>='0'&&ch<='9')&&isHex!=1&&isOct!=1&&isBin!=1){
				number=1;
				temp[count++] = ch;
                ch = fgetc(fp);
			}
						
            if (ch == '.') {
                isFloat=1;
                temp[count++] = ch;
                tempch=ch;
                ch = fgetc(fp);
                if (ch >= '0' && ch <= '9') {
                    while (ch >= '0' && ch <= '9' ){
                        temp[count++] = ch;
                        ch = fgetc(fp);
                    }
                }
				else{
                    //isFloat = -1;      //出错，小数点后没有数字.
                    if(tempch=='.'){
        				subdot=1;
        				sub=0;
        				createNewNode("-.", OPERATOR_DESC, SUBDOT, -1, line);
						write_token(fw,OPERATOR_DESC,SUBDOT,line,"-.");	
					}
                }
            }
            
            if(ch == 'E' || ch == 'e'){
            	isFloat=1;
                temp[count++] = ch;
                ch = fgetc(fp);
                if (ch == '+' || ch == '-') {
                    temp[count++] = ch;
                    ch = fgetc(fp);
                }
                if (ch >= '0' && ch <= '9') {
                    while (ch >= '0' && ch <= '9' ){
                        temp[count++] = ch;
                        ch = fgetc(fp);
                    }
                }
				else{
                    isFloat = -1;    //出错，-1表示 E后面的格式不对.
                }
            }

			if(ch=='>'){
				sub=0;
				point=1;
				createNewNode("->", OPERATOR_DESC, POINT, -1, line);
				write_token(fw,OPERATOR_DESC,POINT,line,"->");		
        		ch=fgetc(fp);
			}
			else if(subdot!=1&&point!=1&&isFloat!=1&&isHex!=1&&isBin!=1&&sub==1&&number==0){
				createNewNode("-", OPERATOR_DESC, SUB, -1, line);
				write_token(fw,OPERATOR_DESC,SUB,line,"-");	
				ch=fgetc(fp);	
			}
			 
            word = (char *)malloc(sizeof(char)*(count+1));
            memcpy(word, temp, count);
            word[count] = '\0';
            if (isFloat==1) {
                createNewNode(word, CONSTANT_DESC, FLOAT_VAL, -1, line);
                write_token(fw,CONSTANT_DESC,FLOAT_VAL,line,word);	
            }
            else if(isHex==1){
            	createNewNode(word, CONSTANT_DESC, HEX_VAL, -1, line);
            	write_token(fw,CONSTANT_DESC,HEX_VAL,line,word);	
			} 
			else if(isOct==1){
				createNewNode(word, CONSTANT_DESC, OCT_VAL, -1, line);
				write_token(fw,CONSTANT_DESC,OCT_VAL,line,word);	
			}
			else if(isBin==1){
				createNewNode(word, CONSTANT_DESC,BIN_VAL, -1, line);
				write_token(fw,CONSTANT_DESC,BIN_VAL,line,word);	
			}
			else if(isFloat!=-1&&isHex!=-1&&isOct!=-1&&isBin!=-1&&point!=1&&subdot!=1&&number==1){
                createNewNode(word, CONSTANT_DESC, DEC_VAL, -1, line);
                write_token(fw,CONSTANT_DESC,DEC_VAL,line,word);	
            }
            
            
			fseek(fp,-1L,SEEK_CUR);
        }
    
        //处理字符型常量
		else if(ch=='\''){
			count=0;
			state=1;
			int isChar=0;
			temp[count++]=ch;
			ch=fgetc(fp);
			if(ch=='\"'||ch == '\t' || ch == '\r' || ch =='\n' ||ch=='\b'||(ch>='0'||ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
				temp[count++]=ch;
				ch=fgetc(fp);
			}
			else if(ch=='\\'){
				temp[count++]=ch;
				ch=fgetc(fp);
				if(ch>='0'||ch<='9'){
					temp[count++]=ch;
					ch=fgetc(fp);
					if(ch>='0'||ch<='9'){
						temp[count++]=ch;
						ch=fgetc(fp);
						if(ch>='0'||ch<='9'){
							temp[count++]=ch;
							ch=fgetc(fp);
						}
					}
				}
				else if(ch=='x'||ch=='X'){
					temp[count++]=ch;
					ch=fgetc(fp);
					if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
						temp[count++]=ch;
						ch=fgetc(fp);
						if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
							temp[count++]=ch;
							ch=fgetc(fp);
						}
					}
				}	
			}
			if(ch=='\''){
				isChar=1;
				temp[count++]=ch;
				ch=fgetc(fp);
			}
			else{
				isChar=-1;
			}
			word = (char *)malloc(sizeof(char)*(count+1));
            memcpy(word, temp, count);
            word[count] = '\0';
            if (isChar==1) {
                createNewNode(word, CONSTANT_DESC, CHAR_VAL, -1, line);
                write_token(fw,CONSTANT_DESC,CHAR_VAL,line,word);	
            }
            fseek(fp,-1L,SEEK_CUR);
		} 
		
		//处理字符串 		
		else if(ch=='\"'){
			count=0;
			state2=2;
			int isString=0;
			temp[count++]=ch;
			ch=fgetc(fp);
			while(state2==2){
			do{
				if(ch=='\''||ch == '\t' || ch == '\r' || ch =='\n' ||ch=='\b'||(ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
					temp[count++]=ch;
					ch=fgetc(fp);
				}
				else if(ch=='\\'){
					temp[count++]=ch;
					ch=fgetc(fp);
					if(ch>='0'&&ch<='9'){
						temp[count++]=ch;
						ch=fgetc(fp);
						if(ch>='0'||ch<='9'){
							temp[count++]=ch;
							ch=fgetc(fp);
							if(ch>='0'||ch<='9'){
								temp[count++]=ch;
								ch=fgetc(fp);
							}
							else{
								isString=-1;
							}
						}
						else{
								isString=-1;
						}
					}
					else if(ch=='x'||ch=='X'){
						temp[count++]=ch;
						ch=fgetc(fp);
						if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
							temp[count++]=ch;
							ch=fgetc(fp);
							if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){
								temp[count++]=ch;
								ch=fgetc(fp);
							}
							else{
								isString=-1;
							}
						}
						else{
							isString=-1;
						}
					}
					else if(ch=='n'||ch=='t'||ch=='r'||ch=='b'){
						temp[count++]=ch;
						ch=fgetc(fp);
					}	
				}
				else{
					temp[count++]=ch;
					ch=fgetc(fp);
				}
			}while(ch!='\"');
			if(ch=='\"'&&isString!=-1){
				state2=3;
				isString=1;
				while(ch=='\"'){
					temp[count++]=ch;
					ch=fgetc(fp);
				}
				if(ch=='\''||(ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='\\')){
					if(ch=='\\'){
						temp[count++]=ch;
						ch=fgetc(fp);
						if(ch=='t'||ch=='b'||ch=='n'||ch=='r'){
							state=2;
						}
					} 
					else if(ch=='\''||(ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
						state2=2;
					else
						state=3;
				}
				else{
					state2=3;
				}
				if(ch==EOF){
					state2=3;
				}
				
			}	
			else{
				isString=-1;
			}
			}
			
			word = (char *)malloc(sizeof(char)*(count+1));
            memcpy(word, temp, count);
            word[count] = '\0';
            if (isString==1&&state2==3) {
            	state2=1;
                createNewNode(word, CONSTANT_DESC, STRING_VAL, -1, line);
                write_token(fw,CONSTANT_DESC,STRING_VAL,line,word);	
            }
            fseek(fp,-1L,SEEK_CUR);
        }
        
        //处理换行
        else if(ch == ' ' || ch == '\t' || ch == '\r' || ch =='\n' ||ch=='\b'){
            if(ch == '\n'){
                line++;
            }
        }

        else if(ch == '*'){
        	count=0;
        	int mulmul=0;
        	int muldot=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='.'){
        		muldot=1;
        		createNewNode("*.", OPERATOR_DESC, MULDOT, -1, line);	
        		write_token(fw,OPERATOR_DESC,MULDOT,line,"*.");	
        		ch=fgetc(fp);
			}
			else if(ch=='*'){
				mulmul=1;
				createNewNode("**", OPERATOR_DESC, MULMUL, -1, line);
				write_token(fw,OPERATOR_DESC,MULMUL,line,"**");		
				ch=fgetc(fp);
			}
			else if(mulmul!=1&&muldot!=1){
				createNewNode("*", OPERATOR_DESC, MUL, -1, line);
				write_token(fw,OPERATOR_DESC,MUL,line,"*");		
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '+'){
        	count=0;
        	int adddot=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='.'){
        		adddot=1;
        		createNewNode("+.", OPERATOR_DESC, ADDDOT, -1, line);
				write_token(fw,OPERATOR_DESC,ADDDOT,line,"+.");		
        		ch=fgetc(fp);
			}
			else if(adddot!=1){
				createNewNode("+", OPERATOR_DESC, ADD, -1, line);	
				write_token(fw,OPERATOR_DESC,ADD,line,"+");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
       
        else if(ch == '/'){
        	count=0;
        	int divdot=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='.'){
        		divdot=1;
        		createNewNode("/.", OPERATOR_DESC, DIVDOT, -1, line);
				write_token(fw,OPERATOR_DESC,DIVDOT,line,"/.");		
        		ch=fgetc(fp);
			}
			else if(divdot!=1){
				createNewNode("/", OPERATOR_DESC,DIV, -1, line);
				write_token(fw,OPERATOR_DESC,DIV,line,"/");		
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '='){
        	count=0;
        	int equalequal=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='='){
        		equalequal=1;
        		createNewNode("==", OPERATOR_DESC, EQUALEQUAL, -1, line);	
        		write_token(fw,OPERATOR_DESC,EQUALEQUAL,line,"==");	
        		ch=fgetc(fp);
			}
			else if(equalequal!=1){
				createNewNode("=", OPERATOR_DESC,EQUAL, -1, line);	
				write_token(fw,OPERATOR_DESC,EQUAL,line,"=");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '<'){
        	count=0;
        	int lr=0;
        	int le=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='>'){
        		lr=1;
        		createNewNode("<>", OPERATOR_DESC, NOTEQUALLR, -1, line);	
        		write_token(fw,OPERATOR_DESC,NOTEQUALLR,line,"<>");	
        		ch=fgetc(fp);
			}
			else if(ch=='='){
				le=1;
				createNewNode("<=", OPERATOR_DESC, LESSEQUAL_THAN, -1, line);	
				write_token(fw,OPERATOR_DESC,LESSEQUAL_THAN,line,"<=");	
				ch=fgetc(fp);
			}
			else if(lr!=1&&le!=1){
				createNewNode("<", OPERATOR_DESC, LESS_THAN, -1, line);	
				write_token(fw,OPERATOR_DESC,LESS_THAN,line,"<");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '>'){
        	count=0;
        	int re=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='='){
        		re=1;
        		createNewNode(">=", OPERATOR_DESC, GREATEREQUAL_THAN, -1, line);
				write_token(fw,OPERATOR_DESC,GREATEREQUAL_THAN,line,">=");		
        		ch=fgetc(fp);
			}
			else if(re!=1){
				createNewNode(">", OPERATOR_DESC,GREATER_THAN, -1, line);
				write_token(fw,OPERATOR_DESC,GREATER_THAN,line,">");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '!'){
        	count=0;
        	int equalequal=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='='){
        		equalequal=1;
        		createNewNode("!=", OPERATOR_DESC, NOTEQUAL, -1, line);	
        		write_token(fw,OPERATOR_DESC,NOTEQUAL,line,"!=");	
        		ch=fgetc(fp);
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '&'){
        	count=0;
        	int andand=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='='){
        		andand=1;
        		createNewNode("&&", OPERATOR_DESC,ANDAND, -1, line);
				write_token(fw,OPERATOR_DESC,ANDAND,line,"&&");		
        		ch=fgetc(fp);
			}
			else if(andand!=1){
				createNewNode("&", OPERATOR_DESC,AND, -1, line);
				write_token(fw,OPERATOR_DESC,AND,line,"&");		
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '|'){
        	count=0;
        	int oror=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='|'){
        		oror=1;
        		createNewNode("||", OPERATOR_DESC,OROR, -1, line);	
        		write_token(fw,OPERATOR_DESC,OROR,line,"||");	
        		ch=fgetc(fp);
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == ':'){
        	count=0;
        	int coequal=0;
        	int colcol=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch=='='){
        		coequal=1;
        		createNewNode(":=", OPERATOR_DESC,EQUAL2, -1, line);
				write_token(fw,OPERATOR_DESC,EQUAL2,line,":=");		
        		ch=fgetc(fp);
			}
			else if(ch==':'){
        		colcol=1;
        		createNewNode("::", OPERATOR_DESC,COLCOL, -1, line);
				write_token(fw,OPERATOR_DESC,COLCOL,line,"::");		
        		ch=fgetc(fp);
			}
			else if(coequal!=1&&colcol!=1){
				createNewNode(":", BOUNTWROD_DESC,COL, -1, line);	
				write_token(fw,BOUNTWROD_DESC,COL,line,":");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '@'){
            createNewNode("@", OPERATOR_DESC, AT, -1, line);
            write_token(fw,OPERATOR_DESC,AT,line,"@");	
        }
        
        else if(ch == '^'){
            createNewNode("^", OPERATOR_DESC, CONNECT, -1, line);
            write_token(fw,OPERATOR_DESC,CONNECT,line,"^");	
        }
        
        else if(ch == ')'){
            createNewNode(")", BOUNTWROD_DESC, RIGHT_PAR, -1, line);
            write_token(fw,BOUNTWROD_DESC,RIGHT_PAR,line,")");	
        }
        
        else if(ch == ';'){
        	count=0;
        	int semsem=0;
        	temp[count++]=ch;
        	ch=fgetc(fp);
        	if(ch==';'){
        		 semsem=1;
        		createNewNode(";;", BOUNTWROD_DESC,SEMSEM, -1, line);	
        		write_token(fw,BOUNTWROD_DESC,SEMSEM,line,";;");	
        		ch=fgetc(fp);
			}
			else if(semsem!=1){
				createNewNode(";", BOUNTWROD_DESC,SEM, -1, line);	
				write_token(fw,BOUNTWROD_DESC,SEM,line,";");	
			}
			fseek(fp,-1L,SEEK_CUR);
        }
        
        else if(ch == '['){
            createNewNode("[", BOUNTWROD_DESC, LEFT_CO, -1, line);
            write_token(fw,BOUNTWROD_DESC,LEFT_CO,line,"[");	
        }
        
        else if(ch == ']'){
            createNewNode("]", BOUNTWROD_DESC, RIGHT_CO, -1, line);
            write_token(fw,BOUNTWROD_DESC,RIGHT_CO,line,"]");	
        }

        else if(ch == ','){
            createNewNode(",", BOUNTWROD_DESC, COM, -1, line);
            write_token(fw,BOUNTWROD_DESC,COM,line,",");	
        }
		
		else if(ch=='.'){
			createNewNode(".", OPERATOR_DESC, DOT, -1, line);
			write_token(fw,OPERATOR_DESC,DOT,line,".");	
		}    
        if (ch != ' '&& ch != '\n') {   //去掉空格和换行符
            charSET[count] = ch;
            count++;
        }
    }
    fclose(fp);  
	fclose(fw);  
    displaynormalNode();   
}
