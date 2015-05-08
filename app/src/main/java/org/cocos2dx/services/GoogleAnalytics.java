package org.cocos2dx.services;

import android.util.Log;

import org.cocos2dx.csc.Service;
import org.cocos2dx.csc.annotation.NativeReflection;
import org.json.JSONObject;

/**
 *
 * Created by ibon on 5/6/15.
 */
public class GoogleAnalytics extends AbstractService {

    public static final String TAG= "ServicesGoogleAnalytics";
    public static final String NAME= "GoogleAnalytics V4";

    @Override
    public String getName() {
        return NAME;
    }

    @NativeReflection
    void logScreen( String name ) {
        Log.d(TAG,"LogScreen called "+name);
    }
    void logScreen2( String name, String name2 ) {
        Log.d(TAG,"LogScreen2 called "+name+" "+name2 );
    }
    void logScreen3( String name, String name2, int i ) {
        Log.d(TAG,"LogScreen3 called "+name+" "+name2+" "+i );
    }

    public static void Ja() {
        Log.d(TAG,"Ja called");
    }
}
