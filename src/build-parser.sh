#!/bin/bash

echo "Building Lexer..."
flex -nounistd -R -o config-lexer.c --prefix=config_  --header-file=config-lexer.h config-lexer.l
echo "Building Parser..."
bison -t -d -v -o config-parser.c --name-prefix=config_ config-parser.y
