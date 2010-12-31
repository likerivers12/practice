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

x = [1,2,3,4,5,6,7,8,9,10]
y = [11,12,13,14,15,16,17,18,19,20]

plt.plot(x, y, linewidth=2.0)


### p12
line, = plt.plot(x, y, '-')
line.set_antialiased(False)

###
x1 = x
y1 = y
x2 = x;
y2 = y;

lines = plt.plot(x1, y1, x2, y2)
plt.setp(lines, color='r', linewidth=2.0)
# or
plt.setp(lines, 'color', 'r', 'linewidth', 2.0)

###################################################
### 3.2 Workiing with multiple figures and axes

###------------------------------------
### p13
import numpy as np
import matplotlib.pyplot as plt

def f(t):
    return np.exp(-t) * np.cos(2*np.pi*t)

t1 = np.arange(0.0, 5.0, 0.1)
t2 = np.arange(0.0, 5.0, 0.02)

plt.figure(1)
plt.subplot(211)
plt.plot(t1, f(t1), 'bo', t2, f(t2), 'k')

plt.subplot(212)
plt.plot(t2, np.cos(2*np.pi*t2), 'r--')



###------------------------------------
# p13,14

import matplotlib.pyplot as plt
plt.figure(1)
plt.subplot(211)
plt.plot([1,2,3])

plt.subplot(212)
plt.plot([4,5,6])

plt.figure(2)
plt.plot([4,5,6])

plt.figure(1)
plt.subplot(211)
plt.title('Easy as 1,2,3')

plt.show()

###############################################
### 3.3 Working with text

###------------------------------------
# p15

import numpy as np
import matplotlib.pyplot as plt

mu, sigma = 100, 15
x = mu + sigma * np.random.randn(10000)

# the histogram of the data
n, bins, patches = plt.hist(x, 50, normed=1, facecolor='g', alpha=0.75)

plt.xlabel('Smarts')
plt.ylabel('Probability')
plt.title('Histogram of IQ')
plt.text(60, .025, r'$\mu=100,\ \sigma=15$')
plt.axis([40, 160, 0, 0.03])
plt.grid(True)

###
#t = plt.xlabel('my data', fontsize=14, color='red')

plt.show()

### 3.3.1 Using mathematical expressions in text
plt.title(r'$\sigma_i=15$')

