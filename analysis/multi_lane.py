# multi_lane.py

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.path import Path


import os
os.chdir('analysis')

df = pd.read_csv('../build/data/multi_lane.csv', index_col=False)
data = df.to_numpy()

t = data[:,0]
car_data = data[:, 1:]
location = car_data[:, ::2]
lane = car_data[:, 1::2]



# defining the car
verts = [
    (-5,0), # lower left
    (-4.8,1.5), # top left
    (-2,1.5), # where the angle starts on the left
    (-1,3), # upper end of the back window
    (1,3), # upper end of the front window
    (2,1.5), # lower end of the front window
    (4.8,1.5),  # top right
    (5,0),  # lower right
    # tires
    (4,0), # start of front tire
    (3, -1), # lowest point of front tire
    (2,0), # end of front tire
    (-2,0), # start of back tire
    (-3, -1), # lowest point of back tire
    (-4,0), # end of back tire
]

codes = [
    Path.MOVETO, #begin the figure in the lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    Path.LINETO, #end curve in lower right
    # tires
    Path.LINETO, # line to the front of tire
    Path.CURVE3, # start a 3 point curve for the first tire
    Path.LINETO, # end fire tire
    Path.LINETO, # connection between the tires
    Path.CURVE3, # start a 3 point curve for the first tire
    Path.LINETO, # line to the front of tire
]

path = Path(verts,codes)

#plt.scatter(x, x, marker=path, s=1000)


# First set up the figure, the axis, and the plot element we want to animate
fig, ax = plt.subplots(figsize=(6,6), dpi=100)

ax.set_xlim(( 0, 1000))
ax.set_ylim((-0.5, 2.2))
scat = ax.scatter([], [], marker=path, s=500)

# initialization function: plot the background of each frame
def init():
    scat.set_offsets([0,0])
    return (scat,)

# animation function. This is called sequentially
def animate(i):
    x = location[i,:]
    y = lane[i, :]
    plot_data = np.array([x,y])
    plot_data = np.transpose(plot_data)
    
    scat.set_offsets(plot_data)
    return (scat,)


# call the animator. blit=True means only re-draw the parts that have changed so that the animations display much more quickly.
anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=len(t), interval=30, blit=True)


anim.save('multi_lane.mp4', writer = 'ffmpeg')
