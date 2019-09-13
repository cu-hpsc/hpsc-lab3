import numpy
import argparse
from matplotlib import pyplot

def parse_args(argv=None):
    parser = argparse.ArgumentParser()
    parser.add_argument("datapath", metavar="path", type=str,
                        help="path to data file")
    args = parser.parse_args(argv)
    return(args)


def main(args):
    imgmat = []
    for line in open(args.datapath,"r"):
        imgmat.append([float(val) for val in line.rstrip().split()])
    ranges = imgmat[0]
    imgmat = imgmat[1:]

    # flip for visualization
    imgmat = numpy.flip(imgmat,1).T
    spanx = ranges[1]-ranges[0]
    spany = ranges[3]-ranges[2]
    shiftx = 0.5*spanx/imgmat.shape[1]
    shifty = 0.5*spany/imgmat.shape[0]
    ranges[0] -= shiftx
    ranges[1] += shiftx
    ranges[2] -= shifty
    ranges[3] += shifty
    pyplot.imshow(imgmat,extent=ranges)
    pyplot.show()



if __name__ == "__main__":
    main(parse_args())
