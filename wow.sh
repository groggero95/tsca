#!/bin/bash

for f in $(cat files)
	do
	file=${f%.*}
	ext=${f##*.}
	echo $ext $file
	if [ $ext = "py" ]
	then
		cat ./preamble/py_preamble.txt $f > tmp && mv tmp $f && echo "$f appended"
	elif [ $ext = "c" ]
	then
		cat ./preamble/c_preamble.txt $f > tmp && mv tmp $f && echo "$f appended"
	elif [ $ext = "h" ]
	then
		cat ./preamble/c_preamble.txt $f > tmp && mv tmp $f && echo "$f appended"
	elif [ $ext = "tex" ]
	then
		cat ./preamble/tex_preamble.txt $f > tmp && mv tmp $f && echo "$f apppended"
	elif echo $f | grep -q "Makefile"
	then
		cat ./preamble/py_preamble.txt $f > tmp && mv tmp $f && echo "$f appended"
	fi;
done
