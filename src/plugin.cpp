#include "plugin.h"

#include "palazzetti.h"

#include <qqml.h>

void PalazzettiPlugin::registerTypes(const char *uri)
{
    // @uri org.kazoe.palazzetti
    qmlRegisterType<Palazzetti>(uri, 1, 0, "Palazzetti");
}
