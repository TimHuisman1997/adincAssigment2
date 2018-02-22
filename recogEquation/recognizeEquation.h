/* recognizeExp.h, Gerard Renardel, 29 January 2014 */

#ifndef RECOGNIZEEXP_H
#define RECOGNIZEEXP_H
#include "scanner.h"

int acceptNumber(List *lp, int *temp);
int acceptIdentifier(List *lp, int *var, char *s);
int acceptCharacter(List *lp, char c, int *plusMin, int *equal);
int acceptExpression(List *lp, int *deg, int *var, char *s, double *arAB, int *plusMin, int *equal);
int acceptEquation(List *lp, int *deg, int *var, char *s, double *arAB, int *plusMin, int *equal);
void recognizeEquation();

#endif
