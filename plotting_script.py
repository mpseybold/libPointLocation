import matplotlib
import pandas
import math


data = pandas.read_csv("vert_2_results.csv")
data.set_index("n")
data.sort_index()

n = data["n"]
sqrt_log = [math.sqrt(num)*math.log(num, 2.0) for num in n]
sqrt_plus = [math.sqrt(num)+math.log(num, 2.0) for num in n]
data["sqrt(n)*log(n)"] = sqrt_log
data["sqrt(n) + log(n)"] = sqrt_plus
ax = data.plot(kind="line", x="n", label="tsd", y="visits", color="b")
data.plot(kind="line", x="n", y="sqrt(n)*log(n)", label="sqrt(n)*log(n)", color="r", ax = ax)
data.plot(kind="line", x="n", y="sqrt(n) + log(n)", label="sqrt(n) + log(n)", color="g", ax = ax)
ax.get_figure().savefig("vertical_query.png")
