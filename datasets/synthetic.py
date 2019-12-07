import sys, os
import random
import numpy as np 

u = float(sys.argv[1])
s = float(sys.argv[2])

def get_obsv_vec(truth_v, relia):
    obsv = []
    for t in truth_v:
        if random.random() < relia:
            obsv.append(t)
        else:
            obsv.append(1 - t)
    return obsv

if __name__ == '__main__':
    truth_v = []
    for i in range(18):
        if random.random() < 0.67:
            truth_v.append(0)
        else:
            truth_v.append(1)
    f = open("./syn_" + str(u) + "_" + str(s) + "_truth.dat", "w")
    for i in range(18):
        f.write(str(truth_v[i]))
        if i == 17:
            f.write("\n")
        else:
            f.write(" ")
    f.close()

    f = open("./syn_" + str(u) + "_" + str(s) + ".dat", "w")
    reliability = np.random.normal(u, s, 1500)
    for i in range(1500):
        obsv = get_obsv_vec(truth_v, reliability[i])
        for i in range(18):
            f.write(str(obsv[i]))
            if i == 17:
                f.write("\n")
            else:
                f.write(" ")
    f.close()
