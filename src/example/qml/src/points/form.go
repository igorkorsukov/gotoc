package points

import (
	"fmt"
	"gotoc"
	"strconv"
)

type FormCreator struct {
}

func (c FormCreator) NewForm(n gotoc.FormNotifer) gotoc.Form {
	f := &Form{BaseForm: gotoc.NewBaseForm(n)}
	f.Points = make([]Point, 700)
	for i := 0; i < 700; i++ {
		f.Points[i] = Point{Title: fmt.Sprint("Point ", i), Subtitle: "click me"}
	}
	return f
}

type Point struct {
	Title    string `json:"title"`
	Subtitle string `json:"subtitle"`
	Counter  int
}

type Form struct {
	*gotoc.BaseForm

	Points []Point
}

func (f *Form) Value(name string) interface{} {

	switch name {
	case "title":
		return "PointForm"
	}
	return nil
}

func (f *Form) Clicked(name string, args []string) {
	fmt.Println("go: Clicked name: ", name, ", args: ", args)

	switch name {
	case "item":
		i, _ := strconv.Atoi(args[0])
		p := f.Points[i]
		p.Counter++
		p.Subtitle = fmt.Sprint("point clicked: ", p.Counter)
		f.Points[i] = p
		f.RowChanged(i)
	}
}

func (f *Form) RowCount() int {
	return len(f.Points)
}

func (f *Form) RowData(index int) interface{} {
	return f.Points[index]
}
