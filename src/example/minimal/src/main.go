package main

import "C"

import (
	"encoding/json"
	"fmt"
	"gotoc"
	"strconv"
	"strings"
	"unsafe"
)

//export GoToC_Init
func GoToC_Init(c_call unsafe.Pointer) {

	fmt.Println("go: GoToC_Init c_call 42: ", c_call)
	gotoc.NewGoToC(c_call)

	gotoc.RegFunc(101, GoInt)
	gotoc.RegFunc(102, GoString)
	gotoc.RegFunc(103, GoSlice)
	gotoc.RegFunc(104, GoStruct)
	gotoc.RegFunc(200, CallC)
}

//export GoToC_Call
func GoToC_Call(fi int, in_p unsafe.Pointer, in_size C.int) (unsafe.Pointer, int) {
	return gotoc.Go_Call(fi, in_p, int(in_size))
}

func main() {}

//go build -o gotoc.a -buildmode=c-archive src/main.go

func GoInt(in []byte) []byte {
	i, _ := strconv.Atoi(string(in))
	return []byte(strconv.Itoa(i + 42))
}

func GoString(in []byte) []byte {
	return []byte(string(in) + " -> hello from go")
}

func GoSlice(in []byte) []byte {
	sl := []string{"bob", "tod", "alice", "anna"}
	return []byte(strings.Join(sl, ","))
}

type Person struct {
	ID   int    `json:"id"`
	Name string `json:"name"`
}

func GoStruct(in []byte) []byte {
	b, _ := json.Marshal(Person{ID: 12, Name: "Bob"})
	return b
}

func CallC(in []byte) []byte {

	fi, _ := strconv.Atoi(string(in))
	switch fi {
	case 201:
		out, _ := gotoc.C_Call(fi, []byte(strconv.Itoa(42)))
		fmt.Println("int from c: ", string(out))
	case 202:
		out, _ := gotoc.C_Call(fi, []byte("hello from go"))
		fmt.Println("string from c: ", string(out))
	case 203:
		out, _ := gotoc.C_Call(fi, nil)
		fmt.Println("slice from c: ", strings.Split(string(out), ","))
	case 204:
		out, _ := gotoc.C_Call(fi, nil)
		p := Person{}
		json.Unmarshal(out, &p)
		fmt.Println("struct from c: ", p)
	}

	gotoc.C_Call(fi, nil)
	return nil
}
