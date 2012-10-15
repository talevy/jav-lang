%{
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "parsetree/PTNodes.hh"
#include "symtab/SymTable.h"
#include "symtab/ScopeStack.h"
#include "codegen/codegen.hh"

using namespace std;
    int yylex(void);
    void yyerror(const char *);
    int yyparse();
  extern int yylineno;
  extern char *yytext;
  
  // global symbol table
  SymTable symtab(GLOBAL);

  // scope stack to deal with getting correct symtab
  ScopeStack ss;
  // root of parse-tree
  PTNode * root;
%}

%union {
  int int_val;
  char * str_val;
  char char_val;
  Program * prog_val;
  ClassDeclarations * cds_val;
  ClassDeclaration * cd_val;
  ClassBody * cb_val;
  ClassBodyDeclarations * cbds_val;
  ClassMemberDeclaration * cmd_val;
  FieldDeclaration * fd_val;
  MethodDeclaration * md_val;
  MethodHeader * mh_val;
  MethodBody * mb_val;
  Type * typ_val;
  PrimitiveType * ptyp_val;
  ReferenceType * rtyp_val;
  SimpleName * sn_val;
  Name * n_val;
  QualifiedName * qn_val;
  MethodDeclarator * mda_val;
  FormalParameterList * fpl_val;
  FormalParameter * fp_val;
  VariableDeclarators * vds_val;
  VariableDeclarator * vd_val;
  LocalVariableDeclarationStatements * lvds_val;
  LocalVariableDeclarationStatement *lvd_val;
  Statements * stmts_val;
  Statement * stmt_val;
  WhileStatement * whilestmnt_val;
  IfThenStatement * ifstmnt_val;
  IfThenElseStatement * elifstmnt_val;
  SimpleBlock * sb_val;
  ContinueStatement * cntstmnt_val;
  ReturnStatement * retstmnt_val;
  EmptyStatement * estmnt_val;
  IOStatement * iostmnt_val;
  StatementExpression *stmntexpr_val;
  Assignment *ass_val;
  LeftHandSide * lhs_val;
  MethodInvocation *methi_val;
  ClassInstanceCreationExpression *clsinstexpr_val;
  ArgumentList *argl_val;
  Expression *expr_val;
  PrimitiveExpression *primexpr_val;
  Primary *primary_val;
  Literal *literal_val;
  FieldAccess *faccess_val;
  
}

%token CLASS CONTINUE EXTENDS IF NEW RETURN WHILE CHAR TRUE FALSE INT NIL STRING VOID INPUT OUTPUT THIS SUPER BOOLEAN


%token <str_val> ID
%token <int_val> INT_CONST
%token <str_val> STR_CONST
%token <char_val> CHAR_CONST

%left OR
%left AND
%left EQ NEQ
%left '<' '>' GEQ LEQ
%left '+' '-'
%left '*' '/'
%right UMINUS
%right UPLUS
%right '!'
%right '='

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <prog_val> Program
%type <cds_val> ClassDeclarations
%type <cd_val> ClassDeclaration
%type <cb_val> ClassBody
%type <cbds_val> ClassBodyDeclarations
%type <cmd_val> ClassMemberDeclaration
%type <fd_val> FieldDeclaration
%type <md_val> MethodDeclaration
%type <mda_val> MethodDeclarator
%type <mh_val> MethodHeader
%type <mb_val> MethodBody
%type <lvd_val> LocalVariableDeclarationStatement
%type <lvds_val> LocalVariableDeclarationStatements
%type <stmts_val> Statements
%type <stmt_val> Statement
%type <retstmnt_val> ReturnStatement
%type <cntstmnt_val> ContinueStatement
%type <estmnt_val> EmptyStatement
%type <sb_val> SimpleBlock
%type <ifstmnt_val> IfThenStatement
%type <elifstmnt_val> IfThenElseStatement
%type <iostmnt_val> IOStatement
%type <whilestmnt_val> WhileStatement
%type <stmntexpr_val> StatementExpression
%type <typ_val> Type
%type <ptyp_val> PrimitiveType
%type <rtyp_val> ReferenceType
%type <sn_val> SimpleName
%type <n_val> Name
%type <qn_val> QualifiedName
%type <fpl_val> FormalParameterList
%type <fp_val> FormalParameter
%type <vd_val> VariableDeclarator
%type <vds_val> VariableDeclarators
%type <ass_val> Assignment
%type <lhs_val> LeftHandSide
%type <methi_val> MethodInvocation
%type <clsinstexpr_val> ClassInstanceCreationExpression
%type <argl_val> ArgumentList
%type <expr_val> Expression
%type <primexpr_val> PrimitiveExpression
%type <primary_val> Primary
%type <literal_val> Literal
%type <faccess_val> FieldAccess


%start Program
%%


Program:
		ClassDeclarations {$$ = new Program($1); root = $$; }
		|                 {$$ = NULL;   root = $$; }
		;
		
ClassDeclarations:
		ClassDeclaration {$$ = new ClassDeclarations($1);}
		| ClassDeclarations ClassDeclaration 
		  {$$ = new ClassDeclarations($1, $2); }
		;

ClassDeclaration:
                CLASS ID
		{         
		          const char * type = "class";
		  	  Entity *cls = new Entity($2, type);
			  ss.cur()->insert(cls);
			  ss.push(cls->getSymTable());
			}
			 EXTENDS ReferenceType ClassBody
			 {
			   // leave class scope, enter global scope
			   ss.pop();
			   LeafNode *id = new LeafNode($2);
			   $$ = new ClassDeclaration(id, $5, $6);
			 }
		| CLASS ID
		{         const char * type = "class";
		          Entity * cls = new Entity($2, type);
			  ss.cur()->insert(cls);
			  ss.push(cls->getSymTable());
			}
			ClassBody 
			{ 
			  // leave class scope, enter global scope
			  ss.pop();
			  IDNode *id = new IDNode($2);
			  $$ = new ClassDeclaration(id, $4); 
		        }
		;

ClassBody:
		'{' ClassBodyDeclarations '}'
		  {$$ = new ClassBody($2); }
		| '{' '}' {$$ = new ClassBody(); }
		;

ClassBodyDeclarations:
		ClassMemberDeclaration 
		    {$$ = new ClassBodyDeclarations($1); }
		| ClassBodyDeclarations ClassMemberDeclaration
		    {$$ = new ClassBodyDeclarations($1, $2); }
		;

ClassMemberDeclaration:
                FieldDeclaration 
		{$$ = $1;}
		| MethodDeclaration 
		{$$ = $1;}
		;

Type:
		PrimitiveType   {$$ = $1;}
		| ReferenceType {$$ = $1;}
		;

PrimitiveType:
		INT
		{$$ = new PrimitiveType(new LeafNode("int")); $$->setType("int"); }
		| BOOLEAN 
		{$$ = new PrimitiveType(new LeafNode("bool")); $$->setType("bool"); }
		| CHAR    
		{$$ = new PrimitiveType(new LeafNode("char")); $$->setType("char"); }
		| STRING  
		{$$ = new PrimitiveType(new LeafNode("str")); $$->setType("str"); }
		;

ReferenceType:
                SimpleName 
		    {
		      $$ = new ReferenceType($1); $$->setType($1->getType());
		    }
		;

Name:
		SimpleName 
		    {
		      $$ = $1;
		    }
		| QualifiedName
		    {
		      $$ = $1; 
		    }
		;

SimpleName:
		ID
		    {
		      LeafNode * n = new LeafNode($1);
		      $$ = new SimpleName(n);
		      /* TODO: check if ID is already in symbol table and set type */
		      // method scope
		      SymTable *params = ss.cur()->find("params")->getSymTable();
		      SymTable *body_symbols = ss.cur()->find("locals")->getSymTable();
		      // check parameters for variable
		      Entity *id = params->find($1);
		      
		      if(id!=NULL) {
			$$->setType(id->getType());
		      } else {
			id = body_symbols->find($1);
			if (id!=NULL){
			  $$->setType(id->getType());
			}
			else {
			  /* try class scope */
			  SymTable *tmp = ss.cur(); // methscope
			  ss.pop();
			  SymTable *cls_scope = ss.cur();
			  id = cls_scope->find($1);
			  if (id!=NULL){
			    $$->setType(id->getType());
			  } else {
			    // look for var in global scope
			    SymTable *tmpcls = ss.cur();
			    ss.pop();
			    id = ss.cur()->find($1);
			    if (id!=NULL) {
			      $$->setType(id->getName());
			    } else {
			      $$->setType("ERROR");
			    }
			    ss.push(tmpcls);
			  }
			  // push back to method scope
			  ss.push(tmp);
			}
		      }
		    }
		;

QualifiedName:
		Name '.' ID
		    {
		      IDNode *id = new IDNode($3); 
		      $$ = new QualifiedName($1, id);
		      // TODO
		      
		    }
		;

FieldDeclaration:
		Type VariableDeclarators ';'
		{  /* loop through symbols and update with type */
		  vector<Entity *>::size_type sz = ss.cur()->table.size();
		  for(unsigned int i = 0; i < sz; i++) {
		    Entity * e = ss.cur()->table[i];
		    if (strcmp(e->getType(), "EMPTY")==0){
		       e->setType($1->getType());
		    }
		  }
		   $$ = new FieldDeclaration($1, $2);
		   $$->setType($1->getType());
		   /* TODO propogate type down to variable declarators */
		   propagateTypeDown($$, $$->_type);
		}
		;

VariableDeclarators:
		 VariableDeclarator
		    {$$ = new VariableDeclarators($1); }
		| VariableDeclarators ',' VariableDeclarator
		    {$$ = new VariableDeclarators($1, $3); }
		;

VariableDeclarator:
                ID {
		    ss.cur()->insert(new Entity($1, "EMPTY"));
		    IDNode *id = new IDNode($1);
		    $$ = new VariableDeclarator(id);
		   }
		;

MethodDeclaration:
		MethodHeader 
			   {
			     // in method scope
			   } 
		MethodBody 
			   {
			    	$$ = new MethodDeclaration($1, $3);
				$$->setType($1->getType());
				// set code
				if(ss.cur()->type==METHOD){
				  ss.cur()->find("code")->setParseTree($$);
				} else {
				  // for testing purpose, seems to work now
				  printf("MESSED UP: %d\n", ss.cur()->type);
				}
				// pop out of method scope, enter class scope
				ss.pop();
			   }
		;

MethodHeader:
		Type MethodDeclarator
		{$$ = new MethodHeader($1, $2); 
		  $$->setType($1->getType());
		  $2->setType($1->getType());
		  $2->_entity->setType($1->getType());
		  $2->_entity->isMethod = true;
		}
		| VOID MethodDeclarator
		{$$ = new MethodHeader(new LeafNode("void"), $2);
		  $$->setType("void");
		  $2->setType("void");
		}
		;

MethodDeclarator:
                ID '(' 
		    {
		      // get Class Name to push into method param
		      // kind of a sketch move, but curr class is last in
		      // global table (last to be inserted)
		      SymTable * clstab = ss.cur();
		      ss.pop();
		      // get class entity from global scope
		      Entity *cls = ss.cur()->table[ss.cur()->table.size()-1];

		      // return back to normal class scope after getting class type
		      ss.push(clstab);
		      
		      Entity * meth = new Entity($1, "meth");
		      ss.cur()->insert(meth);
		      
		      // push into method scope
		      ss.push(meth->getSymTable());

		      SymTable *paramtab = meth->getSymTable()->find("params")->getSymTable();

		      // push into parameter scope for formal parameters
		      ss.push(paramtab);
		      // insert 'this' into the table;
		      paramtab->insert(new Entity("this", cls->getName()));

		      // set codename label for method
		      char method_label[1000];
		      sprintf(method_label, "%s_%s_m", cls->getName(), $1);
		      meth->setCodeName(strdup(method_label));
		    }
                FormalParameterList ')'
		   {
		     LeafNode *id = new LeafNode($1);
		     $$ = new MethodDeclarator(id, $4);
		     // leave parameter scope, enter method scope
		     ss.pop();
		     $$->_entity = ss.find($1);
		   }
		|  ID '(' ')'
		   { 
		     $$ = new MethodDeclarator(new LeafNode($1));
		     $$->_entity = ss.find($1);
		      // get Class Name to push into method param
		      // kind of a sketch move, but curr class is last in
		      // global table (last to be inserted)
		      SymTable * clstab = ss.cur();
		      ss.pop();
		      // get class entity from global scope
		      Entity *cls = ss.cur()->table[ss.cur()->table.size()-1];

		      // return back to normal class scope after getting class type
		      ss.push(clstab);
		      
		      Entity * meth = new Entity($1, "meth");
		      ss.cur()->insert(meth);
		      
		      // push into method scope
		      ss.push(meth->getSymTable());
		      SymTable *paramtab = meth->getSymTable()->find("params")->getSymTable();
		      // push into parameter scope for formal parameters
		      ss.push(paramtab);
		      // insert 'this' into the table;
		      paramtab->insert(new Entity("this", cls->getName()));
		      ss.pop();
		      // back to method scope
		      // set codename label for method
		      char method_label[1000];
		      sprintf(method_label, "%s_%s_m", cls->getName(), $1);
		      meth->setCodeName(strdup(method_label));
		   }
		;

FormalParameterList:
		FormalParameter 
		   {$$ = new FormalParameterList($1);}
		| FormalParameterList ',' FormalParameter
		   {$$ = new FormalParameterList($1, $3); }
		;

FormalParameter:
		Type ID 
		{ 
		  IDNode *id = new IDNode($2);
		  $$ = new FormalParameter($1, id);
		  ss.cur()->insert(new Entity($2, $1->getType()));
		  $$->setType($1->getType());
		}
		;

MethodBody:
		'{' LocalVariableDeclarationStatements Statements '}'
		  { $$ = new MethodBody($2, $3); }
		| '{'  LocalVariableDeclarationStatements '}'
		{ $$ = new MethodBody($2, NULL); }
		| '{' Statements '}'
		{ $$ = new MethodBody(NULL, $2); }
		| '{' '}'
		  { $$ = new MethodBody(); }
		| ';'
		  { $$ = new MethodBody(); }
		;

LocalVariableDeclarationStatement:
		Type 
		     {
		       // now in method body table, push local symbols into scope
		       if (ss.cur()->type==METHOD) {
			 ss.push(ss.cur()->find("locals")->getSymTable());
		       }
		     } 
		VariableDeclarators ';'
		{  /* loop through symbols and update with type */
		  vector<Entity*>::size_type sz = ss.cur()->table.size();
		  vector<Entity *>  table = ss.cur()->table;
		  for(unsigned int i=0;i < sz; i++) {
		    Entity * e = table[i];
		    if(strcmp(e->getType(), "EMPTY")==0){
		     	e->setType($1->getType());
		    }
		  }
		  $$ = new LocalVariableDeclarationStatement($1,$3);
		  
		  propagateTypeDown($$, $1->getType());
		  // pop out of locals scope
		  ss.pop();
		}
		;

LocalVariableDeclarationStatements:
		LocalVariableDeclarationStatement
		    {
		      $$ = new LocalVariableDeclarationStatements($1);
		    }
		 | LocalVariableDeclarationStatements LocalVariableDeclarationStatement
		     {
		       $$ = new LocalVariableDeclarationStatements($1, $2);
		     }
		;

Statement:
		IfThenStatement {$$ = $1; }
		| IfThenElseStatement {$$ = ($1); }
		| WhileStatement {$$ = ($1); }
		| SimpleBlock {$$ = ($1); }
		| EmptyStatement  {$$ = ($1); }
		| StatementExpression ';'{$$ = ($1); }
		| ContinueStatement {$$ = ($1); }
		| ReturnStatement {$$ = ($1); }
		| IOStatement {$$ = ($1); }
		;

Statements:
		Statement {$$ = new Statements($1); }
		| Statements Statement {$$ = new Statements($1,$2);}
		;

SimpleBlock:
		'{' Statements '}' {$$ = new SimpleBlock($2); }
		| '{' '}' {$$ = new SimpleBlock(); }
		;

EmptyStatement:
		 ';' {$$ = new EmptyStatement(); }
		;


StatementExpression:
		Assignment 
		    {
		      $$ = new StatementExpression($1);
		      $$->setType($1->_type);
		    }
		| MethodInvocation 
		    {
		      $$ = new StatementExpression($1);
		      $$->setType($1->_type);
		    }
		| ClassInstanceCreationExpression 
		    {
		      $$ = new StatementExpression($1); 
		      $$->setType($1->_type);
		    }
		;

IfThenStatement:
		IF '(' Expression ')' Statement %prec LOWER_THAN_ELSE
		{$$ = new IfThenStatement($3, $5); }
		;

IfThenElseStatement:
		IF '(' Expression ')' Statement ELSE Statement
		  {$$ = new IfThenElseStatement($3, $5, $7); }
		;

WhileStatement:
		WHILE '(' Expression ')' Statement
		    {$$ = new WhileStatement($3, $5); }
		;

ContinueStatement:
		CONTINUE ';' {$$ = new ContinueStatement(); }
		;

ReturnStatement:
		RETURN Expression ';'
		   {
		   $$ = new ReturnStatement($2);
		   /* check if return type is same as method*/
		   }
		| RETURN ';'
		   {
		   $$ = new ReturnStatement();
		   /* check if method is void type */
		   }
		;

IOStatement:
		INPUT Expression ';' 
		{$$ = new IOStatement( "input", $2);}
		| OUTPUT Expression ';'
		{$$ = new IOStatement( "output", $2);}
		;

Primary:
		Literal 
		    {
		      $$ = new Primary($1);
		      $$->setType($1->_type);
		    }
                | THIS
		    {
		      $$ = new Primary(new LeafNode( "this"));
		      /*TODO: find type of this reference */
		    }
		| '(' Expression ')' 
		    {
		      $$ = new Primary($2);
		      $$->setType($2->_type);
		    }
		| ClassInstanceCreationExpression 
		    {
		      $$ = new Primary($1);
		      $$->setType($1->_type);
		    }
		| FieldAccess 
		    {
		      $$ = new Primary($1);
		      $$->setType($1->_type);
		    }
		| MethodInvocation 
		    {
		      $$ = new Primary($1);
		      $$->setType($1->_type);
		    }
		;

ClassInstanceCreationExpression:
		NEW ReferenceType '(' ArgumentList ')'
		  {
		    $$ = new ClassInstanceCreationExpression($2,$4);
		    $$->setType($2->_type);
		  }
		| NEW ReferenceType '(' ')'
		 {
		   $$ = new ClassInstanceCreationExpression($2);
		   $$->setType($2->_type);
		 }
		;

ArgumentList:
		Expression 
		     {
		       $$ = new ArgumentList($1);
		       $$->setType($1->_type);
		     }
		| ArgumentList ',' Expression
		     {
		       $$ = new ArgumentList($1, $3);
		       $$->setType("nil");
		     }
		;

FieldAccess:
		Primary '.' ID 
		     {
		       IDNode *id = new IDNode($3);
		       $$ = new FieldAccess($1, id);
		     }
		| SUPER '.' ID 
		     {
		       IDNode *id = new IDNode($3);
                       LeafNode *s = new LeafNode( "super");
		       $$ = new FieldAccess(s, id);
		     }
		;

MethodInvocation:
		Name '(' ArgumentList ')'
		{$$ = new MethodInvocation($1, $3); $$->setType($1->getType());}
		| Name '(' ')'
		          {$$ = new MethodInvocation($1);$$->setType($1->getType());}
		| Primary '.' ID '(' ArgumentList ')'
		          {$$ = new MethodInvocation($1, new IDNode($3), $5); }
		| Primary '.' ID '(' ')'
		          {$$ = new MethodInvocation($1, new IDNode($3), NULL); }
		| SUPER '.' ID '(' ArgumentList ')'
		          {$$ = new MethodInvocation(new LeafNode("super"), new IDNode($3), $5); }
		| SUPER '.' ID '(' ')'
		          {$$ = new MethodInvocation(new LeafNode("super"), new IDNode($3), NULL); }
		;

PrimitiveExpression:
                Primary 
		     {
		       $$ = new PrimitiveExpression($1);
		       $$->setType($1->_type);
		     }
		| Name 
		     {
		       $$ = new PrimitiveExpression($1);
		       $$->setType($1->_type);
		     }
		;

Expression:
		Expression '*' Expression
			   {
			     $$ = new Expression($1,"*",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType($1->_type);
			     }
			   }
		| Expression '/' Expression
		  	   {
			     $$ = new Expression($1,"/",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType($1->_type);
			     }
			   }
		| Expression '+' Expression
		  	   {
			     $$ = new Expression($1,"+",$3); 
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType($1->_type);
			     }
			   }
		| Expression '-' Expression
                           {
			     $$ = new Expression($1,"-",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType($1->_type);
			     }
			   }
		| Expression AND Expression
                           {
			     $$ = new Expression($1,"AND",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression OR Expression
                           {
			     $$ = new Expression($1,"OR",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression EQ Expression
                           {
			     $$ = new Expression($1,"EQ",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression NEQ Expression
                           {
			     $$ = new Expression($1,"NEQ",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression '<' Expression
                           {
			     $$ = new Expression($1,"<",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression '>' Expression
                           {
			     $$ = new Expression($1,">",$3);
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression LEQ Expression
                           {
			     $$ = new Expression($1,"LEQ",$3); 
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
		| Expression GEQ Expression
                           {
			     $$ = new Expression($1,"GEQ",$3); 
			     if (strcmp($1->_type, $3->_type)!=0){
			       $$->setType("ERROR");
			     } else {
			       $$->setType("bool");
			     }
			   }
//		| Assignment
//		          {
//			    $$ = new Expression($1);
//			    $$->setType($1->_type);
//			  }
		| '-' Expression
		          {
			    $$ = new Expression("-", $2);
			    $$->setType($2->_type);
			  }
		| '+' Expression 
		          {
			    $$ = new Expression("+", $2);
			    $$->setType($2->_type);
			  }
		| '!' Expression
		         {
			   $$ = new Expression("!", $2);
			   $$->setType("bool");
			 }
		| PrimitiveExpression 
		           {
			     $$ = new Expression($1);
			     $$->setType($1->_type);
			   }
		;

Assignment:
                LeftHandSide '=' Expression
		   {
		     $$ = new Assignment($1, $3);
		     /* check that lefthandside type  = expression*/
		     if (strcmp($1->_type, $3->_type)==0){
		       $$->setType($1->_type);
		     }
		     else {
		       $$->setType("ERROR");
		     }
		   }
		;

LeftHandSide:
		Name 
		    {
		      $$ = new LeftHandSide($1);
		      $$->setType($1->_type);
		    }
		| FieldAccess
		    {
		      $$ = new LeftHandSide($1);
		      $$->setType($1->_type);
		    }
		;

Literal:
		INT_CONST   {$$ = new Literal(new LeafNode($1)); $$->setType("int"); }
		| CHAR_CONST {$$ = new Literal(new LeafNode($1)); $$->setType("char");}
		| STR_CONST {$$ = new Literal(new LeafNode($1)); $$->setType("str");}
                | NIL {$$ = new Literal(new LeafNode("null"));  $$->setType("");}
		| TRUE {$$ = new Literal(new LeafNode(true)); $$->setType("bool");}
		| FALSE {$$ = new Literal(new LeafNode(false)); $$->setType("bool");}
		;


%%

void yyerror(const char *errmsg) {
     fprintf(stderr, "%d: %s at ' %s '\n", yylineno, errmsg, yytext);
     //     exit(1);
}



void test_print(PTNode * root, bool is_last_child, const char * indent){
  if (root==NULL){ return ; }

  // print the node
  switch(root->getModel()){
  case IDNODE:
    printf("%s%s\n", indent, ((IDNode*)root)->_val);
    break;
  case LEAF_STR:
    printf("%s%s\n", indent, ((LeafNode*)root)->_str);
    break;
  case LEAF_INT:
    printf("%s%d\n", indent, ((LeafNode*)root)->_int);
    break;
  case LEAF_CHAR:
    printf("%s%c\n", indent, ((LeafNode*)root)->_char);
    break;
  case LEAF_BOOL:
    printf("%s%s\n", indent, (((LeafNode*)root)->_bool)?"true":"false");
    break;
  default:
    printf("%s%s[%s]\n", indent, STR_MODEL[root->getModel()], root->_type);
  }
  //print the children
  vector<PTNode*>::iterator it;
  vector<PTNode*> * children = root->getChildren();
  for(it = children->begin(); it < children->end(); it++){
    if (*it==NULL) { continue ; }

    bool child_is_last_child = false;
    if (it+1==children->end() || *(it+1)==NULL){
      child_is_last_child = true;
    }

    char* new_indent = new char[strlen(indent)+5];
    strcpy(new_indent, indent);
    //remove old |
    new_indent[strlen(indent)-3] = ' ';
    new_indent[strlen(indent)-2] = ' ';
    new_indent[strlen(indent)-1] = ' ';
    if (is_last_child){
      new_indent[strlen(indent)-4] = ' ';
    }
    //add indentation
    strncat(new_indent, "|___", 4);

    test_print(*it,child_is_last_child, new_indent);
  }
}

int main(int argc, char ** argv) {
  bool debug = false;
  
  if (argc>1) {
    if (strcmp(argv[1], "-d")==0){
      // show debug info: parsetree + symtab
      debug = true;
    } else {
      printf("USAGE: ./analyzer [-d]\n");
      return 0;
    }
  }
  // push the global symbol table onto scope stack
  ss.push(&symtab);
  // parse the input
  yyparse();
  setClassSizes(&symtab);

  genCode(&ss, (iostream&)cout);
  genData(&ss, (iostream&)cout);

  if (debug) {
    symtab.printTable();
    test_print(root, true, "|__");    
    printf("-------------------\n");
  }

  
  return 0;
}

