#strict 2
#include ANIM

//Movement Goals
local gX;
local gY;
local Walkdir;

local Beenergy;
local MaxBeenergy;
local BeeState;

local GrudgeTarget;

local HomeX;
local HomeY;

local Comb;

local Pollen;
local RandomPlant;

// 0 - Fly Wander
// 1 - Go to ground
// 2 - Roam Ground
// 3 - Rest
// 4 - Attack
// 5 - Go Home (Gather, Gossip, Construct)  REQUIRES DAY-NIGHT CYCLE
// 6 - Collect Pollen 
// 7 - Deposit Pollen

func InColony(){ return(HomeX != 0); }

func DayCheck(){
	return(IsDay());
}

func Initialize() {
	Pollen = 0;
	HomeX = 0;
	BeeState = 0;
	MaxBeenergy = RandomX(1000,3500);
	Beenergy = MaxBeenergy;
  SetAction("Fly");
  gX=0;
  gY=0;
  return(1);
}

func Activity(){
	
	if(ColoniesEnabled()){
		if(Pollen > 15) Pollen = 15;
		if(Pollen >= RandomX(1,15)) CreateParticle("PSpark", 0, 0, RandomX(-10,10), -3, 25, RGBa(255,255,0));
	}else{
		if(Pollen > 30) Pollen = 30;
		if(Pollen >= RandomX(1,30)) CreateParticle("PSpark", 0, 0, RandomX(-10,10), -3, 25, RGBa(255,255,0));
		if(Pollen == 30 && !Random(30)){
			Pollen = 0;
			for(var i = 0; i < RandomX(1,2); i++){
			var hon = CreateObject(HONY, RandomX(-5,5), RandomX(-5,5));
			LocalN("OwnedBy",hon) = this();
			}
		}
	}
	
	if(GetXDir() > 0 && GetDir() == DIR_Left)  return(TurnRight());
	if(GetXDir() < 0 && GetDir() == DIR_Right) return(TurnLeft());
	
	if(!Random(15)) Sound("Bsss");
	
	//Random Wandering (Sky)
  if(BeeState == 0){
	if(GetAction() == "Idle") SetAction("Fly");
	if(GetAction() == "Walk") SetAction("Fly");
	if(!Random(50)|| Distance(GetX(),GetY(),gX,gY) <= 5 || GBackSolid(gX-GetY(),gY-GetY()) || GBackLiquid(gX-GetY(),gY-GetY())){
		gX = GetX() + RandomX(-500,500);
		gY = GetY() + RandomX(-500,5000);
	}
	
	if(gY < 50) gY = GetY() + RandomX(100,2000);
	if(GetX() < 70) gX = GetX() + RandomX(1000,5000);
	if(GetX() > LandscapeWidth() - 70) gX = GetX() - RandomX(1000,5000);
	
	if(Beenergy <= 200) BeeState = 1;
	
	Beenergy -= RandomX(1,3);
	SetCommand(this(), "MoveTo", ,gX,gY);
	
	if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
	    return(0);
	}
	
	//Colony Stuff
	if(ColoniesEnabled()){
	if(GetMaterial(GetX(),GetY()) == Material("Tunnel") && !InColony() && !Random(100)){
		DebugLog("A Bumb Set up a Colony!");
		HomeX = GetX();
		HomeY = GetY();
		//Colony Setup
		JoinColony(800);
	}
	
	//inviting strays if part of colony
	if(InColony() && !Random(50)){
		JoinColony(100);
	}
	
	//Collect Pollen, Only the most active bees do it
	if(InColony() && Comb && Beenergy > 1800){
		if(!Random(50)){
			BeeState = 6;
			var PlantList = FindObjects(Find_Func("IsTree"));
			RandomPlant = PlantList[RandomX(0,GetLength(PlantList))];
			return(0);
		}
		
		//or.. deposit it
		if(!Random(50) && Comb && Pollen > 0){
			BeeState = 7;
			return(0);
		}
	}
	
	//is day over? go to colony and chill
	if(InColony() && !DayCheck()){
		BeeState = 5;
	}
	}else{
		//honey production without colony
		if(Beenergy > 1800){
			if(!Random(50)){
			BeeState = 6;
			var PlantList = FindObjects(Find_Func("IsTree"));
			RandomPlant = PlantList[RandomX(0,GetLength(PlantList))];
			return(0);
			}	
		}
	}
  }
  
  //Going to ground
  if(BeeState == 1){
	  gX = GetX() + RandomX(-10,10);
	  gY= LandscapeHeight();
	  
	  if(GetContact(this(), -1) & CNAT_Bottom){
		  if(GetEnergy() <= 25 || !DayCheck()){
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
	if(GetX() < 70) gX = GetX() + RandomX(1000,5000);
	if(GetX() > LandscapeWidth() - 70) gX = GetX() - RandomX(1000,5000);
	  
	  Beenergy -= RandomX(1,5);
	  SetCommand(this(), "MoveTo", ,gX,gY);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	}
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
	  
	  Beenergy += RandomX(1,4);
	  if(Beenergy >= MaxBeenergy) BeeState = 0;
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	 }
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
  
  //Attacking (Ruthless!)
  if(BeeState == 4){
	  if(GrudgeTarget == 0) BeeState = 0;
	  if(BeeState == 0) return(0);
	  
	  if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  if(!GetAlive(GrudgeTarget)){
		BeeState = 0;
		return(0);
	  }
	  
	  if(ObjectDistance(this(), GrudgeTarget) <= 8 && GetCon() >= 80 && GetAction() == "Fly" && !Contained(GrudgeTarget)){
		  SetAction("Attack");
		  Sound("Sting");
		  Punch(GrudgeTarget, RandomX(1,5));
		  InformBumbs();
	  }
	  
	  //if too far or in building, go away
	  if(ObjectDistance(this(), GrudgeTarget) >= 200 || Contained(GrudgeTarget)) BeeState = 0;
	  
	  Beenergy = RandomX(-10,20);
	  
	  SetCommand(this(), "MoveTo", GrudgeTarget);
  }
  
  //Idling in colony at night, includes constructing and gossiping
  if(BeeState == 5){
	  if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  
	  //check if near colony
	  if(Distance(GetX(),GetY(),HomeX, HomeY) > 1000){
		  SetCommand(this(), "MoveTo", ,HomeX,HomeY);
	  }else{
		if(!Random(50)|| Distance(GetX(),GetY(),gX,gY) <= 5 || GBackSolid(gX-GetY(),gY-GetY()) || GBackLiquid(gX-GetY(),gY-GetY())){
		gX = GetX() + RandomX(-1000,1000);
		gY = GetY() + RandomX(-1000,1000);
		}
		
		if(gY < 50) gY = GetY() + RandomX(100,2000);
		if(GetX() < 70) gX = GetX() + RandomX(1000,5000);
		if(GetX() > LandscapeWidth() - 70) gX = GetX() - RandomX(1000,5000);
		
		//Make Comb if you dont have one
		if(!Comb && !FindObject2(Find_ID(HNCB), Find_Distance(15)) && GetMaterial(0,0) == Material("Tunnel")){
		  DebugLog("Bumb created a comb!");
		  SetAction("Attack");
		  Sound("Sting");
		  Comb = CreateObject(HNCB);
		  LocalN("DesignatedBumb",Comb) = this();
		  LocalN("PollenAmount", Comb) = 0;
		}
		
		//gossip
		if(!Random(30)) InformBumbs();
		
		if(Beenergy <= 200) BeeState = 1;
	
		Beenergy -= RandomX(1,3);
		
		SetCommand(this(), "MoveTo", ,gX,gY);
	  }
	  
	  	  if(DayCheck()) BeeState = 0;
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
		}
  }
  
  //Collecting Pollen From Plants
  if(BeeState == 6){
	   if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  SetCommand(this(), "MoveTo",RandomPlant);
	  
	  if(RandomPlant == 0 || RandomPlant->IsDeadTree()) BeeState = 0;
	  
	  if(ObjectDistance(this(), RandomPlant) < 10 && !Random(20)){
		  Pollen += RandomX(1,3);
		  SetAction("Attack");
		  Sound("Sting");
		  if(Pollen > 13 && ColoniesEnabled())
			  BeeState = 7;
		  else
		  BeeState = 0;
	  }
	  
	  if(Beenergy <= 200) BeeState = 1;
	
	  Beenergy -= RandomX(1,3);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	 }
	
	 
  }
  
  //Bring Honey to Combs
   if(BeeState == 7){
	   if(!Random(15)) Sound("Bsss");
	  if(GetAction() == "Idle") SetAction("Fly");
	  if(GetAction() == "Walk") SetAction("Fly");
	  
	  SetCommand(this(), "MoveTo",Comb);
	  
	  if(ObjectDistance(this(), Comb) < 10){
		  LocalN("PollenAmount", Comb) += Pollen;
		  Pollen = 0;
		  SetAction("Attack");
		  Sound("Sting");
		  BeeState = 0;
	  }
	  
	  if(Beenergy <= 200) BeeState = 1;
	
	  Beenergy -= RandomX(1,3);
	  
	  if(GrudgeTarget && ObjectDistance(this(),GrudgeTarget) < 100 && !Contained(GrudgeTarget)){
		BeeState = 4;
		return(0);
	  }
  }
  
  
}

protected func CatchBlow(int iLevel, object pByObject)
{
  if (!Random(3)) Sound("BumbHurt*");
  
  GrudgeTarget = 0;
  if(GetAlive(pByObject)){
	  GrudgeTarget = pByObject;
  }else{
	  GrudgeTarget = GetCursor(GetController(pByObject)); 
  }

  if(GrudgeTarget != 0) InformBumbs();
  
  //Wake up if not tired
  if(Beenergy >= (MaxBeenergy/3)*2) BeeState = 4;
  
  return(1);
}

//cant get hit by honey
func QueryCatchBlow(pByObject){
	if(GetID(pByObject) == HONY){
		return(1);
	}
}

protected func Death() 
{
  Sound("BumbDie*");
  SetDir(0);
  ChangeDef(DBUB);
  SetAction("Dead");
  return(1);
}

protected func RejectCollect(c4ID, pObject)
{
 var iEffectNumber, pSorcerer;
 if (iEffectNumber = GetEffect("PossessionSpell", this()))
  if (pSorcerer = EffectVar(0, this(), iEffectNumber))
   if (!GetEffect("IntCollectionDelay", this()))
    if (!pSorcerer->Contents())
     if (!(pObject-GetOCF() & OCF_Living))
      Collect(pObject, pSorcerer);
 return(1);
}

func RejectEntrance(pIntoObject){
	if(GetAction() == "Rest" && FindObject2(Find_ID(COAM))) return(0);
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

public func InformBumbs(){
	var LocalBumbs = FindObjects(Find_ID(BUMB), Find_Distance(500));
	for(var bumb in LocalBumbs){
		if(LocalN("GrudgeTarget", bumb) != GrudgeTarget){
		LocalN("GrudgeTarget", bumb) = GrudgeTarget;
		bumb->InformBumbs();
		}
	}
}

public func JoinColony(int Distance){
	var LocalBumbs = FindObjects(Find_ID(BUMB), Find_Distance(Distance));
	for(var bumb in LocalBumbs){
		if(!ObjectCall(bumb, "InColony")){
			DebugLog("Bumb Recruited");
			LocalN("HomeX", bumb) = HomeX;
			LocalN("HomeY", bumb) = HomeY;
		}
	}
}