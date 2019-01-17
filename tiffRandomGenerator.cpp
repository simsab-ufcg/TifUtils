#include <bits/stdc++.h>
#include <tiffio.h>

using namespace std;


int main (int argc, char **argv){

	long long width; 
	sscanf(argv[1], "%lld", &width);
	long long height = width;
	double *data;
	data = (double*)malloc(sizeof(double) * width);
	srand(time(NULL));
	double factor;
	sscanf(argv[3], "%lf", &factor);
	TIFF *tif;
	tif = TIFFOpen(argv[2], "w8");

	factor = factor > 1.0 || factor <= 0.0 ? 1.0 : factor;

	factor = 1.0 - factor;

	int newSize = width * height * factor;

	newSize = newSize <= 0 ? 1 : newSize;

	double preRandom[newSize];

	for(int i = 0; i < newSize; i++){
		preRandom[i] = (i + 1.0) / newSize;
	}

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH     , width); 
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH    , height);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE  , 64);
	TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT   , 3);
	TIFFSetField(tif, TIFFTAG_COMPRESSION    , 1);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC    , 1);
	TIFFSetField(tif, TIFFTAG_ORIENTATION    , 1);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP   , 8);
	TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT , 1);
	TIFFSetField(tif, TIFFTAG_XRESOLUTION    , 1);
	TIFFSetField(tif, TIFFTAG_YRESOLUTION    , 1);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG   , PLANARCONFIG_CONTIG   );
	unsigned long l_buffer_size = width * (sizeof (double));
	int tam = 0;
	for (int row = 0; row < height; row++){
		for (int colunm = 0; colunm < width; colunm++) {
			data[colunm] = preRandom[tam%newSize];
			tam++;
		}
		int ret=TIFFWriteScanline(tif, data, row, 0);
		if (ret==-1){
		TIFFClose(tif);
		return -1;
		}
	}

	TIFFClose(tif);
	return 0;
}
