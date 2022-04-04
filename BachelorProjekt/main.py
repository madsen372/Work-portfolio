import itertools

import matplotlib.pyplot as plt
import networkx as nx
import networkx.algorithms.isomorphism as iso
from collections import defaultdict
import time



from rule import *

start = time.time()

# Inserting graph from mød
formaldehyde = nx.Graph()
formaldehyde.add_node(0, attribute="C")
formaldehyde.add_node(1, attribute="O")
formaldehyde.add_node(2, attribute="H")
formaldehyde.add_node(3, attribute="H")
formaldehyde.add_edge(0, 1)
formaldehyde.add_edge(0, 2)
formaldehyde.add_edge(0, 3)
# my_show(formaldehyde)

# Inserting graph from mød
glycolaldehyde = nx.Graph()
glycolaldehyde.add_node(0, attribute="O")
glycolaldehyde.add_node(1, attribute="C")
glycolaldehyde.add_node(2, attribute="C")
glycolaldehyde.add_node(3, attribute="O")
glycolaldehyde.add_node(4, attribute="H")
glycolaldehyde.add_node(5, attribute="H")
glycolaldehyde.add_node(6, attribute="H")
glycolaldehyde.add_node(7, attribute="H")
glycolaldehyde.add_edge(0, 1)
glycolaldehyde.add_edge(0, 4)
glycolaldehyde.add_edge(1, 2)
glycolaldehyde.add_edge(1, 5)
glycolaldehyde.add_edge(1, 6)
glycolaldehyde.add_edge(2, 3)
glycolaldehyde.add_edge(2, 7)


# Display graph function
def my_show(my_graph):
    labels = nx.get_node_attributes(my_graph, 'attribute')
    nx.draw(my_graph, labels=labels)
    plt.show()


# Combine function
def my_combine(graphs):
    g = nx.disjoint_union_all(graphs)
    return g


def my_inv(d):
    inv = {v: k for k, v in d.items()}
    return inv


G = my_combine([formaldehyde, formaldehyde, glycolaldehyde])
# my_show(G)


# The transformation function, verbose and show is by default set to False
# However, should you want to see to see all the graph nodes, edges etc, simply set this to True
# Likewise, if you want a visualization of the graphs, set 'show' to True.
def transformation(graph, rule, verbose=False, show=False):
    nm = iso.categorical_node_match("attribute", default="")
    GM = nx.algorithms.isomorphism.GraphMatcher(graph, rule.left, node_match=nm).subgraph_isomorphisms_iter()
    count = 0
    result = []
    for m in GM:
        if verbose:
            print("G nodes: ", graph.nodes())
            print("G edges: ", graph.edges())
        print("The rule", rule.name, "has been used")
        D = nx.Graph()
        H = nx.Graph()
        # print("Mapping from G to L: ", m)

        # Mapper V(L) -> V(K)
        mapLK = my_inv(rule.mapl)
        # print(mapLK)

        # Mapper V(L) -> V(G)
        mapM = my_inv(m)
        if verbose:
            print("Mapping from L to G: ", mapM)

        # Mapper V(R) -> V(K)
        mapKR = my_inv(rule.mapr)

        # Constructing nodes for D
        # For every node in G, is it in L? If it not, add to D, if it is, is it in K?
        # If it is, add to D, if not, don't add to D
        labels = nx.get_node_attributes(graph, 'attribute')
        # print("labels", labels)
        for G_node in graph.nodes():
            # print(G_node)
            D.add_node(G_node, attribute=labels[G_node])

        for D_node in list(D.nodes()):
            if D_node in mapM:
                if verbose:
                    print("Node in mapM", D_node, "Corresponding to ", mapM[D_node], " in G")
                while D_node not in mapLK and mapM[D_node] in D.nodes():
                    # print("MAP NODE", mapM[D_node])
                    D.remove_node(mapM[D_node])

        # Constructing edges for D
        for edge_G in graph.edges():
            D.add_edge(*edge_G)

        for edge_L in rule.left.edges():
            vL = edge_L[0]
            uL = edge_L[1]
            eG = (vL, uL)
            eK = ()
            eL1 = mapM[vL]
            # print("el1", eL1)
            eL2 = mapM[uL]
            # print("el2", eL2)
            if rule.left.has_edge(m[eL1], m[eL2]):
                if verbose:
                    print("this edge is also in G:", m[eL1], m[eL2])
                D.remove_edge(eL1, eL2)
            eLv1 = m[eL1]
            # print("elv1", eLv1)
            eLv2 = m[eL2]
            if rule.left.has_edge(eLv1, eLv2):
                try:
                    eK = (mapLK[eLv1], mapLK[eLv2])
                    # print("ek = ", eK)
                    if rule.k.has_edge(*eK):
                        # print(eK)
                        D.add_edge(eL1, eL2)
                except KeyError:
                    pass
            # Handling dangling edges
            if not rule.left.has_edge(eLv1, eLv2):
                if rule.left.has_node(eLv1) and rule.left.has_node(eLv2):
                    if (eLv1 not in mapLK) != (eLv2 not in mapLK):
                        print("Dangling edge found")
                        exit()
            # Handling dangling edges
            if not rule.left.has_edge(eLv1, eLv2):
                if (eLv1 in rule.left.nodes()) != (eLv2 in rule.left.nodes()):
                    if eLv1 in rule.left.nodes() and eLv1 not in mapLK:
                        print("Dangling edge found")
                        exit()
                    if eLv2 in rule.left.nodes() and eLv2 not in mapLK:
                        print("Dangling edge found")
                        exit()

        D_keys = list(D.nodes())
        G_values = list(graph.nodes())
        mapGD_iterator = zip(D_keys, G_values)
        mapGD = dict(mapGD_iterator)

        # Handling if L had an edge not in K, it would add a new node with that edge-end
        # Also handling another case of dangling edges.
        for D_node in list(D.nodes()):
            if D_node in mapM:
                # print("Node in mapM", D_node, "Corresponding to ", mapM[D_node], " in G")
                while D_node not in mapLK and mapM[D_node] in D.nodes():
                    for edge_D in D.edges():
                        vD = edge_D[0]
                        uD = edge_D[1]
                        if mapM[D_node] == vD or mapM[D_node] == uD:
                            print("Dangling edge")
                            exit()
                    # print("MAP NODE", mapM[D_node])
                    D.remove_node(mapM[D_node])
        if verbose:
            print("L edges", rule.left.edges())
            print("L nodes", rule.left.nodes())
            print("K edges", rule.k.edges())
            print("K nodes", rule.k.nodes())
            print("D edges:", D.edges())
            print("D nodes", D.nodes())
        # print(testK.nodes())

        # my_show(D)

        D_keys = list(D.nodes())
        K_values = list(rule.k.nodes())
        mapKD_iterator = zip(D_keys, K_values)
        mapKD = dict(mapKD_iterator)
        # print(mapKD)

        # mapRR = {}

        mapRH = {}
        # my_show(D)

        # Constructing nodes for H
        # Adding all nodes from D to H
        labels1 = nx.get_node_attributes(D, 'attribute')
        for D_node in D.nodes():
            H.add_node(D_node, attribute=labels1[D_node])

        # my_show(H)

        # Mapping the ones in R that co-exist with the ones in D and mapping them so they
        # correspond to the correct node in H
        for R_node in rule.right.nodes():
            if R_node in mapKR:
                if R_node in mapKD:
                    if R_node in mapLK:
                        for L_node in rule.left.nodes():
                            R_node = mapM[L_node]
                            mapRH[L_node] = R_node
                            # print("napR_node", mapRH[R_node])

        # Adding those nodes in R to H that doesn't exists in D
        # Also mapping the old node with the new node
        labels2 = nx.get_node_attributes(rule.right, 'attribute')
        for R_node in rule.right.nodes():
            if R_node not in mapKR:
                if R_node not in mapKD:
                    # print(R_node)
                    existingNode = R_node
                    # print(existingNode)
                    while existingNode in H.nodes():
                        existingNode += 1
                    if existingNode not in H.nodes():
                        mapRH[R_node] = existingNode
                        H.add_node(existingNode, attribute=labels2[R_node])

        # Adding edges and mapping through mapRR
        for edge_D in D.edges():
            vD = edge_D[0]
            uD = edge_D[1]
            eD = (vD, uD)
            H.add_edge(*eD)

        for edge_R in rule.right.edges():
            vR = edge_R[0]
            uR = edge_R[1]
            eR = (vR, uR)
            eRK1 = ()
            eRK2 = ()
            eKD1 = ()
            eKD2 = ()
            eRR1 = mapRH[vR]
            eRR2 = mapRH[uR]
            try:
                eRK1 = mapKR[eRR1]
                eRK2 = mapKR[eRR2]
            except KeyError:
                # print("error")
                pass
            try:
                eKD1 = mapKD[eRK1]
                eKD2 = mapKD[eRK2]
            except KeyError:
                # print("error")
                pass
            if not rule.k.has_edge(eRK1, eRK2) and D.has_edge(eKD1, eKD2):
                if rule.right.has_edge(eRR1, eRR2):
                    print("Following edge already exists, parallelism not allowed", (eRR1, eRR2))
                    exit()
            else:
                H.add_edge(eRR1, eRR2)
        if verbose:
            print("R nodes", rule.right.nodes())
            print("R edges", rule.right.edges())
        print("H nodes", H.nodes())
        print("H edges", H.edges())
        count += 1
        print("Number of transformation: ", count)
        print("TRANSFORMATION DONE")
        if show:
            my_show(H)
        print("\n---------------------------")
        # my_show(H)

        result.append(H)
    if not result:
        print("Not transformation from given input")
    else:
        return result


# For all the results that we get in the saving() function
# we can use a rule on those results
def transformation2(rule, verbose = False):
    result = saving()
    # print(result)
    resultList = []
    for i in result:
        print("Transformation of all the graphs generated from using the rules")
        listgraphs = transformation(i, rule)
        for l in listgraphs:
            resultList.append(l)
    if verbose:
        print("The number of transformations using the rule,", rule.name, ", on all of results from the "
                                                                          "'transformation' function using the four "
                                                                          "rules, is: ", len(resultList))
    return resultList


# For all the results that we get in the transformation2-function
# we can use a rule on those results
def transformation3(rule):
    result = transformation2(rule)
    # print(result)
    resultList = []
    for i in result:
        print("Transformation of all the graphs generated from using the rules")
        llgraphs = transformation(i, rule, verbose=False)
        for l in llgraphs:
            resultList.append(l)
    print("The number of transformations using the rule,", rule.name, ", on all of results from the 'transformation' "
                                                                      "function using the four rules, is: ",
          len(resultList))
    return resultList


# Function that saves all the results from our transformation function
# when using the four different rules
def saving():
    result1 = transformation(G, aldoladdf, verbose=False)
    # print("result1", result1)
    result2 = transformation(G, aldoladdb)
    result3 = transformation(G, ketonolF)
    result4 = transformation(G, ketonolB)
    finalResult = []
    for i in result1:
        finalResult.append(i)
    for i in result2:
        finalResult.append(i)
    for i in result3:
        finalResult.append(i)
    for i in result4:
        finalResult.append(i)
    return finalResult


# saving()
transformation(testG, testRule, show=True, verbose=True)

elapsedTime = (time.time()-start)
print("Time spent:", elapsedTime, "seconds.")


