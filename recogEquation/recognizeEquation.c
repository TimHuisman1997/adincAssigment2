/* recognizeExp.c, Gerard Renardel, 29 January 2014
 *
 * In this file a recognizer acceptExpression is definined that can recognize
 * arithmetical expressions generated by the following BNF grammar:
 *
 * <expression>  ::= <term> { '+'  <term> | '-' <term> }
 *
 * <term>       ::= <factor> { '*' <factor> | '/' <factor> }
 *
 * <factor>     ::= <number> | <identifier> | '(' <expression> ')'
 *
 * Input for the recognizer is the token list constructed by the scanner (in scanner.c).
 * For the recognition of a token list the method of *recursive descent* is used.
 * It relies on the use of three functions for the recognition and processing of
 * terms, factors and expressions, respectively.
 * These three functions are defined with mutual recursion, corresponding with the
 * structure of the BNF grammar.
 */

#include <stdio.h>  /* getchar, printf */
#include <stdlib.h> /* NULL */
#include <string.h>
#include "scanner.h"
#include "recognizeEquation.h"


/* The functions acceptNumber, acceptIdentifier and acceptCharacter have as
 * (first) argument a pointer to an token list; moreover acceptCharacter has as
 * second argument a character. They check whether the first token
 * in the list is a number, an identifier or the given character, respectively.
 * When that is the case, they yield the value 1 and the pointer points to the rest of
 * the token list. Otherwise they yield 0 and the pointer remains unchanged.
 */

void calcAB(double temp, double *arAB, int *plusMin, int *equal, int ab){
	if(*equal==0){
		if(*plusMin==0){
			arAB[ab] = arAB[ab] + temp;
		} else {
			arAB[ab] = arAB[ab] - temp;
		}
	} else {
		if(*plusMin==0){
			arAB[ab] = arAB[ab] + temp*-1;
		} else {
			arAB[ab] = arAB[ab] - temp*-1;
		}
	}
	*plusMin=0;
}

void countVar(List *lp, int *var, char *s){
  if(*var==0){
    strcpy(s, ((*lp)->t).identifier);
    *var+=1;
  }
  if(strcmp(s, ((*lp)->t).identifier)){
    *var+=1;
  }
}

int countDeg(List *lp, int *deg){
  if(*lp!=NULL&&(*lp)->tt==Number){
    if(((*lp)->t).number>*deg){
		*deg=((*lp)->t).number;
    }
    *lp=(*lp)->next;
	return 1;
  } else {
	return 0;
    }
}	

int acceptNumber(List *lp, int *temp) {
  if (*lp != NULL && (*lp)->tt == Number) {
	*temp=((*lp)->t).number;
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

int acceptIdentifier(List *lp, int *var, char *s) {
  if (*lp != NULL && (*lp)->tt == Identifier ) {
	countVar(lp, var, s);
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

int acceptCharacter(List *lp, char c, int *plusMin, int *equal) {
  if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c ) {
	if(c=='-') *plusMin=1;
	if(c=='=') *equal=1;
    *lp = (*lp)->next;
    return 1;
  }
  return 0;
}

/* The functions acceptFactor, acceptTerm and acceptExpression have as
 * argument a pointer to a token list. They check whether the token list
 * has an initial segment that can be recognized as factor, term or expression, respectively.
 * When that is the case, they yield the value 1 and the pointer points to the rest of
 * the token list. Otherwise they yield 0 and the pointer remains unchanged.
 */



int acceptTerm(List *lp, int *deg, int *var, char *s, double *arAB, int *plusMin, int *equal) {
	int temp;
	if(acceptNumber(lp, &temp)){
		temp = (double) temp;
		if(acceptIdentifier(lp, var, s)){
			calcAB(temp, arAB, plusMin, equal, 0);
			if(acceptCharacter(lp, '^', plusMin, equal)){
				return countDeg(lp, deg);
			}
            return 1;
		}
		calcAB(temp, arAB, plusMin, equal, 1);
		return 1;
	}
	if(acceptIdentifier(lp, var, s)){
		calcAB((double)1, arAB, plusMin, equal, 0);
		if(acceptCharacter(lp, '^', plusMin, equal)){
			return countDeg(lp, deg);
		}
		return 1;
	}

	return 0;
}


int acceptExpression(List *lp, int *deg, int *var, char *s, double *arAB, int *plusMin, int *equal) {
  if(acceptCharacter(lp, '-', plusMin, equal));
  if ( !acceptTerm(lp, deg, var, s, arAB, plusMin, equal) ) {
    return 0;
  }
  while ( acceptCharacter(lp,'+', plusMin, equal) || acceptCharacter(lp,'-', plusMin, equal) ) {
    if ( !acceptTerm(lp, deg, var, s, arAB, plusMin, equal) ) {
      return 0;
    }
  } /* no + or -, so we reached the end of the expression */
  return 1;
}

int acceptEquation(List *lp, int *deg, int *var, char *s, double *arAB, int *plusMin, int *equal){
	return acceptExpression(lp, deg, var, s, arAB, plusMin, equal) &&
	acceptCharacter(lp, '=', plusMin, equal) &&
	acceptExpression(lp, deg, var, s, arAB, plusMin, equal);
}


void recognizeEquation() {
  char *ar;
  int deg=-1, var=0, sLen, plusMin=0, equal=0;
  double arAB[2] = {0}; 
  List tl, tl1;
  printf("give an equation: ");
  ar = readInput(&sLen);
  while (ar[0] != '!') {
    char *s = malloc((sLen+1)*sizeof(char));
    tl = tokenList(ar);
    printList(tl);
    tl1 = tl;
    if ( acceptEquation(&tl1, &deg, &var, s, arAB, &plusMin, &equal) && tl1 == NULL ) {
      printf("this is an equation");
      if(var==1&&deg==-1){
        deg=1;
      }
      if(var==1){
            printf(" in 1 variable of degree %d\n", deg);
            if(deg==1){
				double n = -1*arAB[1]/arAB[0];
				if((n > -0.0005) && (n < 0.0005)){
					n = 0;
				}
				printf("solution: %.3lf\n", n);
			}
      } else {
        printf(", but not in 1 variable\n");
      }
    } else {
      printf("this is not an equation\n");
    }
    free(ar);
    free(s);
    freeTokenList(tl);
    deg=-1;
    var=0;
    arAB[0]=0;
    arAB[1]=0;
    plusMin=0;
    equal=0;
    printf("\ngive an equation: ");
    ar = readInput(&sLen);
  }
  free(ar);
  printf("good bye\n");
}


