/*  输入欧拉角，能看到四元数，以及再转换回去成欧拉角
    Yaw范围（-180~180）
    Pitch范围（-90~90）
    Roll范围（-180~180）
*/

#include "stdio.h"
#include "math.h"


main()
{
float theta_z , theta_y ,theta_x ;

float cos_z_2;
float cos_y_2;
float cos_x_2;

float sin_z_2;
float sin_y_2;
float sin_x_2;

float Pitch;
float Roll;
float Yaw;
float Q[4];
float   T[3][3];
do{
printf(\"\
Yaw = \");
scanf(\"%f\",&theta_z);
printf(\"\
Pitch = \");
scanf(\"%f\",&theta_y);
printf(\"\
Roll = \");
scanf(\"%f\",&theta_x);

theta_z = theta_z*3.1416/180;
theta_y = theta_y*3.1416/180;
theta_x = theta_x*3.1416/180;

cos_z_2 = cos(0.5*theta_z);
cos_y_2 = cos(0.5*theta_y);
cos_x_2 = cos(0.5*theta_x);

sin_z_2 = sin(0.5*theta_z);
sin_y_2 = sin(0.5*theta_y);
sin_x_2 = sin(0.5*theta_x);

Q[0] = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
Q[1] = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
Q[2] = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
Q[3] = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

printf(\"\
Q=[ %f %f %f %f]\
\
\",Q[0],Q[1],Q[2],Q[3]) ;
printf(\"alpha = %f\
\
\",acos(Q[0])*2*180/3.1416) ;

    T[0][0] =   Q[0]*Q[0]+Q[1]*Q[1]-Q[2]*Q[2]-Q[3]*Q[3] ;
    T[0][1] =                    2*(Q[1]*Q[2]-Q[0]*Q[3]);
    T[0][2] =                    2*(Q[1]*Q[3]+Q[0]*Q[2]);

    T[1][0] =                    2*(Q[1]*Q[2]+Q[0]*Q[3]);
    T[1][1] =   Q[0]*Q[0]-Q[1]*Q[1]+Q[2]*Q[2]-Q[3]*Q[3] ;
    T[1][2] =                    2*(Q[2]*Q[3]-Q[0]*Q[1]);

    T[2][0] =                    2*(Q[1]*Q[3]-Q[0]*Q[2]);
    T[2][1] =                    2*(Q[2]*Q[3]+Q[0]*Q[1]);
    T[2][2] =   Q[0]*Q[0]-Q[1]*Q[1]-Q[2]*Q[2]+Q[3]*Q[3] ;

    printf(\"T[0][0] = %9f,T[0][1] = %9f,T[0][2] = %9f\
\",T[0][0],T[0][1],T[0][2]);
    printf(\"T[1][0] = %9f,T[1][1] = %9f,T[1][2] = %9f\
\",T[1][0],T[1][1],T[1][2]);
    printf(\"T[2][0] = %9f,T[2][1] = %9f,T[2][2] = %9f\
\
\",T[2][0],T[2][1],T[2][2]);

    Pitch = asin(-T[2][0]);
    Roll  = atan( T[2][1]/T[2][2]);
    Yaw   = atan( T[1][0]/T[0][0]);

    if(T[2][2]<0)
    {
        if(Roll < 0)
        {
           Roll = Roll+3.1416;
        }
        else
        {
           Roll = Roll-3.1416;
        }
    }

    if(T[0][0]<0)
    {
        if(T[1][0]>0)
        {
            Yaw = Yaw + 3.1416;
        }
        else
        {
            Yaw = Yaw - 3.1416;
        }
    }

    printf(\"Yaw   = %f\
Pitch = %f\
Roll  = %f\
\",Yaw*180/3.1416,Pitch*180/3.1416,Roll*180/3.1416) ;
}while(1);
    printf(\"Hello, world\
\");
    getch();
}