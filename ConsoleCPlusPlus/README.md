## Trace

This directory contains Trace's files execution's result in **ASReporter**. 

To make some traces, you should run the application :

ConsoleCPlusPlusStarterKit\ASReporter\Release\Reporter.Exe

before you run your code on **Visual Studio Community**

----------


### Main.cpp

Main program for Console project in Visual Studio Community

Open Console Window

Set title in Debug or Release

Set position

Init Traces and call NT_TestMain

### TestMain.cpp

Depending on wich plateform you run call sub program

Declare Menu

Set memory Usage

### TTestx.cpp

Another Test file name should begin with T be be reconized by PM_MENU system

### Adding a new Test

Add a file like TMyTest.cpp : your code

Add a line PMMENU_DECLARE( MyTest ); in TestMain.cpp

Add a line PMMENU_USE( MyTest ) into NT_TestMain function (NT is for Windows NT...)

that is all




