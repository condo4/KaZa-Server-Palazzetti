import QtQml 2.0
import org.kazoe.palazzetti 1.0
import org.kazoe.kaza 1.0

QtObject {
    id: root
    property string name: "palazzetti"

    property Palazzetti palazzetti: Palazzetti {}
    property alias hostname: root.palazzetti.hostname
    property alias refreshRate: root.palazzetti.refreshRate

    property KzObject setPoint: KzObject {
        name: root.name + ".setPoint"
        onValueChanged: {
            palazzetti.setPoint(value)
        }
    }

    property KzObject setPower: KzObject {
        name: root.name + ".setPower"
        onValueChanged: {
            palazzetti.setPower(value)
        }
    }

    property KzObject setFan: KzObject {
        name: root.name + ".setFan"
        onValueChanged: {
            palazzetti.setFan(value)
        }
    }

    property KzObject currentSetPoint: KzObject {
        name: root.name + ".currentSetPoint"
        value: palazzetti.currentSetPoint
    }

    property KzObject feeder: KzObject {
        name: root.name + ".feeder"
        value: palazzetti.feeder
    }

    property KzObject globalCounter: KzObject {
        name: root.name + ".globalCounter"
        value: palazzetti.globalCounter
    }

    property KzObject lastempty: KzObject {
        name: root.name + ".lastempty"
        value: palazzetti.lastempty
    }

    property KzObject temperatureIn: KzObject {
        name: root.name + ".temperatureIn"
        value: palazzetti.temperatureIn
    }

    property KzObject temperatureOut: KzObject {
        name: root.name + ".temperatureOut"
        value: palazzetti.temperatureOut
    }

    property KzObject temperatureExhaust: KzObject {
        name: root.name + ".temperatureExhaust"
        value: palazzetti.temperatureExhaust
    }

    property KzObject state: KzObject {
        name: root.name + ".state"
        value: palazzetti.state
    }

    property KzObject alarm: KzObject {
        name: root.name + ".alarm"
        value: palazzetti.alarm
    }

    property KzObject power: KzObject {
        name: root.name + ".power"
        value: palazzetti.power
    }

    property KzObject roomfan: KzObject {
        name: root.name + ".roomfan"
        value: palazzetti.roomfan
    }

}
