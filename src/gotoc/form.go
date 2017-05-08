package gotoc

import (
	"encoding/json"
	"fmt"
	"strconv"
)

type FormNotifer interface {
	Notify(name string)
	RowChanged(index int)
}

type FormCreator interface {
	NewForm(n FormNotifer) Form
}

type Form interface {
	Value(name string) interface{}
	Invoke(name string, args []string) interface{}
	Changed(name string, args []string)
	Clicked(name string, args []string)

	//List
	RowCount() int
	RowData(index int) interface{}
}

type formWrap struct {
	Key     int
	Creator FormCreator
	Form    Form
	CCaller CCaller
}

func (f *formWrap) GoCall(in []byte) []byte {
	fmt.Println("gotoc Form GoCall in: ", string(in))

	rpc, err := rpcUnmarshal(in)
	if err != nil {
		fmt.Println("GoCall Unmarshal err: ", err.Error())
		return nil
	}

	//! TODO Проверить ошибки и условия

	var val interface{}
	switch rpc.Method {
	case "new":
		f.Form = f.Creator.NewForm(f)
	case "destroy":
		f.Form = nil
	case "value":
		val = f.Form.Value(rpc.Params[0])
	case "invoke":
		val = f.Form.Invoke(rpc.Params[0], rpc.Params[1:])
	case "changed":
		f.Form.Changed(rpc.Params[0], rpc.Params[1:])
	case "clicked":
		f.Form.Clicked(rpc.Params[0], rpc.Params[1:])
	case "rowcount":
		val = f.Form.RowCount()
	case "rowdata":
		i, _ := strconv.Atoi(rpc.Params[0])
		val = f.Form.RowData(i)
	}

	var b []byte
	if val != nil {
		b, err = json.Marshal(val)
		if err != nil {
			fmt.Println("formwrap: marshal err: ", err.Error(), ", val: ", val)
		}
	}
	return b
}

func (f *formWrap) Notify(name string) {
	b, _ := rpcMarshal("notify", []string{name})
	f.CCaller.C_Call(f.Key, b)
}

func (f *formWrap) RowChanged(index int) {
	b, _ := rpcMarshal("rowchanged", []string{strconv.Itoa(index)})
	f.CCaller.C_Call(f.Key, b)
}
