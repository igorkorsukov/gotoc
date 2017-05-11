import QtQuick 2.0
import GoToC 1.0

QGoListModel {

    property QtObject goform

    form: goform ? goform.__form : null
}
