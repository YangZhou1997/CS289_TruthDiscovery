import networkx as nx
import numpy as np
import sys, os
import random

number_nodes = int(sys.argv[1])
new_link_per_nodes = int(sys.argv[2])

if __name__ == '__main__':

    G = nx.Graph()
    G.add_node(0)
    G.add_edge(0,1)

    for node in range(2, number_nodes):
        degree_dic = dict(G.degree)
        total_degree = np.sum([degree_dic[key] for key in degree_dic])
        degree_weight = [degree_dic[key]/total_degree for key in degree_dic]
        dst_list = random.choices(list(degree_dic.keys()), weights=degree_weight, k = new_link_per_nodes)
        for dst in dst_list:
            G.add_edge(node, dst)

    f = open('preferential.dat','w')
    for edge in G.edges:
        f.write(f'{edge[0]} {edge[1]}\n')
    f.close()
