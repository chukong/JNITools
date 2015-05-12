package org.cocos2dx.services.googleanalytics;


/**
 * Google Analytics plugin interface.
 * <p>
 * This plugin tracks information using Google Analytics as backend.
 * At initialization time, the plugin expects a TrackerId of the form XX-YYYYYYYY-Z to be read
 * from the csc_plugin.json configuration file.
 * <p>
 * If no trackerId is found, the plugin won't track any information.
 * The developer can define as many Trackers as needed. A common use case would be to track
 * application activity using a Tracker, and purchase information using another.
 * <p>
 * The developer can create Trackers at will by calling <code>createTracker(string trackerId)</code>.
 * The newly created tracker will be set as the default tracker, meaning that all tracking operations
 * will be eventually aggregated to that TrackerId.
 * You can switch to a new tracker by calling <code>enableTracker(string trackerId)</code>. If no
 * tracker exists with such TrackerId, nothing will happen, and the current Tracker won't be changed.
 * <p>
 * It is expected that each Tracker will be configured as a Mobile App in the Google Analytics
 * admin panel.
 * <p>
 * For testing purposes, the method <code>setDryRun(boolean)</code> can be called which will prevent
 * or allow sending tracking information to the Google Analytics bacend.
 *
 */
interface IGoogleAnalyticsService {


    /*****************************************************************************************
     * Dispatch Methods.
     *
     * Dispatches hits (view, events, etc.) to Google Analytics if a network connection is
     * available.
     */

    /**
     * Manually dispatch hits.
     * By default, data is dispatched from the Google Analytics SDK for Android every 5 minutes.
     */
    void dispatchHits();

    /**
     * Change the dispatch info time period to the desired amount of seconds.
     * @param numberOfSeconds seconds for periodic send.
     */
    void dispatchPeriodically( int numberOfSeconds );

    /**
     * Stop periodically sending info. Then manually the <code>dispatchPeridically</code> or
     * <code>dispatchHits</code> should be called.
     */
    void stopPeriodicalDispatch();

    /**************************************************************************************
     * Session control
     * A session represents a single period of user interaction with your game.
     * Sessions serve as useful containers of measured activity, which includes
     * screen views, events, and ecommerce transactions.
     */

    /**
     * The analytics session is being explictly started at plugin initialization time, but
     * can be started again in case you called <code>stopSession</code>.
     */
    void startSession();

    /**
     * You normally will never stop a session manually.
     */
    void stopSession();

    /**************************************************************************************
     * Screens
     * Screens in Google Analytics represent content users are viewing within your game.
     * A screen view consists of a single string field that will be used as the screen
     * name in your Google Analytics reports.
     */

    /**
     * Log screen info.
     * @param title the title of a screen. Screens are logical units inside your app you'd like
     *              to identify at analytics panel.
     */
    void logScreen(String title);

    /**************************************************************************************
     * Events
     * Events are a useful way to collect data about a user's interaction with interactive
     * components of your game, like the use of a particular item. An event consists of four
     * fields that you can use to describe a user's interaction with your game.
     */

    /**
     * googleAnalytics.LogEvent("Achievement", "Unlocked", "Slay 10 dragons", 5);
     *
     * @param eventCategory Category of events
     * @param eventAction   event action.
     * @param eventLabel    optional. default null.
     * @param value         optional. default 0.
     */
    void logEvent(String eventCategory,
                         String eventAction,
                         String eventLabel,
                         long value);

    /**************************************************************************************
     * Crashes and Exceptions
     * Crash and exception measurement allows you to measure the number and type of
     * caught and uncaught crashes and exceptions that occur in your game.
     */

    /**
     * Log an exception. It is a basic support for in-app events.
     *
     * @param exceptionDescription exception info to be collected.
     * @param isFatal description of exception fatality.
     */
    void logException(String exceptionDescription, boolean isFatal);


    /**************************************************************************************
     * User timing
     * Measuring user timings provides a native way to measure a period of time in
     * Google Analytics. For example, this can be useful to measure resource load times.
     *
     * e.g.: googleAnalytics.LogTiming("Loading", 50L, "Main Menu", "First Load");
     */

    /**
     * Measure a time inside the application.
     *
     * @param timingCategory
     * @param timingInterval measured value. could be seconds or nanos, whatever fits your needs.
     * @param timingName
     * @param timingLabel
     */
    void logTiming(String timingCategory,
                          long timingInterval,
                          String timingName,
                          String timingLabel);


    /**************************************************************************************
     * Social
     * Social interaction measurement allows you to measure a user's interactions
     * with various social network sharing and recommendation widgets embedded in
     * your content.
     *
     * e.g.: googleAnalytics.LogSocial(
     *                  "twitter",
     *                  "retweet",
     *                  "twitter.com/googleanalytics/status/482210840234295296");
     */

    /**
     * Log a social event.
     *
     * @param socialNetwork
     * @param socialAction
     * @param socialTarget
     */
    void logSocial(String socialNetwork,
                          String socialAction,
                          String socialTarget);

    /**************************************************************************************
     * General purpose methods.
     */

    /**
     * While running on dry run, the tracked events won't be sent to the actual analytics account.
     * @param dr whether to enable dry run mode.
     */
    void setDryRun( boolean dr );

    /**
     * Enable advertising tracking when in google's ad vendors.
     * @param e enable or disable
     */
    void enableAdvertisingTracking( boolean e );

    /**
     * Create a tracker identified by the google analytics tracker id XX-YYYYYYYY-Z.
     * If the tracker already existed, no new tracker will be created.
     * In any case, the tracker associated with tracker id will be set as default tracker for
     * analytics operations.
     * @param trackerId a valid tracker id of the form XX-YYYYYYYY-Z.
     */
    void createTracker( String trackerId );

    /**
     * Enable a tracker identified by a trackerId.
     * If the tracker does not exist, nothing will happen.
     * @param trackerId a valid tracker id of the form XX-YYYYYYYY-Z.
     */
    void enableTracker( String trackerId );
}
