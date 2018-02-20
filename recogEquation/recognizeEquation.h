/* recognizeExp.h, Gerard Renardel, 29 January 2014 */

#ifndef RECOGNIZEEXP_H
#define RECOGNIZEEXP_H
#include "scanner.h"

int acceptNumber(List *lp);
int acceptIdentifier(List *lp);
int acceptCharacter(List *lp, char c);
int acceptExpression(List *lp, int *deg, int *var, char *s);
int acceptEquation(List *lp, int *deg, int *var, char *s);
void recognizeEquation();

#endif
