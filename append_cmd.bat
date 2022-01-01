@echo off
set PACKAGE_NAME=%1
set CMD_LINE=%2
rem @echo %PACKAGE_NAME%
rem @echo %CMD_LINE%
rem @echo adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline %CMD_LINE%
adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline %CMD_LINE%