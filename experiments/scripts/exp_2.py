import subprocess
import math
import pandas
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt


subprocess.run(["./build/run_experiments", "1", "albania_clean", "300", "tsd"])
