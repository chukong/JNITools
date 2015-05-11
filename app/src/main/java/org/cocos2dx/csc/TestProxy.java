package org.cocos2dx.csc;

import android.util.Log;

/**
 * Created by ibon on 5/11/15.
 */

public class TestProxy {
    public static final String TAG="ServicesTestProxy";

    private String m1;
    private String m2;

    public TestProxy( String s1, String s2 ) {
        m1=s1;
        m2=s2;
        Log.d(TAG, "TestProxy created with " + s1 + "," + s2);
    }

    public String m1() {
        return m1+"   "+m2;
    }
}