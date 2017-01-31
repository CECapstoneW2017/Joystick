
#include <pt.h>

// every protothread needs an own struct pt variable
static struct pt pt1;

void setup() {
  // put your setup code here, to run once:
  /* Initialize the protothread state variables with PT_INIT(). */     
  PT_INIT(&pt1); 

}

//This is a skeleton of a protothread. 
static int protothread1(struct pt *pt) { 
  PT_BEGIN(pt); 
  while(1) {           
    PT_WAIT_UNTIL(pt, false);  
  } 
  PT_END(pt);
}

void loop() {
  // put your main code here, to run repeatedly:

}
