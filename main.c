#include <stdio.h>
#include <stdlib.h>


#pragma pack(1)
typedef struct {
    unsigned short usbfType;
    unsigned int uibfSize;
    unsigned int uibfReserved;
    unsigned int uibfOffBits;
}BITMAPFILEHEADER;

#pragma pack(1)
typedef struct {
    unsigned int uibiSize;
    long lbiWidth;
    long lbiHeight;
    unsigned short usbiPlanes;
    unsigned short usbiBitCount;
    unsigned int uibiCompression;
    unsigned int uibiSizeImage;
    long lXPelsperMeter;
    long lYPelsPerMeter;
    unsigned int uibiClrUsed;
    unsigned int uibiClrImportant;
}BITMAPINFORMATIONSBLOCK;

#pragma pack(1)
typedef struct {
    char cRed;
    char cGreen;
    char cBlue;
    char cUseless;
}BMPCOLOR;

void write(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock) {
    printf("Header:\n");
    printf("%i\n", tBMPHeader->usbfType);
    printf("%i\n", tBMPHeader->uibfSize);
    printf("%i\n", tBMPHeader->uibfReserved);
    printf("%i\n", tBMPHeader->uibfOffBits);
    printf("\n");
    printf("Informationsblock:\n");
    printf("%i\n", tBMPInfoblock->uibiSize);
    printf("%i\n", tBMPInfoblock->lbiWidth);
    printf("%i\n", tBMPInfoblock->lbiHeight);
    printf("%i\n", tBMPInfoblock->usbiPlanes);
    printf("%i\n", tBMPInfoblock->usbiBitCount);
    printf("%i\n", tBMPInfoblock->uibiCompression);
    printf("%i\n", tBMPInfoblock->uibiSizeImage);
    printf("%i\n", tBMPInfoblock->lXPelsperMeter);
    printf("%i\n", tBMPInfoblock->lYPelsPerMeter);
    printf("%i\n", tBMPInfoblock->uibiClrUsed);
    printf("%i\n", tBMPInfoblock->uibiClrImportant);
}

void read(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock) {
    FILE *fpBMP;
    fpBMP = fopen("naegel.bmp", "r");
                                                    //Header
    fread(&(tBMPHeader->usbfType),2,1,fpBMP);
    fread(&tBMPHeader->uibfSize,4,1,fpBMP);
    fread(&tBMPHeader->uibfReserved,4,1,fpBMP);
    fread(&tBMPHeader->uibfOffBits,4,1,fpBMP);
                                                    //Informationsblock
    fread(&tBMPInfoblock->uibiSize,4,1,fpBMP);
    fread(&tBMPInfoblock->lbiWidth,4,1,fpBMP);
    fread(&tBMPInfoblock->lbiHeight,4,1,fpBMP);
    fread(&tBMPInfoblock->usbiPlanes,2,1,fpBMP);
    fread(&tBMPInfoblock->usbiBitCount,2,1,fpBMP);
    fread(&tBMPInfoblock->uibiCompression,4,1,fpBMP);
    fread(&tBMPInfoblock->uibiSizeImage,4,1,fpBMP);
    fread(&tBMPInfoblock->lXPelsperMeter,4,1,fpBMP);
    fread(&tBMPInfoblock->lYPelsPerMeter,4,1,fpBMP);
    fread(&tBMPInfoblock->uibiClrUsed,4,1,fpBMP);
    fread(&tBMPInfoblock->uibiClrImportant,4,1,fpBMP);
                                                    //Bild
}


int main()
{
    BITMAPFILEHEADER tBMPHeader;
    BITMAPINFORMATIONSBLOCK tBMPInfoblock;
    BMPCOLOR **tBMPImg;
    int iC;

    read(&tBMPHeader, &tBMPInfoblock);
    write(&tBMPHeader, &tBMPInfoblock);

    tBMPImg = (BMPCOLOR**)malloc(tBMPInfoblock.lbiWidth*sizeof(BMPCOLOR*));
    if(NULL==tBMPImg)
        printf("Fehler");exit(1);
    for(iC=0; iC<tBMPInfoblock.lbiWidth;iC++) {
        tBMPImg[iC]=(BMPCOLOR*)malloc(abs(tBMPInfoblock.lbiHeight)*sizeof(BMPCOLOR));
        if(NULL==tBMPImg[iC]);
    }
    return 0;
}
