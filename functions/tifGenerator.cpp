#include <bits/stdc++.h>
#include <tiffio.h>

using namespace std;

double fRand(){
    return (double)rand() / RAND_MAX;
}

int main (int argc, char **argv){

	long long width; 
	sscanf(argv[1], "%lld", &width);
	long long height = width;
	srand(time(NULL));
	double *data;
	data = (double*)malloc(sizeof(double) * width);
	bool totalRandomFlag = false;
	long long x, y;
	if(argc >= 4 && (string(argv[3]) == "-R" || string(argv[3]) == "-r"))totalRandomFlag = true;
	double preRandom[height * 4];
	for(int i = 0; i < height; i++){
		preRandom[i] = fRand();
	}

	TIFF *tif;

	tif = TIFFOpen(argv[2], "w8");

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

	for (int row = 0; row < height; row++){
		for (int colunm = 0; colunm < width; colunm++) {
			data[colunm] = totalRandomFlag ? fRand() : preRandom[rand() % height];
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
