import random
import math

def random_5():
    return random.randint(1, 5)

def random_7():
    p = random_5() / float(5)

    # return int(7.0 * p)  # bad has rounding issues

    wheel = [1.0/7.0, 2.0/7.0, 3.0/7.0, 4.0/7.0, 5.0/7.0, 6.0/7.0, 7.0/7.0]
    ball = 0.0
    idx = 0

    print(p)
    for x in wheel:
        if x > p:
            break
        else:
            idx += 1

    return idx


if __name__ == "__main__":
    vals = []
    for i in range(1000):
        vals.append(random_7())

    import matplotlib.pylab as plt
    plt.hist(vals, 7)
    plt.show()
