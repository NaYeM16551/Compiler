./main.out input.txt #!/bin/bash

# Run flex to generate lex.yy.c from the lexer file
flex 2005078.l

# Check if flex command was successful
if [ $? -eq 0 ]; then
    echo "Flex command successful. Generating lexer file."
    
    # Compile the C++ files
    g++ lex.yy.c symbol_table/2005078_ScopeTable.cpp symbol_table/2005078_symbol_info.cpp symbol_table/2005078_symbol_table.cpp -o main.out

    # Check if compilation was successful
    if [ $? -eq 0 ]; then
        ./main.out input.txt 
    else
        echo "Compilation failed."
    fi
else
    echo "Flex command failed."
fi
