@echo off
setlocal enabledelayedexpansion
set PROGRAM_NAME = %0
set PACKAGE_NAME=%1
set CMD_LINE_ARGS=" "

FOR %%I IN (%*) do (
    @REM echo %%I
    if NOT "%PROGRAM_NAME%" == "%%I" (
        if NOT "%PACKAGE_NAME%" == "%%I" (
            if !CMD_LINE_ARGS! == " " (
                set CMD_LINE_ARGS=%%I
                @REM echo %CMD_LINE_ARGS%
            ) else (
                set CMD_LINE_ARGS=!CMD_LINE_ARGS!/.,;/%%I
                @REM echo %CMD_LINE_ARGS%
            )
        )
    )
)
@REM echo %CMD_LINE_ARGS%
@REM echo adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline %CMD_LINE_ARGS%
adb shell am start -a android.intent.action.MAIN -n %PACKAGE_NAME%/com.epicgames.ue4.GameActivity --es cmdline %CMD_LINE_ARGS%