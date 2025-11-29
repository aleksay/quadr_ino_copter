#!/bin/sh -x


./tools/tools_mk_include_graph_Version2.py

echo $PWD

dot -Tpdf ./tools/include.dot -o ./includes.pdf

