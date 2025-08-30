//this script handles forge effects and repairing
//import it if you want a material affect the object it made

#strict

local MatId;
local Color;
local Mass;
local Speed;
local Power;
local Effect;

func AssignEffects(){
	var fxFound = false;
	//Burnable
	if(WildcardMatch(Effect, "*Burnable*")){
		fxFound = true;
		AddEffect("Burnable", this(), 50, 35, this()); 
	}
	
	//Meltable
	if(WildcardMatch(Effect, "*Meltable*")){
		fxFound = true;
		AddEffect("Meltable", this(), 50, 15, this()); 
	}
	
	//Windy
	if(WildcardMatch(Effect, "*Windy*")){
		fxFound = true;
		AddEffect("Windy", this(), 50, 10, this()); 
	}
	
	//Float
	if(WildcardMatch(Effect, "*Float*")){
		fxFound = true;
		AddEffect("Float", this(), 50, 1, this()); 
	}
	
	//Liquid Melt
	if(WildcardMatch(Effect, "*LQM*")){
		fxFound = true;
		AddEffect("Lquidmelt", this(), 50, 15, this()); 
	}
	
	//Regen
	if(WildcardMatch(Effect, "*Regen*")){
		fxFound = true;
		AddEffect("Regen", this(), 50, 40, this()); 
	}
	
	//Glide
	if(WildcardMatch(Effect, "*Glide*")){
		fxFound = true;
		AddEffect("Glide", this(), 50, 1, this()); 
	}
	
	//Toxic
	if(WildcardMatch(Effect, "*Toxic*")){
		fxFound = true;
		AddEffect("Toxic", this(), 50, 100, this()); 
	}
	
	//Mod2
	if(WildcardMatch(Effect, "*Mod2*")){
		fxFound = true;
		SetObjectBlitMode(2, this());
	}
	
	//Skew
	if(WildcardMatch(Effect, "*Skew*")){
		fxFound = true;
	   AddEffect("Skew", this(), 50, 1, this()); 
	}
	
	//Glow
	if(WildcardMatch(Effect, "*Glow*")){
	   fxFound = true;
	   var wdt = GetDefCoreVal("Width", "DefCore", GetID());
	   SetPlrViewRange(wdt, this());
	}
	
	if(WildcardMatch(Effect, "*Lightning*")){
	   fxFound = true;
	   AddEffect("Lightning", this(), 50, 500, this()); 
	}
	
	if(fxFound) return(1);
	
	AddEffect(Effect, this(), 50, 1, this()); 
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
	
	var HighTemp;
	HighTemp = WildcardMatch(Effect, "*HTM*") && GetTemperature() > 75;
	
	if(GetMaterial() == Material("Lava") || GetMaterial() == Material("DuroLava") || HighTemp){
		
		
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
	
	if(WildcardMatch(Effect, "*LavaLQM*") && GetMaterial(0,0) == Material("Lava")){
		DoCon(-1, pTarget);
	}
	
	if(WildcardMatch(Effect, "*LavaLQM*") && GetMaterial(0,0) == Material("DuroLava")){
		DoCon(-1, pTarget);
	}
}

public func FxRegenTimer(object pTarget, int EffectNumber){
	DoCon(1, pTarget);
}

public func FxGlideTimer(object pTarget, int EffectNumber){
	if(GetYDir() > 0) SetYDir(Mass);
}

public func FxSkewTimer(object pTarget, int EffectNumber){
	SetObjDrawTransform(1000, 10 * GetXDir(), 0,0,1000,0,this());
}

public func FxToxicTimer(object pTarget, int EffectNumber){
	var Close = FindObjects(Find_OCF(OCF_Alive), Find_Distance(50));
				for(var element in Close){
					DoEnergy(-1, element);
				}
}

public func FxLightningTimer(object pTarget, int EffectNumber){
	if(RandomX(0,100) <= GetScenarioVal("Lightning", "Weather", 0)){
		LaunchLightning(GetX(),-LandscapeHeight()+GetY(), -20, 41, +5, 15);
	}
}

public func FxStickyTimer(object pTarget, int EffectNumber){
		SetXDir(0);
		SetYDir(0);
	    var stickMass;
		if(Mass <= 0) stickMass = 1;
		else stickMass = Mass;
		
		if(stickMass >= RandomX(1,40)) return(-1);
}

public func FxStickyStop(object pTarget, int EffectNumber){
	Sound("ArrowHit");
}
	
	
//non effect effects

func Hit(int xdir, int ydir){
	if(WildcardMatch(Effect, "*Sticky*")){
		AddEffect("Sticky", this(), 50, 1, this()); 
		Sound("ArrowHit");
	}else if(WildcardMatch(Effect, "*Bouncy*")){
		HitBounce(this(),2, xdir, ydir);
	}
}

//Apparently without Hit 2 and Hi t3 it wont bounce as much on some objects
func Hit2(int xdir, int ydir){
	if(WildcardMatch(Effect, "*Sticky*")){
		AddEffect("Sticky", this(), 50, 1, this()); 
		Sound("ArrowHit");
	}else if(WildcardMatch(Effect, "*Bouncy*")){
		HitBounce(this(),2, xdir, ydir);
	}
}

func Hit3( int xdir, int ydir){
	if(WildcardMatch(Effect, "*Fragile*")){
		for(var i = 0; i < RandomX(2,10); i++){
			Smoke(RandomX(-10,10), RandomX(-10,10), RandomX(1,20));
		}
		Sound("ClonkHit*");
		CastParticles("PxSpark",RandomX(1,5),100,0,0,15,30,RGB(255,223,127),RGB(255,223,127));
		DoCon(RandomX(-100,-20));
	}
	
	if(WildcardMatch(Effect, "*Explode*")){
		if(Power > 0)
		Explode(Power*10);
	else Explode(8);
	}
	
	//Apparently without Hit 2 and Hi t3 it wont bounce as much on some objects
	if(WildcardMatch(Effect, "*Sticky*")){
		AddEffect("Sticky", this(), 50, 1, this()); 
		Sound("ArrowHit");
	}else if(WildcardMatch(Effect, "*Bouncy*")){
		HitBounce(this(),2, xdir, ydir);
	}
}

//Repairing

func IsBroken(){ return(GetCon() < 100 && !GetEffect("Repair", this()) && GetCategory(this()) & C4D_Vehicle()); }

public func ContextRepair(pCaller){
	[$Repair$|Image=CXCN|Condition=IsBroken]
	if(ObjectDistance(this(), pCaller) > 10){
		AppendCommand(pCaller, "MoveTo", this());
		AppendCommand(pCaller, "Call", this(), pCaller, , , ,"ContextRepair");
	}else{
		ObjectSetAction(pCaller, "Process");
		AddEffect("Repair", pCaller, 50, 15, this()); 
	}
}

func FxRepairTimer(pTarget){
	if(GetCon() >= 100 || GetAction(pTarget) ne "Process" || ObjectDistance(this(), pTarget) > 10) return(-1);
	
	DoCon(1);
}

func FxRepairStop(pTarget){
	if(GetAction(pTarget) eq "Process")
		pTarget->SetAction("Walk");
}

//Inspecting
func CanBeInspected(){ return(ObjectCount(FALW) && !ObjectCount(NOIS) && MatId != 0); }

public func ContextInspect(pCaller){
	[$Inspect$|Image=INSP|Condition=CanBeInspected]
	if(MatId)
	FindObject(FALW)->Inspect(MatId, pCaller);
}