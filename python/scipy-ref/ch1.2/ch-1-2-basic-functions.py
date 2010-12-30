import scipy as sp

# type handling
sp.cast['f'](sp.pi)


# index tricks
concatenate(([3], [0]*5, arange(-1, 1.002, 2/9.0)))
r_[3, [0]*5, -1:1:10j]

mgrid[0:5, 0:5]
mgrid[0:5:4j, 0:5:4j]


# polynomials
p = poly1d([3, 4, 5])
print p
print p*p
print p.integ(k=6)
print p.deriv()
p([4,5])


# vectorize function
def addsubtract(a,b):
    if a > b:
        return a - b
    else:
        return a + b

vec_addsubtract = vectorize(addsubtract)
vec_addsubtract([0,3,6,9], [1,3,5,7])


# other useful functions
from scipy import *
x = r_[-2:3]
x
select([x > 3, x >= 0], [0, x+2])


### 1.2.3 Common functions
#.
