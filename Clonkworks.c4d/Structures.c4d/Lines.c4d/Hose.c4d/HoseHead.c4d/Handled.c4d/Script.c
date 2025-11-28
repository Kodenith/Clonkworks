/*-- Neues Script --*/

#strict 2
#include PCN2

local IsOn;
func Initialize() {
  return(1);
}

public func IsHoseOn(){ return(IsOn); }

public func Fire()
{
  Sound("Click");
  IsOn = !IsOn;
  return(1);
}

//other
func ReturnToObj(Cooldown){
	Sound("Click");
	ChangeDef(HOSH);
	SetCategory(C4D_Object);
	if(Cooldown){
		this()->ApplyCooldown();
	}
}

public func Spew(int Amount, string Mat, int minPower, int maxPower){
	var angle;
	
	angle = GetR()-90;
	 
	 for(var i = 0; i < Amount; i++){
		 CastPXSX(Mat, 1, RandomX(minPower,maxPower),0,-3,angle);
	 }
}