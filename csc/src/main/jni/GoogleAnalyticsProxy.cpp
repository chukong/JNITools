
#include "jni.h"
#include "GoogleAnalyticsProxy.h"
#include "NativeBridge.h"
#include "JNIUtils.h"
#include <string>
#include <memory>

namespace sdkbox {

typedef const std::string cstr;


void GoogleAnalyticsCallback::onEvent1( cstr& event, jobject params ) {
    LOGD("GoogleAnalytics event1");
}

void GoogleAnalyticsCallback::onEvent2( cstr& event, jobject params ) {
    LOGD("GoogleAnalytics event2");
}

GoogleAnalyticsProxy::GoogleAnalyticsProxy() : ServiceProxy("org/cocos2dx/services/googleanalytics/GoogleAnalyticsService") {

    _callbackObj= new GoogleAnalyticsCallback();

    _callbackEvent1= NativeBridge<GoogleAnalyticsCallback>::AddEventListener(
        "GoogleAnalytics.event1",
        _callbackObj,
        &GoogleAnalyticsCallback::onEvent1);

    _callbackEvent2= NativeBridge<GoogleAnalyticsCallback>::AddEventListener(
        "GoogleAnalytics.event2",
        _callbackObj,
        &GoogleAnalyticsCallback::onEvent2);
}

GoogleAnalyticsProxy::~GoogleAnalyticsProxy() {
    NativeBridge<GoogleAnalyticsCallback>::RemoveEventListener("GoogleAnalytics.event1",_callbackEvent1.get());
    NativeBridge<GoogleAnalyticsCallback>::RemoveEventListener("GoogleAnalytics.event2",_callbackEvent2.get());
    delete _callbackObj;
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
    invoke( "logScreen", title );
}

void GoogleAnalyticsProxy::logEvent( cstr& eventCategory, cstr& eventAction, cstr& eventLabel, long value) const {
    invoke(
        "logEvent",
        JNIArray().
            addString(eventCategory).
            addString(eventAction).
            addString(eventLabel).
            addLong(value) );
}

void GoogleAnalyticsProxy::logException( cstr& exceptionDescription, bool isFatal) const {
    invoke(
        "logException",
        JNIArray().
            addString(exceptionDescription).
            addBoolean(isFatal) );
}

void GoogleAnalyticsProxy::logTiming( cstr& timingCategory, long timingInterval, cstr& timingName, cstr& timingLabel) const {
    invoke(
        "logTiming",
        JNIArray().
            addString(timingCategory).
            addLong(timingInterval).
            addString(timingName).
            addString(timingLabel) );

}

void GoogleAnalyticsProxy::logSocial( cstr& socialNetwork, cstr&socialAction, cstr& socialTarget) const {
    invoke(
        "logSocial",
        JNIArray().
            addString(socialNetwork).
            addString(socialAction).
            addString(socialTarget) );
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

int GoogleAnalyticsProxy::getInt() const {
    return invokeInt( "getInt" );
}
long GoogleAnalyticsProxy::getLong() const {
    return invokeLong( "getLong" );
}
string GoogleAnalyticsProxy::getString() const {
    return invokeString( "getString" );
}
bool GoogleAnalyticsProxy::getBoolean() const {
    return invokeBoolean( "getBoolean" );
}

}