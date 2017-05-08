#!/usr/bin/env bash
go build -o gotoc.a -buildmode=c-archive main.go
qmake.exe "CONFIG-=debug" "CONFIG+=release" qml.pro -r -spec win32-g++
#jom.exe
mingw32-make.exe
../bin/qml.exe