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
}


//coffee spelled backwards is effoc
public func FxBurnableTimer(object pTarget, int EffectNumber){
	DebugLog("It works");
	if( FindObject2( Find_OCF(OCF_OnFire), Find_Distance(45) ) && RandomX(1, 6) == 2){
		Incinerate(pTarget);
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
}