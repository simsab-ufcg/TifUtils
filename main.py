from osgeo import gdal, osr
import os
import sys

def main():
    if len(sys.argv) < 2:
        print "Wrong arguments"
        print "Use python main.py compress <path_tiff> <path_output>"
        return 1
    if sys.argv[1] == 'compress':
        if len(sys.argv) < 4:
            print "Need Tiff Path parameter and Tiffoutput"
            print "Format: python main.py compress <path_tiff> <path_output>"
            return 1
        os.system('g++ functions/compactador.cpp -std=c++14 -o functions/compactador -ltiff')
        os.system(' '.join(['./functions/compactador', sys.argv[2], sys.argv[3]]))
        os.system(' '.join(['python', 'functions/copyGeodata.py', sys.argv[2], sys.argv[3]]))
        return 0

if __name__ == '__main__':
    sys.exit(main())