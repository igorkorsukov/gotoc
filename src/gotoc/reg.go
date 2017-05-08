package gotoc

func RegFunc(fi int, fn GoFunc) error {
	return _g_gotoc.RegCall(fi, &fnWrap{Fn: fn})
}

func RegCall(fi int, c GoCall) error {
	return _g_gotoc.RegCall(fi, c)
}

func RegForm(fi int, c FormCreator) error {
	h := &formWrap{Key: fi, Creator: c, CCaller: _g_gotoc}
	return _g_gotoc.RegCall(fi, h)
}
