#!/usr/bin/env python

import os
import sys
import shutil

import numpy as np
import pylab as pl
import matplotlib.pyplot as plt
import argparse

EXTRA_SMALL_SIZE = 10
SMALL_SIZE = 12
MEDIUM_SIZE = 14
BIG_SIZE = 16
EXTRA_BIG_SIZE = 18

plt.rc('font', size=SMALL_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=BIG_SIZE)    # fontsize of the axes title
plt.rc('axes', labelsize=BIG_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=EXTRA_SMALL_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=EXTRA_SMALL_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=BIG_SIZE)   # legend fontsize
plt.rc('figure', titlesize=BIG_SIZE)  # fontsize of the figure title

Linewidth = 1;

def main():
        
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser(description="Transform a .csv into a .jpeg file (for space/state diagram)")
    
    parser.add_argument("--filename", dest="filename", help="The filename of the csv file", required=True)
    
    # parse args
    args = parser.parse_args()
    
    # Store filename
    filename = args.filename
    
    # Read data to plot
    matrix = np.loadtxt(filename, delimiter=",") # Reads the diagram
    m = matrix.shape[0]
    n = matrix.shape[1]
    
    fig1, ax1 = plt.subplots()
    
    #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.binary)
    #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.hot)
    #ax1.imshow(matrix, cmap=plt.cm.hot)
    #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.coolwarm)
    #ax1.imshow(matrix, cmap=plt.cm.coolwarm)
    #ax1.imshow(matrix, interpolation='none')
    #ax1.imshow(matrix, cmap=plt.cm.Greys)
    ax1.imshow(matrix, cmap=plt.cm.Blues)
    #ax1.set_axis_off()
    #ax1.scatter(x_space, matrix[0], color='black')
    ax1.set_xlabel(r"Position $(x)$", fontsize=SMALL_SIZE)
    ax1.set_ylabel(r"Time $(t)$", fontsize=SMALL_SIZE)
    
    ax1.set_title("Space/state for microtubule", fontsize=MEDIUM_SIZE)
    ax1.set_xticks(np.arange(0, n, step=int(n/10)))

    ax1.set_aspect('auto')
    
    #ax1.set_title(r'Time vs Space {filename}')
    #ax1.legend(loc="upper right", bbox_to_anchor=(1.25, 1.0))
    #plt.show()
    
    img_filename = filename + ".jpeg"
    fig1.savefig(img_filename)
    
    plt.close()
        
if __name__ == '__main__':
    main()
    
