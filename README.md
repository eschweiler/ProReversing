ProReversing Framework
============

This project gathers anti-anti-reversing techniques into an open library. The resulting DLL can be easily incorporated into all reverse engineering tools. It works under 

* Windows XP 32 bits
* Windows Vista 32 bits
* Windows 7 32 bits
* Windows Vista 64 bits
* Windows 7 64 bits

For more information, see [my blog](http://malwaremuncher.blogspot.com/2012/11/towards-anti-anti-reversing-framework.html).

Build Prerequisites
--------------

* Install the [Boost](http://www.boost.org/) library. Make sure to install it with runtime-link=static!
* Create an environment variable called BOOSTDIR and make it point to the installed Boost library.
* In order to run the tests successfully, install the [Google C++ Testing Framework](http://code.google.com/p/googletest/).
* Create an environment variable called GTESTDIR pointing to the googletest directory.
* If you want to execute the injection test, you will have to download the Dll-Injector project. If not, comment it out.
