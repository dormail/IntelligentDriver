# RuntimeAnalysis.py
# In this script the runtime of the IDM implementation gets measured and the time complexitiy gets computed

import numpy as np
import matplotlib.pyplot as plt

from IDMInterface import IDMSimulation

import time
from scipy.optimize import curve_fit

car_nums = [10,20,30,40,50,60,70,80,90,100,140,150,160]

runtime_us = []
runtime_eu = []

length = 1500
lanes = 3
steps = 600*30
for car_num in car_nums:
    start = time.time()
    fn, df = IDMSimulation(road_length=length,
                      cars=car_num,
                      lanes=3,
                      step_width=1/30,                
                      steps=steps,
                      EU=True)
    end = time.time()
    T = end - start
    runtime_eu.append(T)
    start = time.time()
    fn, df = IDMSimulation(road_length=length,
                      cars=car_num,
                      lanes=3,
                      step_width=1/30,                
                      steps=steps,
                      EU=False)
    end = time.time()
    T = end - start
    runtime_us.append(T)
    
def simple_polynomial(x,a,N):
    return a * x**N

# fitting a polynomial to the runtime for EU law
popt, pcov = curve_fit(simple_polynomial, car_nums, runtime_eu)
a_eu, N_eu = popt
err = np.sqrt(np.diag(pcov))
a_eu_err, N_eu_err = err

print(f'a_eu = {a_eu:.4f} +/- {a_eu_err:.4}')
print(f'N_eu = {N_eu:.4f} +/- {N_eu_err:.4}')

# now doing the same for US
popt, pcov = curve_fit(simple_polynomial, car_nums, runtime_us)
a_us, N_us = popt
err = np.sqrt(np.diag(pcov))
a_us_err, N_us_err = err

print(f'a_us = {a_us:.4f} +/- {a_us_err:.4}')
print(f'N_us = {N_eu:.4f} +/- {N_us_err:.4}')


# plotting the results
plt.scatter(car_nums, runtime_eu, label='Measured runtime EU law',
           marker='+')
x = np.linspace(0, car_nums[-1]+5)
plt.plot(x, simple_polynomial(x,a_eu,N_eu), color='k',
        label=rf'EU: $a x^N$ with N = {N_eu:.4f}')

plt.xlabel('Cars')
plt.ylabel('Runtime in seconds')
plt.legend()
plt.tight_layout()
plt.savefig('runtime_EU.pdf')

plt.clf()

plt.scatter(car_nums, runtime_us, label='Measured runtime US law',
           marker='+')
x = np.linspace(0, car_nums[-1]+5)
plt.plot(x, simple_polynomial(x,a_us,N_us), color='k',
        label=rf'EU: $a x^N$ with N = {N_us:.4f}')

plt.xlabel('Cars')
plt.ylabel('Runtime in seconds')
plt.legend()
plt.tight_layout()
plt.savefig('runtime_US.pdf')