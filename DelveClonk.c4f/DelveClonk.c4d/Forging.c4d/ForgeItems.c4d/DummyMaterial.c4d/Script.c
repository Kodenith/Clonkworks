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