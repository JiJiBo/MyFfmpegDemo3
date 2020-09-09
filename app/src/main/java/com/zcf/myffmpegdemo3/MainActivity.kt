package com.zcf.myffmpegdemo3

import android.hardware.Camera
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import com.afollestad.assent.Permission
import com.afollestad.assent.runWithPermissions
import com.rulerbug.bugutils.Utils.BugApp
import com.rulerbug.bugutils.Utils.BugDirUtils
import com.rulerbug.bugutils.Utils.BugLogUtils
import com.zcf.ffmpeglibrary.FfmpegDemo
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        BugApp.init(this, Handler(), android.os.Process.myTid())
        runWithPermissions(Permission.READ_EXTERNAL_STORAGE, Permission.WRITE_EXTERNAL_STORAGE) {
            val file = File(
                BugDirUtils.getDirPathByDirName("zcf"),
                "a.mp4"
            )
            val absolutePath =file.absolutePath
            BugLogUtils.e(absolutePath)
            BugLogUtils.e(file.isFile)
            FfmpegDemo().prepare(
                absolutePath
            )
        }

    }


}
