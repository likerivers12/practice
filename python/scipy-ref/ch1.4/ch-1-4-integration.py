### ch 1.4 Integration
# p10



###-----------------------------------
### ch 1.4.1 General integration(quad)

from scipy import integrate, special

result = integrate.quad(lambda x:special.jv(2.5,x), 0, 4.5)
print result

I = sqrt(2/pi) * (18.0/27 *sqrt(2) *cos(4.5) - 4.0 /27 *sqrt(2) *sin(4.5) + sqrt(2 *pi) *special.fresnel(3 /sqrt(pi))[0])
print I

print abs(result[0] - I)



###-----------------------------------
### p11
from scipy.integrate import quad

def integrand(t,n,x):
    return exp(-x*t) / t**n

def expint(n,x):
    return quad(integrand, 1, Inf, args=(n, x))[0]

vec_expint = vectorize(expint)
vec_expint(3, arange(1.0, 4.0, 0.5))
special.expn(3, arange(1.0, 4.0, 0.5))

result = quad(lambda x:expint(3, x), 0, inf)
print result

I3 = 1.0/3.0
print I3

print I3 - result[0]


###-----------------------------------
### 11
from scipy.integrate import quad, dblquad
def I(n):
    return dblquad(lambda t, x: exp(-x*t) / t**n, 0, Inf, lambda x: 1, lambda x: Inf)

print I(4)



###-----------------------------------
### ch 1.4.2 Gaussian quadrature
### p12

### ch 1.4.3 Integrating using samples
### p12

### ch 1.4.4 Ordinary differential equations(odeint)
### p13
### p14

from scipy.integrate import odeint
from scipy.special import gamma, airy
y1_0 = 1.0/3**(2.0/3.0) / gamma(2.0/3.0)
y0_0 = -1.0/3**(1.0/3.0) / gamma(1.0/3.0)
y0 = [y0_0, y1_0]

def func(y, t):
    return [t*y[1], y[0]]

def gradient(y, t):
    return [[0, t], [1, 0]]

x = arange(0, 4.0, 0.01)
t = x
ychk = airy(x)[0]
y = odeint(func, y0, t)
y2 = odeint(func, y0, t, Dfun=gradient)

print ychk[:36:6]
print y[:36:6,1]
print y2[:36:6,1]
