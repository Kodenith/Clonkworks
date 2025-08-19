//this script handles forge effects
//import it if you want a material affect the object it made

#strict

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func AssignEffects(){
	//Burnable
	if(WildcardMatch(Effect, "*Burnable*")){
		AddEffect("Burnable", this(), 50, 35, this()); 
	}
	
	//Meltable
	if(WildcardMatch(Effect, "*Meltable*")){
		AddEffect("Meltable", this(), 50, 15, this()); 
	}
	
	//Windy
	if(WildcardMatch(Effect, "*Windy*")){
		AddEffect("Windy", this(), 50, 10, this()); 
	}
	
	//Float
	if(WildcardMatch(Effect, "*Float*")){
		AddEffect("Float", this(), 50, 1, this()); 
	}
	
	//Liquid Melt
	if(WildcardMatch(Effect, "*LQM*")){
		AddEffect("Lquidmelt", this(), 50, 15, this()); 
	}
	
	//Regen
	if(WildcardMatch(Effect, "*Regen*")){
		AddEffect("Regen", this(), 50, 40, this()); 
	}
}


//coffe spelled backwards is effoc

public func FxBurnableTimer(object pTarget, int EffectNumber){
	if((FindObject2( Find_OCF(OCF_OnFire), Find_Distance(45) ) && RandomX(1, 6) == 2)) {
		Incinerate(pTarget);
	}
	
	if(GetMaterial() == Material("Lava")){
		Incinerate(pTarget);
	}
	
	if(GetMaterial() == Material("DuroLava")){
		Incinerate(pTarget);
	}
	
	if(OnFire(pTarget)){
			DoCon(-1, pTarget);
			if(WildcardMatch(Effect, "*Fuming*")){
				var Close = FindObjects(Find_OCF(OCF_Alive), Find_Distance(50));
				for(var element in Close){
					DoEnergy(-1, element);
				}
			}
	}
}

public func FxMeltableTimer(object pTarget, int EffectNumber){
	
	if(GetMaterial() == Material("Lava") || GetMaterial() == Material("DuroLava")){
		
		
		//Making the thing look boiling red
		var ReddishHue;
		ReddishHue = Color;
		if(GetRGBaValue(Color, 2) > 25){
			ReddishHue = DoRGBaValue(ReddishHue, -25, 2);
		}else {
			ReddishHue = SetRGBaValue(ReddishHue, 0, 2);
		}
		
		if(GetRGBaValue(Color, 3) > 25){
			ReddishHue = DoRGBaValue(ReddishHue, -25, 3);
		}else {
			ReddishHue = SetRGBaValue(ReddishHue, 0, 3);
		}
		
		if(GetRGBaValue(Color, 1) < 230){
			ReddishHue = DoRGBaValue(ReddishHue, 25, 1);
		}else {
			ReddishHue = SetRGBaValue(ReddishHue, 255, 1);
		}
		
		
		if(GetRGBaValue(GetClrModulation(), 1) != GetRGBaValue(ReddishHue, 1)){
			
			var r = GetRGBaValue(ReddishHue, 1);
			var g = GetRGBaValue(ReddishHue, 2);
			var b = GetRGBaValue(ReddishHue, 3);
			var mod = GetClrModulation();
			
			if(GetRGBaValue(mod, 1) < r) mod = DoRGBaValue(mod, 1, 1);
			if(GetRGBaValue(mod, 1) > r) mod = DoRGBaValue(mod, -1, 1);
			
			if(GetRGBaValue(mod, 2) < g) mod = DoRGBaValue(mod, 1, 2);
			if(GetRGBaValue(mod, 2) > g) mod = DoRGBaValue(mod, -1, 2);
			
			if(GetRGBaValue(mod, 3) < g) mod = DoRGBaValue(mod, 1, 3);
			if(GetRGBaValue(mod, 3) > g) mod = DoRGBaValue(mod, -1, 3);
			
			SetClrModulation(mod);
		}else{
			DoCon(-1, pTarget);
		}
		
	}else{
		
		//Normalizing color
			var r = GetRGBaValue(Color, 1);
			var g = GetRGBaValue(Color, 2);
			var b = GetRGBaValue(Color, 3);
			var mod = GetClrModulation();
			
			if(GetRGBaValue(mod, 1) < r) mod = DoRGBaValue(mod, 1, 1);
			if(GetRGBaValue(mod, 1) > r) mod = DoRGBaValue(mod, -1, 1);
			
			if(GetRGBaValue(mod, 2) < g) mod = DoRGBaValue(mod, 1, 2);
			if(GetRGBaValue(mod, 2) > g) mod = DoRGBaValue(mod, -1, 2);
			
			if(GetRGBaValue(mod, 3) < g) mod = DoRGBaValue(mod, 1, 3);
			if(GetRGBaValue(mod, 3) > g) mod = DoRGBaValue(mod, -1, 3);
			
			SetClrModulation(mod);
			
	}
}

public func FxWindyTimer(object pTarget, int EffectNumber){
	if(GetMass() < Abs(GetWind())){
		var Vel = Abs(GetWind()) / 10;
		if(GetWind() >= 0)
			SetXDir(GetXDir() + Vel);
		else
			SetXDir(GetXDir() - Vel);
	}
}

public func FxFloatTimer(object pTarget, int EffectNumber){
	if(WildcardMatch(Effect, "*Explode*")){
		Explode(Power*10);
	}
}

public func FxLquidmeltTimer(object pTarget, int EffectNumber){
	if(WildcardMatch(Effect, "*WaterLQM*") && GetMaterial(0,0) == Material("Water")){
		DoCon(-1, pTarget);
	}
	
	if(WildcardMatch(Effect, "*AcidLQM*") && GetMaterial(0,0) == Material("Acid")){
		DoCon(-1, pTarget);
	}
	
	if(WildcardMatch(Effect, "*OilLQM*") && GetMaterial(0,0) == Material("Oil")){
		DoCon(-1, pTarget);
	}
}

public func FxRegenTimer(object pTarget, int EffectNumber){
	DoCon(1, pTarget);
}

//non effect effects

func Hit(){
	if(WildcardMatch(Effect, "*Explode*")){
		Explode(Power*10);
	}
}