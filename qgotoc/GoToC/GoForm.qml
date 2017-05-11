import QtQuick 2.0
import GoToC 1.0

QtObject {

    id: _blank

    property alias key: _form.key

    signal notify(string name)

    function value(name) {
        return _form.value(name)
    }

    function invoke(name, args) {
        _form.invoke(name, args)
    }

    function changed(name, args) {
        _form.changed(name, args)
    }

    function clicked(name, args) {
        _form.clicked(name, args)
    }

    function bind(notifyName, func) {
        _binds[notifyName] = func
    }

    property QtObject __form: QGoForm {
        id: _form

        onNotify: {
            console.info("onNotify: " + name)
            _invokeBind(name)
            _blank.notify(name)
        }

    }

    property var _binds: ({})

    function _invokeBind(name) {
        var fn = _binds[name]
        if (fn !== undefined) {
            fn()
        }
    }
}


