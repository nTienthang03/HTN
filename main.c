#include <main.h>

void main(void){
  set_TRIS_D(0X00);
  set_TRIS_C(0X00);
  PORTD=0xff;
  PORTC=0x00;

  while(true){
      check_button();
      if(chedo>3) chedo = 0;
      run();
   }
}

void check_button(){
   if(input(PIN_B0) == 1) chedo = 0;
   if(input(PIN_B1) == 1) {chedo = 1; chedo_chay = 1;}
   if(input(PIN_B2) == 1) {chedo = 2; chedo_chay = 2;}
   if(input(PIN_B3) == 1) chedo = 3;
   if(input(PIN_B5) == 1) noidung = 1;
   if(input(PIN_B6) == 1) noidung = 2;
}

void load_right_to_left(unsigned int u){
   unsigned char i;
   for(i=0;i<32;i++){
      data_loaded[i] = data_column[u];
      u++;
   }
}

void load_left_to_right(unsigned int u){
   unsigned char i;
   for(i=0;i<32;i++){
      data_loaded[31-i] = data_column[u];
      u--;
   }
}

void data_latch(){
   for(tocdo=0;tocdo<10;tocdo++){
      send_data();
   }
}

void run(void){
   switch(noidung){
      case 1:  
         gioihandau = 0;
         gioihancuoi = 63;
         break;
      case 2:  
         gioihandau = 64;
         gioihancuoi = 208;
         break;
   }
   
   switch(chedo){
      case 0: chedo_macdinh();  break;
      case 1: chedo1(); break;
      case 2: chedo2(); break;
      case 3: chedo3(); break;
   }
}

void send_data() {
   unsigned char i;
   a=0xfe;
   h=0x80;
   for(sohang=0;sohang<8;sohang++){
      st=0;
      for(i=0;i<32;i++){
         check_button();
         sh=0;
         g = data_loaded[i];
         if((g&h)==h) ds=1;
         else ds=0;
         sh=1;
      }
      st=1;
      h=h>>1;
      PORTD=a;
      delay_ms(5);
      PORTD=0xff;
      a=(a<<1)+1;
   }
}

void chedo_macdinh(){
   noidung = 1;
   for(int8 i=0;i<32;i++) data_loaded[i] = data_column[i+32];
   send_data();
}

void chedo1() {
   for(socot = gioihandau; socot < gioihancuoi; socot++) {
      load_right_to_left(socot);
      data_latch();
   }
}

void chedo2() {
   for(socot = gioihancuoi; socot > gioihandau; socot--) {
      load_left_to_right(socot+31);
      data_latch();
   }
}

void chedo3() {
   if(chedo_chay == 1) {
      for(socot = gioihandau; socot < gioihancuoi; socot++) {
         if(dem%5==0) {
            load_right_to_left(0);
            delay_ms(500);
         }
         else load_right_to_left(socot);
         data_latch();
         if(dem <500) dem++;
         else dem = 0;
      }
   }
   
   if(chedo_chay == 2) {
      for(socot = gioihancuoi; socot > gioihandau; socot--) {
         if(dem%5==0) {
            load_right_to_left(0);
            delay_ms(500);
         }
         else load_left_to_right(socot+31);
         data_latch();
         if(dem <500) dem++;
         else dem = 0;
      }
   }
}
