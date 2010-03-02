TEMPLATE  = app
TARGET    = NeronGPS
CONFIG   += qtopia
QTOPIA   *= whereabouts

FORMS=ui/clockform.ui ui/cacheform.ui ui/gpxform.ui ui/journeyform.ui ui/logform.ui ui/serverform.ui ui/magnificateform.ui ui/zoomform.ui ui/poiform.ui ui/keyboardform.ui

HEADERS=qtmoko/power.h qtmoko/gpssourceabout.h include/nerongps.h include/tracerecorder.h include/directory.h include/trace.h include/tracesegment.h include/gpxloader.h include/gpxbuilder.h include/converter.h include/tilel1cache.h include/tile.h include/mapwidget.h include/mapdrawlist.h include/tilemap.h include/tracepainter.h include/button.h include/buttonsboard.h include/tilehttp.h include/tilehttpwheel.h include/tilehttpmux.h include/tilel3cache.h include/tilel3cachemux.h include/tilecompressed.h include/tilel2cache.h include/tilebatchloader.h include/clockform.h include/cacheform.h include/gpxform.h include/journeyform.h include/logform.h include/serverform.h include/magnificateform.h include/zoomform.h include/poiform.h include/keyboardform.h include/keyboard.h include/gpssourcenmea.h include/gpssourcegpsd.h include/gpssample.h include/gpsstate.h include/gpsdata.h include/gpsclock.h include/drawstate.h include/mapcursor.h include/maptarget.h include/mapcross.h include/maptrailer.h include/maptraces.h include/messageboard.h include/messagefeeder.h include/settings.h include/tileserver.h include/tileserverdirect.h include/actionsmanager.h include/painter.h include/painterelement.h include/tileref.h include/gpsstatistics.h include/tileclient.h include/tiletransaction.h include/log.h include/logfeeder.h include/tilehttpname.h include/tileasyncloader.h include/tilehttptrans.h include/tilecomposer.h include/tilecompotrans.h include/tilecontainer.h include/global.h include/textfeeder.h include/poiserver.h include/mapcentering.h include/thread.h include/threadmanager.h include/nmeaparser.h

SOURCES=qtmoko/main.cpp qtmoko/platform.cpp qtmoko/power.cpp qtmoko/gpssourceabout.cpp src/nerongps.cpp src/tracerecorder.cpp src/directory.cpp src/trace.cpp src/tracesegment.cpp src/gpxloader.cpp src/gpxbuilder.cpp src/converter.cpp src/tilel1cache.cpp src/tile.cpp src/mapwidget.cpp src/mapdrawlist.cpp src/tilemap.cpp src/tracepainter.cpp src/button.cpp src/buttonsboard.cpp src/tilehttp.cpp src/tilehttpwheel.cpp src/tilehttpmux.cpp src/tilel3cache.cpp src/tilel3cachemux.cpp src/tilecompressed.cpp src/tilel2cache.cpp src/tilebatchloader.cpp src/clockform.cpp src/cacheform.cpp src/gpxform.cpp src/journeyform.cpp src/logform.cpp src/serverform.cpp src/magnificateform.cpp src/zoomform.cpp src/poiform.cpp src/keyboardform.cpp src/keyboard.cpp src/gpssourcenmea.cpp src/gpssourcegpsd.cpp src/gpssample.cpp src/gpsstate.cpp src/gpsdata.cpp src/gpsclock.cpp src/drawstate.cpp src/mapcursor.cpp src/maptarget.cpp src/mapcross.cpp src/maptrailer.cpp src/maptraces.cpp src/messageboard.cpp src/messagefeeder.cpp src/settings.cpp src/tileserver.cpp src/tileserverdirect.cpp src/actionsmanager.cpp src/painter.cpp src/painterelement.cpp src/tileref.cpp src/gpsstatistics.cpp src/tileclient.cpp src/tiletransaction.cpp src/log.cpp src/logfeeder.cpp src/tilehttpname.cpp src/tileasyncloader.cpp src/tilehttptrans.cpp src/tilecomposer.cpp src/tilecompotrans.cpp src/tilecontainer.cpp src/global.cpp src/textfeeder.cpp src/poiserver.cpp src/mapcentering.cpp src/thread.cpp src/threadmanager.cpp src/nmeaparser.cpp

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

