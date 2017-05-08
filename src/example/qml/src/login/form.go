package login

import (
	"fmt"
	"gotoc"
)

type FormCreator struct {
}

func (c FormCreator) NewForm(n gotoc.FormNotifer) gotoc.Form {
	f := &Form{BaseForm: gotoc.NewBaseForm(n)}
	f.Login = Item{Title: "Login", Value: ""}
	f.Pass = Item{Title: "Pass", Value: ""}

	f.BaseForm.RegValue("login", func() interface{} { return f.Login })
	f.BaseForm.RegValue("pass", func() interface{} { return f.Pass })

	return f
}

type Item struct {
	Title string `json:"title"`
	Value string `json:"value"`
}

type Form struct {
	*gotoc.BaseForm

	Login Item
	Pass  Item

	Sign string
}

func (f *Form) SetSign(val string) {
	if f.Sign != val {
		f.Sign = val
		f.Notify(fmt.Sprint("sign_", val))
	}
}

func (f *Form) Value(name string) interface{} {

	fmt.Println("Login form Value ", name)

	//! NOTE Значения можно возращать тут используя свитч или через регистрацию функций в базовой форме
	switch name {
	case "title":
		return "LoginForm ( login == pass - success)"
	}
	return f.BaseForm.Value(name)
}

func (f *Form) Changed(name string, args []string) {

	fmt.Println("go: Changed name: ", name, ", args: ", args)
	switch name {
	case "login":
		f.Login.Value = args[0]
	case "pass":
		f.Pass.Value = args[0]
	}

	f.SetSign("reset")
}

func (f *Form) Clicked(name string, args []string) {

	fmt.Println("go: Clicked name: ", name)
	switch name {
	case "sign":
		if f.Login.Value == f.Pass.Value {
			f.SetSign("success")
		} else {
			f.SetSign("failed")
		}
	}
}
