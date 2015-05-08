package org.cocos2dx.csc;

import android.content.Context;
import android.util.Log;

import org.cocos2dx.services.AbstractService;
import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Constructor;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

/**
 *
 * Created by ibon on 5/6/15.
 */
public final class ServicesRegistry {

    private static final String ASSET_SRCONFIG= "srconfig.json";    // Services Registry config
    private static final String TAG=            "ServicesRegistry";

    private static final String JSON_CLASS=         "class";
    private static final String JSON_INIT_CONFIG=   "init-config";
    private static final String JSON_CREATE_CONFIG= "create-config";

    private enum ServicesRegistryStatus {
        NONE,
        CREATED,
        INITIALIZED
    }

    private List<ServiceInfo>           _createdServices;
    private HashMap<String, Service>    _services;              // initialized services.
    private Context                     _context;
    private ServicesRegistryStatus      _status= ServicesRegistryStatus.NONE;
    private NativeBridge                _nativeBridge;

    private class ServiceInfo {

        private String      name;
        private String      className;
        private JSONObject  initConfig;
        private JSONObject  createConfig;
        private AbstractService service;

        /**
         *
         * @param json {
         *              "pluginName" : {
         *                  "class" : "",
         *                  "config" : {
         *
         *                  }
         *              }
         *          }
         */
        public ServiceInfo( JSONObject json ) {
            try {
                Iterator<String> keys= json.keys();
                if ( keys.hasNext() ) {

                    name= keys.next();

                    JSONObject obj= json.getJSONObject(name);

                    className = obj.getString(JSON_CLASS);

                    initConfig= obj.optJSONObject(JSON_INIT_CONFIG);
                    createConfig= obj.optJSONObject(JSON_CREATE_CONFIG);
                }

                if ( keys.hasNext() ) {
                    Log.d(TAG,"Service definition with multiple keys. Name set to '"+name+"'");
                }

            } catch(Exception x) {
                Log.e(TAG,"Can't get Service class.");
            }
        }

        public void create() {

            try {

                Class serviceClass = Class.forName(className);
                Constructor<AbstractService> ctor= null;

                try {
                    ctor = serviceClass.getConstructor(JSONObject.class);
                } catch( Exception x ) {
                    // eat it.
                }

                if (ctor != null) {
                    service = ctor.newInstance(createConfig);
                } else {
                    if (createConfig != null) {
                        Log.d(TAG, "Service '" + name + "' has create config data but no suitable constructor. Creating default.");
                    }
                    service = (AbstractService) serviceClass.newInstance();
                }

                service.setRegistry( ServicesRegistry.this );

                Log.d(TAG, "Created Service: " + name + " with service description: '" + service.getName() + "'");
            } catch(Exception x) {
                Log.e(TAG,"Error instantiating Service: "+className, x );
            }
        }

        public Service initialize() {

            try {
                service.initialize(initConfig);
            } catch(Exception x) {
                Log.e(TAG,"Error initializing Service '"+name+"'", x);
            }

            return service;
        }
    }

    public ServicesRegistry( Context ctx ) {

        _context= ctx;
        _services= new HashMap<>();
        _nativeBridge= new NativeBridge();
    }

    public void create() {
        List<ServiceInfo> servicesInfo= getConfigData();

        for( ServiceInfo serviceInfo : servicesInfo ) {
            serviceInfo.create();
        }

        _createdServices= servicesInfo;
        _status= ServicesRegistryStatus.CREATED;

        nativeInit();
    }

    public void initialize() {
        if ( _status==ServicesRegistryStatus.CREATED ) {

            for( ServiceInfo si: _createdServices ) {
                Service s= si.initialize();
                if ( null!=s ) {
                    _services.put( si.className, s );
                }
            }

        } else {
            throw new IllegalStateException("ServicesRegistry is not created yet.");
        }

        _status= ServicesRegistryStatus.INITIALIZED;

        // get rid of the configuration since it won't be needed anymore.
        _createdServices= null;
    }

    private List<ServiceInfo> getConfigData() {

        ArrayList<ServiceInfo> servicesInfo= new ArrayList<>();

        String json= getAssetJSONContents( ASSET_SRCONFIG );

        if ( null!=json ) {

            try {
                JSONObject jsonObject = new JSONObject(json);

                JSONArray servicesArray= jsonObject.getJSONArray("services");

                for( int i=0; i<servicesArray.length(); i++ ) {
                    JSONObject serviceJSONDef= servicesArray.getJSONObject(i);
                    servicesInfo.add( new ServiceInfo(serviceJSONDef) );
                }

            } catch(Exception x) {
                Log.e(TAG, "Can't parse registry config.", x );
            }
        }

        return servicesInfo;
    }

    private String getAssetJSONContents( String asset ) {

        String ret= null;

        try {
            InputStream is= _context.getAssets().open(asset);
            BufferedReader reader = new BufferedReader(new InputStreamReader( is, "iso-8859-1"), 8);
            StringBuilder sb = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null) {
                sb.append(line);
                sb.append("\n");
            }
            is.close();
            ret= sb.toString();

        } catch(Exception x) {

            Log.e(TAG, "Can't get registry config.", x );
        }

        return ret;
    }

    public void onResume() {
        for( Map.Entry<String,Service> entry : _services.entrySet() ) {
            entry.getValue().onResume();
        }
    }

    public void onPause() {
        for( Map.Entry<String,Service> entry : _services.entrySet() ) {
            entry.getValue().onPause();
        }
    }

    public void onDestroy() {
        for( Map.Entry<String,Service> entry : _services.entrySet() ) {
            entry.getValue().onDestroy();
        }
    }

    public void onTrimMemory( int level ) {
        for( Map.Entry<String,Service> entry : _services.entrySet() ) {
            entry.getValue().onTrimMemory(level);
        }
    }

    /**
     * dispatch a message to a registered plugin.
     * @param serviceClass a full qualified service class
     * @param method a class method to call to
     */
    public void dispatchMessage( String serviceClass, String method ) {
        dispatchMessage(serviceClass, method, null);
    }

    public void nativeEmit( String event, Object params ) {
        _nativeBridge.emit( event, params );
    }

    public void addEventListener( String event, long nativePtr ) {
        _nativeBridge.addEventListener(event, nativePtr);
    }

    public void removeEventListener( String event, long nativePtr ) {
        _nativeBridge.removeEventListener(event, nativePtr);
    }

    /**
     * Message passing function. Call a plugin method with a set of parameters.
     * The method can be static and have any scope access control.
     *
     * This method will return whatever return value resulted from calling the method.
     * If the method returned primitive types, they are auto-boxed.
     * If the method returned void, the return value will be null.
     *
     * If any error happens during the dispatchMessage function, this call will return null
     * as well.
     *
     * @param serviceClass a Class canonical representation.
     * @param method a method name from that class. No matter what scope access has.
     * @param params an array of parameters to invoke the method with. Method to invoke will be
     *               inferred from the parameters signature so calling overloaded methods is ok.
     *
     * @return the result on invoking the method.
     */
    public final Object dispatchMessage( String serviceClass, String method, Object[] params ) {

        // just in case, since this method can be called from native, and native uses slash as
        // package separators.
        serviceClass= serviceClass.replaceAll("/",".");
        Service service= _services.get(serviceClass);

        Object ret= null;

        if ( null!=service ) {

            Method m= getMethodWithParams( service.getClass(), method, params );

            if ( null!=m ) {

                try {
                    m.setAccessible(true);
                    ret= m.invoke( service, params);
                } catch(Exception x ) {
                    Log.e(TAG,"Got error invoking "+serviceClass+"->"+method+".", x);
                }

            } else {

                StringBuilder signature= new StringBuilder();

                signature.append("'");
                if ( null!=params ) {
                    for (Object param : params) {
                        signature.append(param.getClass().getCanonicalName());
                        signature.append(" ");
                    }
                } else {
                    signature.append("void");
                }
                signature.append("'");

                Log.e( TAG,
                        "Service with class '"+serviceClass+
                        "' has no method '"+method+
                        "' with signature "+signature.toString() );
            }

        } else {
            Log.e(TAG,"dispatching message to non existent Service:"+ serviceClass);
        }

        return ret;
    }

    public final Object dispatchStaticMessage( String className, String method, Object[] params ) {

        // just in case, since this method can be called from native, and native uses slash as
        // package separators.
        className= className.replaceAll("/",".");

        Class<?> clazz;
        try {
            clazz= Class.forName(className);
        } catch(Exception x) {
            Log.e(TAG,"dispatchStatic class.forName error "+className, x);
            return null;
        }

        Object ret= null;

        Method m= getMethodWithParams( clazz, method, params );

        if ( null!=m ) {

            try {
                m.setAccessible(true);
                ret= m.invoke( null, params);
            } catch(Exception x ) {
                Log.e(TAG,"Got error invoking "+className+"->"+method+".", x);
            }

        } else {

            StringBuilder signature= new StringBuilder();

            signature.append("'");
            if ( null!=params ) {
                for (Object param : params) {
                    signature.append(param.getClass().getCanonicalName());
                    signature.append(" ");
                }
            } else {
                signature.append("void");
            }
            signature.append("'");

            Log.e( TAG,
                    "Static method "+method+" in class '"+className+
                    "' has wrong signature: "+signature.toString() );
        }

        return ret;
    }

    private Method getMethodWithParams( Class<?> c, String method, Object[] params ) {

        Class[] paramsClass= new Class[ params!=null ? params.length : 0 ];

        // coherce parameters types to primitives
        if ( null!=params ) {
            for (int i = 0; i < paramsClass.length; i++) {
                Class<?> paramClass = params[i].getClass();

                // watch out for autoBoxed class objects.
                if ( paramClass.equals(Integer.class) ) {
                    paramClass= int.class;
                } else if ( paramClass.equals(Double.class) ) {
                    paramClass= double.class;
                } else if ( paramClass.equals(Boolean.class) ) {
                    paramClass= boolean.class;
                } else if ( paramClass.equals(Character.class) ) {
                    paramClass= char.class;
                } else if ( paramClass.equals(Byte.class) ) {
                    paramClass= byte.class;
                } else if ( paramClass.equals(Short.class) ) {
                    paramClass= short.class;
                } else if ( paramClass.equals(Long.class) ) {
                    paramClass= long.class;
                } else if ( paramClass.equals(Float.class) ) {
                    paramClass= float.class;
                } else if ( paramClass.equals(Void.class) ) {
                    paramClass = void.class;
                }

                paramsClass[i]= paramClass;
            }
        }

        Method m= null;

        try {
            m= c.getDeclaredMethod(method, paramsClass);
        } catch( NoSuchMethodException e ) {
            // eat it
        }

        return m;
    }

    public native void nativeInit( );
}
