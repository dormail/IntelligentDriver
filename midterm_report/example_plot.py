# example_plot.py

import matplotlib.pyplot as plt
import numpy as np

density = np.linspace(1,10, 1000)

model1 = np.exp(-1 * density / 10)
model2 = 1 - np.exp(-1 * density / 10)

crit = np.argmin(np.abs(model1 - model2))

plt.plot(density, model1, label='Model 1')
plt.plot(density, model2, label='Model 2')

plt.scatter(density[crit], model1[crit],
        color='r',
        marker='+',
        label='Critical Point')

plt.xlabel('Parameter $X$')
plt.ylabel('Performance measure $Y$')

plt.legend()
plt.tight_layout()
plt.savefig('build/example_plot.pdf')

