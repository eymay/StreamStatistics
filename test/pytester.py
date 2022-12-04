import pandas as pd
import subprocess

stats = ["mean", "std", "min", "firstq", "median", "thirdq", "max"]

def test_file(stat, i):
    input_file = "input_"+ stat + "_" + str(10**i)  + ".txt"
    print(input_file)
    output_file = "output_"+ stat + "_" + str(10**i)  + ".txt"
    print(output_file)
    process = subprocess.Popen(["../homework2", "inputs/" + input_file])
    process.wait()
    real_data = pd.read_csv("outputs/" + output_file, header=None)
    real_data.columns = ["first_date", "first_time", "last_date", "last_time" , stat]

    test_data = pd.read_csv("./" + 'out.txt', header=None)
    test_data.columns = ["first_date", "first_time", "last_date", "last_time", stat]

    print(test_data.compare(real_data))

def traverse_all_files():

    for stat in stats:
        for i in range(1, 5):
            test_file(stat, i)

for stat in stats:
    test_file(stat, 1)
