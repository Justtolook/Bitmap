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

void read(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock) {   //Datei hier ändern
    FILE *fpBMP;
    fpBMP = fopen("fueller.bmp", "r");      //Datei hier ändern
                                                    //Header
    fread(&tBMPHeader->usbfType,2,1,fpBMP);
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

}

void greyscale(BMPCOLOR **tBMPImg, BMPCOLOR **tBMPGrey, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {
    long dGrey;
    int iX;
    int iY;

    FILE *fpGrey;
    fpGrey=fopen("grau.bmp", "w");

    fseek(fpGrey,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<tBMPInfoblock->lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock->lbiWidth;iY++) {
            dGrey=0;
            dGrey=0.299* tBMPImg[iY][iX].cRed + 0.587 * tBMPImg[iY][iX].cGreen + 0.114 * tBMPImg[iY][iX].cBlue;  //Error: "error reading variable dGrey"
            if(dGrey>255) {
                dGrey=255;
            }
            fputc(dGrey,fpGrey);                                                                           //i guess something is with those pointers wrong
            fputc(dGrey,fpGrey);
            fputc(dGrey,fpGrey);
        }
        fwrite("0", tBMPInfoblock->lbiWidth % 4,1,fpGrey);
    }


    fclose(fpGrey);
}

void copy(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {
    FILE *fpCopy2;
    int iX;
    int iY;
    fpCopy2=fopen("Copy2.bmp", "w");
    fseek(fpCopy2,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<tBMPInfoblock->lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock->lbiWidth;iY++) {
            fputc(tBMPImg[iY][iX].cBlue,fpCopy2);
            fputc(tBMPImg[iY][iX].cGreen,fpCopy2);
            fputc(tBMPImg[iY][iX].cRed,fpCopy2);
        }
        fwrite("0", tBMPInfoblock->lbiWidth%4,1,fpCopy2);
    }
}

void mirroring(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {
    FILE *fpMirrored;
    int iX;
    int iY;

    fpMirrored=fopen("Mirrored.bmp", "w");
    fseek(fpMirrored,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<abs(tBMPInfoblock->lbiHeight);iX++) {
        for(iY=tBMPInfoblock->lbiWidth-1;iY>=0;iY--) {
            fputc(tBMPImg[iY][iX].cBlue,fpMirrored);
            fputc(tBMPImg[iY][iX].cGreen,fpMirrored);
            fputc(tBMPImg[iY][iX].cRed,fpMirrored);
        }
        fwrite("0", tBMPInfoblock->lbiWidth%4,1,fpMirrored);
    }

    fclose(fpMirrored);
}

void rotate(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {
    FILE *fpRotated;
    int iX;
    int iY;
    fpRotated = fopen("Rotated.bmp", "w");

    fseek(fpRotated,14, 0);

    fwrite(&tBMPInfoblock->uibiSize,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lbiHeight,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lbiWidth,4,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiPlanes,2,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiBitCount,2,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiCompression,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiSizeImage,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lXPelsperMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lYPelsPerMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrUsed,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrImportant,4,1,fpRotated);

    for(iY=tBMPInfoblock->lbiWidth;iY>0;iY--) {
        for(iX=0;iX<tBMPInfoblock->lbiHeight;iX++) {
            fputc(tBMPImg[iX][iY].cBlue,fpRotated);
            fputc(tBMPImg[iX][iY].cGreen,fpRotated);
            fputc(tBMPImg[iX][iY].cRed,fpRotated);
        }
        fwrite("0", tBMPInfoblock->lbiHeight%4,1,fpRotated);
    }
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
    FILE *fpCopy2;
    FILE *fpGrey;
    FILE *fpMirrored;
    FILE *fpRotated;
    fpBMP = fopen("fueller.bmp", "r");  //Datei hier ändern
    fpCopy2 = fopen("Copy2.bmp", "w+");
    fpGrey = fopen("grau.bmp", "w+");
    fpMirrored = fopen("Mirrored.bmp", "w+");
    fpRotated = fopen("Rotated.bmp", "w+");

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

    printf("Was möchten Sie machen?\n\n");
    printf("1 Graustufenbild erzeugen\n");
    printf("2 Kopieren\n");
    printf("3 Vertikal Spiegeln");
    printf("4 um 90 Grad drehen");
    scanf("%d", &iAus);

    switch(iAus) {
    case 1:
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpGrey);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpGrey);
        greyscale(tBMPImg, tBMPGrey, &tBMPInfoblock, &tBMPHeader);
        break;
    case 2:
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpCopy2);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpCopy2);
        copy(tBMPImg, &tBMPInfoblock, &tBMPHeader);
        break;
    case 3:
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpMirrored);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpMirrored);
        mirroring(tBMPImg, &tBMPInfoblock, &tBMPHeader);
    case 4:
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpRotated);
        rotate(tBMPImg, &tBMPInfoblock, &tBMPHeader);
    }


    fclose(fpBMP);


    return 0;
}
