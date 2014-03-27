OG-Maths
========

Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies.

Please see LICENSE.txt for license.

Projects under external/ are copyright of their respective authors - please see
subdirectories of external/ for the corresponding licenses.

Refreshing the expression enum
------------------------------

Since it is difficult to work with generated Java code in eclipse, we generate and then
commit the expression enum for the Java side. In order to regenerate it, use the
`exprenum_java` target:

```
$ make exprenum_java
[100%] Regenerating ExprEnum.java
[100%] Built target exprenum_java
```
