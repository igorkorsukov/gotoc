package gotoc

import "fmt"

func RegFunc(fi int, fn GoFunc) error {
	err := _g_gotoc.RegCall(fi, &fnWrap{Fn: fn})
	if err != nil {
		fmt.Println("gotoc: regfunc fi: ", fi, ", err: ", err.Error())
	}
	return err
}

func RegCall(fi int, c GoCall) error {
	return _g_gotoc.RegCall(fi, c)
}

func RegForm(fi int, c FormCreator) error {
	h := &formWrap{Key: fi, Creator: c, CCaller: _g_gotoc}
	return _g_gotoc.RegCall(fi, h)
}
