from IDMInterface import IDMSimulation
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

from multiprocessing import Pool, Process, Value, Array

def max_ave_velocity(cars, length=1500, lanes=3, EU=False):
    steps = 800*30
    fn, df = IDMSimulation(road_length=length,
                          cars=cars,
                          lanes=3,
                          step_width=1/30,                
                          steps=steps,
                          EU=EU)
    
    t = df['t']
    velocities = np.zeros(shape=(steps, cars))
    for car_index in range(0,cars):
        for step in range(0, steps):
            velocities[step, car_index] = df['v' + str(car_index)][step]
    average_velocity = np.mean(velocities, axis=1)
    return average_velocity.max()

# defining car number
car_num = [10,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,170,180,190,200]# a bit long for testing

processes = l = [None] * len(car_num)
average_max_velocity = np.array(car_num)

average_max_velocity = Array('d', range(len(car_num)))

def calculate_for_index(index, car_num, target):
    target[i] = max_ave_velocity(car_num[i], EU=True)

for i in range(0, len(car_num)):
    p = Process(target=calculate_for_index, args=(i, car_num, average_max_velocity))
    p.start()
    processes[i] = p
    
for i in range(len(processes)):
    processes[i].join()
    
average_max_velocity_EU = np.array(average_max_velocity)

processes = l = [None] * len(car_num)
average_max_velocity = np.array(car_num)

average_max_velocity = Array('d', range(len(car_num)))

def calculate_for_index(index, car_num, target):
    target[i] = max_ave_velocity(car_num[i], EU=False)

for i in range(0, len(car_num)):
    p = Process(target=calculate_for_index, args=(i, car_num, average_max_velocity))
    p.start()
    processes[i] = p
    
for i in range(len(processes)):
    processes[i].join()
    
average_max_velocity_US = np.array(average_max_velocity)

# comparing velocities
plt.scatter(car_num, average_max_velocity_EU, label='EU Law',
           marker='+')
plt.scatter(car_num, average_max_velocity_US, label='US Law',
           marker='+')

plt.xlabel('Cars on the road')
plt.ylabel('Average speed / (m/s)')
plt.legend()
plt.tight_layout()

plt.savefig('build/ave_speed_comparison.pdf')
plt.savefig('build/ave_speed_comparison.png')

plt.clf()
# comparing throughput
plt.scatter(car_num, car_num * average_max_velocity_EU / 4500, label='EU Law',
           marker='+')
plt.scatter(car_num, car_num * average_max_velocity_US / 4500, label='US Law',
           marker='+')

plt.xlabel('Cars on the road')
plt.ylabel('Throughput / (cars / second / lane)')
plt.legend()
plt.tight_layout()

plt.savefig('build/ave_throughput_comparison.pdf')
plt.savefig('build/ave_throughput_comparison.png')

# througput(speed)
plt.clf()
plt.scatter(average_max_velocity_EU, car_num * average_max_velocity_EU / 4500, label='EU Law',
           marker='+')
plt.scatter(average_max_velocity_US, car_num * average_max_velocity_US / 4500, label='US Law',
           marker='+')

plt.xlabel('Peak Average Speed / (m/s)')
plt.ylabel('Throughput / (cars / second / lane')
plt.legend()
plt.tight_layout()

plt.savefig('build/ave_throughput_speed.pdf')
plt.savefig('build/ave_throughput_speed.png')
