#!/usr/bin/env python3
import sys
import numpy as np


def dd2dms(deg):
    d = int(deg)
    md = abs(deg - d) * 60.0
    m = int(md)
    sd = (md - m) * 60.0
    return [d, m, sd]


if __name__ == "__main__":
    # Parse command line arguments
    if len(sys.argv) != 2:
        print("Invalid arguments!")
        print("Usage: dd2dms.py <path to csv file>")
        print("Example: dd2dms.py data.csv")
        exit(-1)

    # Open data file
    csv_file = sys.argv[1]
    data = np.genfromtxt(csv_file, delimiter=",", dtype=int)

    # Output DMS format
    outfile = open("dms.csv", "w")
    outfile.write("lat_degs,lat_mins,lat_secs,")
    outfile.write("lon_degs,lon_mins,lon_secs\n")
    for row in data:
        lat_dms = dd2dms(float(row[0]) * 1e-7)
        lon_dms = dd2dms(float(row[1]) * 1e-7)
        outfile.write("%f,%f,%f," % (lat_dms[0], lat_dms[1], lat_dms[2]))
        outfile.write("%f,%f,%f" % (lon_dms[0], lon_dms[1], lon_dms[2]))
        outfile.write("\n")
    outfile.close()
