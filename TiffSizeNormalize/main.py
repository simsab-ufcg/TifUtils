from osgeo import gdal, osr
import os
import sys

def get_coordinate(source_tif_path):
	'''
	Takes a tiff as parameter and returns its upper left point coordinates.
	'''

	source_tiff = gdal.Open(source_tif_path)

	width_tiff = source_tiff.RasterXSize
	height_tiff = source_tiff.RasterYSize

	coord_tiff = source_tiff.GetGeoTransform()
	
	UL = (coord_tiff[0], coord_tiff[3])
	source_tif = None

	return UL


if __name__ == '__main__':
	if len(sys.argv) != 4:
		print "Invalid arguments."

	coordinateA = get_coordinate(sys.argv[1])
	coordinateB = get_coordinate(sys.argv[2])
	os.system(' '.join(['./size', sys.argv[1], str(coordinateA[0]),
		str(coordinateA[1]), sys.argv[2], str(coordinateB[0]), str(coordinateB[1]), sys.argv[3] ]))
