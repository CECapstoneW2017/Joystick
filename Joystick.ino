#include <lc-addrlabels.h>
#include <lc-switch.h>
#include <lc.h>
#include <pt-sem.h>
#include <pt.h>

void setup() {
  // put your setup code here, to run once:

}

//This is a skeleton of a protothread. 
static int protothread1(struct pt *pt) { 
  PT_BEGIN(pt); 
  while(1) {           
    PT_WAIT_UNTIL(pt, true);  
  } 
  PT_END(pt);
}

void loop() {
  // put your main code here, to run repeatedly:

}
