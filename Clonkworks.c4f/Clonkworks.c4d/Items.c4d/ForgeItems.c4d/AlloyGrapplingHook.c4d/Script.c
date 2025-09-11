/*-- Enterhaken --*/

#strict 2
#include CW5P
#include DUMM

func Initialize(){
	return SetPicture(8, 0, 64, 64);
}

func FRGUpdate(){
	//SetColorDw(Color, this());
	 SetGraphics("2", this(),ACWP,0,1);
     SetGraphics("2", this(),ACWP,0,3);
	 SetGraphics("RopeOnly", this(),ACWP,1,1);
     SetGraphics("RopeOnly", this(),ACWP,1,3);
	len = Max(Power * 150,150);
	SetMass(5*Mass, this());
	SetClrModulation(Color, this());
}