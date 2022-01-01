This is an Unreal Engine plugin that can easily specify command-line parameters for Android through adb.

Usage:

```bash
adb shell am start -a android.intent.action.MAIN -n com.imzlp.gworld/com.epicgames.ue4.GameActivity --es cmdline -test123/-test456/-test789
```

Log:

```txt
Final commandline: ../../../Blank426/Blank426.uproject -test123 -test456 -test789
```

