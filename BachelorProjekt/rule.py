import networkx as nx
import matplotlib.pyplot as plt


class Rule:
    def __init__(self, left, right, k, mapl, mapr, name):
        self.left = left
        self.right = right
        self.k = k
        self.mapl = mapl
        self.mapr = mapr
        self.name = name


# Defining keto-enol rule
L = nx.Graph()
L.add_node(0, attribute="C")
L.add_node(1, attribute="C")
L.add_node(2, attribute="O")
L.add_node(3, attribute="H")
L.add_edge(0, 1)
L.add_edge(0, 3)
L.add_edge(1, 2)
K = nx.Graph()
K.add_node(0, attribute="C, C")
K.add_node(1, attribute="C, C")
K.add_node(2, attribute="O, O")
K.add_node(3, attribute="H, H")
K.add_edge(0, 1)
K.add_edge(1, 2)
R = nx.Graph()
R.add_node(0, attribute="C")
R.add_node(1, attribute="C")
R.add_node(2, attribute="O")
R.add_node(3, attribute="H")
R.add_edge(0, 1)
R.add_edge(1, 2)
R.add_edge(2, 3)
mapL = {0: 0, 1: 1, 2: 2, 3: 3}
mapR = {0: 0, 1: 1, 2: 2, 3: 3}

ketonolF = Rule(L, R, K, mapL, mapR, "ketonolF")

# Defining the enol-keto rule (inverse of ketonol_F)
L1 = nx.Graph()
L1.add_node(0, attribute="C")
L1.add_node(1, attribute="C")
L1.add_node(2, attribute="O")
L1.add_node(3, attribute="H")
L1.add_edge(0, 1)
L1.add_edge(1, 2)
L1.add_edge(2, 3)
K1 = nx.Graph()
K1.add_node(0, attribute="C, C")
K1.add_node(1, attribute="C, C")
K1.add_node(2, attribute="O, O")
K1.add_node(3, attribute="H, H")
K1.add_edge(0, 1)
K1.add_edge(1, 2)
R1 = nx.Graph()
R1.add_node(0, attribute="C")
R1.add_node(1, attribute="C")
R1.add_node(2, attribute="O")
R1.add_node(3, attribute="H")
R1.add_edge(0, 1)
R1.add_edge(0, 3)
R1.add_edge(1, 2)
mapL1 = {}
mapL1[0] = 0
mapL1[1] = 1
mapL1[2] = 2
mapL1[3] = 3
mapR1 = {}
mapR1[0] = 0
mapR1[1] = 1
mapR1[2] = 2
mapR1[3] = 3

ketonolB = Rule(L1, R1, K1, mapL1, mapR1, "ketonolB")

# Defining aldol addition rule
L2 = nx.Graph()
L2.add_node(0, attribute="C")
L2.add_node(1, attribute="C")
L2.add_node(2, attribute="O")
L2.add_node(3, attribute="H")
L2.add_node(4, attribute="O")
L2.add_node(5, attribute="C")
L2.add_edge(0, 1)
L2.add_edge(1, 2)
L2.add_edge(2, 3)
L2.add_edge(4, 5)
K2 = nx.Graph()
K2.add_node(0, attribute="C,C")
K2.add_node(1, attribute="C,C")
K2.add_node(2, attribute="O,O")
K2.add_node(3, attribute="H,H")
K2.add_node(4, attribute="O,O")
K2.add_node(5, attribute="C,C")
K2.add_edge(0, 1)
K2.add_edge(1, 2)
K2.add_edge(4, 5)
R2 = nx.Graph()
R2.add_node(0, attribute="C")
R2.add_node(1, attribute="C")
R2.add_node(2, attribute="O")
R2.add_node(3, attribute="H")
R2.add_node(4, attribute="O")
R2.add_node(5, attribute="C")
R2.add_edge(0, 1)
R2.add_edge(0, 5)
R2.add_edge(1, 2)
R2.add_edge(3, 4)
R2.add_edge(4, 5)
mapL2 = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5}
mapR2 = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5}

aldoladdf = Rule(L2, R2, K2, mapL2, mapR2, "aldoladdf")

# defining aldol splitting rule
L3 = nx.Graph()
L3.add_node(0, attribute="C")
L3.add_node(1, attribute="C")
L3.add_node(2, attribute="O")
L3.add_node(3, attribute="H")
L3.add_node(4, attribute="O")
L3.add_node(5, attribute="C")
L3.add_edge(0, 1)
L3.add_edge(0, 5)
L3.add_edge(1, 2)
L3.add_edge(3, 4)
L3.add_edge(4, 5)
K3 = nx.Graph()
K3.add_node(0, attribute="C, C")
K3.add_node(1, attribute="C, C")
K3.add_node(2, attribute="O, O")
K3.add_node(3, attribute="H, H")
K3.add_node(4, attribute="O, O")
K3.add_node(5, attribute="C, C")
K3.add_edge(0, 1)
K3.add_edge(1, 2)
K3.add_edge(4, 5)
R3 = nx.Graph()
R3.add_node(0, attribute="C")
R3.add_node(1, attribute="C")
R3.add_node(2, attribute="O")
R3.add_node(3, attribute="H")
R3.add_node(4, attribute="O")
R3.add_node(5, attribute="C")
R3.add_edge(0, 1)
R3.add_edge(1, 2)
R3.add_edge(2, 3)
R3.add_edge(4, 5)
mapL3 = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5}
mapR3 = {0: 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5}


aldoladdb = Rule(L3, R3, K3, mapL3, mapR3, "aldoladdb")

listofrules = [ketonolF, ketonolB, aldoladdb, aldoladdf]

testG = nx.Graph()
testG.add_node(1, attribute='O')
testG.add_node(2, attribute='C')
testG.add_node(3, attribute='C')
testG.add_edge(1, 3)
testG.add_edge(2, 3)
testG.add_edge(1, 2)


testL = nx.Graph()
testL.add_node(1, attribute='O')



testK = nx.Graph()
testK.add_node(1, attribute='O')


testmapL = {1: 1}

testR = nx.Graph()
testR.add_node(1, attribute='O')
testR.add_node(2, attribute='C')
testR.add_node(3, attribute='H')
testR.add_node(4, attribute='P')
testR.add_edge(1, 2)
testR.add_edge(1, 3)
testR.add_edge(2, 3)
testR.add_edge(2, 4)

testmapR = {1: 1}

testRule = Rule(testL, testR, testK, testmapL, testmapR, "testrule")


# Display graph function
def my_show(my_graph):
    labels = nx.get_node_attributes(my_graph, 'attribute')
    nx.draw(my_graph, labels=labels)
    plt.show()


def show_map(map):
    lists = map.items()
    # print(lists)
    x, y = zip(*lists)
    # print(x, y)
    plt.plot(x, y)
    plt.show()






