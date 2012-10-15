#include "PTNodes.hh"
using namespace std;

/**********************
 * class PTNode
**********************/

PTNode :: PTNode( MODEL_T model )
{
  _model = model;
  _type = "";
  _entity = NULL;
  t_label = NULL;
  f_label = NULL;
}

PTNode :: PTNode()
{
  _model = PTNODE;
  _type = "";
  _entity = NULL;
}


PTNode :: ~PTNode()
{
}

vector<PTNode *> *
PTNode :: getChildren()
{
  return &_children;
}

void
PTNode :: genCode(ScopeStack * ss,  iostream & ios )
{
  ios<<"PTNODE gencode..."<<endl;
}

void
PTNode :: setType (const char * type)
{
  _type = type;
}

const char *
PTNode :: getType()
{
  return _type;
}

MODEL_T
PTNode :: getModel()
{
  return _model;
}

/**********************
 * class LeafNode
**********************/

LeafNode :: LeafNode( MODEL_T model )
  : PTNode(model)
{
}

LeafNode :: LeafNode (const char * val)
{
  _str = val;
  _model = LEAF_STR;
}

LeafNode :: LeafNode (int val)
{
  _int = val;
  _model = LEAF_INT;
}

LeafNode :: LeafNode (char val)
{
  _char = val;
  _model = LEAF_CHAR;
}

LeafNode :: LeafNode (bool val)
{
  _bool = val;
  _model = LEAF_BOOL;
  t_label = genLabel();
  f_label = genLabel();
}


LeafNode :: ~LeafNode ()
{
}

void
LeafNode :: genCode(ScopeStack * ss, iostream & ios)
{
}


/**********************
 * class Id
**********************/

IDNode :: IDNode(const char * val)
  : PTNode(IDNODE)
{
  _val = val;
}

void
IDNode :: genCode(ScopeStack * ss, iostream & ios)
{
}
/**********************
 * class Program
**********************/

Program :: Program ()
  : PTNode(PROGRAM)
{
  _children.push_back(NULL);
}


Program :: Program (PTNode * c)
  : PTNode(PROGRAM)
{
  _children.push_back(c);
}

PTNode *
Program :: classDeclarations()
{
  return _children[0];
}

void
Program :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ClassDeclarations
**********************/

ClassDeclarations :: ClassDeclarations ()
  : PTNode(CLASS_DECLARATIONS)
{
}

ClassDeclarations :: ClassDeclarations (PTNode * cd)
  : PTNode(CLASS_DECLARATIONS)
{
  _children.push_back(NULL);
  _children.push_back(cd);
}

ClassDeclarations :: ClassDeclarations (PTNode * cds, PTNode * cd)
  : PTNode(CLASS_DECLARATIONS)
{
  _children.push_back(cds);
  _children.push_back(cd);
}

void
ClassDeclarations :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ClassDeclaration
**********************/

ClassDeclaration :: ClassDeclaration (PTNode * id, PTNode * body)
  : PTNode(CLASS_DECLARATION)
{
  _children.push_back(id);
  _children.push_back(body);
}

ClassDeclaration :: ClassDeclaration (PTNode * id, PTNode * cls, 
				      PTNode * body)
  : PTNode(CLASS_DECLARATION)
{
  _children.push_back(id);
  _children.push_back(body);
  _children.push_back(cls);
}

void
ClassDeclaration :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ClassBody
**********************/

ClassBody :: ClassBody()
  : PTNode (CLASS_BODY)
{
  _children.push_back( NULL );
}

ClassBody :: ClassBody (PTNode * bodyDecs)
  : PTNode (CLASS_BODY)
{
  _children.push_back(bodyDecs);
}

void
ClassBody :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ClassBodyDeclarations
**********************/

ClassBodyDeclarations :: ClassBodyDeclarations()
  : PTNode (CLASS_BODY_DECLARATIONS)
{
}

ClassBodyDeclarations :: ClassBodyDeclarations(PTNode *d)
  : PTNode(CLASS_BODY_DECLARATIONS)
{
  _children.push_back(d);
  _children.push_back(NULL);
}

ClassBodyDeclarations :: ClassBodyDeclarations(PTNode *d, PTNode *d2)
  : PTNode (CLASS_BODY_DECLARATIONS)
{
  _children.push_back(d);
  _children.push_back(d2);
}

void
ClassBodyDeclarations :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ClassMemberDeclaration
**********************/

ClassMemberDeclaration :: ClassMemberDeclaration()
  : PTNode (CLASS_MEMBER_DECLARATION)
{
}

ClassMemberDeclaration :: ClassMemberDeclaration (MODEL_T t)
  : PTNode (t)
{
}

ClassMemberDeclaration :: ClassMemberDeclaration(PTNode * fd)
  : PTNode (CLASS_MEMBER_DECLARATION)
{
  _children.push_back(fd);
}

void
ClassMemberDeclaration :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class FieldDeclaration
**********************/

FieldDeclaration :: FieldDeclaration (PTNode *t, PTNode * vds)
  : ClassMemberDeclaration(FIELD_DECLARATION)
{
  _children.push_back(t);
  _children.push_back(vds);
}

void
FieldDeclaration :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class MethodDeclaration
**********************/

MethodDeclaration :: MethodDeclaration (PTNode * h, PTNode * b)
  : ClassMemberDeclaration(METHOD_DECLARATION)
{
  _children.push_back(h);
  _children.push_back(b);
}

PTNode *
MethodDeclaration :: methodHeader()
{
  return _children[0];
}

PTNode *
MethodDeclaration :: methodBody()
{
  return _children[1];
}

void
MethodDeclaration :: genCode(ScopeStack *ss, iostream & ios)
{
  
  methodHeader()->genCode(ss, ios);

  _entity = methodHeader()->_entity;
  const char * meth_label = _entity->codeName();
  ss->setCurMethLabel(meth_label);
  stringstream sstream;
  methodBody()->genCode(ss, (iostream&)sstream);

  const char * meth_name = _entity->getName();
  bool isMain = (strcmp(meth_name, "main")==0)?true:false;
  int space_for_vars = ss->cur()->size();
  int space_for_regs = 8;
  int frame_size = space_for_vars + space_for_regs;
  int ra_offset = frame_size - space_for_vars;
  int fp_offset = ra_offset - 4;
  // prologue
  ios<<endl<<"\t.text"<<endl;

  if (isMain){
    ios<<"\t.globl main"<<endl;
    ios<<"main:"<<endl;
  } else {
    ios<<"\t.ent "<< meth_label<<endl;
    ios<<meth_label<<":"<<endl;
  }
  // allocate frame stack
  ios<<"sub\t$sp, $sp, " <<frame_size<<endl;
  // push return address
  ios<<"sw\t$ra, "<<ra_offset<<"($sp)"<<endl;
  // push caller's fp
  ios<<"sw\t$fp, "<<fp_offset<<"($sp)"<<endl;
  // new frame pointer
  ios<<"add\t$fp, $sp, "<<frame_size<<endl;

  // code
  ios<<sstream.str();

  ios<<meth_label<<"ret:"<<endl;
  // epilogue
  ios<<"lw\t$fp,"<<fp_offset<<"($sp)"<<endl;
  ios<<"lw\t$ra,"<<ra_offset<<"($sp)"<<endl;
  ios<<"add\t$sp, "<<frame_size<<endl;
  if (strcmp(meth_name, "main") == 0){
    // exit program
    ios<<"li\t$v0, 10"<<endl;
    ios<<"syscall"<<endl;
  } else{
    // jump back to caller
    ios<<"jr\t$ra"<<endl<<endl;
  }
}

/**********************
 * class MethodHeader
**********************/

MethodHeader :: MethodHeader (PTNode * t, PTNode * md)
  : PTNode (METHOD_HEADER)
{
  _children.push_back(t);
  _children.push_back(md);
}

PTNode *
MethodHeader :: methodDeclarator()
{
  return _children[1];
}

void
MethodHeader :: genCode(ScopeStack * ss, iostream & ios)
{
  methodDeclarator()->genCode(ss, ios);
  _entity = methodDeclarator()->_entity;
}

/**********************
 * class LocalVariableDeclarationStatement
**********************/

LocalVariableDeclarationStatement :: LocalVariableDeclarationStatement( PTNode * t, PTNode * vds)
  : PTNode (LOCAL_VARIABLE_DECLARATION_STATEMENT)
{
  _children.push_back(t);
  _children.push_back(vds);
}

void
LocalVariableDeclarationStatement :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class LocalVariableDeclarationStatements
**********************/

LocalVariableDeclarationStatements :: LocalVariableDeclarationStatements (PTNode * l)
  : PTNode (LOCAL_VARIABLE_DECLARATION_STATEMENTS)
{
  _children.push_back(l);
  _children.push_back(NULL);
}

LocalVariableDeclarationStatements :: LocalVariableDeclarationStatements (PTNode * l, PTNode *ls)
  : PTNode (LOCAL_VARIABLE_DECLARATION_STATEMENTS)
{
  _children.push_back(l);
  _children.push_back(ls);
}


LocalVariableDeclarationStatements :: LocalVariableDeclarationStatements ()
  : PTNode (LOCAL_VARIABLE_DECLARATION_STATEMENTS)
{
}

void
LocalVariableDeclarationStatements :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class Statement
**********************/

Statement :: Statement ()
  : PTNode (STATEMENT)
{
}

Statement :: Statement (MODEL_T t)
  : PTNode(t)
{
}

Statement :: Statement (PTNode * p)
  : PTNode(STATEMENT)
{
  _children.push_back(p);
}


void
Statement :: genCode(ScopeStack * ss, iostream & ios)
{
  if (_children[0]!=NULL){
    _children[0]->genCode(ss, ios);
  }
}

/**********************
 * class Statements
**********************/

Statements :: Statements ()
  : PTNode(STATEMENTS)
{
}

Statements :: Statements (PTNode *s)
  : PTNode(STATEMENTS)
{
  _children.push_back(NULL);
  _children.push_back(s);
}

Statements :: Statements (PTNode *ss, PTNode * s)
  : PTNode (STATEMENTS)
{
  _children.push_back(ss);
  _children.push_back(s);
}

void
Statements :: genCode (ScopeStack *ss, iostream & ios)
{
  if(_children[0]!=NULL){
    _children[0]->genCode(ss, ios);
  }
  _children[1]->genCode(ss, ios);
}

/**********************
 * class MethodBody
**********************/

MethodBody :: MethodBody()
  : PTNode (METHOD_BODY)
{
}

MethodBody :: MethodBody (PTNode * lvds, PTNode * stmnts)
  : PTNode (METHOD_BODY)
{
  _children.push_back(lvds);
  _children.push_back(stmnts);
}

MethodBody :: MethodBody (PTNode * p)
  : PTNode (METHOD_BODY)
{
  _children.push_back(p);
  _children.push_back(NULL);
}

PTNode *
MethodBody :: statements()
{
  if (_children.size()>1){
    return _children[1];
  } else {
    return NULL;
  }
}
void
MethodBody :: genCode(ScopeStack * ss, iostream & ios)
{
  if (statements() != NULL){
    statements()->genCode(ss, ios);
  }
}

/**********************
 * class IfThenStatement
**********************/

IfThenStatement :: IfThenStatement (PTNode *expr, PTNode *stmnt)
  : Statement (IF_THEN_STATEMENT)
{
  _children.push_back(expr);
  _children.push_back(stmnt);
}

void
IfThenStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  Expression * expr = (Expression*)_children[0];
  PTNode * stmnt = _children[1];
  t_label = genLabel();
  f_label = genLabel();
  expr->genCode(ss, ios, t_label, f_label);
  ios<<t_label<<":"<<endl;
  stmnt->genCode(ss, ios);
  ios<<f_label<<":"<<endl;
}

/**********************
 * class IfThenElseStatement
**********************/

IfThenElseStatement :: IfThenElseStatement (PTNode *expr, PTNode *s1,
					    PTNode * s2)
  : Statement(IF_THEN_ELSE_STATEMENT)
{
  _children.push_back(expr);
  _children.push_back(s1);
  _children.push_back(s2);
}

void
IfThenElseStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  Expression * expr = (Expression *)_children[0];
  PTNode * s1 = _children[1];
  PTNode * s2 = _children[2];
  t_label = genLabel();
  f_label = genLabel();
  const char * s_next = genLabel();

  expr->genCode(ss, ios, t_label, f_label);
  ios<<t_label<<":"<<endl;
  s1->genCode(ss, ios);
  ios<<"j\t"<<s_next<<endl;
  ios<<f_label<<":"<<endl;
  s2->genCode(ss, ios);
  ios<<s_next<<":"<<endl;
}

/**********************
 * class WhileStatement
**********************/

WhileStatement :: WhileStatement ( PTNode *expr, PTNode *st )
  : Statement(WHILE_STATEMENT)
{
  _children.push_back(expr);
  _children.push_back(st);
}

void
WhileStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  Expression * expr = (Expression*) _children[0];
  PTNode * stmnt = _children[1];

  const char * s_begin = genLabel();
  t_label = genLabel();
  f_label = genLabel();

  ios<<s_begin<<":"<<endl;
  expr->genCode(ss, ios, t_label, f_label);
  ios<<t_label<<":"<<endl;
  stmnt->genCode(ss, ios);
  ios<<"j\t"<<s_begin<<endl;
  ios<<f_label<<":"<<endl;
}

/**********************
 * class SimpleBlock
**********************/

SimpleBlock :: SimpleBlock ()
  : Statement (SIMPLE_BLOCK)
{
}

SimpleBlock :: SimpleBlock (PTNode *stmns)
  : Statement (SIMPLE_BLOCK)
{
  _children.push_back(stmns);
}

PTNode *
SimpleBlock :: statements()
{
  if (_children[0] != NULL){
    return _children[0];
  } else {
    return NULL;
  }
}
void
SimpleBlock :: genCode(ScopeStack * ss, iostream & ios)
{
  if (statements() != NULL){
    statements()->genCode(ss, ios);
  }
}


/**********************
 * class EmptyStatement
**********************/

EmptyStatement :: EmptyStatement ()
  : Statement (EMPTY_STATEMENT)
{
}

void
EmptyStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  // do nothing
}

/**********************
 * class StatementExpression
**********************/

StatementExpression :: StatementExpression (PTNode *p)
  : Statement(STATEMENT_EXPRESSION)
{
  _children.push_back(p);
}

void
StatementExpression :: genCode(ScopeStack * ss, iostream & ios)
{
  _children[0]->genCode(ss, ios);
}

/**********************
 * class ContinueStatement
**********************/

ContinueStatement :: ContinueStatement ()
  : Statement (CONTINUE_STATEMENT)
{
}

void
ContinueStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  // jump back to start of while...
  //ios<<"j\t"<<endl;
}

/**********************
 * class ReturnStatement
**********************/

ReturnStatement :: ReturnStatement()
  : Statement (RETURN_STATEMENT)
{
  _children.push_back(NULL);
}

ReturnStatement :: ReturnStatement(PTNode *e)
  : Statement (RETURN_STATEMENT)
{
  _children.push_back(e);
}

void
ReturnStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  PTNode * expr = _children[0];
  if (expr != NULL){
    expr->genCode(ss, ios);
  }
  // store return value in $v0
  if (expr->_entity != NULL){
    // var
    ios<<"lw\t$v0, "<<expr->_entity->getOffset()<<"($fp)"<<endl;
  } else{
    ios<<"li\t$v0, "<<expr->_val<<endl;
  }
  // jump to return
  const char * meth_label = ss->getMethLabel();
  ios<<"j\t"<<meth_label<<"ret"<<endl;
}

/**********************
 * class IOStatement
**********************/

IOStatement :: IOStatement (const char * op, PTNode * expr)
  : Statement(IO_STATEMENT)
{
  _op = op;
  _children.push_back(expr);
}

PTNode *
IOStatement :: expression()
{
  return _children[0];
}
void
IOStatement :: genCode(ScopeStack * ss, iostream & ios)
{
  expression()->genCode(ss, ios);
  Entity * e = expression()->_entity;
  int print_service = 1;
  int read_service = 5;
  if (e == NULL){
    ios<<"li\t$a0, "<<expression()->_val <<endl;
  } else {
    ios<<"lw\t$a0, "<<e->getOffset()<<"($fp)"<<endl;
  }
  if(strcmp(_op, "output") == 0){
    if (strcmp(expression()->_type, "int")){
      print_service = 1;
    }
    ios<<"li\t$v0, "<<print_service<<endl;
    ios<<"syscall"<<endl;
  } else {
    if (strcmp(expression()->_type, "int")){
      read_service = 5;
    }
    ios<<"li\t$v0, "<<read_service<<endl;
    ios<<"syscall"<<endl;
    ios<<"sw\t$v0, "<<e->getOffset()<<"($fp)"<<endl;
  }
  // new line
  ios<<"la\t$a0, EOL"<<endl;
  ios<<"li\t$v0, 4"<<endl;
  ios<<"syscall"<<endl;
}

/**********************
 * class Assignment
**********************/

Assignment :: Assignment (PTNode *lhs, PTNode *expr)
  : PTNode (ASSIGNMENT)
{
  _children.push_back(lhs);
  _children.push_back(expr);
}

PTNode *
Assignment :: LHS()
{
  return _children[0];
}

PTNode *
Assignment :: expression()
{
  return _children[1];
}

void
Assignment :: genCode(ScopeStack * ss, iostream & ios)
{
  expression()->genCode(ss, ios);
  if (expression()->_entity == NULL){
    // this is a primitive
    // store in reg $t0
    ios<<"li\t$t0, "<<expression()->_val<<endl;
  }

  LHS()->genCode(ss, ios);
  
}

/**********************
 * class LeftHandSide
**********************/

LeftHandSide :: LeftHandSide (PTNode *p)
  : PTNode(LEFT_HAND_SIDE)
{
  _children.push_back(p);
}

PTNode *
LeftHandSide :: name()
{
  return _children[0];
}

void
LeftHandSide :: genCode(ScopeStack * ss, iostream & ios)
{
  name()->genCode(ss, ios);
  _entity = name()->_entity;
  // handle storage here for the recipient
  if (name()->getModel() == SIMPLE_NAME){
    ios<<"sw\t$t0, "<< _entity->_offset <<"($fp)"<<endl;
  }
}

/**********************
 * class MethodInvocation
**********************/

MethodInvocation :: MethodInvocation (PTNode * p1)
  : PTNode(METHOD_INVOCATION)
{
  _children.push_back(p1);
  _children.push_back(NULL);
  _children.push_back(new ArgumentList());
}

MethodInvocation :: MethodInvocation (PTNode * p1, PTNode * p2)
  : PTNode(METHOD_INVOCATION)
{
  _children.push_back(p1);
  _children.push_back(NULL);
  _children.push_back(p2);
}

MethodInvocation :: MethodInvocation (PTNode *p1, PTNode * p2,
				      PTNode *p3)
  : PTNode(METHOD_INVOCATION)
{
  _children.push_back(p1);
  _children.push_back(p2);
  _children.push_back(p3);
}


PTNode *
MethodInvocation :: primary()
{
  return _children[0];
}

PTNode *
MethodInvocation :: name()
{
  return _children[0];
}

PTNode *
MethodInvocation :: super()
{
  return _children[0];
}

PTNode *
MethodInvocation :: id()
{
  return _children[1];
}

PTNode *
MethodInvocation :: argumentList()
{
  return _children[2];
}

void
MethodInvocation :: genCode(ScopeStack * ss, iostream & ios)
{
  bool isQualified = false;
  if (argumentList()!=NULL){
    argumentList()->genCode(ss, ios);
  }
  name()->genCode(ss, ios);
  
  // genCode for parameter 'this'
  Entity * e_this = ss->find("this");
  ios<<"sub\t$sp, "<<e_this->getSize() <<endl;
  ios<<"lw\t$t0, "<< e_this->getOffset() <<"($fp)"<<endl;
  ios<<"sw\t$t0, "<< e_this->getSize() <<"($sp)"<<endl;
  name()->genCode(ss, ios);
  SymTable *localTab = ss->cur();
  int paramsize = name()->_entity->getSymTable()->find("params")->getSymTable()->size();
  const char * meth_label = name()->_entity->codeName();
  ios<<"jal\t"<< meth_label <<endl;
  ios<<"add\t$sp, "<< paramsize <<endl;
  // store return value into temporary variable
  _type = name()->_entity->getType();
  _entity = genTemp(ss->cur(), _type);
  ios<<"sw\t$v0, "<<_entity->getOffset()<<"($fp)"<<endl;
  ios<<"lw\t$t0, "<<_entity->getOffset()<<"($fp)"<<endl;
}
  
/**********************
 * class ClassInstanceCreationExpression
**********************/

ClassInstanceCreationExpression :: ClassInstanceCreationExpression(PTNode *t, PTNode *al)
  : PTNode(CLASS_INSTANCE_CREATION_EXPRESSION)
{
  _children.push_back(t);
  _children.push_back(al);
}

ClassInstanceCreationExpression :: ClassInstanceCreationExpression(PTNode *t)
  : PTNode(CLASS_INSTANCE_CREATION_EXPRESSION)
{
  _children.push_back(t);
}

void
ClassInstanceCreationExpression :: genCode(ScopeStack * ss, iostream & ios)
{

  PTNode *refType = _children[0];
  refType->genCode(ss, ios);

  _type = refType->_type;
  _entity = genTemp(ss->cur(), _type);
  
  
  Entity *cls = ss->find(_type);
  if (cls==NULL){printf("could not find: %s\n", _type); }
  // allocate new from heap
  ios<<"li\t$v0, 9"<<endl;
  ios<<"li\t$a0, "<<cls->getSize()<<endl;
  ios<<"syscall"<<endl;
  ios<<"sw\t$v0, "<<_entity->getOffset()<<"($fp)"<<endl;
}

/**********************
 * class ArgumentList
**********************/

ArgumentList :: ArgumentList ()
  : PTNode (ARGUMENT_LIST)
{
}

ArgumentList :: ArgumentList (PTNode *al, PTNode *e)
  : PTNode(ARGUMENT_LIST)
{
  _children.push_back(al);
  _children.push_back(e);
}

ArgumentList :: ArgumentList (PTNode *e)
  : PTNode (ARGUMENT_LIST)
{
  _children.push_back(NULL);
  _children.push_back(e);
}

PTNode *
ArgumentList :: expression()
{
  return _children[1];
}

PTNode *
ArgumentList :: argumentList()
{
  return _children[0];
}

void
ArgumentList :: genCode(ScopeStack * ss, iostream & ios)
{
  if (_children.size()==0){ return ;}
  expression()->genCode(ss, ios);

  Entity * e = expression()->_entity;
  int size= 0;
  if(expression()->_type == "char"){
    size = 1;
  } else{
    size = 4;
  }
  ios<<"sub\t$sp, "<< size <<endl;
  if (e == NULL){
    ios<<"li\t$t0, "<< expression()->_val<<endl;
  } else{
    ios<<"lw\t$t0, "<< e->getOffset() <<"($fp)"<<endl;
  }
  ios<<"sw\t$t0, "<< size <<"($sp)"<<endl;

 
  if (argumentList() != NULL){
    argumentList()->genCode(ss, ios);
  }

}

/**********************
 * class Expression
**********************/

Expression :: Expression ()
  : PTNode (EXPRESSION)
{
}

Expression :: Expression (MODEL_T t)
  : PTNode (t)
{
}

Expression :: Expression (PTNode * a, const char *op, PTNode * b)
  : PTNode (EXPRESSION)
{
  _children.push_back(new LeafNode(op));
  _children.push_back(a);
  _children.push_back(b);
}

Expression :: Expression (const char * op, PTNode * a)
  : PTNode (EXPRESSION)
{
  _children.push_back(new LeafNode(op));
  _children.push_back(a);
}

Expression :: Expression (PTNode * a)
  : PTNode(EXPRESSION)
{
  _children.push_back(NULL);
  _children.push_back(a);
}

PTNode *
Expression :: OP()
{
  return _children[0];
}

Expression *
Expression :: E1()
{
  return (Expression *)_children[1];
}

Expression *
Expression :: E2()
{
  return (Expression *)_children[2];
}

void
Expression :: genCode(ScopeStack * ss, iostream &ios,
		      const char * trueLabel,
		      const char * falseLabel)
{
  Expression * e1 = E1();
  Expression * e2 = E2();
  if (OP()==NULL){
    // primitive expression (true, false, id.type=bool)
    if (goesToLiteral(e1)){
      stringstream e1_stream;
      e1->genCode(ss, (iostream &) e1_stream);
      _val = e1->_val;
      _entity = e1->_entity;
      if (_val==1){
	//true
	ios<<"j\t"<<trueLabel<<endl;
      } else {
	//false
	ios<<"j\t"<< falseLabel<<endl;
      }
    }
    return;
  }
  const char * op = ((LeafNode *)OP())->_str;    
  if (e2 == NULL){
    // unary expression
    
  } else {
    
    // do operatation
    if( strcmp(op, "AND") == 0){
      e1->t_label = genLabel();
      e1->f_label = falseLabel;
      e2->t_label = trueLabel;
      e2->f_label = falseLabel;
      e1->genCode(ss, ios, e1->t_label, e1->f_label);
      ios<<e1->t_label<<":"<<endl;
      e2->genCode(ss, ios, e2->t_label, e2->f_label);

    } else if (strcmp(op, "OR") == 0){
      e1->t_label = trueLabel;
      e1->f_label = genLabel();
      e2->t_label = trueLabel;
      e2->f_label = falseLabel;
      e1->genCode(ss, ios, e1->t_label, e1->f_label);
      ios<<e1->f_label<<":"<<endl;
      e2->genCode(ss, ios, e2->t_label, e2->f_label);

    } else /* op */{ 
      e1->genCode(ss, ios);
      e2->genCode(ss, ios);
      if (e1->_entity != NULL){
	//id
	ios<<"lw\t$t0, "<<e1->_entity->getOffset()<<"($fp)"<<endl;
      } else {
	ios<<"li\t$t0, "<<e1->_val<<endl;
	// literal
      }
      
      if (e2->_entity != NULL){
	// var
	ios<<"lw\t$t1, "<<e2->_entity->getOffset()<<"($fp)"<<endl;
      } else {
	// literal
	ios<<"li\t$t1, "<<e2->_val<<endl;
      }
      
      if (strcmp(op, "EQ") == 0){
	ios<< "beq\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;
      } else if (strcmp(op, "NEQ") == 0){
	ios<< "bne\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;
      } else if (strcmp(op, "LEQ") == 0){
	ios<< "ble\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;	
      } else if (strcmp(op, "GEQ") == 0){
	ios<< "bge\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;
      } else if (strcmp(op, "<") == 0){
	ios<< "blt\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;
      } else if (strcmp(op, ">") == 0){
	ios<< "bgt\t$t0, $t1, "<< trueLabel<<endl;
	ios<< "j\t"<< falseLabel<<endl;
      }


    }
  }
  
}

void
Expression :: genCode(ScopeStack * ss, iostream & ios)
{
  E1()->genCode(ss, ios);  

  if(E2() != NULL) { E2()->genCode(ss, ios); }

  if (strcmp(E1()->_type, "bool")==0){
    if (OP()==NULL){
      _val = E1()->_val;
      _entity = E1()->_entity;
      return;
    }
    const char * op = ((LeafNode *)OP())->_str;    
    if (E2() == NULL){
      // unary expression
      
    } else {
      if (E1()->_entity != NULL){
	//var
      } else {
	// literal
      }

      if (E2()->_entity != NULL){
	// var
      } else {
	// literal
      }
    }
  }
  else if (strcmp(E1()->_type, "int")==0){
    if (OP()==NULL){
      // pass info up
      _val = E1()->_val;
      _entity = E1()->_entity;
      return;
    }    
    const char * op = ((LeafNode *)OP())->_str;    
    if (E2() == NULL){
      // Unary Expression
      
      
    }
    else {
      if (E1()->_entity != NULL){
	// if var
	ios<<"lw\t$t0, "<< E1()->_entity->_offset <<"($fp)" <<endl;
      }
      else {
	// is a literal
	ios<<"li\t$t0, "<< E1()->_val<<endl;
      }
      
      if (E2()->_entity != NULL){
	// if e2 is symbol
	ios<<"lw\t$t1, "<< E2()->_entity->_offset <<"($fp)"<<endl;
      }
      else{
	ios<<"li\t$t1, "<< E2()->_val<<endl;
      }
      
      //gen temp
      _entity = genTemp(ss->cur(), "int");
      // TODO: evaluate new offsets
      if ( strcmp(op, "+") == 0 ){
	ios<<"add\t$t0, $t0, $t1"<<endl;
      }
      else if(strcmp(op, "-")==0){
	ios<<"sub\t$t0, $t0, $t1"<<endl;      
      }
      else if(strcmp(op, "*")==0){
	ios<<"mul\t$t0, $t0, $t1"<<endl;
      }
      else if(strcmp(op, "/")==0){
	ios<<"div\t$t0, $t0, $t1"<<endl;
      }
      
    }
    // store the value from register
    ios<<"sw\t$t0, "<< _entity->_offset <<"($fp)"<<endl;
  }
  else /* other type */{
    // object type
    if (OP()==NULL){
      // pass info up
	_val = E1()->_val;
	_entity = E1()->_entity;
	// store the value from register
	ios<<"sw\t$t0, "<< _entity->_offset <<"($fp)"<<endl;
    }
  }
  //TODO: MORE TYPES, CHAR, STR 
}
/**********************
 * class PrimitiveExpression
**********************/

PrimitiveExpression :: PrimitiveExpression (PTNode *p)
  : Expression (PRIMITIVE_EXPRESSION)
{
  _children.push_back(p);
}

void
PrimitiveExpression :: genCode(ScopeStack * ss, iostream & ios)
{
  _children[0]->genCode(ss, ios);
  _val = _children[0]->_val;
  _type = _children[0]->_type;
  _entity = _children[0]->_entity;
  t_label = _children[0]->t_label;
  f_label = _children[0]->f_label;
}

/**********************
 * class Primary
**********************/

Primary :: Primary (PTNode *p)
  : PTNode (PRIMARY)
{
  _children.push_back(p);
}

void
Primary :: genCode(ScopeStack * ss, iostream & ios)
{
  PTNode * child = _children[0];
  child->genCode(ss, ios);
  _val = child->_val;
  _type = child->_type;
  _entity = child->_entity;
  t_label = child->t_label;
  f_label = child->f_label;
}

/**********************
 * class Literal
**********************/

Literal :: Literal (PTNode *p)
  : PTNode (LITERAL)
{
  _children.push_back(p);
}

void
Literal :: genCode(ScopeStack * ss, iostream & ios)
{
  // gen .asciiz and data for the literals
  // only ints for now
  LeafNode * child = (LeafNode *)_children[0];
  if (child->getModel()==LEAF_INT){
    _val = child->_int;
    _type = "int";
  } else if (child->getModel()==LEAF_BOOL){
    _val = (child->_bool)?1:0;
    _type = "bool";
  }
  t_label = child->t_label;
  f_label = child->f_label;
  _entity = child->_entity;
}

/**********************
 * class FieldAccess
**********************/

FieldAccess :: FieldAccess (PTNode *n, PTNode *l)
  : PTNode(FIELD_ACCESS)
{
  _children.push_back(n);
  _children.push_back(l);
}

void
FieldAccess :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class Type
**********************/

Type :: Type (PTNode *pt)
  : PTNode (TYPE)
{
  _children.push_back(pt);
}

Type :: Type(MODEL_T t)
  : PTNode(t)
{
}

void
Type :: genCode(ScopeStack * ss, iostream & ios)
{
  _children[0]->genCode(ss, ios);
  _entity = _children[0]->_entity;
  _type = _children[0]->_type;
}

/**********************
 * class PrimitiveType
**********************/

PrimitiveType :: PrimitiveType(PTNode *p)
  : Type(PRIMITIVE_TYPE)
{
  _children.push_back(p);
}

void
PrimitiveType :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class ReferenceType
**********************/

ReferenceType :: ReferenceType (PTNode *t)
  : Type(REFERENCE_TYPE)
{
  _children.push_back(t);
}

void
ReferenceType :: genCode(ScopeStack * ss, iostream & ios)
{

  PTNode *name = _children[0];
  name->genCode(ss, ios);
  _entity = name->_entity;
  _type = name->_type;
}

/**********************
 * class Name
**********************/

Name :: Name(PTNode *p)
  : PTNode(NAME)
{
  _children.push_back(p);
}

Name :: Name (MODEL_T t)
  : PTNode(t)
{
}

void
Name :: genCode(ScopeStack * ss, iostream & ios)
{
  if(_children[0] != NULL){
    _children[0]->genCode(ss, ios);
    _entity = _children[0]->_entity;
  }
}

/**********************
 * class SimpleName
**********************/

SimpleName :: SimpleName (PTNode *p)
  : Name(SIMPLE_NAME)
{
  _children.push_back(p);
}

PTNode *
SimpleName :: ID()
{
  return _children[0];
}

void
SimpleName :: genCode(ScopeStack * ss, iostream & ios)
{
  const char * id = ((LeafNode*)ID())->_str;
  _entity = ss->find(id);
  if (strcmp(_entity->getType(), "class")==0){
    _type = id;
  } else{
    // normal ID, not class type
  _type = _entity->getType();
  }
}

/**********************
 * class QualifiedName
**********************/

QualifiedName :: QualifiedName (PTNode *n, PTNode *id)
  : Name(QUALIFIED_NAME)
{
  _children.push_back(n);
  _children.push_back(id);
}

void
QualifiedName :: genCode(ScopeStack * ss, iostream & ios)
{
  PTNode * name = _children[0];
  name->genCode(ss, ios);

  Entity *clsEnt = ss->find(name->_entity->getType());
  // should be leafnode
  const char * id = ((LeafNode *)_children[1])->_str;
  Entity * idEnt = ss->find(id);
  
  if (clsEnt != NULL){
    Entity * field = clsEnt->getSymTable()->find(id);
    _entity = field;
    _type = field->getType();
  } else {
    _entity = name->_entity;
    _type = name->_type;
  }
}

/**********************
 * class MethodDeclarator
**********************/

MethodDeclarator :: MethodDeclarator (PTNode *id, PTNode *fpl)
  : PTNode(METHOD_DECLARATOR)
{
  _children.push_back(id);
  _children.push_back(fpl);
}

MethodDeclarator :: MethodDeclarator (PTNode *id)
  : PTNode(METHOD_DECLARATOR)
{
  _children.push_back(id);
}

PTNode *
MethodDeclarator :: ID()
{
  return _children[0];
}

void
MethodDeclarator :: genCode(ScopeStack * ss, iostream & ios)
{
  const char * id = ((LeafNode *)ID())->_str;
  _entity = ss->find(id);
}

/**********************
 * class FormalParameterList
**********************/

FormalParameterList :: FormalParameterList (PTNode *fp)
  : PTNode(FORMAL_PARAMETER_LIST)
{
  _children.push_back(fp);
}

FormalParameterList :: FormalParameterList (PTNode *fpl, PTNode *fp)
  : PTNode(FORMAL_PARAMETER_LIST)
{
  _children.push_back(fp);
  _children.push_back(fpl);
}

PTNode *
FormalParameterList :: formalParameter()
{
  return _children[0];
}
PTNode *
FormalParameterList :: formalParameterList()
{
  return _children[1];
}

void
FormalParameterList :: genCode(ScopeStack * ss, iostream & ios)
{

}

/**********************
 * class FormalParameter
**********************/

FormalParameter :: FormalParameter (PTNode *type, PTNode *id)
  : PTNode (FORMAL_PARAMETER)
{
  _children.push_back(type);
  _children.push_back(id);
}

PTNode *
FormalParameter :: ID()
{
  return _children[1];
}

void
FormalParameter :: genCode(ScopeStack * ss, iostream & ios)
{
  const char * id = ((LeafNode *)ID())->_str;
  _entity = ss->find(id);
}

/**********************
 * class VariableDeclarators
**********************/

VariableDeclarators :: VariableDeclarators (PTNode *vd)
  : PTNode(VARIABLE_DECLARATORS)
{
  _children.push_back(vd);
}

VariableDeclarators :: VariableDeclarators (PTNode *vd, PTNode *vds)
  : PTNode (VARIABLE_DECLARATORS)
{
  _children.push_back(vd);
  _children.push_back(vds);
}

void
VariableDeclarators :: genCode(ScopeStack * ss, iostream & ios)
{
}

/**********************
 * class VariableDeclarator
**********************/

VariableDeclarator :: VariableDeclarator(PTNode * id)
  : PTNode(VARIABLE_DECLARATOR)
{
  _children.push_back(id);
}

void
VariableDeclarator :: genCode(ScopeStack * ss, iostream & ios)
{
}
