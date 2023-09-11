#include <iostream>

using namespace std;

class Animal {
   private: 
	  int position;  
   public:
	  Animal(int pos) : position(pos) {}
	  // return new postion 
	  int walk(int quantity) {

              return position += quantity;   
	  } 
};

class Cat : public Animal {
     public:
            Cat(int pos) : Animal(pos) {}  		
      void meow() {}
      void destroySofa() {}
      bool wildMood() {return true;}
};

int main() {
   Cat c(50);
   c.meow();
   if (c.wildMood()) {
      c.destroySofa();
   }
   
   c.walk(2);
   return 0;
}
