<!-- Written in -*- Markdown -*- -->
# lambda-rmi

This source tree contains a set of applications that demonstrate the
use of lambda functions to make RMI bearable.

This uses [ZeroC Ice][ice] as the distributed system toolkit, since it
generates excellent asynchronous bindings for remote clients.

## How to build

### What you need

* ZeroC Ice - "a modern object-oriented toolkit that enables you to
  build distributed applications with minimal effort."

* `make` - There's a Makefile in the root directory wrapping the other
  build commands.

* `CMake` - C++ libraries and apps are built using CMake.  While it
  _should_ be portable and compile with MS Visual C++, I haven't tried
  compiling it yet.

* `ant` - Builds Java remote bindings.

* `sbt` - Builds Scala apps

### Instructions

1. Install `Ice.jar` into the local Maven repository.  You only have
   to do this once.

    make mvn-install-ice

2. Build.  You can build everything, or just the scala-stuff.  In
   order to build the cpp-stuff, you need to have a C++ compiler that
   is C++0x capable, such as GCC 4.5 or later.

    make             # builds everything
    make CXX=g++-4.5 # builds everything, using GCC 4.5
    make scala-stuff # or just the scala stuff

3. Run the server.

    make run-server
    
4. If you built the cpp-stuff, you can run that.

    ./client-cpp/target/cmake/client-cpp        # defaults to sync
    ./client-cpp/target/cmake/client-cpp async  # query asynchronously 
    ./client-cpp/target/cmake/client-cpp lambda # query async, using lambda

    
5. Otherwise, you can just run the scala webapp.  You can browse to
   http://localhost:8080

    make run-webapp
    

 [ice]: http://www.zeroc.com/ice.html
