/*
Autor: Henrik Lammert
Thema: Bitmap
Letzte Änderung: 22.12.2016
*/

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

void read(BITMAPFILEHEADER *tBMPHeader, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, char *sDateiName[100]) {
    FILE *fpBMP;
    fpBMP = fopen(sDateiName, "r");
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

    free(fpBMP);
    fclose(fpBMP);
}

void greyscale(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSpeicherName[100]) {
    long dGrey;
    int iX;
    int iY;

    FILE *fpGrey;
    fpGrey=fopen(sSpeicherName, "w");

    fseek(fpGrey,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<tBMPInfoblock->lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock->lbiWidth;iY++) {
            dGrey=0;
            dGrey=0.299* tBMPImg[iY][iX].cRed + 0.587 * tBMPImg[iY][iX].cGreen + 0.114 * tBMPImg[iY][iX].cBlue;
            if(dGrey>255) {
                dGrey=255;
            }
            fputc(dGrey,fpGrey);
            fputc(dGrey,fpGrey);
            fputc(dGrey,fpGrey);
        }
        fwrite("0", tBMPInfoblock->lbiWidth % 4,1,fpGrey);
    }
    free(fpGrey);
    fclose(fpGrey);
}

void copy(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSpeicherName[100]) {
    FILE *fpCopy;
    int iX;
    int iY;
    fpCopy=fopen(sSpeicherName, "w");
    fseek(fpCopy,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<tBMPInfoblock->lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock->lbiWidth;iY++) {
            fputc(tBMPImg[iY][iX].cBlue,fpCopy);
            fputc(tBMPImg[iY][iX].cGreen,fpCopy);
            fputc(tBMPImg[iY][iX].cRed,fpCopy);
        }
        fwrite("0", tBMPInfoblock->lbiWidth%4,1,fpCopy);
    }
    free(fpCopy);
    fclose(fpCopy);
}

void mirroring(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSpeicherName[100]) {
    FILE *fpMirrored;
    int iX;
    int iY;

    fpMirrored=fopen(sSpeicherName, "w");
    fseek(fpMirrored,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen

    for(iX=0;iX<abs(tBMPInfoblock->lbiHeight);iX++) {
        for(iY=tBMPInfoblock->lbiWidth-1;iY>=0;iY--) {
            fputc(tBMPImg[iY][iX].cBlue,fpMirrored);
            fputc(tBMPImg[iY][iX].cGreen,fpMirrored);
            fputc(tBMPImg[iY][iX].cRed,fpMirrored);
        }
        fwrite("0", tBMPInfoblock->lbiWidth%4,1,fpMirrored);
    }
    free(fpMirrored);
    fclose(fpMirrored);
}

void rotate_l(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSavingName[100]) {
    FILE *fpRotated;
    int iX;
    int iY;
    fpRotated = fopen(sSavingName, "w");

    fseek(fpRotated,14, 0);     //Filepointer auf Infoblock setzen
                                //Infoblock schreiben
    fwrite(&tBMPInfoblock->uibiSize,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lbiHeight,4,1,fpRotated);    //Höhe und Breite getauscht
    fwrite(&tBMPInfoblock->lbiWidth,4,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiPlanes,2,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiBitCount,2,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiCompression,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiSizeImage,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lXPelsperMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lYPelsPerMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrUsed,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrImportant,4,1,fpRotated);

                                        //Bilddaten schreiben
for(iX=0;iX<tBMPInfoblock->lbiWidth;iX++) {
        for(iY=tBMPInfoblock->lbiHeight-1;iY>=0;iY--) {
            fputc(tBMPImg[iX][iY].cBlue,fpRotated);
            fputc(tBMPImg[iX][iY].cGreen,fpRotated);
            fputc(tBMPImg[iX][iY].cRed,fpRotated);
        }
        fwrite("0", tBMPInfoblock->lbiHeight%4,1,fpRotated);    //Fill up unused Bytes
    }

    free(fpRotated);
    fclose(fpRotated);
}

void rotate_r(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSavingName[100]) {
    FILE *fpRotated;
    int iX;
    int iY;
    fpRotated = fopen(sSavingName, "w");

    fseek(fpRotated,14, 0);     //Filepointer auf Infoblock setzen
                                //Infoblock schreiben
    fwrite(&tBMPInfoblock->uibiSize,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lbiHeight,4,1,fpRotated);    //Höhe und Breite getauscht
    fwrite(&tBMPInfoblock->lbiWidth,4,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiPlanes,2,1,fpRotated);
    fwrite(&tBMPInfoblock->usbiBitCount,2,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiCompression,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiSizeImage,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lXPelsperMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->lYPelsPerMeter,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrUsed,4,1,fpRotated);
    fwrite(&tBMPInfoblock->uibiClrImportant,4,1,fpRotated);

                                        //Bilddaten schreiben
    for(iX=tBMPInfoblock->lbiWidth-1;iX>=0;iX--) {
        for(iY=0;iY<tBMPInfoblock->lbiHeight;iY++) {
            fputc(tBMPImg[iX][iY].cBlue,fpRotated);
            fputc(tBMPImg[iX][iY].cGreen,fpRotated);
            fputc(tBMPImg[iX][iY].cRed,fpRotated);
        }
        fwrite("0", tBMPInfoblock->lbiHeight%4,1,fpRotated);    //Fill up unused Bytes
    }

    free(fpRotated);
    fclose(fpRotated);
}

void secret_text_write(BMPCOLOR **tBMPImg, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader, char *sSavingName[100]) {
    char sText[1000];
    FILE *fpSecretText;
    int iX;
    int iY;
    int iSBytes; //Geheime Bytes
    int iSX;
    int iWrittenBytes=0;
    fpSecretText=fopen(sSavingName, "w");
    fseek(fpSecretText,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen


    printf("Bitte geben Sie ihren geheim Text ein (keine Leerzeichen):\n\n");
    scanf("%s", &sText);
    fflush(stdin);

    iSBytes=tBMPInfoblock->lbiWidth%4;

    for(iY=0;iY<tBMPInfoblock->lbiHeight;iY++) {
        for(iX=0;iX<tBMPInfoblock->lbiWidth;iX++) {
            fputc(tBMPImg[iX][iY].cBlue,fpSecretText);
            fputc(tBMPImg[iX][iY].cGreen,fpSecretText);
            fputc(tBMPImg[iX][iY].cRed,fpSecretText);
        }
        for(iSX=0;iSX<iSBytes;iSX++) {
            if(iSX>999) {
                fputc("0", fpSecretText);
            }
            else {
                fputc(sText[iWrittenBytes], fpSecretText);
            }
            iWrittenBytes++;
        }

    }
    free(sText);
    free(fpSecretText);
    fclose(fpSecretText);
}

void secret_text_read(char *sDateiName, BITMAPINFORMATIONSBLOCK *tBMPInfoblock, BITMAPFILEHEADER *tBMPHeader) {
    char sText[1000];
    FILE *fpSecretText;
    int iX;
    int iY;
    int iSBytes; //Geheime Bytes
    int iSX;
    int iReadBytes=0;
    fpSecretText=fopen(sDateiName, "r");
    fseek(fpSecretText,54,SEEK_SET);          //Filepointer auf anfang der Bilddaten setzen


    iSBytes=tBMPInfoblock->lbiWidth%4;      //Anzahl der geheimen Bytes berechnen

    for(iY=0;iY<tBMPInfoblock->lbiHeight;iY++) {
        for(iX=0;iX<tBMPInfoblock->lbiWidth;iX++) {
            fseek(fpSecretText,3,SEEK_CUR);
        }
        for(iSX=0;iSX<iSBytes;iSX++) {
            sText[iReadBytes]=fgetc(fpSecretText);
            printf("%c", sText[iReadBytes]);        //Ausgabe des geheimen Bytes
            iReadBytes++;
        }

    }
    free(sText);
    free(fpSecretText);
    fclose(fpSecretText);
}

int main()
{
    BITMAPFILEHEADER tBMPHeader;
    BITMAPINFORMATIONSBLOCK tBMPInfoblock;
    BMPCOLOR **tBMPImg;
    int iC;
    int iX;
    int iY;
    int iAus;
    char sSpeicherName[50];
    char sDateiName[50];
    FILE *fpBMP;
    FILE *fpAusgabe;

    printf("Welche Datei möchten Sie bearbeiten?\n");
    printf("Dateiname: ");
    scanf("%s", &sDateiName);

    fpBMP = fopen(sDateiName, "r");

    read(&tBMPHeader, &tBMPInfoblock, sDateiName);  //Einlesen vom Header und Infoblock

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

    fseek(fpBMP,54,0);          //Filepointer auf Anfang der Bilddaten setzen

                                //Einlesen der Bilddaten
    for(iX=0;iX<tBMPInfoblock.lbiHeight;iX++) {
        for(iY=0;iY<tBMPInfoblock.lbiWidth;iY=iY++) {
            tBMPImg[iY][iX].cBlue = fgetc(fpBMP);
            tBMPImg[iY][iX].cGreen = fgetc(fpBMP);
            tBMPImg[iY][iX].cRed = fgetc(fpBMP);
        }
        fseek(fpBMP,tBMPInfoblock.lbiWidth%4,SEEK_CUR);     //Unbenutze Bilddaten überspringen
    }

    printf("Was moechten Sie machen?\n\n");
    printf("1 Graustufenbild erzeugen\n");
    printf("2 Kopieren\n");
    printf("3 Vertikal Spiegeln\n");
    printf("4 um 90 Grad nach links drehen\n");
    printf("5 um 90 Grad nach rechts drehen\n");
    printf("6 Header und Infoblock ausgeben\n");
    printf("7 Versteckte Bytes auslesen\n");
    printf("8 Geheimtext schreiben\n");
    printf("Starte: ");
    scanf("%d", &iAus);
    printf("\n");
    if(iAus==7 || iAus==6) {    //Kein Speichename für das Ausgeben vom Head und des Geheimtextes benötigt
        goto A;
    }
    printf("Speichern unter: ");
    scanf("%s", &sSpeicherName);
    fpAusgabe = fopen(sSpeicherName, "w+");
    A:
                                        //TODO FILEPOINTER GENERALISIEREN!
    switch(iAus) {
    case 1:     //GREYSCALING
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpAusgabe);
        greyscale(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    case 2:     //COPY
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpAusgabe);
        copy(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    case 3:     //MIRRORING
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpAusgabe);
        mirroring(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    case 4:     //ROTATING left
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        rotate_l(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    case 5:     //ROTATING right
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        rotate_r(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    case 6:     //PRINT HEAD AND INFOBLOCK
        write(&tBMPHeader, &tBMPInfoblock);
        break;
    case 7:     //READ HIDDEN BYTES
        secret_text_read(sDateiName, &tBMPInfoblock, &tBMPHeader);
        break;
    case 8:     //WRITE HIDDEN BYTES
        fwrite(&tBMPHeader, sizeof(tBMPHeader), 1, fpAusgabe);
        fwrite(&tBMPInfoblock, sizeof(tBMPInfoblock), 1, fpAusgabe);
        secret_text_write(tBMPImg, &tBMPInfoblock, &tBMPHeader, sSpeicherName);
        break;
    }

    free(fpAusgabe);
    free(fpBMP);
    fclose(fpAusgabe);
    fclose(fpBMP);
    return 0;
}
