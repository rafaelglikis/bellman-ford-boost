#!/usr/bin/env bash
make &&
./build/bf &&
pkill gwenview
#neato -Tpng graph.txt -o graph.png
neato -Tpng bellman-eg.dot -o bellman-eg.png &&
xdg-open  bellman-eg.png
