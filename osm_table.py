import subprocess
import math
import pandas
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

osm_data_tsd = pandas.read_csv("data/results/osm/new_calidoniatsd.csv")
osm_data_seg_ds = pandas.read_csv("data/results/osm/new_calidoniaseg_ds.csv")
osm_data_r_tree = pandas.read_csv("data/results/osm/new_calidoniar_tree.csv")
osm_data_r_tree_linear = pandas.read_csv("data/results/osm/new_calidoniar_tree_linear.csv")
osm_data_r_tree_star = pandas.read_csv("data/results/osm/new_calidoniar_tree_star.csv")

av_k = osm_data_tsd["k"].mean()

result = {
    "average_k": [osm_data_tsd["k"].mean()],
    "TSD": [osm_data_tsd["tsd"].mean()],
    "SDS": [osm_data_seg_ds["seg_ds"].mean()],
    "r_tree_quad": [osm_data_r_tree["r_tree"].mean()],
    "r_tree_lin": [osm_data_r_tree_linear["r_tree_linear"].mean()],
    "r*tree": [osm_data_r_tree_star["r_tree_star"].mean()]
}

print(pandas.DataFrame(result))