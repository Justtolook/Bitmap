#include <stdio.h>
#include <stdlib.h>

//TODO erstellen vom Bild in eine Funktion packen

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

void graustufen(BMPCOLOR **tBMPImg, BMPCOLOR **tBMPGrey, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {      //Grau = 0,299*Rot + 0,587*Grün + 0,144*Blau
    double dGrey;
    int iX;
    int iY;

    FILE *fpGrey;
    fpGrey=fopen("grau.bmp", "w+");

    fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpGrey);
    fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpGrey);

    for(iX=0;iX<&tBMPInfoblock->lbiWidth;iX++) {
        for(iY=0;iY<&tBMPInfoblock->lbiHeight;iY++) {
            dGrey=0.299*tBMPImg[iY][iX].cRed+0.587*tBMPImg[iY][iX].cGreen+0.144*tBMPImg[iY][iX].cBlue;
            tBMPGrey[iY][iX].cRed=dGrey;
            tBMPGrey[iY][iX].cGreen=dGrey;
            tBMPGrey[iY][iX].cBlue=dGrey;
        }
    }

    for(iX=0;iX<&tBMPInfoblock->lbiWidth;iX++) {
        for(iY=0;iY<&tBMPInfoblock->lbiHeight;iY++) {
            fputc(tBMPGrey[iY][iX].cBlue,fpGrey);
            fputc(tBMPGrey[iY][iX].cGreen,fpGrey);
            fputc(tBMPGrey[iY][iX].cRed,fpGrey);
        }
        fwrite("0", tBMPInfoblock->lbiWidth % 4,1,fpGrey);
    }

    fclose(fpGrau);
}


int main()
{
    BITMAPFILEHEADER tBMPHeader;
    BITMAPINFORMATIONSBLOCK tBMPInfoblock;
    BMPCOLOR **tBMPImg;
    BMPCOLOR **tBMPGrey;
    int iC;
    int iX;
    int iY;
    int iAus;
    FILE *fpBMP;
    FILE *fpCopy;
    fpBMP = fopen("wuerfel.bmp", "r");
    fpCopy = fopen("Kopie.bmp", "w+");

    read(&tBMPHeader, &tBMPInfoblock);
    write(&tBMPHeader, &tBMPInfoblock);
                                        //Speicherreservierung für Bilddaten
    tBMPImg = (BMPCOLOR**)malloc(tBMPInfoblock.lbiWidth*sizeof(BMPCOLOR*));
    if(NULL==tBMPImg) {
        printf("Fehler");
        exit(1);
    }
    for(iC=0; iC<tBMPInfoblock.lbiWidth;iC++) {
        tBMPImg[iC]=(BMPCOLOR*)malloc(abs(tBMPInfoblock.lbiHeight)*sizeof(BMPCOLOR));
        if(NULL==tBMPImg[iC]);
    }
                                        //Speicherreservierung für Graustufenbilddaten
    tBMPGrey = (BMPCOLOR**)malloc(tBMPInfoblock.lbiWidth*sizeof(BMPCOLOR*));
    if(NULL==tBMPGrey) {
        printf("Fehler");
        exit(1);
    }
    for(iC=0; iC<tBMPInfoblock.lbiWidth;iC++) {
        tBMPGrey[iC]=(BMPCOLOR*)malloc(abs(tBMPInfoblock.lbiHeight)*sizeof(BMPCOLOR));
        if(NULL==tBMPGrey[iC]);
    }

    fseek(fpBMP,54,0);          //Filepointer auf anfang der Bilddaten setzen

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
        fwrite("0", tBMPInfoblock.lbiWidth%4,1,fpCopy);
    }

    printf("Was möchten Sie machen?\n\n");
    printf("1 Graustufenbild erzeugen\n");
    printf("2 Bild um 90° drehen\n");
    scanf("%d", &iAus);

    switch(iAus) {
    case 1: graustufen(tBMPImg, tBMPGrey, &tBMPInfoblock, &tBMPHeader);
    }



    fclose(fpCopy);
    fclose(fpBMP);


    return 0;
}
