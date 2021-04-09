import matplotlib
import pandas

for file in ["128", "512",
"1024", "2048", "4096"]:
    data = pandas.read_csv(file + ".csv")
    data.set_index("k")
    data.sort_index()
    ax = data.plot(kind="scatter", x="k", label="tsd", y="non-naive", color="b")
    data.plot(kind="scatter", x="k", y="seg_tree", label="seg_tree", color="r", ax = ax)
    ax.get_figure().savefig(file + ".png")
