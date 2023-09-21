#!/bin/zsh

clang++ -std=c++17 *.cpp ../common/*.cpp -I/opt/homebrew/Cellar/sfml/2.6.0/include -L/opt/homebrew/Cellar/sfml/2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

