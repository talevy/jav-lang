
class A {
      int f(int a){
         if (a<=1){ return 1; }
	 return a * f(a-1);
      }

      void main(){
	int c, d;
	c = 4;
	output f(c);
	   
      }

}
