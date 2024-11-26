@echo off
REM files are path from "build" folder, since we pushd

set files=..\code\main.c ..\code\combat.c ..\code\map.c ..\code\message.c ..\code\monster.c ..\code\player.c

set libs=C:\libs\raylib-5.0_win64_msvc16\lib\raylib.lib opengl32.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib

set incpath=C:\libs\raylib-5.0_win64_msvc16\include

mkdir ..\build
pushd ..\build
cl /Zi %files% /I %incpath% /link %libs% /OUT:"dcc.exe" /NODEFAULTLIB:LIBCMT
popd
