package gotoc

import "encoding/json"

type rpc struct {
	Method string   `json:"method"`
	Params []string `json:"params"`
}

func rpcUnmarshal(in []byte) (rpc, error) {
	r := rpc{}
	err := json.Unmarshal(in, &r)
	return r, err
}

func rpcMarshal(method string, params []string) ([]byte, error) {
	r := rpc{Method: method, Params: params}
	return json.Marshal(r)
}
