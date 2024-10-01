#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import argparse

EXTRA_SMALL_SIZE = 10
SMALL_SIZE = 12
MEDIUM_SIZE = 14
BIG_SIZE = 16
EXTRA_BIG_SIZE = 18

plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=EXTRA_BIG_SIZE)    # fontsize of the axes title
plt.rc('axes', labelsize=BIG_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=BIG_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=BIG_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=EXTRA_BIG_SIZE)   # legend fontsize
plt.rc('figure', titlesize=MEDIUM_SIZE)  # fontsize of the figure title

Linewidth = 1;

def main():
        
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser(description="Plots the time space diagram")
    parser.add_argument("--filename", dest="filename", help="The file storing the time vs space (weighted) information", required=True)
    
    # parse args
    args = parser.parse_args()
    
    # Construct filename
    filename = args.filename
    
    # -------------------------------------------------------------------------------------
    # Read data to plot
    # -------------------------------------------------------------------------------------
    matrix = np.loadtxt(filename, delimiter=",") # Reads the diagram
    m = matrix.shape[0]
    n = matrix.shape[1]
    
    x_space = list(range(0, n))
    
    # -------------------------------------------------------------------------------------
    # Time space plot
    # -------------------------------------------------------------------------------------
    fig1, ax1 = plt.subplots()
    
    # Save image to file
    #plt.imsave('my_image.png', matrix, cmap=plt.cm.binary)
    
    ax1.imshow(matrix, interpolation='none', cmap=plt.cm.binary)
    #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.hot)
    #ax1.imshow(matrix, cmap=plt.cm.hot)
    #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.coolwarm)
    #ax1.imshow(matrix, cmap=plt.cm.coolwarm)
    #ax1.imshow(matrix, interpolation='none')
    #ax1.set_axis_off()
    #ax1.scatter(x_space, matrix[0], color='black')
    ax1.set_xlabel(r'Space $(x)$')
    ax1.set_ylabel(r'Time $(t)$')
    
    title_string = "Time vs Space"
    ax1.set_title(title_string)
    ax1.set_xticks(np.arange(0, n, step=int(n/10)))
    #ax1.set_title(r'Time vs Space {filename}')
    #ax1.legend(loc="upper right", bbox_to_anchor=(1.25, 1.0))
    plt.show()
    
    # Save plot to file
    #img_filename = filename + '.png'
    #fig1.savefig(img_filename)
    #fig1.savefig('my_image.png', transparent=True)
    
    plt.close()
        
if __name__ == '__main__':
    main()
    
