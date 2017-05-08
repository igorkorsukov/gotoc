package main

import "C"

import (
	"fmt"
	"gotoc"
	"login"
	"points"
	"service"
	"unsafe"
)

//export GoToC_Init
func GoToC_Init(c_call unsafe.Pointer) {

	fmt.Println("go: GoToC_Init c_call: ", c_call)
	gotoc.NewGoToC(c_call)

	s := service.Service{}

	gotoc.RegFunc(110, s.ItemsJSON)
	gotoc.RegFunc(111, s.ItemsEasyJSON)
	gotoc.RegFunc(112, s.ItemsArrRawJSON)
	gotoc.RegFunc(120, s.SaveItemsB)
	gotoc.RegFunc(140, CallEcho)

	lf := login.FormCreator{}
	gotoc.RegForm(201, lf)

	pf := points.FormCreator{}
	gotoc.RegForm(202, pf)
}

//export GoToC_Call
func GoToC_Call(fi int, in_p unsafe.Pointer, in_size C.int) (unsafe.Pointer, int) {
	return gotoc.Go_Call(fi, in_p, int(in_size))
}

func CallEcho([]byte) []byte {

	g := gotoc.GetGoToC()
	g.Echo()

	return nil
}

func main() {}

//go build -o gotoc.a -buildmode=c-archive src/main.go
