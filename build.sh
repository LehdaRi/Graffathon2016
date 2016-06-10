#!/bin/sh

[ ! -d bin ] && mkdir bin
g++ -lGL -lGLEW -lglfw -I include src/* -o bin/deom.exe
