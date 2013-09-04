#!/bin/bash

echo "Building Lexer..."
flex -nounistd -R -o src/config-lexer.c --prefix=config_  --header-file=src/config-lexer.h src/config-lexer.l
echo "Building Parser..."
bison -d -v -o src/config-parser.c --name-prefix=config_ src/config-parser.y
