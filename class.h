#ifndef CLASS_H
#define CLASS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct st_class{
	int code;		// class code
	char name[30]; 	// class name
	int unit;		// credites
	int grading;	// grading (1:A+~F, 2:P/F)
};

extern char kname[2][10];

int loadData(struct st_class* c[]);
void printAllClasses(struct st_class* c[], int csize);
void findClasses(char* name, struct st_class* c[], int csize);
void saveAllClasses(struct st_class* c[], int csize);

int addNewClass(struct st_class* c[], int csize);
void editClass(struct st_class* c[], int csize);

int applyMyClasses(int my[], int msize, struct st_class* c[], int csize);
void printMyClasses(int my[], int msize, struct st_class* c[], int csize);
void saveMyClass(int my[], int msize, struct st_class* c[], int csize);

#endif