package org.cocos2dx.csc.activity;

import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

import org.cocos2dx.csc.ServicesRegistry;


public class CSC extends ActionBarActivity {

    private static final String TAG = "ServicesActivity";
    private ServicesRegistry sr;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        System.loadLibrary("msgbroker");

        setContentView(org.cocos2dx.csc.activity.R.layout.activity_csc);

        sr= new ServicesRegistry(this);
        sr.create();
        sr.initialize();

        sr.dispatchMessage("kk", "kk", null);
        sr.dispatchMessage("GoogleAnalyticsService", "kk");
        sr.dispatchMessage("org.cocos2dx.services.googleanalytics.GoogleAnalyticsService","logScreen",null);
        sr.dispatchMessage("org.cocos2dx.services.googleanalytics.GoogleAnalyticsService","logScreen",new Object[] {"Screen"});
        sr.dispatchMessage("org.cocos2dx.services.googleanalytics.GoogleAnalyticsService","logScreen",new Object[] {"Screen", 5});
        sr.dispatchMessage("org.cocos2dx.services.googleanalytics.GoogleAnalyticsService","Ja");


        new Thread() {
            public void run() {
                try {
                    testSendMessages();
                } catch(Exception x) {
                    Toast.makeText( CSC.this, x.getMessage(), Toast.LENGTH_LONG ).show();
                }

                new Thread() {
                    public void run() {

                        Log.d(TAG,"Emitting to native");
                        for( int i=0; i<5; i++ ) {
                            try {
                                Thread.currentThread().sleep(2000);
                            } catch(Exception x) {

                            }
                            sr.nativeEmit( "GoogleAnalytics.Tick", "msg num :"+i );

                            if ( this.isInterrupted() ) {
                                break;
                            }
                        }
                        Log.d(TAG,"Done Emitting to native");
                    }
                }.start();

            }
        }.start();


    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(org.cocos2dx.csc.activity.R.menu.menu_csc, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == org.cocos2dx.csc.activity.R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void onPause() {
        super.onPause();
        sr.onPause();
    }

    public void onDestroy() {
        super.onDestroy();
        sr.onDestroy();
    }

    public void onResume() {
        super.onResume();
        sr.onResume();
    }

    public void onTrimMemory( int level ) {
        super.onTrimMemory(level);
        sr.onTrimMemory(level);
    }

    public native void testSendMessages();

    public static Object test_call( String a, String b ) {
        String str= a+" <--> "+b;
        Log.d(TAG, str );
        return str;
    }
}
