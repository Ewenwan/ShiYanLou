/*
 * 文件名:	LCD12864.h
 * 用途:	LCD12864程序外部调用头文件
 * 作者: 万有文
 * 时间:	2015.2.20
 *
 */
  #ifndef LCD_12864_H
  #define LCD_12864_H
  void delay();                      //毫秒延时函数
  void LCDPort_Init();               //LCD液晶显示端口初始化函数
  void LCD_Write_cmd();              //向LCD写命令函数
  void Write_cmd();                  //写命令函数
  void LCD_Write_Byte();             //写数据函数
  void Write_char();
  void Write_data();
  void LCD_Init();                   //LCD初始化函数
  void LCD_pos();                    //LCD位置显示位置函数
  void Set_pos();                    //自然习惯的位置
  void LCD_flash();                  //LCD液晶闪烁函数
  void Write_Data();                 //写数字函数
  #endif /* LCD_12864_H*/
