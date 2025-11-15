/*-- Neues Script --*/

#strict 2
local Essence;

//craftstuff
public func IsAnvilProduct(){ return(1); }
public func IsAdvancedProduct(){ return(1); }

func Initialize() {
  Update();
  AddEffect("Syrinf",this(),1,0,this());
  return(1);
}

func Update(){
	if(GetAction() != "Filled"){
		SetPicture(17,36,29,28);
	}else{
		SetPicture(11,92,40,40);
	}
}

public func Activate(pClonk){
	[$TxtInject$]
	if(!Essence) SetAction("Idle");
	var NotFilled = true;
	if(GetAction() == "Filled") NotFilled = false;
	
	if(GetEffect("Injection",pClonk)) return(1);
	
	if(NotFilled){
	var Injectable = FindObject2(Find_Func("HasEssence"), Find_AtPoint(), Find_NoContainer());
	if(!Injectable){
		Message("$ErrNoCorpse$",Contained());
		Contained()->Sound("CommandFailure1");
		return(1);
	}
	
	AddEffect("Injection",pClonk,300,1,this());
	}
	
	if(!NotFilled){
		return(1);
	}
	
	return(1);
}

//Injection effect! comes with cool animation :D
protected func FxInjectionStart(pTarget,iEffectNumber,iTemp){
	//make the clonk kneel down to inject.
	if(GetActMapVal("Name","KneelDown",GetID(pTarget)))
	pTarget->SetAction("KneelDown");

	//stopping the target
	SetComDir(COMD_None,pTarget);
}

protected func FxInjectionTimer(pTarget,iEffectNumber,iEffectTime){
	//not kneeling? lets see how to end this...
	if(GetAction(pTarget) != "KneelDown"){
		//if the target has a kneeling animation
		if(GetActMapVal("Name","KneelDown",GetID(pTarget)) && GetActMapVal("Name","KneelUp",GetID(pTarget))){
			if(GetAction(pTarget) == "KneelUp") EffectVar(0,pTarget,iEffectNumber) = 1; //ok, can do injection!
			return(-1);
		}else{
			if(GetAction(pTarget) == "Walk" || GetAction(pTarget) == "Idle") EffectVar(0,pTarget,iEffectNumber) = 1; //ok, can do injection!
			return(-1);
		}
	}
}

protected func FxInjectionStop(pTarget,iEffectNumber,iReason,fTemp){
	if(EffectVar(0,pTarget,iEffectNumber) == 1) Inject();
}


protected func Inject(){
	var NotFilled = true;
	if(GetAction() == "Filled") NotFilled = false;
	
	DoExtraction();
}

public func DoExtraction(){
	var Injectable = FindObject2(Find_Func("HasEssence"), Find_AtPoint(), Find_NoContainer());
	if(!Injectable) return(0);
	
	SetColorDw(Injectable->~EssenceInfo("Color")); //get and set color
	Essence = GetID(Injectable);
	Sound("StabExtract");
	SetAction("Filled");
	Punch(Injectable,10);
}



//other
func Hit(){
	if(!Random(3)) Sound("Crystal*",0,this(),25);
}

//oooh! info!
public func FxSyrinfInfo(object pTarget, int iEffectNumber){
	var i = Format("$TxtEssence$",Essence,GetName(,Essence));
	if(Essence) return(i);
}