package gotoc

/*
#include <stdlib.h>
static inline void call_c_call(void* fp, int i, void* in_p, int in_size, void** out_p, int* out_size) {
 	void (*Fn)(int, void*, int, void**, int*);
	Fn = (void(*)(int, void*, int, void**, int*))(fp);
    (*Fn)(i, in_p, in_size, out_p, out_size);
}
*/
import "C"
import (
	"errors"
	"fmt"
	"strconv"
	"unsafe"
)

type GoFunc func([]byte) []byte
type GoCall interface {
	GoCall([]byte) []byte
}

type CCaller interface {
	C_Call(i int, in_b []byte) ([]byte, error)
}

var _g_gotoc *GoToC

type fnWrap struct {
	Fn GoFunc
}

func (w *fnWrap) GoCall(in []byte) []byte {
	return w.Fn(in)
}

type GoToC struct {
	calls  map[int]GoCall
	c_call unsafe.Pointer
	debug  bool
}

func NewGoToC(c_call unsafe.Pointer) *GoToC {
	_g_gotoc = &GoToC{}
	_g_gotoc.calls = make(map[int]GoCall)
	_g_gotoc.c_call = c_call

	_g_gotoc.calls[9] = &fnWrap{Fn: _g_gotoc.onDebug}
	_g_gotoc.calls[10] = &fnWrap{Fn: _g_gotoc.onEcho}
	_g_gotoc.calls[11] = &fnWrap{Fn: _g_gotoc.doEcho}
	_g_gotoc.calls[20] = &fnWrap{Fn: _g_gotoc.onBenchGo}

	return _g_gotoc
}

func GetGoToC() *GoToC {
	return _g_gotoc
}

func (g *GoToC) RegFunc(fi int, fn GoFunc) error {
	return g.RegCall(fi, &fnWrap{Fn: fn})
}

func (g *GoToC) RegCall(fi int, c GoCall) error {

	if fi < 100 {
		return fmt.Errorf("gotoc: fi < 100 are reserved, fi: %v", fi)
	}

	_, ok := g.calls[fi]
	if ok {
		return fmt.Errorf("gotoc: already registered fi: %v", fi)
	}
	g.calls[fi] = c
	return nil
}

func Go_Call(fi int, in_p unsafe.Pointer, in_size int) (unsafe.Pointer, int) {
	if _g_gotoc != nil {
		return _g_gotoc.Go_Call(fi, in_p, in_size)
	}
	return nil, 0
}

func (g *GoToC) Go_Call(fi int, in_p unsafe.Pointer, in_size int) (unsafe.Pointer, int) {

	call, ok := g.calls[fi]

	if ok {

		var in_b []byte
		if in_p != nil && in_size > 0 {
			in_b = C.GoBytes(in_p, C.int(in_size))
			C.free(in_p)
		}

		out_b := call.GoCall(in_b)

		if g.debug {
			fmt.Println("gotoc: go_Call fi: ", fi, ", in size: ", len(in_b), ", out_size: ", len(out_b))
		}

		if out_b != nil {
			p := C.CBytes(out_b)
			return p, len(out_b)
		}

	} else {
		if g.debug {
			fmt.Println("gotoc: go_Call not found fi: ", fi)
		}
	}
	return nil, 0
}

func (g *GoToC) C_Call(i int, in_b []byte) ([]byte, error) {

	if g.c_call == nil {
		return nil, errors.New("gotoc: not set c_call ptr")
	}

	p := C.CBytes(in_b)

	var out_p unsafe.Pointer
	var out_size C.int

	C.call_c_call(g.c_call, C.int(i), p, C.int(len(in_b)), &out_p, &out_size)

	if out_p != nil && out_size > 0 {
		out_b := C.GoBytes(out_p, out_size)
		C.free(out_p)
		return out_b, nil
	}
	return nil, nil
}

func (g *GoToC) Echo() {
	b, _ := g.C_Call(10, []byte("hello from go"))
	fmt.Println("gotoc: echo ret: ", string(b))
}

func (g GoToC) onEcho(in []byte) []byte {
	s := string(in)
	fmt.Println("gotoc: onEcho in: ", s)
	s += " -> echo go"
	return []byte(s)
}

func (g GoToC) doEcho(in []byte) []byte {
	g.Echo()
	return nil
}

func (g *GoToC) onDebug(in []byte) []byte {
	fmt.Println("gotoc: onDebug in: ", in)
	if in != nil {
		g.debug = in[0] == 1
	}
	fmt.Println("gotoc: debug: ", g.debug)
	return nil
}

func (g *GoToC) onBenchGo(in []byte) []byte {
	size, _ := strconv.Atoi(string(in))
	return make([]byte, size)
}
