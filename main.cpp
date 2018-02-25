#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <math.h>
#define M_PI 3.14159265358979323846

FEHMotor botright_motor(FEHMotor::Motor0,7.2);
FEHMotor topright_motor(FEHMotor::Motor1,7.2);
FEHMotor topleft_motor(FEHMotor::Motor2,7.2);
FEHMotor botleft_motor(FEHMotor::Motor3,7.2);
DigitalInputPin bump_switch(FEHIO::P0_1);
AnalogInputPin cdsCell(FEHIO::P3_7);



void StartLight()
{

while(cdsCell.Value()>2.3){
}
LCD.WriteLine("Light Active");

}

void Stop(){
    botright_motor.Stop();
    topright_motor.Stop();
    topleft_motor.Stop();
    botleft_motor.Stop();
}

void DriveHeading(double degree,double percent){
    float vertRad = 0, horzRad = 0,h = percent/100.0,dRad = 0,vert = 0,horz = 0;
    int mot1,mot2,mot3,mot4,x,y;
    double rem = fmodf(degree,90.0);
    //motors are in ascending order starting at bottom right
    dRad = ((degree * M_PI)/180.0)+(M_PI/4);

    vert = h*sin(dRad);
    horz = h*cos(dRad);
    y=abs(vert*100);
    x=abs(horz*100);
    LCD.WriteLine(x);
    LCD.WriteLine(y);

    if(degree<=0 && degree <90){
        mot1=-y;
        mot2=x;
        mot3=y;
        mot4=-x;
    }else if(degree<=90 && degree <180){
        mot1=-y;
        mot2=-x;
        mot3=y;
        mot4=x;
    }else if(degree<=180 && degree <270){
        mot1=y;
        mot2=-x;
        mot3=-y;
        mot4=x;
    }else{
        mot1=y;
        mot2=x;
        mot3=-y;
        mot4=-x;
    }

    botright_motor.SetPercent(mot1);
    topright_motor.SetPercent(mot2);
    topleft_motor.SetPercent(mot3);
    botleft_motor.SetPercent(mot4);
    LCD.WriteLine("X Ratio:");
    LCD.WriteLine(horz);
    LCD.WriteLine("Y Ratio:");
    LCD.WriteLine(vert);


}

int main(void)
{

    float x,y;

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    while( true )
    {
        if( LCD.Touch(&x,&y) )
        {
            StartLight();
            LCD.WriteLine( "Import More Memes");
            DriveHeading(90,40);
            Sleep(1.1);
            Stop();
            DriveHeading(0,40);
            Sleep(2.2);
            Stop();
            while(bump_switch.Value()){
            DriveHeading(90,40);
            }
            Stop();
            DriveHeading(180,40);
            Sleep(1.5);
            Stop();


        }



    }
    return 0;
}
