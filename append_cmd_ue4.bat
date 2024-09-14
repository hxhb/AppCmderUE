@echo off
setlocal enabledelayedexpansion
set PROGRAM_NAME=%0
set PACKAGE_NAME=%1
set CMD_LINE_ARGS=%*
set REPLACE_CMDLINE_ARGS=!CMD_LINE_ARGS:%PACKAGE_NAME%=!

rem echo !REPLACE_CMDLINE_ARGS!
echo adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline "%REPLACE_CMDLINE_ARGS%"
adb shell am force-stop %PACKAGE_NAME%
adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline "%REPLACE_CMDLINE_ARGS%"
endlocal