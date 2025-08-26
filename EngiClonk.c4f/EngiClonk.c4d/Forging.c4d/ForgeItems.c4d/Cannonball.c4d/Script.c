/*-- Neues Objekt --*/

#strict

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func Initialize() {
   
  return(1);
}

func FRGUpdate(){
	//SetColorDw(Color, this());
	SetMass(5*Mass, this());
	SetClrModulation(Color, this());
}

func AssignEffects(){
	DebugLog("Cannonball is not affected by Effects!");
}

func Hit(){
  Sound("RockHit*");
  return(1);
}

func Hit3(int xdir, int ydir){
	if(Power > 0){
	if(xdir > 30 || ydir > 30){	
		var VeryCloseBuildings;
		var CloseBuildings;
		var FarBuildings;
		
		VeryCloseBuildings = FindObjects( Find_Category(C4D_Structure), Find_Distance(10*Power) );
		CloseBuildings = FindObjects( Find_Category(C4D_Structure), Find_Distance(27*Power) );
		FarBuildings = FindObjects( Find_Category(C4D_Structure), Find_Distance(50*Power));
		
		for(var bld in VeryCloseBuildings){
			DoCon(RandomX(-3,-20)*Power, bld);
		}
		
		for(var bld in CloseBuildings){
			DoCon(RandomX(-3,-15)*Power, bld);
		}
		
		for(var bld in FarBuildings){
			DoCon(RandomX(0,-4)*Power, bld);
		}
		
		Explode(Power * 20);
		
		return(1);
	}
	}
}
