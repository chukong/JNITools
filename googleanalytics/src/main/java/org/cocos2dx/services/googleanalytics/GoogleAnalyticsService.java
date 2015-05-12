package org.cocos2dx.services.googleanalytics;

import android.util.Log;

import com.google.android.gms.analytics.GoogleAnalytics;
import com.google.android.gms.analytics.HitBuilders;
import com.google.android.gms.analytics.Tracker;

import org.cocos2dx.services.AbstractService;
import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

/**
 *
 * Created by ibon on 5/11/15.
 */
public class GoogleAnalyticsService extends AbstractService implements IGoogleAnalyticsService {

    public static final String PLUGIN_LOG_TAG="ServicesGoogleAnalytics";

    private static final String ERROR_NO_TRACKER_ID=    "No Tracker ID found in plugin config.";
    private static final String ERROR_TRACKER=          " called w/o valid tracker.";
    private static final String ERROR_START_SESSION=    "Start session" + ERROR_TRACKER;
    private static final String ERROR_END_SESSION=      "Start session" + ERROR_TRACKER;
    private static final String ERROR_LOG_SCREEN=       "Log Screen"    + ERROR_TRACKER;
    private static final String ERROR_LOG_EVENT=        "Log Event"     + ERROR_TRACKER;
    private static final String ERROR_LOG_EXCEPTION=    "Log Exception" + ERROR_TRACKER;
    private static final String ERROR_LOG_TIMING=       "Log Timing"    + ERROR_TRACKER;
    private static final String ERROR_LOG_SOCIAL=       "Log Social"    + ERROR_TRACKER;
    private static final String ERROR_LOG_ADVERTISING=  "Advertising"   + ERROR_TRACKER;

    private static final String DEFAULT_SCREEN_NAME=    "CocosServiceCenter";

    public static final String INIT_TRACKER_ID=         "tracker-id";

    private GoogleAnalytics     ga=         null;
    private Tracker             tracker=    null;
    private Map<String,Tracker> trackers=   null;

    public GoogleAnalyticsService( JSONObject createConfig ) {
        super();
    }

    public boolean initialize( JSONObject obj ) {

        trackers= new HashMap<String, Tracker>();
        ga= GoogleAnalytics.getInstance( getRegistry().getContext() );

        try {
            String trackerId = obj.getString( INIT_TRACKER_ID );

            if (null == trackerId) {
                Log.d(PLUGIN_LOG_TAG, "tracker id is not found at configure time.");
            } else {

                createTracker(trackerId);
                tracker.setScreenName(DEFAULT_SCREEN_NAME);
                trackers.put(trackerId, tracker);

                startSession();

                Log.d(PLUGIN_LOG_TAG, "Google analytics initialized.");
            }

        } catch(Exception x ) {

        }

        return true;
    }

    private String getString( String s ) {
        return s!=null ? s : "";
    }

    ////////////////////////////////////////////////////
    //// START IPluginGoogleAnalytics implementation ////
    ////////////////////////////////////////////////////

    @Override
    public void dispatchHits() {
        ga.dispatchLocalHits();
    }

    @Override
    public void dispatchPeriodically( int numberOfSeconds ) {
        ga.setLocalDispatchPeriod(numberOfSeconds);
    }

    @Override
    public void stopPeriodicalDispatch() {
        // negative numbers stop periodic dispatching.
        ga.setLocalDispatchPeriod(-1);
    }

    @Override
    public void startSession( ) {

        if ( tracker!=null ) {
            tracker.send(
                    new HitBuilders.ScreenViewBuilder()
                            .setNewSession()
                            .build() );
        } else {
            Log.e(PLUGIN_LOG_TAG, ERROR_START_SESSION);
        }
    }

    /**
     * Stop the current analytics session.
     * There should be no need to end a session explicitly.
     */
    @Override
    public void stopSession() {

        if ( tracker!=null ) {
            tracker.send(
                    new HitBuilders.ScreenViewBuilder()
                            .set( "&sc", "end" )
                            .build()
            );
        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_END_SESSION );
        }

    }

    @Override
    public void logScreen(String screenName) {

        if ( null!=tracker ) {
            tracker.setScreenName(screenName);
            tracker.send(new HitBuilders.ScreenViewBuilder().build());
        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_LOG_SCREEN );
        }
    }

    @Override
    public void logEvent(String eventCategory, String eventAction, String eventLabel, long value) {

        if ( null!=tracker ) {

            tracker.send( new HitBuilders.EventBuilder()
                    .setCategory(getString(eventCategory))
                    .setAction(getString(eventAction))
                    .setLabel(getString(eventLabel))
                    .setValue(value)
                    .build());
        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_LOG_EVENT );
        }
    }

    @Override
    public void logException(String exceptionDescription, boolean isFatal) {

        if ( null!=tracker ) {
            tracker.send(new HitBuilders.ExceptionBuilder()
                    .setDescription(exceptionDescription)
                    .setFatal(isFatal)
                    .build());

        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_LOG_EXCEPTION );
        }
    }

    @Override
    public void logTiming(String timingCategory, long timingInterval, String timingName, String timingLabel) {

        if ( null!=tracker ) {

            tracker.send(new HitBuilders.TimingBuilder()
                    .setCategory(timingCategory)
                    .setValue(timingInterval)
                    .setVariable(getString(timingName))
                    .setLabel(getString(timingLabel))
                    .build());
        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_LOG_TIMING );
        }
    }

    @Override
    public void logSocial(String socialNetwork, String socialAction, String socialTarget) {

        if ( tracker!=null ) {
            tracker.send(new HitBuilders.SocialBuilder()
                    .setNetwork(socialNetwork)
                    .setAction(socialAction)
                    .setTarget(socialTarget)
                    .build());
        } else {
            Log.e( PLUGIN_LOG_TAG, ERROR_LOG_SOCIAL );
        }

    }

    @Override
    public void setDryRun( boolean dr ) {
        ga.setDryRun( dr );
    }

    @Override
    public void enableAdvertisingTracking( boolean e ) {
        if ( null!=tracker ) {
            tracker.enableAdvertisingIdCollection(e);
        } else {
            Log.e(PLUGIN_LOG_TAG, ERROR_LOG_ADVERTISING);
        }
    }

    @Override
    public void createTracker( String trackerId ) {
        Tracker tr= trackers.get(trackerId);
        if ( null==tr ) {
            tr= ga.newTracker( trackerId );
            trackers.put(trackerId, tr);
        }

        enableTracker(trackerId);

    }

    @Override
    public void enableTracker( String trackerId ) {

        if ( null==trackerId ) {
            return;
        }

        Tracker tr= trackers.get(trackerId);
        if ( null==tr ) {
            Log.d( PLUGIN_LOG_TAG, "Trying to enable unknown tracker: "+trackerId );
        } else {
            Log.d(PLUGIN_LOG_TAG, "Selected tracker: " + trackerId);
            tracker = tr;
        }
    }

    public String getName() {
        return "GoogleAnalytics V4";
    }

}
