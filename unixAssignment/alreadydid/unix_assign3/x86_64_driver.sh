#!/bin/bash

USAGE="usage: $0 <filename>"
filename=$1

if [ ! -f "$filename" ]; then 
	echo $1" does not exists"
	exit 1
fi

new_file=$( echo $filename | cut -d'.' -f1 )
rm -f $new_file'.s'
echo ' 
.data
	format: .asciz  "%d\n"
.bss
	a:  .quad   0
	b:  .quad   0
	c:  .quad   0
	d:  .quad   0
	e:  .quad   0
	f:  .quad   0
	g:  .quad   0
	h:  .quad   0
	i:  .quad   0
	j:  .quad   0
	k:  .quad   0
	l:  .quad   0
	m:  .quad   0
	n:  .quad   0
	o:  .quad   0
	p:  .quad   0
	q:  .quad   0
	r:  .quad   0
	s:  .quad   0
	t:  .quad   0
	u:  .quad   0
	v:  .quad   0
	w:  .quad   0
	x:  .quad   0
	y:  .quad   0
	z:  .quad   0
.text
.globl main            


	
main:' >> $new_file'.s'


./lexyacc-code/calc3i.exe < "$filename"  >> $new_file'.s'
echo '	movq    $1,      %rdi   
        call    exit '	>>	$new_file'.s'

gcc -o -no -pie programs $new_file'.s' lib/functions.o


