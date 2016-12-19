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
    unsigned char cRed;
    unsigned char cGreen;
    unsigned char cBlue;
    //unsigned char cUseless;
}BMPCOLOR;

void write(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock) {
    printf("Header:\n");
    printf("bfType: %i\n", tBMPHeader->usbfType);
    printf("bfSize: %i\n", tBMPHeader->uibfSize);
    printf("bfReserved: %i\n", tBMPHeader->uibfReserved);
    printf("bfOffBits: %i\n", tBMPHeader->uibfOffBits);
    printf("\n");
    printf("Informationsblock:\n");
    printf("biSize: %i\n", tBMPInfoblock->uibiSize);
    printf("biWidth: %i\n", tBMPInfoblock->lbiWidth);
    printf("biHeight: %i\n", tBMPInfoblock->lbiHeight);
    printf("biPlanes: %i\n", tBMPInfoblock->usbiPlanes);
    printf("biBitCount: %i\n", tBMPInfoblock->usbiBitCount);
    printf("biCompression: %i\n", tBMPInfoblock->uibiCompression);
    printf("biSizeImage: %i\n", tBMPInfoblock->uibiSizeImage);
    printf("biXPelsPerMeter: %i\n", tBMPInfoblock->lXPelsperMeter);
    printf("biYPelsPerMeter: %i\n", tBMPInfoblock->lYPelsPerMeter);
    printf("biClrUsed: %i\n", tBMPInfoblock->uibiClrUsed);
    printf("biClrImportant: %i\n", tBMPInfoblock->uibiClrImportant);
}

void read(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock) {
    FILE *fpBMP;
    fpBMP = fopen("wuerfel.bmp", "r");
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
    int iX;
    int iY;
    FILE *fpBMP;
    FILE *fpCopy;
    fpBMP = fopen("wuerfel.bmp", "r");
    fpCopy = fopen("Kopie.bmp", "w+");

    read(&tBMPHeader, &tBMPInfoblock);
    write(&tBMPHeader, &tBMPInfoblock);

    tBMPImg = (BMPCOLOR**)malloc(tBMPInfoblock.lbiWidth*sizeof(BMPCOLOR*));
    if(NULL==tBMPImg) {
        printf("Fehler");
        exit(1);
    }
    for(iC=0; iC<tBMPInfoblock.lbiWidth;iC++) {
        tBMPImg[iC]=(BMPCOLOR*)malloc(abs(tBMPInfoblock.lbiHeight)*sizeof(BMPCOLOR));
        if(NULL==tBMPImg[iC]);
    }
    fseek(fpBMP,54,0);
    for(iX=0;iX<tBMPInfoblock.lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock.lbiWidth;iY=iY++) {
            tBMPImg[iY][iX].cBlue = fgetc(fpBMP);
            tBMPImg[iY][iX].cGreen = fgetc(fpBMP);
            tBMPImg[iY][iX].cRed = fgetc(fpBMP);
        }
        fseek(fpBMP,tBMPInfoblock.lbiWidth%4,SEEK_CUR);
    }

    fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpCopy);
    fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpCopy);

    for(iX=0;iX<tBMPInfoblock.lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock.lbiWidth;iY++) {
            fputc(tBMPImg[iY][iX].cBlue,fpCopy);
            fputc(tBMPImg[iY][iX].cGreen,fpCopy);
            fputc(tBMPImg[iY][iX].cRed,fpCopy);
        }
        //fputc(tBMPImg[iX][iY].cUseless,fpCopy);
        fwrite("0", tBMPInfoblock.lbiWidth%4,1,fpCopy);
    }
    fclose(fpCopy);
    fclose(fpBMP);


    return 0;
}
