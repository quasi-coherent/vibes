/* vibe.c -- the vibe language
 *
 * cc -std=c99 -o vibe vibe.c
 *
 * A language where the only token is the word "vibe".
 * Everything else is ignored. Zero-vibe lines are comments.
 *
 * Opcodes are fuzzy -- you don't need the exact count, just roughly right.
 *
 *   1       vibe  = PUSH (next line's count is the value, exact)
 *   2-3     vibes = POP
 *   4-6     vibes = ADD
 *   7-10    vibes = SUB
 *   11-16   vibes = DUP
 *   17-24   vibes = EMIT (ASCII char)
 *   25-35   vibes = PRINT (number)
 *   36-44   vibes = SWAP
 *   45-55   vibes = JZ (next line = target, exact)
 *   56-70   vibes = JNZ (next line = target, exact)
 *   71+     vibes = HALT
 *
 * Eleven opcodes. Values after PUSH are exact.
 * Jump targets after JZ/JNZ are exact.
 * Everything else is a vibe.
 *
 * Just stop thinking.
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_LINES 4096
#define STACK_SIZE 1024

static int ops[MAX_LINES];
static int nlines=0;

static int stack[STACK_SIZE];
static int sp=0;

enum{PUSH=1,POP,ADD,SUB,DUP,EMIT,PRINT,SWAP,JZ,JNZ,HALT};
static const int fib_lo[]={1,2,4,7,11,17,25,36,45,56,71};
#define N_OPS 11

static const char*opname(int op){
  const char*names[]={"?","PUSH","POP","ADD","SUB","DUP",
                       "EMIT","PRINT","SWAP","JZ","JNZ","HALT"};
  return(op>=1&&op<=11)?names[op]:"???";
}

static int decode(int v){
  if(v<=0)return 0;
  if(v>=fib_lo[N_OPS-1])return HALT;
  for(int i=N_OPS-1;i>=0;i--)
    if(v>=fib_lo[i])return i+1;
  return 0;
}

static int count_vibes(const char*line){
  int n=0;
  const char*p=line;
  while(*p){
    while(*p&&!isalpha(*p))p++;
    if(!*p)break;
    const char*w=p;
    while(*p&&isalpha(*p))p++;
    int len=p-w;
    if(len==4&&(strncmp(w,"vibe",4)==0||strncmp(w,"VIBE",4)==0||
                strncmp(w,"Vibe",4)==0))
      n++;
  }
  return n;
}

static void load_file(const char*path){
  FILE*f=fopen(path,"r");
  if(!f){fprintf(stderr,"bad vibes: can't open %s\n",path);exit(1);}
  char buf[8192];
  while(fgets(buf,sizeof(buf),f)&&nlines<MAX_LINES){
    int v=count_vibes(buf);
    if(v>0)ops[nlines++]=v;
  }
  fclose(f);
}

static void push_s(int v){
  if(sp<STACK_SIZE)stack[sp++]=v;
  else{fprintf(stderr,"vibe overflow\n");exit(1);}
}

static int pop_s(void){
  if(sp>0)return stack[--sp];
  fprintf(stderr,"vibe underflow\n");exit(1);
  return 0;
}

static void run(void){
  int ip=0;
  while(ip<nlines){
    int raw=ops[ip];
    int op=decode(raw);
    switch(op){
      case PUSH:
        ip++;
        if(ip<nlines){push_s(ops[ip]);ip++;}
        break;
      case POP:
        pop_s();ip++;break;
      case ADD:
        {int b=pop_s(),a=pop_s();push_s(a+b);}ip++;break;
      case SUB:
        {int b=pop_s(),a=pop_s();push_s(a-b);}ip++;break;
      case DUP:
        {int a=pop_s();push_s(a);push_s(a);}ip++;break;
      case SWAP:
        {int b=pop_s(),a=pop_s();push_s(b);push_s(a);}ip++;break;
      case EMIT:
        putchar(pop_s());ip++;break;
      case PRINT:
        printf("%d",pop_s());ip++;break;
      case JZ:
        ip++;
        if(ip<nlines){
          int target=ops[ip];
          int v=pop_s();
          if(v==0)ip=target;
          else ip++;
        }break;
      case JNZ:
        ip++;
        if(ip<nlines){
          int target=ops[ip];
          int v=pop_s();
          if(v!=0)ip=target;
          else ip++;
        }break;
      case HALT:
        return;
      default:
        ip++;break;
    }
  }
}

static void repl(void){
  printf("vibe v0.3\n");
  printf("just stop thinking.\n\n");
  char buf[8192];
  int awaiting_value=0;
  while(1){
    printf("~ ");
    fflush(stdout);
    if(!fgets(buf,sizeof(buf),stdin))break;
    int v=count_vibes(buf);
    if(v==0)continue;
    int op=decode(v);
    if(awaiting_value){
      push_s(v);
      printf("  [pushed %d]\n",v);
      awaiting_value=0;
      continue;
    }
    printf("  [%d vibes -> %s]\n",v,opname(op));
    switch(op){
      case PUSH:awaiting_value=1;break;
      case POP:pop_s();break;
      case ADD:{int b=pop_s(),a=pop_s();push_s(a+b);}break;
      case SUB:{int b=pop_s(),a=pop_s();push_s(a-b);}break;
      case DUP:{int a=pop_s();push_s(a);push_s(a);}break;
      case SWAP:{int b=pop_s(),a=pop_s();push_s(b);push_s(a);}break;
      case EMIT:putchar(pop_s());fflush(stdout);break;
      case PRINT:printf("%d\n",pop_s());break;
      case HALT:printf("good vibes.\n");return;
      default:break;
    }
  }
  printf("\ngood vibes.\n");
}

int main(int argc,char**argv){
  if(argc<2){repl();return 0;}
  load_file(argv[1]);
  run();
  return 0;
}
