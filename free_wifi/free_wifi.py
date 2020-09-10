#!/usr/bin/python3
import math
lim_lower = -2.0
lim_upper = 2.0
dx = 1e-6

x = lim_lower
ans = 0.0
while (x <= lim_upper):
    ans += ((x**3) * math.cos(x / 2.0) + 0.5) * math.sqrt(4 - x**2) * dx
    x += dx

print("integral: %.9f" %(ans))
print("Panda is da best!")
