/*-- Neues Script --*/
#strict 2
#include L_FU

/* GENERAL AUTOMATED PRODUCTION TEMPLATE */

local Parent;
public func RejectConstruction(iX,iY,pBuilder){
	if(!ObjectOnConveyor(pBuilder)){
		Message("$TxtWrongPlacement$",pBuilder);
		pBuilder->Sound("Error");
		return(1);
	}
	
	return(0);
}

protected func Construction(){
	var Look;
	if(Look = FindObject2(Find_Func("IsConveyor"),Find_OnLine(0,0,0,10))){
		Parent = Look;
		LocalN("Extension",Parent) = this();
	}
	else RemoveObject();
	
	return(_inherited());
}

public func GetCDir(){
	if(!Parent) return(0);
	if(LocalN("Speed",Parent) == 0) return(0);
	if(LocalN("Speed",Parent) < 0) return(-1);
	return(1);
}

public func IsBuilt(){ return(GetCon() >= 100); }
protected func BuildNeedsMaterial(){ return(1); }

/* END */

protected func RejectCollect(idObj,pObj){
	if(GetCDir() == 0) return(1);
  if(pObj->~SmeltResult() || pObj->~FurnaceFuelValue()){
  if(Contained(pObj)) return(0);
	if(GetCDir() == 1){
		if(GetX(pObj) < GetX()-20) return(0);
	}
	if(GetCDir() == -1){
		if(GetX(pObj) > GetX()+20) return(0);
	}
	}
	return(1);
}

protected func Collection(pObj,fPut){
	if(fPut) return(1);
	Sound("Clonk");
}

local Smelted;
local Tiredness;

func Update(){
  if(!Parent){
    RemoveObject();
    return(0);
  }
  if(OnFire(Parent)){
    Incinerate();
    return(0);
  }

  if(!IsBuilt() || GetCDir() == 0 || !ActIdle()) return(0);
  SetPhase(0);
  if(CheckContainedFuel()>=10 && Tiredness < 5){
  Tiredness++;
    for(var smeltMe in FindObjects(Find_Container(this) ) ){
        if(!smeltMe->~SmeltResult()) continue;
        ConsumeFuel(10,this,0,true);
        SetAction("Burning");
        Smelted = smeltMe;
        return(1);
    }
  }
  else if(FindJunk()){
      Tiredness = 0;
      SetAction("Filling");
  }else{
      Tiredness = 0;
  }
}

private func FindJunk(){
  var Found = FindObjects(Find_Container(this),Find_Exclude(this));
  if(!Found || !GetLength(Found)) return(0);
  var fn;
  for(var i in Found){
    if(i->~SmeltResult()) continue;
    if(i->~FurnaceFuelValue()) continue;
    fn = i;
    break;
  }

  if(fn)
  return(fn);
}

func Depos(){
  if(Inside(GetPhase(),6,10)){
    Sound("Pshshsh");
    if(!Random(2))
    Smoke(RandomX(2,10),5,RandomX(10,25));
    if(GetCDir() != 0 && FindJunk()){
      Exit(FindJunk(),28*GetCDir(),23);
    }
  }
}

func Burning(){
  if(!Random(2))
  Smoke(-17,-25,RandomX(10,25));
  if(GetActTime() > 38){
    SetAction("Idle");
    Sound("Pshshsh");
    ChangeDef(Smelted->SmeltResult(),Smelted);
    Smelted = 0;
  }
}

func RejectContents(){ if(!ActIdle()) return(1); }

public func GetResearchBase(){ return(FNDR); }

func LeverSound(){ Sound("Lever1"); }