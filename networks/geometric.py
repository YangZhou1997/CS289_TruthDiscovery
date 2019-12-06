import sys,os
import pandas as pd
import networkx as nx
import numpy as np
import random

number_nodes = int(sys.argv[1])
distance_link = float(sys.argv[2])

if __name__ == '__main__':
    loction = [np.random.uniform(0,1,number_nodes).tolist(), np.random.uniform(0,1,number_nodes).tolist()]

    G = nx.Graph()
    for src in range(number_nodes):
        for dst in range(src + 1, number_nodes):
            distance = np.sqrt((loction[0][src] - loction[0][dst])**2 + (loction[1][src] - loction[1][dst])**2)
            if distance < distance_link:
                G.add_edge(src, dst)

    f = open('geometric.dat','w')
    for edge in G.edges:
        f.write(f'{edge[0]} {edge[1]}\n')
    f.close()
