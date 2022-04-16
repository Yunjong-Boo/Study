grammar Expr;

// parser rules
prog : (start=expr ';'? NEWLINE?)*;
expr : '(' expr ')'         # parensExpr
     | '(' op=('+'|'-') expr ')'	#unaryExpr
     | left=expr op=('*'|'/') right=expr  # infixExpr
     | left=expr op=('+'|'-') right=expr  # infixExpr
     | left=expr op='=' right=expr	    # Assign
     | op='min' '(' left=expr ',' right=expr ')'	# FuncExpr
     | op='max' '(' left=expr ',' right=expr ')'	# FuncExpr
     | op='pow' '(' left=expr ',' right=expr ')'	# FuncExpr
     | op='sqrt' '(' left=expr ')'	# FuncExpr
     | n=NUM          # Number
     | id=ID		#ID
     ;

// lexer rules
OP_ADD: '+' ;
OP_SUB: '-' ;
OP_MUL: '*' ;
OP_DIV: '/' ;
OP_ASG: '=' ;
OP_SQRT: 'sqrt' ;
OP_MIN: 'min' ;
OP_MAX: 'max' ;
OP_POW: 'pow' ;
NEWLINE: [\r\n]+ ;
NUM: [0-9]+ ('.'[0-9]+)? ;          // should handle negatives
ID: [a-zA-Z]+ ;
WS: [ \t\r\n]+ -> skip ;
