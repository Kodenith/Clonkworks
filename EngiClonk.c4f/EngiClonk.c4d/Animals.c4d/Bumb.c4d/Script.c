#strict 2
#Include ANIM

//Movement Goals
local gX;
local gY;
local Walkdir;

local Beenergy;
local MaxBeenergy;
local BeeState;

local GrudgeTarget;

// 0 - Fly Wander
// 1 - Go to ground
// 2 - Roam Ground
// 3 - Rest
// 4 - Attack (TBA)
// 5 - Gather (TBA)

func Initialize() {
	BeeState = 0;
	MaxBeenergy = RandomX(1000,3500);
	Beenergy = MaxBeenergy;
  SetAction("Fly");
  gX=0;
  gY=0;
  
  return(1);
}

func Activity(){
	
	if(GetXDir() > 0 && GetDir() == DIR_Left)  return(TurnRight());
	if(GetXDir() < 0 && GetDir() == DIR_Right) return(TurnLeft());
	
	if(!Random(15)) Sound("Bsss");
	
	//Random Wandering (Sky)
  if(BeeState == 0){
	if(GetAction() == "Idle") SetAction("Fly");
	if(GetAction() == "Walk") SetAction("Fly");
	if(!Random(50)|| Distance(GetX(),GetY(),gX,gY) <= 5 || GBackSolid(gX-GetY(),gY-GetY()) || GBackLiquid(gX-GetY(),gY-GetY())){
		gX = GetX() + RandomX(-1000,1000);
		gY = GetY() + RandomX(-1000,1000);
	}
	
	if(gY < 50) gY = GetY() + RandomX(100,2000);
	if(gX < 50) gX = GetX() + RandomX(1000,2000);
	if(gX > LandscapeWidth() - 50) gX = GetX() - RandomX(1000,2000);
	
	if(Beenergy <= 200) BeeState = 1;
	
	Beenergy -= RandomX(1,3);
	SetCommand(this(), "MoveTo", ,gY,gY);
  }
  
  //Going to ground
  if(BeeState == 1){
	  gX = GetX() + RandomX(-10,10);
	  gY= LandscapeHeight();
	  
	  if(GetContact(this(), -1) & CNAT_Bottom){
		  if(GetEnergy() <= 50){
			  BeeState = 3;
			  return(0);
		  }
		  else BeeState = 2;
	  }
	  
	  if(GBackLiquid(0, 30)){
		gX = GetX() + RandomX(-1000,1000);
		gY = GetY()-10;
	}
	
	if(gY < 50) gY = GetY() + RandomX(100,2000);
	if(gX < 50) gX = GetX() + RandomX(1000,2000);
	if(gX > LandscapeWidth() - 50) gX = GetX() - RandomX(1000,2000);
	  
	  Beenergy -= RandomX(1,5);
	  SetCommand(this(), "MoveTo", ,gY,gY);
  }
  
  //Roaming Ground
  if(BeeState == 2){
	  if(GetContact(this(), -1) & CNAT_Bottom){
		if(GetAction() == "Idle") SetAction("Walk");
		if(GetAction() == "Fly") SetAction("Walk");
	  }else{
		 SetAction("Idle");
		 return(0);
	  }
	  
	  if(!Random(7)){
	    var direction = Random(2);
		if(!direction){
			Walkdir = COMD_Stop;
		}else if(direction == 1){
			Walkdir = COMD_Left;
		}else{
			Walkdir = COMD_Right;
		}
		
		SetComDir(Walkdir);
	  }
	  
	  Beenergy += RandomX(1,2);
	  if(Beenergy >= MaxBeenergy) BeeState = 0;
  }
  
  //Sleeping
  if(BeeState == 3){
	  if(GetAction() != "Rest" || GetAction() != "Tumble") SetAction("Rest");
	  if(GetAction() == "Rest"){
		  if(!Random(3))
		  DoEnergy(1);
			 
	    if(!Random(5))
	    CreateParticle("Zzz",0,0, RandomX(-10,10), RandomX(-1,-10), RandomX(25,50), RGBa(255,255,255), ,true);
		Beenergy += RandomX(1,4);
		if(Beenergy >= MaxBeenergy) BeeState = 0;
	  }
  }
}

protected func CatchBlow(int iLevel, object pByObject)
{
  if (!Random(3)) Sound("BumbHurt*");
  return(1);
}

protected func Death() 
{
  Sound("BumbDie*");
  SetDir(0);
  ChangeDef(DBUB);
  SetAction("Dead");
  return(1);
}

func RejectCollect(c4ID, pObject){
	return(1);
}

func RejectEntrance(pIntoObject){
	if(GetAction() == "Rest") return(0);
	return(1);
}

public func TurnRight()
{
  if (GetXDir() < 0) SetXDir(0);
  SetDir(DIR_Right, this());
  SetComDir(COMD_Right); 
  return(1);
}

public func TurnLeft()
{
  if (GetXDir() > 0) SetXDir(0);
  SetDir(DIR_Left);
  SetComDir(COMD_Left); 
  return(1);
}