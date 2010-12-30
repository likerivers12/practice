#### ch3 - Pyplot tutorial
### p9

###----------------------------------------
import matplotlib.pyplot as plt
plt.plot([1,2,3,4])
plt.ylabel('some numbers')
plt.show()

###

plt.plot([1,2,3,4], [1,4,9,16])

###
plt.plot([1,2,3,4], [1,4,9,16], 'ro')
plt.axis([0, 6, 0, 20])
plt.show()


###----------------------------------------
### p11
import numpy as np
import matplotlib.pyplot as plt

# evenly sampled time at 200ms intervals
t = np.arange(0., 5., 0.2)

# red dashes, blue squares and green triangles
plt.plot(t, t, 'r--', t, t**2, 'bs', t, t**3, 'g^')

###----------------------------------------
### 3.1 Controlling line properties
### p11

x = [1:10:2j];

plt.plot(x, y, linewidth=2.0)

