/*-- Neues Script --*/

#strict 2

local Target;
local TagString;
local LastUser;

func Initialize() {
	TagString = "Hallo! Ich bin dumm.";
    return(1);
}

protected func Activate(pByObj){
	LastUser = GetOwner(pByObj);
	Target = FindObject2( Find_Exclude(this()), Find_Exclude(pByObj), Find_AtPoint(0,5), Find_Or( Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living)), Find_Not(Find_OCF(OCF_CrewMember)), Find_NoContainer());
	if(!Target) return(0);
	for(var tg in FindObjects(Find_Container(Target), Find_ID(TAG_))){
		if(tg->isTagged()){
			return(0);
		}
	}
	if(GetOCF(pByObj) & OCF_CrewMember)
	CallMessageBoard(this(), false, Format("$Tag$: %s", GetName(Target)), GetController(pByObj));
	else InputCallback(TagString);
}

func InputCallback (string szInput){
	TagString = szInput;
	if(Target && FindObject2( Find_Exclude(this()), Find_Exclude(Contained()), Find_AtPoint(0,5), Find_Or( Find_Category(C4D_Structure), Find_Category(C4D_Vehicle), Find_Category(C4D_Living)), Find_Not(Find_OCF(OCF_CrewMember)), Find_NoContainer()) == Target){
		Enter(Target, this());
		SetName(TagString, Target);
		Sound("Click");
		AddEffect("Tagged", this(), 50, 7, this());
		return(1);
	}else{
		Message("$TagFail$", this());
		return(0);
	}
}

protected func FxTaggedTimer(pTarget, iEffectNumber){
	CustomMessage(TagString, this(), NO_OWNER, , ,GetPlrColorDw(GetOwner()),,,2);
	
	if(Contained() != Target){
		SetName(0, Target);
		return(-1);
	}
}

public func IsAnvilProduct(){ return(true); }
public func isTagged() {
	return(GetEffect("Tagged", this()) != 0);
}