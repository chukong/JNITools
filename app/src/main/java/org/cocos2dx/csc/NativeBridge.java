package org.cocos2dx.csc;

import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;

/**
 *
 * This class is a general purpose message-driven bridge to communicate Java with Native side.
 *
 * Created by ibon on 5/8/15.
 */
public class NativeBridge {

    private static final String TAG = "ServicesNativeBridge";

    private HashMap<String, ArrayList<Long>> _registeredCallbacksByEvent;

    public NativeBridge() {
        _registeredCallbacksByEvent= new HashMap<>();

    }

    /**
     * Adds an event type interest to a given native-side object Callback.
     *
     * @param event a string describing an event. Events are arbitrary, and will
     * @param nativePtrCallback .
     */
    public void addEventListener( String event, long nativePtrCallback ) {

        Log.d(TAG, "Adding native Event listener");

        ArrayList<Long> callbacks= _registeredCallbacksByEvent.get( event );
        if ( null==callbacks ) {
            callbacks= new ArrayList<>();
            _registeredCallbacksByEvent.put(event, callbacks);
        }

        callbacks.add( nativePtrCallback );

    }

    public void removeEventListener( String event, long nativePtrCallback ) {
        ArrayList<Long> callbacks= _registeredCallbacksByEvent.get( event );
        if ( null!=callbacks ) {
            callbacks.remove(nativePtrCallback);
        } else {
            Log.e(TAG, "Removing event listener "+event+" for unknown callback.");
        }
    }

    public void emit( String event, Object params ) {
        ArrayList<Long> callbacks= _registeredCallbacksByEvent.get( event );
        if ( null!=callbacks ) {
            for( Long callback : callbacks ) {
                emit( callback, event, params );
            }
        }
    }

    private native void emit( long nativePtrCallback, String event, Object params );
}
