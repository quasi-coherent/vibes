/* gen.c -- generate .vibe files from strings
 *
 * cc -std=c99 -o gen gen.c
 * ./gen "Hello, World!" > hello.vibe
 *
 *   PUSH = 1, EMIT = 25-35 (we use 30), HALT = 71+ (we use 80)
 */
#include<stdio.h>

static void emit_vibes(int n){
  for(int i=0;i<n;i++){
    if(i>0)putchar(' ');
    printf("vibe");
  }
  putchar('\n');
}

int main(int argc,char**argv){
  if(argc<2){fprintf(stderr,"usage: gen \"string\"\n");return 1;}
  const char*s=argv[1];
  while(*s){
    emit_vibes(1);   /* PUSH */
    emit_vibes(*s);  /* ASCII value */
    emit_vibes(20);  /* EMIT (17-24, middle of the window) */
    s++;
  }
  emit_vibes(1);     /* PUSH */
  emit_vibes(10);    /* newline */
  emit_vibes(20);    /* EMIT */
  emit_vibes(80);    /* HALT */
  return 0;
}
