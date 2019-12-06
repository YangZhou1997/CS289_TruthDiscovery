import sys,os
import pandas as pd
import networkx as nx
import numpy as np
import random

number_nodes = int(sys.argv[1])
distance_link = float(sys.argv[2])

if __name__ == '__main__':
    df = pd.read_csv('cmu_supermarket.csv', sep = ',')
    df = df.iloc[random.sample(range(df.shape[0]), number_nodes),:]
    df = df.reset_index()[['X','Y']]

    G = nx.Graph()
    for src in range(number_nodes):
        for dst in range(src + 1, number_nodes):
            distance = np.sqrt((df.iloc[src,0] - df.iloc[dst,0])**2 + (df.iloc[src,1] - df.iloc[dst,1])**2)
            if distance < distance_link:
                G.add_edge(src, dst)

    f = open('supermaket.dat','w')
    for edge in G.edges:
        f.write(f'{edge[0]} {edge[1]}\n')
    f.close()
