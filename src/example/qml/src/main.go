package main

import "C"

import (
	"fmt"
	"gotoc"
	"login"
	"points"
	"unsafe"
)

//export GoToC_Init
func GoToC_Init(c_call unsafe.Pointer) {

	fmt.Println("go: GoToC_Init c_call: ", c_call)
	gotoc.NewGoToC(c_call)

	lf := login.FormCreator{}
	gotoc.RegForm(201, lf)

	pf := points.FormCreator{}
	gotoc.RegForm(202, pf)
}

//export GoToC_Call
func GoToC_Call(fi int, in_p unsafe.Pointer, in_size C.int) (unsafe.Pointer, int) {
	return gotoc.Go_Call(fi, in_p, int(in_size))
}

func main() {}

//go build -o gotoc.a -buildmode=c-archive src/main.go
