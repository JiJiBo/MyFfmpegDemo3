package com.zcf.ffmpeglibrary;

import android.util.Log;

import com.rulerbug.bugutils.Manager.BugThreadManager;
import com.rulerbug.bugutils.Utils.BugThreadUtils;

public class FfmpegDemo {
    public native void prepare(String url);
    public native void start( );

    public void havePrepare() {
                start();
    }

    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("postproc");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
    }
}
