
#include "jni.h"
#include "GoogleAnalyticsProxy.h"
#include "JNIUtils.h"
#include <string>
#include <memory>

namespace sdkbox {

typedef const std::string cstr;

GoogleAnalyticsProxy::GoogleAnalyticsProxy() : ServiceProxy("org/cocos2dx/services/googleanalytics/GoogleAnalyticsService") {
}

GoogleAnalyticsProxy::~GoogleAnalyticsProxy() {
}

void GoogleAnalyticsProxy::dispatchHits() const {
    invoke( "dispatchHits" );
}

void GoogleAnalyticsProxy::dispatchPeriodically( int numberOfSeconds ) const {
    invoke( "dispatchPeriodically", numberOfSeconds );
}

void GoogleAnalyticsProxy::stopPeriodicalDispatch() const {
    invoke( "stopPeriodicalDispatch" );
}

void GoogleAnalyticsProxy::startSession() const {
    invoke( "startSession" );
}

void GoogleAnalyticsProxy::stopSession() const {
    invoke( "stopSession" );
}

void GoogleAnalyticsProxy::logScreen( cstr& title) const {
LOGD(" ----logScreen");
    invoke( "logScreen", title );
}

void GoogleAnalyticsProxy::logEvent( cstr& eventCategory, cstr eventAction, cstr eventLabel, long value) const {
    invoke(
        "logEvent",
        JNIArray(NULL).
            addString(eventCategory).
            addString(eventAction).
            addString(eventLabel).
            addLong(value).
            get() );
}

void GoogleAnalyticsProxy::logException( cstr& exceptionDescription, bool isFatal) const {
    invoke(
        "logException",
        JNIArray(NULL).
            addString(exceptionDescription).
            addBoolean(isFatal).
            get() );
}

void GoogleAnalyticsProxy::logTiming( cstr& timingCategory, long timingInterval, cstr& timingName, cstr& timingLabel) const {
    invoke(
        "logTiming",
        JNIArray(NULL).
            addString(timingCategory).
            addLong(timingInterval).
            addString(timingName).
            addString(timingLabel).
            get() );

}

void GoogleAnalyticsProxy::logSocial( cstr& socialNetwork, cstr&socialAction, cstr& socialTarget) const {
    invoke(
        "logSocial",
        JNIArray(NULL).
            addString(socialNetwork).
            addString(socialAction).
            addString(socialTarget).
            get() );
}

void GoogleAnalyticsProxy::setDryRun( bool dr ) const {
    invoke( "setDryRun", dr );
}

void GoogleAnalyticsProxy::enableAdvertisingTracking( bool e ) const {
    invoke( "enableAdvertisingTracking", e );
}

void GoogleAnalyticsProxy::createTracker( cstr& trackerId ) const {
    invoke( "createTracker", trackerId );
}

void GoogleAnalyticsProxy::enableTracker( cstr& trackerId ) const {
    invoke( "enableTracker", trackerId );
}

}