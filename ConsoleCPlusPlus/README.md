## TCplusplus

Tests in C++ 

### DoSystemTime

Show you how to get System Time and to convert a **Ctring** into a **const char** to display System Time. 


## TMem

Understand the trace memory management, and the stack memory management

## TInput

Tells you how makre intercation with user demanding to enter values


## TMenu

Testing the smart Menu system in PMLite TraceReporter.

Look at this code to understand how to make a smart menu for your console application. 

## TTrace

This directory contains Trace's files execution's result in **ASReporter**. 

To make some traces, you should run the application :

ConsoleCPlusPlusStarterKit\ASReporter\Release\Reporter.Exe

before you run your code on **Visual Studio Community**

----------


### Main.cpp

Main program for Console project in Visual Studio Community

Open Console Window

Set title in Debug or Release

Set console window position

Init Traces and call NT_TestMain

### TestMain.cpp

Depending on wich plateform you run call sub program

Declare Menus

Set memory Usage

### TMyTest.cpp

Another Test file you add for TESTS.

The name of a test file should begin with **T** be be reconized by PM_MENU system

----------

# Adding your own new Test

Add a file like TMyTest.cpp : your code

Add a line PMMENU_DECLARE( MyTest ); in TestMain.cpp

Add a line PMMENU_USE( MyTest ) into NT_TestMain function (NT is for Windows NT...)

That's All Folks




