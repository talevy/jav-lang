#ifndef PTNODES_H
#define PTNODES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "./../symtab/ScopeStack.h"
#include "./../symtab/Entity.h"
#include "./../codegen/codegen.hh"
class Entity;
class ScopeStack;
using namespace std;
#define stringify( name ) # name

enum MODEL_T{
  IDNODE, PTNODE, PROGRAM, CLASS_DECLARATIONS, CLASS_DECLARATION,
  CLASS_BODY, CLASS_BODY_DECLARATIONS, CLASS_MEMBER_DECLARATION,
  FIELD_DECLARATION, METHOD_DECLARATION, METHOD_HEADER, METHOD_BODY,
  TYPE, PRIMITIVE_TYPE, REFERENCE_TYPE, SIMPLE_NAME, NAME,
  QUALIFIED_NAME, METHOD_DECLARATOR, FORMAL_PARAMETER_LIST,
  FORMAL_PARAMETER, VARIABLE_DECLARATORS, VARIABLE_DECLARATOR,
  LOCAL_VARIABLE_DECLARATION_STATEMENTS,
  LOCAL_VARIABLE_DECLARATION_STATEMENT, STATEMENTS, STATEMENT,
  WHILE_STATEMENT, IF_THEN_STATEMENT, IF_THEN_ELSE_STATEMENT,
  SIMPLE_BLOCK, CONTINUE_STATEMENT,
  RETURN_STATEMENT, EMPTY_STATEMENT, IO_STATEMENT,
  STATEMENT_EXPRESSION, ASSIGNMENT, LEFT_HAND_SIDE,
  METHOD_INVOCATION, CLASS_INSTANCE_CREATION_EXPRESSION,
  ARGUMENT_LIST, EXPRESSION, PRIMITIVE_EXPRESSION, PRIMARY,
  LITERAL, FIELD_ACCESS, LEAF_INT, LEAF_STR, LEAF_CHAR, LEAF_BOOL
};

static const char * STR_MODEL[] = 
  {
    stringify( IDNODE ),
    stringify( PTNODE ),
    stringify( PROGRAM ),
    stringify( CLASS_DECLARATIONS ),
    stringify( CLASS_DECLARATION ),
    stringify( CLASS_BODY ),
    stringify( CLASS_BODY_DECLARATIONS ), 
    stringify( CLASS_MEMBER_DECLARATION ),
    stringify( FIELD_DECLARATION ),
    stringify( METHOD_DECLARATION ),
    stringify( METHOD_HEADER ),
    stringify( METHOD_BODY ),
    stringify( TYPE ),
    stringify( PRIMITIVE_TYPE ),
    stringify( REFERENCE_TYPE ),
    stringify( SIMPLE_NAME ),
    stringify( NAME ),
    stringify( QUALIFIED_NAME ),
    stringify( METHOD_DECLARATOR ),
    stringify( FORMAL_PARAMETER_LIST ),
    stringify( FORMAL_PARAMETER ),
    stringify( VARIABLE_DECLARATORS ),
    stringify( VARIABLE_DECLARATOR ),
    stringify( LOCAL_VARIABLE_DECLARATION_STATEMENTS ),
    stringify( LOCAL_VARIABLE_DECLARATION_STATEMENT ),
    stringify( STATEMENTS ),
    stringify( STATEMENT ),
    stringify( WHILE_STATEMENT ),
    stringify( IF_THEN_STATEMENT ),
    stringify( IF_THEN_ELSE_STATEMENT ),
    stringify( SIMPLE_BLOCK ),
    stringify( CONTINUE_STATEMENT ),
    stringify( RETURN_STATEMENT ),
    stringify( EMPTY_STATEMENT ),
    stringify( IO_STATEMENT ),
    stringify( STATEMENT_EXPRESSION ),
    stringify( ASSIGNMENT ),
    stringify( LEFT_HAND_SIDE ),
    stringify( METHOD_INVOCATION ),
    stringify( CLASS_INSTANCE_CREATION_EXPRESSION ),
    stringify( ARGUMENT_LIST ),
    stringify( EXPRESSION ),
    stringify( PRIMITIVE_EXPRESSION ),
    stringify( PRIMARY ),
    stringify( LITERAL ),
    stringify( FIELD_ACCESS ),
    stringify( LEAF_INT ),
    stringify( LEAF_STR ),
    stringify( LEAF_CHAR ),
    stringify( LEAF_BOOL )
  };


class PTNode {
protected:
  vector <PTNode *> _children;
 public:
  MODEL_T _model;
  const char * _type;
  Entity * _entity;
  int _val;
  const char * t_label;
  const char * f_label;
  PTNode(MODEL_T model);
  PTNode();
  ~PTNode();
  vector < PTNode *> * getChildren();
  virtual void genCode(ScopeStack * , iostream &) = 0;
  MODEL_T getModel();
  void setType(const char *);
  const char * getType();
};

class LeafNode : public PTNode {
 public:
  const char *_str;
  int _int;
  char _char;
  bool _bool;
  LeafNode(MODEL_T t);
  LeafNode(const char * val);
  LeafNode(int val);
  LeafNode(char val);
  LeafNode(bool val);
  ~LeafNode();
  virtual void genCode(ScopeStack *, iostream &);
};

class IDNode : public PTNode {
public:
  IDNode(const char *);
  const char *_val;
  virtual void genCode(ScopeStack *, iostream &);
};

class Program : public PTNode {
 public:
  Program();
  Program(PTNode * c);
  ~Program();
  PTNode * classDeclarations();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassDeclarations : public PTNode {
 public:
  ClassDeclarations();
  ClassDeclarations(PTNode *cd);
  ClassDeclarations(PTNode *cds, PTNode *cd);
  PTNode * classDeclaration();
  PTNode * classDeclarations();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassDeclaration : public PTNode {
 public:
  ClassDeclaration(PTNode * id, PTNode * body);
  ClassDeclaration(PTNode *id, PTNode *clsT, PTNode *body);
  PTNode * id();
  PTNode * classType();
  PTNode * classBody();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassBody : public PTNode {
 public:
  ClassBody();
  ClassBody(PTNode *bodyDecs);
  PTNode * classBodyDeclarations();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassBodyDeclarations : public PTNode {
 public:
  ClassBodyDeclarations();
  ClassBodyDeclarations(PTNode * d);
  ClassBodyDeclarations(PTNode * ds, PTNode *d);
  PTNode * classMemberDeclaration();
  PTNode * classBodyDeclarations();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassMemberDeclaration : public PTNode {
 public:
  ClassMemberDeclaration(PTNode *fd);
  ClassMemberDeclaration();
  ClassMemberDeclaration(MODEL_T t);
  PTNode * fieldDeclaration();
  PTNode * methodDeclaration();
  virtual void genCode(ScopeStack *, iostream &);
};

class FieldDeclaration : public ClassMemberDeclaration {
 public:
  FieldDeclaration(PTNode * t, PTNode * vds);
  PTNode * type();
  PTNode * varDeclarators();
  virtual void genCode(ScopeStack *, iostream &ios);
};

class MethodDeclaration : public ClassMemberDeclaration {
 public:
  MethodDeclaration(PTNode * h, PTNode * b);
  PTNode * methodHeader();
  PTNode * methodBody();
  virtual void genCode(ScopeStack *, iostream&);
};

class MethodHeader : public PTNode {
 public:
  MethodHeader(PTNode *type, PTNode * md);
  PTNode * retType();
  PTNode * methodDeclarator();
  virtual void genCode(ScopeStack *, iostream &);
};


class LocalVariableDeclarationStatement : public PTNode {
 public:
  LocalVariableDeclarationStatement(PTNode * t, PTNode * vds);
  PTNode * type();
  PTNode * variableDeclarators();
  virtual void genCode(ScopeStack *, iostream &);
};

class LocalVariableDeclarationStatements : public PTNode {
 public:
  LocalVariableDeclarationStatements();
  LocalVariableDeclarationStatements(PTNode * l);
  LocalVariableDeclarationStatements(PTNode *ls, PTNode *l);
  PTNode * localVariableDeclarationStatements();
  PTNode * localVariableDeclarationStatement();
  virtual void genCode(ScopeStack *, iostream &);
};

class Statement : public PTNode {
 public:
  Statement();
  Statement(PTNode * p);
  Statement(MODEL_T t);
  PTNode * statement();
  virtual void genCode(ScopeStack *, iostream &);
};

class Statements : public PTNode {
 public:
  Statements();
  Statements(PTNode *s);
  Statements(PTNode *stmns, PTNode *s);
  PTNode * statements();
  PTNode * statement();
  virtual void genCode(ScopeStack *, iostream &);
};

class MethodBody : public PTNode {
 public:
  MethodBody(PTNode *lvds, PTNode *stmts);
  MethodBody(PTNode * p);
  MethodBody();
  PTNode * localVariableDeclarationStatements();
  PTNode * statements();
  virtual void genCode(ScopeStack *, iostream &);
};

class IfThenStatement : public Statement {
 public:
  IfThenStatement(PTNode *expr, PTNode *stmnt);
  PTNode * expression();
  PTNode * statement();
  virtual void genCode(ScopeStack *, iostream &);
};

class IfThenElseStatement : public Statement {
 public:
  IfThenElseStatement(PTNode *expr, PTNode *s1, PTNode *s2);
  PTNode * getExpression();
  PTNode * getStatementIf();
  PTNode * getStatementElse();
  virtual void genCode(ScopeStack *, iostream &);
};

class WhileStatement : public Statement {
 public:
  WhileStatement(PTNode *expr, PTNode *stmnt);
  PTNode * expression();
  PTNode * statement();
  virtual void genCode(ScopeStack *, iostream &);
};

class SimpleBlock : public Statement {
 public:
  SimpleBlock(PTNode *stmnts);
  SimpleBlock();
  ~SimpleBlock(){}
  PTNode * statements();
  virtual void genCode(ScopeStack *, iostream &);
};

class EmptyStatement : public Statement {
 public:
  EmptyStatement();
  ~EmptyStatement();
  virtual void genCode(ScopeStack *, iostream &);
};

class StatementExpression : public Statement {
 public:
  StatementExpression(PTNode *p);
  ~StatementExpression();
  PTNode * assignment();
  PTNode * methodInvocation();
  PTNode * classInstanceCreationExpression();
  virtual void genCode(ScopeStack *, iostream &);
};

class ContinueStatement : public Statement {
 public:
  ContinueStatement();
  ~ContinueStatement();
  virtual void genCode(ScopeStack *, iostream &);
};

class ReturnStatement : public Statement {
 public:
  ReturnStatement();
  ReturnStatement(PTNode *p);
  PTNode * expression();
  virtual void genCode(ScopeStack *, iostream &);
};

class IOStatement : public Statement {
  private:
    const char * _op;
 public:
  IOStatement(const char * op, PTNode *expr);
  ~IOStatement();
  PTNode * expression();
  virtual void genCode(ScopeStack *, iostream &);
};

class Assignment : public PTNode {
 public:
  Assignment(PTNode *lhs, PTNode *expr);
  ~Assignment(){}
  PTNode *LHS();
  PTNode *expression();
  virtual void genCode(ScopeStack *, iostream &);
};

class LeftHandSide : public PTNode {
 public:
  LeftHandSide(PTNode * p);
  ~LeftHandSide();
  PTNode * name();
  PTNode * fieldAccess();
  virtual void genCode(ScopeStack *, iostream &);
};

class MethodInvocation : public PTNode {
 public:
  MethodInvocation(PTNode * p1, PTNode *p2);
    MethodInvocation(PTNode *p1, PTNode *p2, PTNode *p3);
  MethodInvocation(PTNode *p);
  ~MethodInvocation();
  PTNode * primary();
  PTNode * name();
  PTNode * super();
  PTNode * id();
  PTNode * argumentList();
  virtual void genCode(ScopeStack *, iostream &);
};

class ClassInstanceCreationExpression : public PTNode{
 public:
  ClassInstanceCreationExpression(PTNode *t, PTNode *al);
  ClassInstanceCreationExpression(PTNode *t);
  ~ClassInstanceCreationExpression();
  PTNode * referenceType();
  PTNode * argumentList();
  virtual void genCode(ScopeStack *, iostream &);
};

class ArgumentList : public PTNode {
 public:
  ArgumentList();
  ArgumentList(PTNode *al, PTNode *e);
  ArgumentList(PTNode *e);
  ~ArgumentList();
  PTNode * expression();
  PTNode * argumentList();
  virtual void genCode(ScopeStack *, iostream &);
};

class Expression : public PTNode {
 public:
  Expression();
  Expression(MODEL_T t);
  Expression(PTNode *a, const char *op, PTNode *b);
  Expression(const char *op, PTNode * a);
  Expression(PTNode *a);
  ~Expression(){}
  PTNode * OP();
  Expression * E1();
  Expression * E2();
  virtual void genCode(ScopeStack *, iostream &);
  void genCode(ScopeStack *, iostream &,
	       const char * trueLabel,
	       const char * falseLabel);
};

class PrimitiveExpression : public Expression {
 public:
  PrimitiveExpression(PTNode *p);
  ~PrimitiveExpression();
  PTNode * primary();
  PTNode * name();
  virtual void genCode(ScopeStack *, iostream &);
};

class Primary : public PTNode {
 public:
  Primary(PTNode *p);
  ~Primary();
  PTNode * literal();
  PTNode * THIS();
  PTNode * classInstanceCreationExpression();
  PTNode * fieldAccess();
  PTNode * methodInvocation();
  virtual void genCode(ScopeStack *, iostream &);
};


class Literal : public PTNode {
 public:
  Literal(PTNode * p);
  ~Literal();
  virtual void genCode(ScopeStack *, iostream &);
};


class FieldAccess : public PTNode {
 public:
  FieldAccess(PTNode *n, PTNode *l);
  ~FieldAccess();
  PTNode * primary();
  PTNode * SUPER();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};


class Type : public PTNode {
 public:
  Type(PTNode * pt);
  Type(MODEL_T t);
  PTNode * primitiveType();
  PTNode * referenceType();
  virtual void genCode(ScopeStack *, iostream &);
};


class PrimitiveType : public Type {
 public:
  PrimitiveType(PTNode * p);
  ~PrimitiveType();
  virtual void genCode(ScopeStack *, iostream &);
};

class ReferenceType : public Type {
 public:
  ReferenceType(PTNode *sn);
  ~ReferenceType();
  PTNode * simpleName();
  virtual void genCode(ScopeStack *, iostream &);
};

class Name : public PTNode {
 public:
  Name(PTNode * p);
  Name(MODEL_T t);
  PTNode * qualifiedName();
  PTNode * simpleName();
  virtual void genCode(ScopeStack *, iostream &);
};


class SimpleName : public Name {
 public:
  SimpleName(PTNode *p);
  ~SimpleName();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};

class QualifiedName : public Name {
 public:
  QualifiedName(PTNode *n, PTNode *id);
  ~QualifiedName();
  PTNode * name();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};

class MethodDeclarator : public PTNode {
 public:
  MethodDeclarator(PTNode *id, PTNode * fpl);
  MethodDeclarator(PTNode *id);
  ~MethodDeclarator();
  PTNode * formalParameterList();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};

class FormalParameterList : public PTNode {
 public:
  FormalParameterList(PTNode * fp);
  FormalParameterList(PTNode * fpl, PTNode *fp);
  ~FormalParameterList();
  PTNode * formalParameter();
  PTNode * formalParameterList();
  virtual void genCode(ScopeStack *, iostream &);
};

class FormalParameter : public PTNode {
 public:
  FormalParameter(PTNode * type, PTNode *id);
  ~FormalParameter();
  PTNode * type();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};

class VariableDeclarators : public PTNode {
 public:
  VariableDeclarators(PTNode *vd);
  VariableDeclarators(PTNode *vds, PTNode *vd);
  ~VariableDeclarators();
  PTNode * variableDeclarators();
  PTNode * variableDeclarator();
  virtual void genCode(ScopeStack *, iostream &);
};

class VariableDeclarator : public PTNode {
 public:
  VariableDeclarator(PTNode * id);
  ~VariableDeclarator();
  PTNode * ID();
  virtual void genCode(ScopeStack *, iostream &);
};

#endif
