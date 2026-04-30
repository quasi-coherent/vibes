/* dbg.c -- vibe disassembler
 *
 * cc -std=c99 -o dbg dbg.c
 * ./dbg program.vibe
 */
#include<stdio.h>
#include<string.h>
#include<ctype.h>

static const int fib_lo[]={1,2,4,7,11,17,25,36,45,56,71};
static const char*names[]={"PUSH","POP","ADD","SUB","DUP",
                            "EMIT","PRINT","SWAP","JZ","JNZ","HALT"};
#define N_OPS 11

static int decode(int v){
  if(v<=0)return -1;
  if(v>=fib_lo[N_OPS-1])return N_OPS-1;
  for(int i=N_OPS-1;i>=0;i--)
    if(v>=fib_lo[i])return i;
  return -1;
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

int main(int argc,char**argv){
  if(argc<2){fprintf(stderr,"usage: dbg file.vibe\n");return 1;}
  FILE*f=fopen(argv[1],"r");
  if(!f){fprintf(stderr,"can't open %s\n",argv[1]);return 1;}
  char buf[8192];
  int line=0,ctx=0;
  while(fgets(buf,sizeof(buf),f)){
    int v=count_vibes(buf);
    if(v==0)continue;
    if(ctx==1){
      printf("%4d: [value %d]\n",line,v);
      ctx=0;
    }else if(ctx==2){
      printf("%4d: [jump -> line %d]\n",line,v);
      ctx=0;
    }else{
      int op=decode(v);
      if(op>=0)
        printf("%4d: %s (%d vibes)\n",line,names[op],v);
      else
        printf("%4d: ??? (%d vibes)\n",line,v);
      if(op==0)ctx=1;
      else if(op==8||op==9)ctx=2;
    }
    line++;
  }
  printf("\n%d vibe-lines total\n",line);
  fclose(f);
  return 0;
}
