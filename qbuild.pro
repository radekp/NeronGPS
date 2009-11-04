TEMPLATE  = app
TARGET    = NeronGPS
CONFIG   += qtopia
QTOPIA   *= whereabouts

FORMS=ui/clockform.ui ui/cacheform.ui ui/traceform.ui ui/journeyform.ui ui/logform.ui ui/serverform.ui ui/magnificateform.ui ui/zoomform.ui

HEADERS=include/appli.h include/tracerecorder.h include/power.h include/directory.h include/trace.h include/tracesegment.h include/traceloader.h include/converter.h include/tilel1cache.h include/tile.h include/mapwidget.h include/tilemap.h include/tracepainter.h include/button.h include/buttonsboard.h include/tilehttp.h include/tilehttpwheel.h include/tilehttpmux.h include/tilel3cache.h include/tilel3cachemux.h include/tilecompressed.h include/tilel2cache.h include/tilebatchloader.h include/clockform.h include/cacheform.h include/traceform.h include/journeyform.h include/logform.h include/serverform.h include/magnificateform.h include/zoomform.h include/gpsstate.h include/gpsdata.h include/gpsclock.h include/traceserver.h include/drawstate.h include/locationpointer.h include/messageboard.h include/messagefeeder.h include/settings.h include/tileserver.h include/tileserverdirect.h include/actionsmanager.h include/painter.h include/painterelement.h include/tileref.h include/gpsstatistics.h include/tileclient.h include/tiletransaction.h include/log.h include/logfeeder.h include/tilehttpname.h include/tileasyncloader.h include/tilehttptrans.h include/tilecomposer.h include/tilecompotrans.h include/tilecontainer.h include/global.h include/textfeeder.h

SOURCES=main.cpp appli.cpp tracerecorder.cpp power.cpp directory.cpp trace.cpp tracesegment.cpp traceloader.cpp converter.cpp tilel1cache.cpp tile.cpp mapwidget.cpp tilemap.cpp tracepainter.cpp button.cpp buttonsboard.cpp tilehttp.cpp tilehttpwheel.cpp tilehttpmux.cpp tilel3cache.cpp tilel3cachemux.cpp tilecompressed.cpp tilel2cache.cpp tilebatchloader.cpp clockform.cpp cacheform.cpp traceform.cpp journeyform.cpp logform.cpp serverform.cpp magnificateform.cpp zoomform.cpp gpsstate.cpp gpsdata.cpp gpsclock.cpp traceserver.cpp drawstate.cpp locationpointer.cpp messageboard.cpp messagefeeder.cpp settings.cpp tileserver.cpp tileserverdirect.cpp actionsmanager.cpp painter.cpp painterelement.cpp tileref.cpp gpsstatistics.cpp tileclient.cpp tiletransaction.cpp log.cpp logfeeder.cpp tilehttpname.cpp tileasyncloader.cpp tilehttptrans.cpp tilecomposer.cpp tilecompotrans.cpp tilecontainer.cpp global.cpp textfeeder.cpp

LIBS=-lpthread -lnsl -lm

desktop.files=NeronGPS.desktop
desktop.path=/apps/Applications
desktop.hint=desktop

pics.files=pics/*
pics.path=/pics/NeronGPS
pics.hint=pics

INSTALLS+=desktop pics

pkg.name=NeronGPS
pkg.desc=GPS Application
pkg.version=0.5.0-1
pkg.maintainer=Thierry Vuillaume
pkg.license=GPL
pkg.domain=window
AVAILABLE_LANGUAGES=en_US
LANGUAGES=$$AVAILABLE_LANGUAGES

