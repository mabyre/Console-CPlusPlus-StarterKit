## PMLiteC

Portable Protocol Machinery in C

It's a big mess not so evident to make it evoluate but, the aim was to make multiplateform code
so, sources are "inside" the PMLiteC and you should deliver for one or for another platform.

I wanted to keep this organisation cause I don't have other needs but, if you make some modifications in PMLite depending on the use you make in your Console Project, you should release carefully.

### Take In Hands

To use PMLiteC you have two solutions:

- Add the lib \PMLiteC\lib\Debug\PMLiteC.lib to the project

- Add the project \PMLitec\Wrk6\PMLiteC\PMLiteC.dsp to the workspace and check the PMLiteC box into
the dependencies

If you Add the PMLiteC project in your solution then keep in mind that ***PMLiteC.lib*** is deliver in the directory :

Console-CPlusPlus-StarterKit\PMLiteC\PMLitec\Wrk6\PMLiteC\Debug

If you added the inc directory :

Console-CPlusPlus-StarterKit\PMLiteC\inc 

has the included files use in your console you must synchornise with the "inside" .h

### Delivery

The PMLiteC sources directory of the PMLiteC and a console which is used for the contains tests
specific to PMLiteC.

When a .h file is modified, it must be placed in "inc" the emitted library must be
placed in "lib"

The sources of the PMLiteC are in the PMLiteC directory which serves as a console for the tests
specific to PMLiteC.

