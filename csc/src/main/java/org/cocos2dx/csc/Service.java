package org.cocos2dx.csc;

import org.json.JSONObject;


/**
 * This interface is Service's lifecycle interface.
 * Every Service must implement this interface which connects the Service with the application
 * lifecycle.
 * It also defines the contract for Service initialization. The method
 * <code>initialize( JSONObject )</code> will be automatically called by the
 * <code>ServiceRegistry</code> with all the information set for the service in the initialization
 * file (srconfig.json).
 *
 * This interface will be implemented by the <code>AbstractService</code> object which has convenience
 * empty methods for the application lifecycle, and has Java-to-Native event emission capabilities.
 *
 * Created by ibon on 5/6/15.
 *
 */
public interface Service {

    /********************
     * Lifecycle
     ********************/

    /**
     * Notified when the Activity pauses.
     */
    void onPause();

    /**
     * Notified when the Activity resumes.
     */
    void onResume();

    /**
     * Notified when the Activity is destroyed.
     */
    void onDestroy();

    /**
     * Notify when the OS want some memory back.
     * @param level memory requirement level.
     */
    void onTrimMemory( int level );

    /**
     * Called by the ServicesRegistry when the plugin is going to be initialized.
     * Before this is has been created.
     * After calling this method it is safe for the Service to interact with the Services
     * environment.
     *
     * @param initData Map with initialization values specific for this Service. These values are
     *             read from a json file passed in to the ServicesRegistry object.
     * @return whether the Service initialized properly.
     */
    boolean initialize( JSONObject initData );

    /**
     * Identify this service by name.
     * @return Service name. Used for internal purposes.
     */
    String getName();
}
