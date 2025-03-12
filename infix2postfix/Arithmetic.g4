grammar Arithmetic;

expression:   NUM # Num
						| expression op=('*' | '/') expression # Operation
						| expression op=('+' | '-') expression # Operation
						| '(' expression ')' # Parentheses
            ;

NUM: [0-9]+ ;

WS:   [ \t\n\r]+ -> skip ;
SL_COMMENT:   '//' .*? '\n' -> skip ;
