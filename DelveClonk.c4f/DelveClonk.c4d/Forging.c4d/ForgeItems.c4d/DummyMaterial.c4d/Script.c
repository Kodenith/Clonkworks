//this script handles forge effects

#strict
#appendto ALRY
#appendto ACAN

local Color;
local Mass;
local Speed;
local Power;
local Effect;

func EffectCheck(){
	//Burnable
	if(WildcardMatch(Effect, "*Burnable*")){
		if(FindObject2(Find_OCF(OCF_OnFire), Find_Distance(10))){
			if(RandomX(0, 10) == 5) Incinerate();
			if(OnFire(this()) && RandomX(0, 2) == 1) DoCon(-1, this());
			//Fuming
			if(WildcardMatch(Effect, "*Fuming*") && OnFire(this()) && RandomX(0, 2) == 1){
				DoEnergy(-1, FindObject2(Find_OCF(OCF_Alive), Find_Distance(60)));
			}
		}
	}
}
