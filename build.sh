#!/bin/sh

[ ! -d bin ] && mkdir bin
g++ -g -O0 -Wall -Wextra -pedantic -Wshadow -lGL -lGLEW -lsfml-system -lsfml-window -I include src/* -o bin/deom.exe

