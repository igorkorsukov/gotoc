package gotoc

type ValueFn func() interface{}

type BaseForm struct {
	Notifer FormNotifer

	Values map[string]ValueFn
}

func NewBaseForm(n FormNotifer) *BaseForm {
	f := &BaseForm{Notifer: n}
	f.Values = make(map[string]ValueFn)
	return f
}

func (f *BaseForm) Notify(name string) {
	f.Notifer.Notify(name)
}

func (f *BaseForm) RowChanged(model string, index int) {
	f.Notifer.RowChanged(model, index)
}

func (f *BaseForm) RegValue(name string, fn ValueFn) {
	f.Values[name] = fn
}

func (f *BaseForm) Value(name string) interface{} {
	if fn, ok := f.Values[name]; ok {
		return fn()
	}
	return nil
}

func (f *BaseForm) Invoke(name string, args []string) interface{} {
	return nil
}

func (f *BaseForm) Changed(name string, args []string) {

}

func (f *BaseForm) Clicked(name string, args []string) {

}

func (f *BaseForm) RowCount(model string) int {
	return 0
}
func (f *BaseForm) RowData(model string, index int) interface{} {
	return nil
}
