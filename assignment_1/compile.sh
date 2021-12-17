#!/bin/zsh

clang++ -std=c++11 *.cpp -o bin/sfmlgame -I../SFML-2.5.1-macos-clang/include -L../SFML-2.5.1-macos-clang/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

