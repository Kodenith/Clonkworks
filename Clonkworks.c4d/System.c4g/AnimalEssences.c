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
	if(type == "Jump") return(3100);
	if(type == "Walk") return(1600);
	if(type == "Dig") return(1600);
	}
	
	if(GetID() == MONS){
	if(type == "Color") return(RGBa(1,123,1));
	if(type == "Walk") return(-2000);
	if(type == "Fight") return(1200);
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
	if(type == "Walk") return(-2300);
	if(type == "Fight") return(1500);
	if(type == "Climb") return(700);
	if(type == "Hangle") return(700);
	if(type == "Start" && pObj){
		AddEffect("BanBurnPSpell", pObj, 180, (38*60), 0, GetID());
	}
	}
	
	if(GetID() == FISH){
	if(type == "Color") return(RGBa(230,230,255));
	if(type == "Swim") return(4500);
	if(type == "Breath") return(3000);
	}
	
	if(GetID() == SHRK){
	if(type == "Color") return(RGBa(230,230,255));
	if(type == "Swim") return(1500);
	if(type == "Breath") return(-1000);
	if(type == "Start" && pObj){
		SetPhysical("BreatheWater",1,3,pObj);
	}
	if(type == "Stop" && pObj){
		ResetPhysical(pObj,"BreatheWater");
	}
	}
	
	if(GetID() == SNKE){
	if(type == "Color") return(RGBa(1,255,1));
	if(type == "Walk") return(2500);
	if(type == "Climb") return(1500);
	if(type == "Hangle") return(1500);
	}
	
	if(GetID() == BIRD){
	if(type == "Color") return(RGBa(264,86,41));
	if(type == "Jump") return(3500);
	}
}

//anti burn effect from clonk docs.
//for fire monster essence

protected func FxBanBurnPSpellStart(pTarget, iEffectNumber, iTemp)
{
  // Beim Start des Effektes: Clonk löschen, wenn er brennt
  if (!iTemp) Extinguish(pTarget);
  return(1);
}

protected func FxBanBurnPSpellEffect(szNewEffect, iEffectTarget, iEffectNumber, iNewEffectNumber, var1, var2, var3)
{
  // Feuer abblocken
  if (WildcardMatch(szNewEffect, "*Fire*")) return(-1);
  // Alles andere ist OK
  return(0);
}