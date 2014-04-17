Welcome to OpenGamma Maths!
========

The OpenGamma maths project (OG-Maths) is intended to provide core maths functionality to the OpenGamma Analytics library. However, it is packaged standalone, so it can also be used entirely independently of OpenGamma products.

[![OpenGamma](http://developers.opengamma.com/res/display/default/chrome/masthead_logo.png "OpenGamma")](http://developers.opengamma.com)


WARNING NOTE: THIS PROJECT IS UNDER ACTIVE DEVELOPMENT; BETA-TESTING JAR BUILDS CONTAINING LIMITED FUNCTIONALITY WILL BE AVAILABLE SOON ONCE THEY HAVE BEEN THROUGH IN-HOUSE TESTING. DO NOT RELY ON API CONSISTENCY OR USE THIS SOFTWARE IN PRODUCTION - IT'S VERSION <1 FOR A REASON!


What is it?
-----------
The OG-Maths library is a vector-orientated maths library designed for performance and scalability. It supports a range of common matrix layouts (scalar, dense, sparse, diagonal) and various mathematical operations in both the real and complex domain. The library is intelligent, domain aware and it essentially tries to work out what will perform best.


The library can be used from Java OR C++. In either case, all the mathematical computation is performed in native code.


### Java Users:
For Java users it comes in the form of a JAR that requires zero configuration, no libraries to install, and no setting paths - [just plug-n-play with the Maven pom.xml](INSTALL.md#for-java-users). The JAR is 64-bit only and cross-platform, supporting Linux, Windows, and Mac OS X in a single JAR, using automatic detection of the platform to select appropriate native code to use. It can be integrated into existing Java maths projects (after all, the integration into OG-Analytics is after OG-Analytics was started - you don't need to build on OG-Maths from the ground up, though it will obviously be easier).


### C++ Users:
For C++ users, the libraries can be [built from source](INSTALL.md#for-developersc-users) (CMake toolchain), and used in the same way any other dynamic libraries would be used.


Implementation
--------------
Beneath the Java interface, all the computations use native code. The Java-side API builds expression trees and sends them through a native code interface as late as possible. Typically this is the point at which results must be rendered for presentation to another component. This delayed evaluation scheme enables the native side to see a “global view” of numerical methods implemented using the Java API with the potential for optimising based on this information, and minimises the overhead of switching between Java and native code.

The single OG-Maths JAR contains builds of native routines based on a standard x86_64 instruction set, as well as ones optimised for SSE4.1 and AVX instruction sets. Based on the CPU detected at runtime, the most optimised compatible versions of routines are selected and linked back into the running JVM.

For many operations, industry-standard, battle-tested native code libraries including BLAS and LAPACK are used. Transparent integration of NLOPT, components of SuiteSparse and other libraries will follow.

## Licence

The OG-Maths library is open source software using the Apache License v2, and includes components provided under other licences. For a full list of licences and the source files to which they pertain, see [LICENSE.txt](LICENSE.txt). The company behind OG-Maths ([OpenGamma](http://www.opengamma.com)) also offers support and some additional commercial components. (The commercial components typically have dependencies with restrictive licensing incompatible with open source.) This README only refers to the open source components.

**NOTE: Prior to version #### there are missing license notations, details of the licenses which apply to these prior versions can be found in [LICENSE.txt](LICENSE.txt).**


Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies.