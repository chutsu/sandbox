import numpy as np
from numpy import genfromtxt
import matplotlib.pylab as plt

image_data = genfromtxt('img.raw', delimiter=' ')
print(image_data.shape)
plt.matshow(image_data)
plt.show()
