/*-- Neues Script --*/

#strict

func Initialize() {
  SetAction("OnGround");
  return(1);
}

func CheckGround(){
	//check if its on the ground, if not - liftoff!
	if(CanGlide() && GetYDir()>10){
		SetAction("Fly");
		return(1);
	}
	return(1);
}

func Flight(){
	//flying logic
	if(!CanGlide()){
		SetAction("OnGround");
		return(0);
	}
	
	if(Abs(GetWind(0,0)) > 95) SetYDir(RandomX(2,6));
	else if(Abs(GetWind(0,0)) > 75) SetYDir(RandomX(4,6));
	else if(Abs(GetWind(0,0)) > 25) SetYDir(RandomX(6,12));
	else SetYDir(RandomX(8,16));
	
	if(Abs(GetXDir()) < Abs(GetWind(0,0)/3))
	if(GetWind(0,0) < 0){
		SetXDir(GetXDir()-1);
	}else{
		SetXDir(GetXDir()+1);
	};
	
	if(GetWind(0,0) < 0 && GetXDir() > 0) SetXDir(GetXDir()-2);
	if(GetWind(0,0) > 0 && GetXDir() < 0) SetXDir(GetXDir()+2);
	
	if(GetXDir() < 0) SetDir(0);
	if(GetXDir() > 0) SetDir(1);
}

func ContactBottom(){
	SetAction("OnGround");
}

protected func CanGlide(){
		if(GBackSky(0,0)) //is in the sky?
			if(Abs(GetWind(0,0)) > 25) //is wind good?
				if(!Contained()) // not contained
				return(1);
	return(0);
}