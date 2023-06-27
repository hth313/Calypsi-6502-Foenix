Calypsi board support for Foenix 256 Junior
===========================================

This project contains a library and linker control files to make
it easy to get started on using the
[Calypsi C compiler](https://www.calypsi.cc/) for the F256 Junior or the F256K.

Startup module
--------------

This project contains a startup module for F256 Foenix Junior that is
suitable for starting an application in its envionment. The default
startup module provided by the Calypsi tools is aimed at running from
a flash.

Notes on stack
--------------

The linker control files have been defined not to set up a stack and
relies on that the stack is already set up by the operating system (or
language environment) which load and run the application.

Alternative use
---------------

If you want more control, e.g. take over the machine completely then
you can also take a look at the corresponding startup file and
example linker files provided in the Calypsi C compiler package.
