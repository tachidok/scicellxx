import matplotlib.pyplot as plt
import numpy as np

# Filename
filename = 'RESLT/raw_output.dat'
Linewidth = 2;

t, x1, vx1, y1, vy1, z1, vz1, x2, vx2, y2, vy2, z2, vz2, x3, vx3, y3, vy3, z3, vz3 = np.loadtxt(filename, delimiter='\t', unpack=True)

fig1 = plt.figure(1)
plt.plot(t, x1, label='$x_1$', color='blue', linestyle='solid', linewidth=Linewidth)
plt.plot(t, x2, label='$x_2$', color='red', linestyle='dashed', linewidth=Linewidth)
plt.plot(t, x3, label='$x_3$', color='green', linestyle='dotted', linewidth=Linewidth)
plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel('t')
plt.ylabel('x-position')
plt.title("Bodies Position")
plt.legend()
plt.show()

fig2 = plt.figure(2)
plt.plot(t, y1, label='$y_1$', color='blue', linestyle='solid', linewidth=Linewidth)
plt.plot(t, y2, label='$y_2$', color='red', linestyle='dashed', linewidth=Linewidth)
plt.plot(t, y3, label='$y_3$', color='green', linestyle='dotted', linewidth=Linewidth)
plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel('t')
plt.ylabel('y-position')
plt.title("Bodies Position")
plt.legend()
plt.show()

fig3 = plt.figure(3)
plt.plot(t, z1, label='$z_1$', color='blue', linestyle='solid', linewidth=Linewidth)
plt.plot(t, z2, label='$z_2$', color='red', linestyle='dashed', linewidth=Linewidth)
plt.plot(t, z3, label='$z_3$', color='green', linestyle='dotted', linewidth=Linewidth)
plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel('t')
plt.ylabel('z-position')
plt.title("Bodies Position")
plt.legend()
plt.show()

fig4 = plt.figure(4)
plt.plot(x1, y1, label='$body_1$', color='blue', linestyle='solid', linewidth=Linewidth)
plt.plot(x2, y2, label='$body_2$', color='red', linestyle='dashed', linewidth=Linewidth)
plt.plot(x3, y3, label='$body_3$', color='green', linestyle='dotted', linewidth=Linewidth)
plt.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
#plt.xlim([0,10])
#plt.ylim([0,1])
plt.xlabel('x-position')
plt.ylabel('y-position')
plt.title("Bodies Trajectory")
plt.legend()
plt.show()

