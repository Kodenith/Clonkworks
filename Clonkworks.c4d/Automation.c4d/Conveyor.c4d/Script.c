/*-- Neues Script --*/

#strict 2
#include BRDG
local Mask;

local TargetSpeed;
local Speed;
local Deaccel;
local tracker;

local PlayingSound;

local RequireRotor;

protected func Initialize() {
	//TargetSpeed = 60;
	Deaccel = 1;
	RequireRotor = 1;
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    Lock(1, 1);
  return(1);
}

private func Lock(quiet, dont_descend) {
  SetAction("Movement");
  if(!Mask) Mask = CreateObject(CNVM);
  is_locked = 1;

  if (!quiet)
    Sound("Connect");

  //clonks often get stuck when locking this, so there's unstuck behaviour.
  var StuckClonk;
  while(StuckClonk = FindObject2(Find_InRect(-43,-13,86,20),Find_NoContainer(),Find_OCF(OCF_CrewMember))){
	  SetY(GetY(StuckClonk) - 1, StuckClonk);
  }

  // Verankern
  if (!dont_descend)
    if (!GBackSolid(0, 10))
      SetPosition(GetX(), GetY()+10);
  return(1);
}

private func Release(quiet, dont_ascend) {
  SetAction("Idle");
  Speed = 0;
  if(Mask){
	  RemoveObject(Mask);
	  Mask = 0;
  }
  is_locked = 0;

  if (!quiet)
    Sound("Connect");

  // Aus der Erde lösen
  if (!dont_ascend) {
    var ascend_pixels = 0;
    while (Stuck() && (++ascend_pixels < 12))
      SetPosition(GetX(), GetY()-1);
  }

  return(1);
}

private func Destruction(){
  if(Mask){
	  RemoveObject(Mask);
	  Mask = 0;
  }
}

private func Timer(){
	if(Mask){
		SetXDir(0,Mask);
		SetYDir(0,Mask);
		SetX(GetX(),Mask);
		SetY(GetY(),Mask);
	}
	
	var NotConnected = 1;
	for(var i in FindObjects(Find_ID(GERL))){
		if(i->IsConvSet(this())) NotConnected = 0;
	}
	if(NotConnected && RequireRotor) TargetSpeed = 0;
	
	if(GetAction() == "Movement"){
		
		if(GetActTime() % 5 == 0 && Deaccel){
		if(TargetSpeed > Speed) Speed++;
		if(TargetSpeed < Speed) Speed--;
		}
		if(!Deaccel) Speed = TargetSpeed;
		
		Speed = BoundBy(Speed,-570,570);
		
		DoMovement();
		
		if(Abs(Speed) > 20 && !PlayingSound){
			PlayingSound = true;
			Sound("Elevator",0,this(),0,0,+1);
		}else if(PlayingSound && Abs(Speed) <= 20){
			PlayingSound = false;
			Sound("Elevator",0,this(),0,0,-1);
		}
	}
}

private func DoMovement(){
	var maxLimit = 37 * 38;
	tracker %= maxLimit;
	
	tracker += Speed;
	var curr = (tracker / 38) % 37;
	if(curr < 0){
		curr+=37;
	}
	
	SetPhase(curr);
	
	MoveItems();
}

private func MoveItems(){
	//above
	var OnTop = FindObjects(Find_OnLine(-38,-10,38,-10),Find_Not(Find_Category(C4D_Structure)),Find_Not(Find_Category(C4D_StaticBack)), Find_Not(Find_ID(BRDG)), Find_Not(Find_ID(CNVY)));
	for(var i in OnTop){
		if(Contained(i)) continue;
		var limi, diff,neg;
		if(Speed > 0){
		neg = false;
		Limi = GetX(i)+Speed/30;
		diff = GetX(i);
		while(!GBackSolid(AbsX(diff),AbsY(GetY(i))) && diff < Limi) diff++;
		}else{
			neg=true;
			Limi = GetX(i)+Speed/30;
			diff = GetX(i);
			while(!GBackSolid(AbsX(diff),AbsY(GetY(i))) && diff > Limi) diff--;
		}
		SetX(diff,i);
		if(neg && Stuck(i)) SetX(diff-GetDefOffset(GetID(i))/2,i);
		if(!neg && Stuck(i)) SetX(diff+GetDefOffset(GetID(i))/2,i);
		if(!ObjectOnConveyor(i)){
			SetXDir(Speed/3,i);
		}
	}
	
	//below (Climbing)
	var Below = FindObjects(Find_OnLine(-38,10,38,10),Find_FuncEqual("GetProcedure()","HANGLE"));
	for(var i in Below){
		if(Contained(i)) continue;
		var Limi = GetX(i)-Speed/30;
		var diff = GetX(i);
		while(!GBackSolid(AbsX(diff),AbsY(GetY(i))) && diff > Limi) diff--;
		SetX(diff,i);
		if(!ObjectBelowConveyor(i)){
			SetXDir(-Speed/3,i);
		}
	}
}

public func ObjectOnTop(pObj){
	//if(Contained(pObj)) return(0);
	if(GetAction() != "Movement") return(0);
	var OnTop = FindObjects(Find_OnLine(-38,-10,38,-10),Find_Not(Find_Category(C4D_Structure)),Find_Not(Find_Category(C4D_StaticBack)), Find_Not(Find_ID(BRDG)), Find_Not(Find_ID(CNVY)));
	if(InArray(pObj,OnTop) != -1) return(1);
}

public func ObjectOnConveyor(pObj){
	for(var i in FindObjects(Find_ID(CNVY))){
		if(i->ObjectOnTop(pObj)) return(1);
	}
	return(0);
}

public func ObjectBelow(pObj){
	//if(Contained(pObj)) return(0);
	if(GetAction() != "Movement") return(0);
	var Below = FindObjects(Find_OnLine(-38,10,38,10),Find_FuncEqual("GetProcedure()","HANGLE"),Find_NoContainer());
	if(InArray(pObj,Below) != -1) return(1);
}

public func ObjectBelowConveyor(pObj){
	for(var i in FindObjects(Find_ID(CNVY))){
		if(i->ObjectBelow(pObj)) return(1);
	}
	return(0);
}

public func SetConveyorSpeed(iSpeed){
	TargetSpeed = iSpeed;
}

public func ConveyorDeAccelerateMode(ix){
	Deaccel = ix;
}

public func ConveyorRequireRotor(ix){
	RequireRotor = ix;
}

//For the line
public func GetNeighboursLeft(Limit){
	if(GetAction() != "Movement") return(0);
	if(Limit == 0) Limit = 999;
	var x = 0;
	var found;
	var List = [];
	while(found = FindObject2(Find_AtPoint(x,0),Find_ID(GetID()),Find_NoContainer())){
		ArrayAdd(List,found,true);
		x -= 86;
		if(GetLength(List) > Limit) break;
	}
	return(List);
}

public func GetNeighboursRight(Limit){
	if(GetAction() != "Movement") return(0);
    if(Limit <= 0) Limit = 999;
	var x = 0;
	var found;
	var List = [];
	while(found = FindObject2(Find_AtPoint(x,0),Find_ID(GetID()),Find_NoContainer())){
		ArrayAdd(List,found,true);
		x += 86;
		if(GetLength(List) > Limit) break;
	}
	return(List);
}

public func IsAdvancedProduct(){
	return(1);
}

public func GetResearchBase(){
	return(BRDG);
}

//what advanced lines can be connected?
public func ALKConnectType(){
	if(Par(0) == 0) return(0);
	for(var i in FindObjects(Find_ID(GERL))){
		if(i->IsConvSet(this())) return(0);
	}
	return([GERL]);
}