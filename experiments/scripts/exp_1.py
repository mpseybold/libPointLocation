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
    "r_tree": [],
    "rs_tsd": [],
    "r_tree*": [],
    "tsd_t": [],
    "seg_ds_t": [],
    "r_tree_t": [],
    "rs_tsd_t": [],
    "r_tree*_t": []
}

hor_result_data = {
    "i": [],
    "av_k": [],
    "tsd": [],
    "seg_ds": [],
    "r_tree": [],
    "rs_tsd": [],
    "r_tree*": [],
    "tsd_t": [],
    "seg_ds_t": [],
    "r_tree_t": [],
    "rs_tsd_t": [],
    "r_tree*_t": []
}

slant_result_data = {
    "i": [],
    "av_k": [],
    "tsd": [],
    "seg_ds": [],
    "r_tree": [],
    "rs_tsd": [],
    "r_tree*": [],
    "tsd_t": [],
    "seg_ds_t": [],
    "r_tree_t": [],
    "rs_tsd_t": [],
    "r_tree*_t": []
}

for i in range(7, 15):

    print("random_horizontal")
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "100", "r_tree"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "100", "r_tree_star"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "100", "tsd"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "100", "seg_ds"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_horizontal", "100", "rs_tsd"])

    print("random_slanted")
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "100", "r_tree"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "100", "r_tree_star"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "100", "tsd"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "100", "seg_ds"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "random_slanted", "100", "rs_tsd"])

    print("seg tree worst case")
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "100", "r_tree"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "100", "r_tree_star"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "100", "tsd"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "100", "seg_ds"])
    subprocess.run(["./build/run_experiments", str(math.pow(2, i)), "seg_tree_worst_case", "100", "rs_tsd"])

    seg_data_tsd = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "tsd.csv")
    hor_data_tsd = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "tsd.csv")
    slant_data_tsd = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "tsd.csv")
    seg_data_r_tree = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "r_tree.csv")
    hor_data_r_tree = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "r_tree.csv")
    slant_data_r_tree = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "r_tree.csv")
    seg_data_r_tree_star = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "r_tree_star.csv")
    hor_data_r_tree_star = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "r_tree_star.csv")
    slant_data_r_tree_star = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "r_tree_star.csv")
    seg_data_seg_ds = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "seg_ds.csv")
    hor_data_seg_ds = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "seg_ds.csv")
    slant_data_seg_ds = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "seg_ds.csv")
    seg_data_rs = pandas.read_csv("data/results/seg_tree_worst_case/" + str(int(math.pow(2, i))) + "rs_tsd.csv")
    hor_data_rs = pandas.read_csv("data/results/random_horizontal/" + str(int(math.pow(2, i))) + "rs_tsd.csv")
    slant_data_rs = pandas.read_csv("data/results/random_slanted/" + str(int(math.pow(2, i))) + "rs_tsd.csv")


    print(seg_data_tsd)

    seg_result_data["i"].append(i)
    seg_result_data["av_k"].append(seg_data_tsd["k"].mean())
    seg_result_data["tsd_t"].append(seg_data_tsd["time"].mean())
    seg_result_data["r_tree_t"].append(seg_data_r_tree["time"].mean())
    seg_result_data["seg_ds_t"].append(seg_data_seg_ds["time"].mean())
    seg_result_data["rs_tsd_t"].append(seg_data_rs["time"].mean())
    seg_result_data["r_tree*_t"].append(seg_data_r_tree_star["time"].mean())
    seg_result_data["tsd"].append(seg_data_tsd["comp_count"].mean())
    seg_result_data["r_tree"].append(seg_data_r_tree["comp_count"].mean())
    seg_result_data["seg_ds"].append(seg_data_seg_ds["comp_count"].mean())
    seg_result_data["rs_tsd"].append(seg_data_rs["comp_count"].mean())
    seg_result_data["r_tree*"].append(seg_data_r_tree_star["comp_count"].mean())



    slant_result_data["i"].append(i)
    slant_result_data["av_k"].append(slant_data_tsd["k"].mean())
    slant_result_data["tsd_t"].append(slant_data_tsd["time"].mean())
    slant_result_data["r_tree_t"].append(slant_data_r_tree["time"].mean())
    slant_result_data["seg_ds_t"].append(slant_data_seg_ds["time"].mean())
    slant_result_data["rs_tsd_t"].append(slant_data_rs["time"].mean())
    slant_result_data["r_tree*_t"].append(slant_data_r_tree_star["time"].mean())
    slant_result_data["tsd"].append(slant_data_tsd["comp_count"].mean())
    slant_result_data["r_tree"].append(slant_data_r_tree["comp_count"].mean())
    slant_result_data["seg_ds"].append(slant_data_seg_ds["comp_count"].mean())
    slant_result_data["rs_tsd"].append(slant_data_rs["comp_count"].mean())
    slant_result_data["r_tree*"].append(slant_data_r_tree_star["comp_count"].mean())

    hor_result_data["i"].append(i)
    hor_result_data["av_k"].append(hor_data_tsd["k"].mean())
    hor_result_data["tsd_t"].append(hor_data_tsd["time"].mean())
    hor_result_data["r_tree_t"].append(hor_data_r_tree["time"].mean())
    hor_result_data["seg_ds_t"].append(hor_data_seg_ds["time"].mean())
    hor_result_data["rs_tsd_t"].append(hor_data_rs["time"].mean())
    hor_result_data["r_tree*_t"].append(hor_data_r_tree_star["time"].mean())
    hor_result_data["tsd"].append(hor_data_tsd["comp_count"].mean())
    hor_result_data["r_tree"].append(hor_data_r_tree["comp_count"].mean())
    hor_result_data["seg_ds"].append(hor_data_seg_ds["comp_count"].mean())
    hor_result_data["rs_tsd"].append(hor_data_rs["comp_count"].mean())
    hor_result_data["r_tree*"].append(hor_data_r_tree_star["comp_count"].mean())


seg_frame = pandas.DataFrame(seg_result_data)
hor_frame = pandas.DataFrame(hor_result_data)
slant_frame = pandas.DataFrame(slant_result_data)

ax = seg_frame.plot(kind="line", x="i", y="seg_ds_t", label="SDS", color="y", ylabel="Time")
seg_frame.plot(kind="line", x="i", y="tsd_t", label="tsd", color="g", ax = ax)
seg_frame.plot(kind="line", x="i", y="rs_tsd", label="rs", color="r", ax = ax)
# seg_frame.plot(kind="line", x="i", y="r_tree", label="r_tree", color="b", ax = ax)
# seg_frame.plot(kind="line", x="i", y="r_tree*", label="r_tree_star", color="m", ax = ax)

ax.get_figure().savefig("data/results/seg_worst_case_comp.png")

ax2 = slant_frame.plot(kind="line", x="i", y="seg_ds", label="SDS", color="y", ylabel="Time")
slant_frame.plot(kind="line", x="i", y="tsd", label="tsd", color="g", ax = ax2)
slant_frame.plot(kind="line", x="i", y="rs_tsd", label="rs", color="r", ax = ax2)
# slant_frame.plot(kind="line", x="i", y="r_tree", label="r_tree", color="b", ax = ax2)
# slant_frame.plot(kind="line", x="i", y="r_tree*_t", label="r_tree_star", color="m", ax = ax2)


ax2.get_figure().savefig("data/results/random_slanted_comp.png")

ax3 = hor_frame.plot(kind="line", x="i", y="seg_ds", label="SDS", color="y", ylabel="Time")
hor_frame.plot(kind="line", x="i", y="tsd", label="tsd", color="g", ax = ax3)
hor_frame.plot(kind="line", x="i", y="rs_tsd", label="rs", color="r", ax = ax3)
hor_frame.plot(kind="line", x="i", y="r_tree", label="r_tree", color="b", ax = ax3)
hor_frame.plot(kind="line", x="i", y="r_tree*", label="r_tree_star", color="m", ax = ax3)


ax3.get_figure().savefig("data/results/random_horizontal_comp.png")






