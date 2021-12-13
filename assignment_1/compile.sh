#!/bin/zsh

clang++ *.cpp -o sfml.app -I../SFML-2.5.1-macos-clang/include -L../SFML-2.5.1-macos-clang/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

