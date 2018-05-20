#!/usr/bin/env bash
echo "[i] Cleaning the mess"
rm graph.* bf-result.*
echo "[i] Compiling"
make &&
echo "[i] Running"
./build/bf &&
echo "[i] Killing Unnecessary processes"
pkill gwenview
echo "[i] Building graph images"
neato -Tpng graph.dot -o graph.png && xdg-open  graph.png
neato -Tpng shortest-path.dot -o shortest-path.png && xdg-open  shortest-path.png

