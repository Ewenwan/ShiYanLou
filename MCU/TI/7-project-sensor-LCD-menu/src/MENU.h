//Timer_A 库函数文件  库函数的头文件  ======================== TA_PWM.h====================//
  #ifndef MENU_H_
  #define MENU_H_
  extern void welcom_m();
  extern void gund_1();
  extern void gund_2();
  extern void gund_3();
  extern void gund_4();
  extern void manu_1();
  extern void manu_211();
  extern void manu_212();
  extern void manu_22();

  extern void manu_31();
  extern void manu_32();
  extern void manu_33();
  extern void manu_34();
  extern void manu_35();
  extern void manu_36();
  extern void manu_37();

  typedef unsigned char u8;                              //数据类型定义
  typedef unsigned int  u16;

  extern u16 T_set,M_set,CO_set,CH4_set,jq_set,ben_set,pm_set;
  extern u8 pm_dj,baoj;

  #endif /* TA_PWM_H_ */
