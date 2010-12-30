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


