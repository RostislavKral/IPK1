#!/bin/sh
import os, subprocess
path='tests/'
cmd1 = './ipkcpc -h 0.0.0.0 -p 2023 -m udp'

for file in os.listdir(path):
    if file[-11:] != '-output.txt':
        proc = subprocess.Popen(cmd1 + " < "+ path+file, stdout=subprocess.PIPE, shell=True)
        for line in iter(proc.stdout.readline,''):
            tmp = (line.rstrip()).decode('UTF-8')
            proc.terminate()
            print("INPUT: " + tmp)
            outputFile = open(path+file[:-4]+'-output.txt')
            output = outputFile.read()
            print("EXPECTED OUTPUT: " + output)
            if tmp == output: print('\033[96mTEST: '+file+'\033[92m OK\33[0m')
            else: print('\033[96mTEST: '+file+'\033[91m FAIL\33[0m')
            break;