/*函数的递归
 * 6!=6*5*4*3*2*1
 * digui(num)=num * digui(num-1)
 *    (num-1) *(num-1-1)!
 *      ... ...
 * 
 */
void setup() {
  Serial.begin(115200);
  Serial.println();
  int n;
  n = digui(6);
Serial.println(n);
}

void loop() {

}

int digui(int num){
  if(num > 1){
    num = num * digui(num - 1);
  }
  return num;
}
