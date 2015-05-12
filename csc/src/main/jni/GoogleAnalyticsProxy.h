#ifndef _GOOGLEANALYTICS_PROXY_H
#define _GOOGLEANALYTICS_PROXY_H

#include "jni.h"
#include "Proxy.h"
#include <string>
#include <memory>

namespace sdkbox {

    class GoogleAnalyticsProxy;

    typedef std::shared_ptr<GoogleAnalyticsProxy>        SPGoogleAnalyticsProxy;
    typedef std::shared_ptr<const GoogleAnalyticsProxy>  CSPGoogleAnalyticsProxy;


    class GoogleAnalyticsProxy : public ServiceProxy {

    private:
        typedef const std::string cstr;


    public:

        GoogleAnalyticsProxy( );
        virtual ~GoogleAnalyticsProxy();

        void dispatchHits() const;
        void dispatchPeriodically(      int numberOfSeconds ) const;
        void stopPeriodicalDispatch() const;
        void startSession() const;
        void stopSession() const;
        void logScreen(                 cstr& title) const;
        void logEvent(                  cstr& eventCategory, cstr eventAction, cstr eventLabel, long value) const;
        void logException(              cstr& exceptionDescription, bool isFatal) const;
        void logTiming(                 cstr& timingCategory, long timingInterval, cstr& timingName, cstr& timingLabel) const;
        void logSocial(                 cstr& socialNetwork, cstr&socialAction, cstr& socialTarget) const;
        void setDryRun(                 bool dr ) const;
        void enableAdvertisingTracking( bool e ) const;
        void createTracker(             cstr& trackerId ) const;
        void enableTracker(             cstr& trackerId ) const;

    };

}

#endif