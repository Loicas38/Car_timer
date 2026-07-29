#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

int compareFloats(const void *a, const void *b);
void insertionSort(int arr[], int n);
bool rechercherString(String tableau[], int taille, String recherche);
int getIdElement(String array[], int size, String element);
void shiftLeft(String arr[], int size, int depart);
int rechercheInt(int array[], int size, int recherche);


int calculMinutes(unsigned long temps);
int calculSecondes(unsigned long temps);
int calculMilli(unsigned long temps);

#endif