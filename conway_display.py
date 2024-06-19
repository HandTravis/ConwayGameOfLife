import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Load the grid data from the file
grid = np.loadtxt('conway_grid.txt', dtype=int)
grid = grid.reshape((20, 20))  # Reshape the grid to 2D

# Set up the figure and axis
fig, ax = plt.subplots()
ax.set_axis_off()

# Create a colormap for live cells (white) and dead cells (black)
cmap = plt.cm.binary

# Create the animation function
def update(frame):
    ax.clear()
    ax.set_axis_off()
    img = ax.imshow(grid, cmap=cmap, interpolation='nearest')
    return [img]

# Create the animation
ani = animation.FuncAnimation(fig, update, frames=range(len(grid)), interval=500, repeat=False)

# Display the animation
plt.show()

