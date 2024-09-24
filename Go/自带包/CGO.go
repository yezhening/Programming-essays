package main

/*
#include <stdio.h>

void print()
{
	printf("Hello World2\n");
}
*/
import "C"

func cPkg() {
	s := C.CString("Hello world")
	C.puts(s)

	C.print()
}

/*
输出：
Hello world
Hello world2
*/
