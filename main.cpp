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
DigitalInputPin bumpleft1(FEHIO::P0_0);
DigitalInputPin bumpleft2(FEHIO::P0_1);
DigitalInputPin bumptop1(FEHIO::P0_2) ;
DigitalInputPin bumptop2(FEHIO::P0_3);
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
    float h = percent/100.0,dRad = 0,vert = 0,horz = 0;
    int mot1,mot2,mot3,mot4,x,y;
    //motors are in ascending order starting at bottom right

    dRad = ((degree * M_PI)/180.0)+(M_PI/4);

    vert = h*sin(dRad);
    horz = h*cos(dRad);
    y=abs(vert*100);
    x=abs(horz*100);

    if(degree>=135 && degree<315){
        mot1=y;
        mot3=-y;
    }else{
        mot1=-y;
        mot3=y;
    }

    if(degree>=45 && degree<225){
        mot2=-x;
        mot4=x;
    }else{
        mot2=x;
        mot4=-x;
    }

    botright_motor.SetPercent(mot1);
    topright_motor.SetPercent(mot2);
    topleft_motor.SetPercent(mot3+1);
    botleft_motor.SetPercent(mot4);
    /*LCD.WriteLine("X Ratio:");
    LCD.WriteLine(horz);
    LCD.WriteLine("Y Ratio:");
    LCD.WriteLine(vert);
    LCD.WriteLine("motor1:");
    LCD.WriteLine(mot1);
    LCD.WriteLine("motor2:");
    LCD.WriteLine(mot2);
    LCD.WriteLine("motor3:");
    LCD.WriteLine(mot3);
    LCD.WriteLine("motor4:");
    LCD.WriteLine(mot4);*/
}

void HitJack(){
    //down from start
    DriveHeading(90,40);
    Sleep(1.1);
    Stop();
    //left towards jack
    DriveHeading(0,40);
    Sleep(2.2);
    Stop();
    //down to jack wall
    while(bumptop1.Value() || bumptop2.Value()){
    DriveHeading(90,40);
    }
    Stop();
    //hit jack
    DriveHeading(180,40);
    Sleep(1.25);
    Stop();
}

void SecondPart(){
    //back off of jack
    DriveHeading(0,40);
    Sleep(.1);
    Stop();
    //square against wall
    //down to jack wall
    while(bumptop1.Value() || bumptop2.Value()){
    DriveHeading(90,40);
    }
    Stop();
    //up
    DriveHeading(270,40);
    Sleep(3.5);
    Stop();
    //over to far wall
    while(bumpleft1.Value() || bumpleft2.Value()){
        DriveHeading(180,40);
    }
    Stop;
    //hit button panel
    while(bumptop2.Value()){
    DriveHeading(90,40);
    }
    Stop();
    //square up
    //over to far wall
    while(bumpleft1.Value() || bumpleft2.Value()){
        DriveHeading(180,40);
    }
    Stop;
    //up ramp
    DriveHeading(270,80);
    Sleep(5.0);
    Stop();
}

double ReRead(){
    double min=2.3,cur=2.3;
    double start=0;
    cur=cdsCell.Value();
    DriveHeading(180,20);
    Sleep(.05);
    start = TimeNow();
    while(TimeNow()-start<.5){
    cur=cdsCell.Value();
    if(cur<min){
    min=cur;
    }
    }
    Stop();
    LCD.WriteLine("Min:");
    LCD.WriteLine(min);
    Sleep(5.0);
    return min;
}

int ReadCdS(){

    int col=0;
    double val = ReRead();
    if (val<.5){
        col=1;
        LCD.Clear( FEHLCD::Black);
        LCD.WriteLine("Red");
    }else if (val<.8){
        col = 2;
        LCD.Clear( FEHLCD::Black);
        LCD.WriteLine("Blue");
    }else{
        LCD.Clear( FEHLCD::Black);
        LCD.WriteLine("Something Fucked Up");
        LCD.Write(cdsCell.Value());
    }
    return col;
}

void PressButton(int color){
    int dir = 0;
if (color == 2){
   dir = 180;
}
DriveHeading(dir,40);
Sleep(.60);
Stop();
DriveHeading(90,60);
Sleep(.5);
Stop();
DriveHeading(270,60);
Sleep(.5);
Stop();
}

void toCdS(){
    int color=0;
    //down from start
    DriveHeading(90,40);
    Sleep(1.9);
    Stop();
    //over to diagnostic light
    DriveHeading(180,40);
    Sleep(1.9);
    Stop();
    color = ReadCdS();
    Stop();
    PressButton(color);
}

void HitWrench(){
//over to far wall
    LCD.WriteLine("Off to Wrench");
    while(bumpleft1.Value() || bumpleft2.Value()){
    DriveHeading(180,40);
}
    Stop();
    DriveHeading(0,40);
    Sleep(4.7);
    DriveHeading(90,40);
    Sleep(1.2);
    DriveHeading(0,40);
    Sleep(.8);
    Stop();
    while(bumpleft2.Value()){
    DriveHeading(180,40);
    }
    Stop();
    DriveHeading(270,40);
    Sleep(3.6);
    Stop();
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
            toCdS();
            HitWrench();


        }



    }
    return 0;
}
