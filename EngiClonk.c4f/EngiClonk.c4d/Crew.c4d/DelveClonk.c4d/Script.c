#strict 2

#include CLNK

local ControlDirection;

/* Air Control + WallJumps */
func ControlLeft(){
	//must be climbing to wall jump
	if(GetAction() == "Scale" || GetAction() == "ScaleDown"){
		//is facing right?
		if(GetDir() == 1){
			//Do The Jump
			Fling(this(), -(100000/(110000-GetPhysical("Jump", 0, 0)))-2,-3);
			SetDir(0);
			SetAction("Jump");
		}
	}
	
	return(_inherited());
}

func ControlRight(){
	//must be climbing to wall jump
	if(GetAction() == "Scale" || GetAction() == "ScaleDown"){
		//is facing left?
		if(GetDir() == 0){
			//Do The Jump
			Fling(this(), (100000/(110000-GetPhysical("Jump", 0, 0)))+2,-3);
			SetDir(1);
			SetAction("Jump");
		}
	}
	
	return(_inherited());
}

protected func ControlUpdate(object self, int comdir, bool dig, bool throw)
{
  ControlDirection = 0;
  if(comdir == 3) ControlDirection = 1;
  if(comdir == 7) ControlDirection = -1;
  return(_inherited(self, comdir, dig, throw));
}

func DoFallControl(){
	//fall control logic
	if(GetAction() == "Jump"){
		if(GetDir() == DIR_Right && ControlDirection == 1){
			SetXDir(GetXDir()+ControlDirection*2);
			return(0);
		}
		if(GetDir() == DIR_Left && ControlDirection == -1){
			SetXDir(GetXDir()+ControlDirection*2);
			return(0);
		}
		SetXDir(GetXDir()+ControlDirection*3);
	}
	
	if(GetAction() == "Tumble"){
		SetXDir(GetXDir()+ControlDirection*Random(3));
	}
}