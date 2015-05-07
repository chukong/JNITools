package org.cocos2dx.services;

import android.util.Log;

import org.cocos2dx.csc.Service;
import org.cocos2dx.csc.annotation.NativeReflection;
import org.json.JSONObject;

/**
 *
 * Created by ibon on 5/6/15.
 */
public class GoogleAnalytics implements Service {

    public static final String TAG= "ServicesGoogleAnalytics";
    public static final String NAME= "GoogleAnalytics V4";

    @Override
    public void onPause() {
        Log.d("Service","GA onPause");
    }

    @Override
    public void onResume() {
        Log.d("Service", "GA onResume");

    }

    @Override
    public void onDestroy() {
        Log.d("Service", "GA onDestroy");

    }

    @Override
    public void onTrimMemory(int level) {
        Log.d("Service", "GA onTrimMemory "+level);

    }

    @Override
    public boolean initialize(JSONObject data) {
        return false;
    }

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
