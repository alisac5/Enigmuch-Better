# This will inverse the permutation
# From https://stackoverflow.com/questions/9185768/inverting-permutations-in-python
import numpy as np

def get_perm():
    arr = np.random.permutation(27)
    return [elem for elem in arr]


def inverse(p):
    inv = [0] * len(p)
    for idx, dest in enumerate(p):
        inv[dest] = idx
    return inv

ps = [get_perm() for _ in range(10)]

print ps
print [inverse(p) for p in ps]

