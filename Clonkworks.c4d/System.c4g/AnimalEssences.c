/*-- Neues Script --*/

#strict 2
#appendto WIPF
#appendto MONS
#appendto FMNS
#appendto FISH
#appendto SHRK
#appendto SNKE
#appendto BIRD

public func HasEssence(){
	return(1);
}

public func EssenceInfo(type, object pObj){
	//Color - Color
	//PHYSICAL - physical :)
	//Start - OnStart, only plays once even when multiple are combined
	//Stop - OnEnd, only plays once even when multiple are combined
	
	if(GetID() == WIPF){
	if(type == "Color") return(RGBa(224,138,0));
	if(type == "Jump") return(2800);
	if(type == "Walk") return(1400);
	if(type == "Dig") return(1400);
	}
	
	if(GetID() == MONS){
	if(type == "Color") return(RGBa(1,123,1));
	if(type == "Walk") return(-1000);
	if(type == "Fight") return(1000);
	if(type == "Climb") return(600);
	if(type == "Hangle") return(600);
	if(type == "Start" && pObj){
		SetPhysical("CorrosionResist",1,2,pObj);
	}
	if(type == "Stop" && pObj){
		SetPhysical("CorrosionResist",0,2,pObj);
	}
	}
	
	if(GetID() == FMNS){
	if(type == "Color") return(RGBa(118,15,1));
	if(type == "Walk") return(-1500);
	if(type == "Fight") return(1500);
	if(type == "Climb") return(700);
	if(type == "Hangle") return(700);
	if(type == "Start" && pObj){
		Incinerate(pObj);
	}
	}
	
	if(GetID() == FISH){
	if(type == "Color") return(RGBa(230,230,255));
	if(type == "Swim") return(3000);
	if(type == "Start" && pObj){
		SetPhysical("BreatheWater",1,2,pObj);
	}
	if(type == "Stop" && pObj){
		SetPhysical("BreatheWater",0,2,pObj);
	}
	}
	
	if(GetID() == SHRK){
	if(type == "Color") return(RGBa(230,230,255));
	if(type == "Swim") return(1500);
	if(type == "Breath") return(1500);
	if(type == "Fight") return(900);
	}
	
	if(GetID() == SNKE){
	if(type == "Color") return(RGBa(1,255,1));
	if(type == "Walk") return(2500);
	if(type == "Fight") return(500);
	if(type == "Climb") return(1500);
	if(type == "Hangle") return(1500);
	}
	
	if(GetID() == BIRD){
	if(type == "Color") return(RGBa(264,86,41));
	if(type == "Jump") return(3000);
	if(type == "Fight") return(800);
	}
}