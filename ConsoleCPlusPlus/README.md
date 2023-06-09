# ConsoleCPlusPlus

Your sources code under PMLiteC control

## Application start

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

### Sub Menu Modules

Sources code are arranged in TestMenu

## TContainers

Use of containers like array, vector

### DoUseVector2

Demonstrate the use of auto&&

[Stackoverflow - What does auto&& tell us?](https://stackoverflow.com/questions/13230480/what-does-auto-tell-us)

## TCplusplus

Tests in pure C++ 

### DoSystemTime

Show you how to get System Time and to convert a **Ctring** into a **const char** to display System Time. 

## TMem

Understand the trace memory management, and the stack memory management

## TInput

Tells you how makre interaction with user demanding to enter values

## TMenu

Testing the smart Menu system in PMLite TraceReporter.

Look at this code to understand how to make a smart menu for your console application. 

## TTrace

This directory contains Trace's files execution's result in **ASReporter**. 

To make some traces, you should run the application :

ConsoleCPlusPlusStarterKit\ASReporter\Release\Reporter.Exe

before you run your code on **Visual Studio Community**

# Adding your own new Test

Add a file like **TMyTest.cpp** and write your code

Add a line PMMENU_DECLARE( MyTest ); in **TestMain.cpp**

Add a line PMMENU_USE( MyTest ) into **NT_TestMain** function (NT is for Windows NT...)

## Some other Test idea

To get some more tests have a look into:

- [PMLite/PMLiteSources/TPMLiteC](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/PMLite/PMLiteSources/TPMLiteC)




