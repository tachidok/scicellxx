import matplotlib.pyplot as plt
import numpy as np

Linewidth = 1;
Dot_size = 10;

# ---------------------------------
# Filename
filename = 'RESLT/output.dat'
filename_nodes = 'RESLT/nodes.dat'

fig1, ax1 = plt.subplots()
x, fx = np.loadtxt(filename, delimiter=' ', unpack=True)
#ax1.plot(x, fx, label='$f(x)$', color='red', linestyle='solid', linewidth=Linewidth)
ax1.scatter(x, fx, label='$f(x)$', color='red', s=Dot_size)
# Supporting nodes
x_nodes = np.loadtxt(filename_nodes, delimiter=' ', unpack=True)
n_nodes = len(x_nodes)
zeroes = []
for i in range(n_nodes):
    zeroes.append(0)
    
ax1.scatter(x_nodes, zeroes, label='Support nodes', color='blue', s=Dot_size*2)

ax1.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
ax1.axis([0,1,0,1])
ax1.set_xlabel('$x$')
ax1.set_ylabel('$f(x)$')
ax1.legend()
plt.show()

# ---------------------------------
# Filename
filename = 'RESLT/error.dat'

fig2, ax2 = plt.subplots()
x, e = np.loadtxt(filename, delimiter=' ', unpack=True)
#ax2.plot(x, e, label='error', color='red', linestyle='solid', linewidth=Linewidth)
ax2.scatter(x, e, label='error', color='red', s=Dot_size)
ax2.scatter(x_nodes, zeroes, label='Support nodes', color='blue', s=Dot_size*2)

ax2.grid()
#plt.xticks(np.arange(0,10,step=1))
#plt.yticks(np.arange(0,1.1,step=0.1))
ax2.axis([0,1,-1,1])
ax2.set_xlabel('$x$')
ax2.set_ylabel('$error(x)$')
ax2.set_title("Bodies Position")
ax2.legend()
plt.show()

