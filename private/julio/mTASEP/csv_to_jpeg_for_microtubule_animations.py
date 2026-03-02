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
    parser = argparse.ArgumentParser(description="Transform .csv into .jpeg files (for microtubule animations)")
    
    parser.add_argument("--filename_prefix", dest="filename_prefix", help="The filename prefix of the csv files", required=True)
    parser.add_argument("--initial_index", dest="initial_index", type=int, help="The initial index of the csv files", required=True)
    parser.add_argument("--final_index", dest="final_index", type=int, help="The final index of the csv files", required=True)
    parser.add_argument("--width_digits", dest="width_digits", type=int, help="The width for the digits", default = 5)
    
    # parse args
    args = parser.parse_args()
    
    # Construct filename
    filename_prefix = args.filename_prefix
    initial_index = args.initial_index
    final_index = args.final_index
    width_digits = args.width_digits

    # Create the indices of the files
    files_indices = np.linspace(initial_index, final_index, final_index - initial_index + 1)

    # Transform every csv file in the range
    for idx in files_indices:

        # Transform to integer
        int_idx = int(idx)
        # Construct the filename
        filename = filename_prefix + "{:05d}.csv".format(int_idx)
        
        # Read data to plot
        matrix = np.loadtxt(filename, delimiter=",") # Reads the diagram
        m = matrix.shape[0]
        n = matrix.shape[1]

        fig1, ax1 = plt.subplots()

        ax1.imshow(matrix, interpolation='none', cmap=plt.cm.binary)
        #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.hot)
        #ax1.imshow(matrix, cmap=plt.cm.hot)
        #ax1.imshow(matrix, interpolation='none', cmap=plt.cm.coolwarm)
        #ax1.imshow(matrix, cmap=plt.cm.coolwarm)
        #ax1.imshow(matrix, interpolation='none')
        #ax1.set_axis_off()
        #ax1.scatter(x_space, matrix[0], color='black')
        ax1.set_xlabel(r"Position $(x)$", fontsize=SMALL_SIZE)
        ax1.set_ylabel(r"Channel", fontsize=SMALL_SIZE)
        
        ax1.set_title("Multichannel-microtubule", fontsize=MEDIUM_SIZE)
        ax1.set_xticks(np.arange(0, n, step=int(n/10)))
        
        ax1.axes.yaxis.set_ticklabels([])
        
        #ax1.legend(loc="upper right", bbox_to_anchor=(1.25, 1.0))
        #plt.show()

        # We cannot set this to auto because the particles are shown distorted
        #ax1.set_aspect('auto')
        
        img_filename = filename_prefix + "{:05d}.jpeg".format(int_idx)
        fig1.savefig(img_filename)
        
        plt.close()
        
if __name__ == '__main__':
    main()
    
