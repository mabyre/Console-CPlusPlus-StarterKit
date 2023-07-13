# Console C++ Starter Kit #

For you developers who want to totaly masterise there developments in C/C++.

It's a very huge project, in tree parts, hard to take in hands but if you do so, you will be able to mastrerise all your source codes.

#### Terms:
- **PM** mean Protocol or Protable Machinery for multiplatform development.
- **AS** is for AbyreSoft the beginnings of SoDevLog.

## Features ##

- Masterise your code for multi-plateform development in C/C++
- Driven by console in a smart menu system
- Powerfull functions for memory management
 
## Requirements ##

Fully portable core software in C language so you can pickup some C sources to another plateform.

Any IDE that can make a reference to a library. Here it's ***Visual Studio Community*** but there are many exemples of porting the solution under Eclipse for Linux.

## Summary ##

- [Generate](#Generate) generation is required
- [ASDLLTrace](#ASDLLTrace) the Communication DLL between your code and the Reporter
- [ASReporter](#ASReporter) or Reporter is a windows application that display traces
- [PMLiteC](#PMLiteC) code adapted to your platform for Traces communication
- [PMLite](#PMLite) more tests in language C
- [ConsoleCPlusPlus](#ConsoleCPlusPlus) your project under test using your code in C/C++
- [Conclusion](#Conclusion) a new world opens in front of your eyes

## Want to know more ##

I have wanted to bring to light this project for a long time. I wanted to do this so I could develop with Visual Studio Community to enhence my C++. That's it.

Here is a dedicated blog to ***Trace Reporter***:

- [Blog TraceReporter](https://trace-reporter.sodevlog.com/ "Based on TraceReporter")

Another GitHub project with a Console [MinGW](https://sourceforge.net/projects/mingw/) and the TraceReporter that can help:

- [ConsoleMinGW TraceReporter](https://github.com/mabyre/TraceReporter "ConsoleMinGW with TraceReporter")

A modern MFC Project in C++ that use the Trace Reporter:

- [MFC-CPlusPlus-StarterKit](https://github.com/mabyre/MFC-CPlusPlus-StarterKit)

## For little story ##

One day I made it, I rebuilding this project with ***Visual Studio Community 2019***. I unearthed it from a very old computer for an ***IoT*** project that I had to develop.

For my notes:

\Mabyre\Visual Studio\CPlusPlus\ConsoleCPlusPlusStarterKit\PMLiteC\PMLitec\Wrk6\PMLiteC

Exhumation of a very old project **Protocol Machinery Lite** from :

\Oracle Virtual Box\Windows 7 Ineo 000\Partage\Visual C++ 6.0\Console

From a very old computer named:
\\Mabyre\SouthOfFrance\Visual C++ 6.0

Now it's ready for thousands of years.

## Generate ##

- Clone the repository

> :warning: If you directly open the [ConsoleCPlusPlus.sln](https://github.com/mabyre/Console-CPlusPlus-StarterKit/blob/master/ConsoleCPlusPlus/ConsoleCPlusPlus.sln) and run, it will not work.

- Generate [ASDLLTrace](#ASDLLTrace) 

First of all generate ASDLLTrace, open the first solution [ASTrace.sln](https://github.com/mabyre/Console-CPlusPlus-StarterKit/blob/master/ASDLLTrace/ASDLLTraceSources/ASTrace.sln)

Verify you are well in configuration **Debug x86** right-click on the project ASTrace:

<img style="margin: 10px" src="Images/2023-06-07_16h46_16.png" alt="Open Solution ASTrace.sln" />

 Choose Generate:

<img style="margin: 10px" src="Images/2023-06-07_16h52_07.png" alt="Open Solution ASTrace.sln" />

Open the second solution [ConsoleCPlusPlus.sln](https://github.com/mabyre/Console-CPlusPlus-StarterKit/blob/master/ConsoleCPlusPlus/ConsoleCPlusPlus.sln)

Inside you have two projects, it's your code under test with ConsoleCPlusPlus and PMLiteC

<img style="margin: 10px" src="Images/2023-06-07_12h36_41.png" alt="Open Solution ConsoleCPlusPlus" />

First of all verify you are in configuration **Debug x86**:

<img style="margin: 10px" src="Images/2023-06-07_12h42_00.png" alt="Verify you are in x86 configuration" />

- Generate [PMLiteC](#PMLiteC)

<img style="margin: 10px" src="Images/2023-06-07_12h45_46.png" alt="Generate PMLiteC" />

- Generate [ConsoleCPlusPlus](#ConsoleCPlusPlus)

<img style="margin: 10px" src="Images/2023-06-07_12h47_21.png" alt="Generate ConsoleCplusPlus" />

You are now ready to develop your source code in ConsoleCplusPlus and make Traces to the Reporter.

- Run your project

First of All run the Reporter:

Click On Reporter.exe you will find in directory \ASReporter\Release\

<img style="margin: 10px" src="Images/2023-06-07_11h21_48.png" alt="The Reporter hear ConsoleCplusPlus" />

The reporter is now listening to traces coming from your application.

> Have a look to the "Tip of the day" : The Reporer can also receive on your desktop traces emited by your application running at one of your client Office during Beta-Testing stage. This imrpoves greatly bugs tracking.

At each time you will need to execute the Reporter before your project therefor the Reporter will listen on sockets to Traces that may come in.

### ASDLLTrace ###

The communication DLL for Traces using socket TCP/IP, used by the Reporter and your project to emite Traces.

Sources Code: [ASDLLTrace](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/ASDLLTrace)

### ASReporter ###

The application that display Traces in a non-MFC windows application

Sources Code: [ASReporter](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/ASReporter)

### PMLite ###

More Tests in language C

Sources Code: [PMLite](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/PMLite/PMLiteSources/TPMLiteC)

### PMLiteC ###

The Protocole Machinery adapted to your project

Sources Code: [PMLiteC](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/PMLiteC)

### ConsoleCPlusPlus ###

Your project, your code under tests under Traces

Once you will run your project you can get the following result :

<img style="margin: 10px" src="Images/2023-06-07_16h59_00.png" alt="The Reporter hear ConsoleCplusPlus" />

Sources Code: [ConsoleCPlusPlus](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/ConsoleCPlusPlus)

## Conclusion ##

I hope that you will open this new world in front of you, this world of development in C and C++ language appeased by the total control of your sources code.

Indeed the TraceReporter allows you to develop in a serene way by adding a total control of the functionalities that you develop.

The tracing system by recording the traces allows you to do non-regression simply by including the results files you will realize your errors:

[Trace](https://github.com/mabyre/Console-CPlusPlus-StarterKit/tree/master/ConsoleCPlusPlus/Trace)

Samples in C:

[Double Linked List](https://github.com/mabyre/Console-CPlusPlus-StarterKit/blob/master/PMLiteC/PMLitec/Test/TDLList.c)

Last thing, have a look to the source code you can find here. This one is to test your machine byte ordering depending on the type platform you are running on: LITTLE INDIAN or BIG INDIAN:

[TBOrder.c](https://github.com/mabyre/Console-CPlusPlus-StarterKit/blob/master/PMLiteC/PMLitec/Test/TBOrder.c)

Have fun and don't forget it's only software!
