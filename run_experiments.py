import subprocess
import math
import pandas
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt


seg_result_data = {
    "i": [],
    "av_k": [],
    "tsd": [],
    "seg_ds": [],
    "r_tree": []
}

hor_result_data = {
    "i": [],
    "av_k": [],
    "tsd": [],
    "seg_ds": [],
    "r_tree_star": [],
    "r_tree_linear": [],
    "r_tree": []
}

slant_result_data = {
    "i": [],
    "av_k": [],
    "tsd": [],
    "seg_ds": [],
    "r_tree": []
}

# subprocess.run(["./build/run_experiments", str(math.pow(2, 1)), "osm", "300", "r_tree"])
# subprocess.run(["./build/run_experiments", str(math.pow(2, 1)), "osm", "300", "r_tree_linear"])
# subprocess.run(["./build/run_experiments", str(math.pow(2, 1)), "osm", "300", "r_tree_star"])
# subprocess.run(["./build/run_experiments", str(math.pow(2, 1)), "osm", "300", "tsd"])
# subprocess.run(["./build/run_experiments", str(math.pow(2, 1)), "osm", "300", "seg_ds"])

for i in range(7, 14):
    print(i)
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "r_tree"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "r_tree_linear"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "r_tree_star"])
    # print(i)
    # print("random_horizontal")
    # print("tsd")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "tsd"])
    # print("seg_ds")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "seg_ds"])
    # print("r_tree")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "300", "r_tree"])
    # print("seg_tree_worst_case")
    # print("tsd")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "300", "r_tree_star"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "300", "r_tree_linear"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "300", "tsd"])
    # print("seg_ds")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "300", "seg_ds"])
    # print("r_tree")
    # # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "300", "r_tree"])
    # print("tsd")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "300", "r_tree_star"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "300", "r_tree_linear"])
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "300", "tsd"])
    # print("seg_ds")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "300", "seg_ds"])
    # print("r_tree")
    # subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "300", "r_tree"])

    seg_data_tsd = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "tsd.csv")
    hor_data_tsd = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "tsd.csv")
    slant_data_tsd = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "tsd.csv")
    seg_data_r_tree = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "r_tree.csv")
    hor_data_r_tree = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "r_tree.csv")
    slant_data_r_tree = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "r_tree.csv")
    seg_data_seg_ds = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "seg_ds.csv")
    hor_data_seg_ds = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "seg_ds.csv")
    slant_data_seg_ds = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "seg_ds.csv")

    hor_data_r_tree_star = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "r_tree_star.csv")
    hor_data_r_tree_linear = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "r_tree_linear.csv")


    # seg_data_tsd = seg_data_tsd[seg_data_tsd.k  < 5*i]
    hor_data_tsd = hor_data_tsd[hor_data_tsd.k < 5*i]
    slant_data_tsd = slant_data_tsd[slant_data_tsd.k < 5*i]
    seg_data_r_tree = seg_data_r_tree[seg_data_r_tree.k < 5*i]
    hor_data_r_tree = hor_data_r_tree[hor_data_r_tree.k < 5*i]
    slant_data_r_tree = slant_data_r_tree[slant_data_r_tree.k < 5*i]
    # seg_data_seg_ds = seg_data_seg_ds[seg_data_seg_ds.k < 5*i]
    hor_data_seg_ds = hor_data_seg_ds[hor_data_seg_ds.k < 5*i]
    slant_data_seg_ds = slant_data_seg_ds[slant_data_seg_ds.k < 5*i]

    hor_data_r_tree_star = hor_data_r_tree_star[hor_data_r_tree_star.k < 5*i]
    hor_data_r_tree_linear = hor_data_r_tree_linear[hor_data_r_tree_linear.k < 5*i]

    # seg_data_tsd = seg_data_tsd[seg_data_tsd.k >= .5*i]
    hor_data_tsd = hor_data_tsd[hor_data_tsd.k >= .5*i]
    slant_data_tsd = slant_data_tsd[slant_data_tsd.k >= .5*i]
    # seg_data_r_tree = seg_data_r_tree[seg_data_r_tree.k >= .5*i]
    hor_data_r_tree = hor_data_r_tree[hor_data_r_tree.k >= .5*i]
    slant_data_r_tree = slant_data_r_tree[slant_data_r_tree.k >= .5*i]
    # seg_data_seg_ds = seg_data_seg_ds[seg_data_seg_ds.k >= .5*i]
    hor_data_seg_ds = hor_data_seg_ds[hor_data_seg_ds.k >= .5*i]
    slant_data_seg_ds = slant_data_seg_ds[slant_data_seg_ds.k >= .5*i]

    hor_data_r_tree_star = hor_data_r_tree_star[hor_data_r_tree_star.k >= .5*i]
    hor_data_r_tree_linear = hor_data_r_tree_linear[hor_data_r_tree_linear.k >= .5*i]

    # dfs = [seg_data, horizontal_data]
    # dfs = [seg_data]

    seg_result_data["i"].append(i)
    seg_result_data["av_k"].append(seg_data_tsd["k"].mean())
    seg_result_data["tsd"].append(seg_data_tsd["tsd"].mean())
    seg_result_data["r_tree"].append(seg_data_r_tree["r_tree"].mean())
    seg_result_data["seg_ds"].append(seg_data_seg_ds["seg_ds"].mean())

    hor_result_data["i"].append(i)
    hor_result_data["av_k"].append(hor_data_tsd["k"].mean())
    hor_result_data["tsd"].append(hor_data_tsd["tsd"].mean())
    hor_result_data["r_tree"].append(hor_data_r_tree["r_tree"].mean())
    hor_result_data["seg_ds"].append(hor_data_seg_ds["seg_ds"].mean())
    hor_result_data["r_tree_star"].append(hor_data_r_tree_star["r_tree_star"].mean())
    hor_result_data["r_tree_linear"].append(hor_data_r_tree_linear["r_tree_linear"].mean())

    slant_result_data["i"].append(i)
    slant_result_data["av_k"].append(slant_data_tsd["k"].mean())
    slant_result_data["tsd"].append(slant_data_tsd["tsd"].mean())
    slant_result_data["r_tree"].append(slant_data_r_tree["r_tree"].mean())
    slant_result_data["seg_ds"].append(slant_data_seg_ds["seg_ds"].mean())


seg_frame = pandas.DataFrame(seg_result_data)
hor_frame = pandas.DataFrame(hor_result_data)
slant_frame = pandas.DataFrame(slant_result_data)

plt.rc('legend',fontsize=18)

print(seg_frame)
    # seg_data.sort_values(by=["k"])
    # horizontal_data.sort_values(by=["k"])

    # for df in dfs:
ax = seg_frame.plot(kind="line", x="i", y="seg_ds", label="SDS", color="y", ylabel="Comparisons")
# seg_frame.plot(kind="line", x="av_k", y="r_tree", label="r_tree", color="r", ax = ax)
seg_frame.plot(kind="line", x="i", y="tsd", label="tsd", color="g", ax = ax)

    #     type = "random_horizontal" if df.equals(horizontal_data) else "seg_tree_worst_case"
    #     print(type)
ax.get_figure().savefig("data/results/seg_worst_case.png")

ax2 = hor_frame.plot(kind="line", x="i", y="seg_ds", label="SDS", color="y", ylabel="Comparisons")
hor_frame.plot(kind="line", x="i", y="r_tree", label="r_tree_quad", color="r", ax = ax2)
hor_frame.plot(kind="line", x="i", y="tsd", label="tsd", color="g", ax = ax2)
hor_frame.plot(kind="line", x="i", y="r_tree_linear", label="r_tree_lin", color="b", ax = ax2)
hor_frame.plot(kind="line", x="i", y="r_tree_star", label="r_*tree", color="m", ax = ax2)
    #     type = "random_horizontal" if df.equals(horizontal_data) else "seg_tree_worst_case"
    #     print(type)
ax2.get_figure().savefig("data/results/random_horizontal.png")

ax3 = slant_frame.plot(kind="line", x="i", y="seg_ds", label="SDS", color="y", ylabel="Comparisons")
# slant_frame.plot(kind="line", x="av_k", y="r_tree", label="r_tree", color="r", ax = ax3)
slant_frame.plot(kind="line", x="i", y="tsd", label="tsd", color="g", ax = ax3)
    #     type = "random_horizontal" if df.equals(horizontal_data) else "seg_tree_worst_case"
    #     print(type)
ax3.get_figure().savefig("data/results/random_slanted.png")
