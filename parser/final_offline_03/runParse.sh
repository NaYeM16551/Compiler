#!/bin/bash

yacc -d -y parser.y
echo 'Generated the parser C file as well the header file'
g++ -w -c -o y.o y.tab.c
echo 'Generated the parser object file'
flex 2005078.l
echo 'Generated the scanner C file'
g++ -w -c -o l.o lex.yy.c
# if the above command doesn't work try g++ -fpermissive -w -c -o l.o lex.yy.c
echo 'Generated the scanner object file'

# g++ -w -c -o utilities.o utilities.cpp
# echo 'Generated the utlities object file'
# g++ y.o l.o utilities.o -lfl -o out

g++ y.o l.o -lfl symbol_table/2005078_ScopeTable.cpp symbol_table/2005078_symbol_info.cpp symbol_table/2005078_symbol_table.cpp symbol_table/ParseTreeNode.cpp -o out
echo 'All ready, running'
