/*
 * circle.c
 *
 *  Created on: 2016-4-5
 *      Author: Administrator
 */
#include<math.h> // 引用 求平方根 sqrt、绝对值fabs 函数


// 浮点数判同   是否相等
int double_equals(float const a, float const b)
{
    static const float ZERO = 1e-9;
    return fabs(a - b) < ZERO;
}
// 两点之间距离的平方
float distance_sqr(float a1, float b1,float a2, float b2)
{
    return (a1-a2) * (a1-a2) + (b1-b2) * (b1-b2);    //dx^ + dy^
}


// 两点之间的距离
float distance(float a1, float b1,float a2, float b2)
{
	float mm;
	mm=((a1-a2) * (a1-a2) + (b1-b2) * (b1-b2));
    return sqrt(mm);
}

/*
* 两圆相交函数
* 参数:
* 输入 circle[0]～circle[5]   分别为圆 的 X1 Y1 R1 X2 Y2 R2
* 输出 points[0]～points[3] 分别为交点的坐标值
* 如果用到了两个交点, 那么返回后, 横坐标大的在前, 如果横坐标一样, 则纵坐标大的在前.
* 返回值:
*    -1 如果两个圆一模一样;
*    其它整数值: 交点个数.
*/
int insect(float *circle, float*points)
{
    float d, a, b, c, p, q, r;       // a, b, c, p, q, r (用参数方程求解两圆交点需要用到的变量)
    float cos_value[2], sin_value[2];// 交点的在 circles[0] 上对应的正余弦取值
                                       // 余弦值 cos_value 参数方程里的 cosθ
    //两个圆重合（同一个圆）    返回-1
  if(double_equals(circle[0],circle[3])&& double_equals(circle[1],circle[4])&& double_equals(circle[2], circle[5]))
         {
          return -1;
         }
    //两个圆心的距离
    d = distance(circle[0], circle[1],circle[3], circle[4]); // 圆心距离
   
    //两圆不相交   （圆外相离和圆内相离）   返回0
    if (d > circle[2] + circle[5] || d < fabs(circle[2] - circle[5])) 
              {  return 0;
              }

    a = 2.0 * circle[2] * (circle[0] - circle[3]);
    b = 2.0 * circle[2] * (circle[1] - circle[4]);
    c = circle[5] * circle[5] - circle[2] * circle[2] - d * d;
    p = a * a + b * b;
    q = -2.0 * a * c;


    // 如果交点仅一个                  外切或内切
    if (double_equals(d,(circle[2] + circle[5])) || double_equals(d, fabs(circle[2] - circle[5])))
      {
        cos_value[0] = -q / p / 2.0;                            //cos值
        sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);   //sin值

        points[0]  = circle[2] * cos_value[0] + circle[0];
        points[1]  = circle[2] * sin_value[0] + circle[1];

        // 在这里验证解是否正确, 如果不正确, 则将纵坐标符号进行变换    求出的交点是否在圆2上
        if(!double_equals(distance_sqr(points[0],points[1],circle[3],circle[4]),circle[5] * circle[5])) 
          {
            points[1] = circle[1] - circle[2] * sin_value[0];
          }
        return 1;
      }

   //两个交点
    r = c * c - b * b;
    cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
    cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);
    sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);

    points[0] = circle[2] * cos_value[0] + circle[0];                //x=r*cos +x0
    points[1] = circle[2] * sin_value[0] + circle[1];                //y=r*sin +y0
    points[2] = circle[2] * cos_value[1] + circle[0];  
    points[3] = circle[2] * sin_value[1] + circle[1]; 

    // 验证解是否正确, 两个解都需要验证.
    if (!double_equals(distance_sqr(points[0],points[1],circle[3],circle[4]),circle[5] * circle[5]))
        {
         points[1] = circle[1] - circle[2] * sin_value[0];
        }
    if (!double_equals(distance_sqr(points[2],points[3],circle[3],circle[4]),circle[5] * circle[5]))
        {
         points[3] = circle[1] - circle[2] * sin_value[1];
        }
    // 如果求得的两个点坐标相同, 则必然其中一个点的纵坐标反号可以求得另一点坐标
    if (double_equals(points[0], points[2]) && double_equals(points[1], points[3])) 
        {
        if(points[1]> 0)
          {  points[3] = -points[3];
          } 
        else 
          {   points[1]= -points[1];
           }
         }
    return 2;
}

/*
*输入三圆  x1 y1 r1 x2 y2 r2 x3 y3 r3
*输出  三圆交汇中心点坐标
*/

 int  threecircle(float *cir, float *point)
  {
      float points1[4],points2[4],points3[4];
      float circle1[6],circle2[6],circle3[6];
      int points1flag=0,points2flag=0,points3flag=0;
      int i,j,k,t;
      for(i=0;i<6;i++) 
       { circle1[i]=cir[i];}
      for(j=0;j<6;j++) 
       { circle2[j]=cir[j+3];}
      for(k=0;k<3;k++) 
       { circle3[k]=cir[k];}
      for(t=3;t<6;t++)
       { circle3[t]=cir[t+3];}
   if(insect(circle1,points1)==2)
       {
         if(distance_sqr(points1[0],points1[1],cir[6],cir[7])>(cir[8]*cir[8]))    
          points1flag=1;  else points1flag=0;
       }
   if(insect(circle2,points2)==2)
       {
         if(distance_sqr(points2[0],points2[1],cir[0],cir[1])>(cir[2]*cir[2]))    
          points2flag=1;  else points2flag=0;
       }
   if(insect(circle3,points3)==2)
       {
         if(distance_sqr(points3[0],points3[1],cir[3],cir[4])>(cir[5]*cir[5]))
          points3flag=1;  else points3flag=0;
       }

   if((points1flag==0)&&(points2flag==0)&&(points3flag==0))      
      {
       point[0]=(points1[0]+points2[0]+points3[0])/3.0;
       point[1]=(points1[1]+points2[1]+points3[1])/3.0;
       }
   if((points1flag==0)&&(points2flag==0)&&(points3flag==1))      
      {
       point[0]=(points1[0]+points2[0]+points3[2])/3.0;
       point[1]=(points1[1]+points2[1]+points3[3])/3.0;
       }
   if((points1flag==0)&&(points2flag==1)&&(points3flag==0))      
      {
       point[0]=(points1[0]+points2[2]+points3[0])/3.0;
       point[1]=(points1[1]+points2[3]+points3[1])/3.0;
       }
   if((points1flag==0)&&(points2flag==1)&&(points3flag==1))      
      {
       point[0]=(points1[0]+points2[2]+points3[2])/3.0;
       point[1]=(points1[1]+points2[3]+points3[3])/3.0;
       }
  if((points1flag==1)&&(points2flag==1)&&(points3flag==1))      
      {
       point[0]=(points1[2]+points2[2]+points3[2])/3.0;
       point[1]=(points1[3]+points2[3]+points3[3])/3.0;
       }
  if((points1flag==1)&&(points2flag==0)&&(points3flag==0))      
      {
       point[0]=(points1[2]+points2[0]+points3[0])/3.0;
       point[1]=(points1[3]+points2[1]+points3[1])/3.0;
       }
  if((points1flag==1)&&(points2flag==0)&&(points3flag==1))      
      {
       point[0]=(points1[2]+points2[0]+points3[2])/3.0;
       point[1]=(points1[3]+points2[1]+points3[3])/3.0;
       }
if((points1flag==1)&&(points2flag==1)&&(points3flag==0))      
      {
       point[0]=(points1[2]+points2[2]+points3[0])/3.0;
       point[1]=(points1[3]+points2[3]+points3[1])/3.0;
       }
return 1;
  }
