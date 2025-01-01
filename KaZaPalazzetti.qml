import QtQml 2.0
import org.kazoe.palazzetti 1.0
import org.kazoe.kaza 1.0

QtObject {
    id: root
    property string name: "palazzetti"

    property Palazzetti palazzetti: Palazzetti {}
    property alias hostname: root.palazzetti.hostname
    property alias refreshRate: root.palazzetti.refreshRate

    property KaZaObject setPoint: KaZaObject {
        name: root.name + ".setPoint"
        onValueChanged: {
            palazzetti.setPoint(value)
        }
    }

    property KaZaObject setPower: KaZaObject {
        name: root.name + ".setPower"
        onValueChanged: {
            palazzetti.setPower(value)
        }
    }

    property KaZaObject setFan: KaZaObject {
        name: root.name + ".setFan"
        onValueChanged: {
            palazzetti.setFan(value)
        }
    }

    property KaZaObject currentSetPoint: KaZaObject {
        name: root.name + ".currentSetPoint"
        value: palazzetti.currentSetPoint
    }

    property KaZaObject feeder: KaZaObject {
        name: root.name + ".feeder"
        value: palazzetti.feeder
    }

    property KaZaObject globalCounter: KaZaObject {
        name: root.name + ".globalCounter"
        value: palazzetti.globalCounter
    }

    property KaZaObject lastempty: KaZaObject {
        name: root.name + ".lastempty"
        value: palazzetti.lastempty
    }

    property KaZaObject temperatureIn: KaZaObject {
        name: root.name + ".temperatureIn"
        value: palazzetti.temperatureIn
    }

    property KaZaObject temperatureOut: KaZaObject {
        name: root.name + ".temperatureOut"
        value: palazzetti.temperatureOut
    }

    property KaZaObject temperatureExhaust: KaZaObject {
        name: root.name + ".temperatureExhaust"
        value: palazzetti.temperatureExhaust
    }

    property KaZaObject state: KaZaObject {
        name: root.name + ".state"
        value: palazzetti.state
    }

    property KaZaObject alarm: KaZaObject {
        name: root.name + ".alarm"
        value: palazzetti.alarm
    }

    property KaZaObject power: KaZaObject {
        name: root.name + ".power"
        value: palazzetti.power
    }

    property KaZaObject roomfan: KaZaObject {
        name: root.name + ".roomfan"
        value: palazzetti.roomfan
    }

}
