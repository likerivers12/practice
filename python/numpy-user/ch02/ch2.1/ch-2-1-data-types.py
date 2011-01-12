##################################################
### Ch2 - Numpy Basics
### 2.1 Data types
### p9


###------------------------------------------------
### 2.1.1 Array types and conversions between types

#-------------------
# p10

import numpy as np
x = np.float32(1.0)
x

y = np.int_([1,2,4])
y

z = np.arange(3, dtype=np.uint8)
z

np.array([1, 2, 3], dtype = 'f')

z.astype(float)
np.int8(z)

z.dtype

d = np.dtype(int)
d

np.issubdtype(d, int)
np.issubdtype(d, float)

###------------------------------------------------
### 2.1.2 Array Scalars
### p10

#...

###------------------------------------------------
### 2.2 Array creation
### p11
x = np.array([2,3,1,0])
x = np.array([2, 3, 1, 0])
x = np.array([[1,2.0], [0,0], (1+1j, 3.)])
x = np.array([[ 1.+0.j, 2.+0.j], [ 0.+0.j, 0.+0.j], [1.+1.j, 3.+0.j]])


###------------------------------------------------
### p12
np.zeros((2,3))
np.arrange(10)
np.arange(2, 10, dtype=np.float)
np.arange(2, 3, 0.1)

np.linspace(1., 4., 6)

###------------------------------------------------
### ch 2.3 I/O with Numpy
### p13

import numpy as np
from StringIO import StringIO

data = "1, 2, 3\n4, 5, 6"
np.genfromtxt(StringIO(data), delimiter=",")

data = " 1 2 3\n 4 5 67\n890123 4"
np.genfromtxt(StringIO(data), delimiter=3)

data = "123456789\n   4   7 9\n    4567 9"
np.genfromtxt(StringIO(data), delimiter=(4,3,2))


### The autostrip argument
### p14

