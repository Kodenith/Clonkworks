/*-- Neues Script --*/

#strict 2
#include BRDG
local Mask;

local TargetSpeed;
local Speed;
local Deaccel;

//tracker helps with dynamic phases, Xtracker helps with moving objects along the phases. the conveyors must be nice and detailed :)
local tracker;
local Xtracker;

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

  // Verankern
  if (!dont_descend)
    if (!GBackSolid(0, 10))
      SetPosition(GetX(), GetY()+10);
  
    //clonks often get stuck when locking this, so there's unstuck behaviour.
  var StuckClonk;
  while(StuckClonk = FindObject2(Find_InRect(-43,-13,86,20),Find_NoContainer(),Find_OCF(OCF_CrewMember))){
	  SetY(GetY(StuckClonk) - 1, StuckClonk);
  }
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
  
  //if(FindObject2(Find_ID(GERL),Find_ActionTarget(this()))) RemoveObject(FindObject2(Find_ID(GERL),Find_ActionTarget(this())));

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
	
	Xtracker += (Speed * 1);
	MoveItems(Xtracker/38);
	Xtracker %= 38;
	
	SetPhase(curr);
}

private func WillObjectGetStuck(pObj,Xd){
	var Increment = Xd/Abs(Xd);
	var Original = GetX(pObj);
	for(var i = 0; i < Abs(Xd); i++){
		SetX(Original+(i*Increment),pObj);
		if(Stuck(pObj)){
			SetX(Original,pObj);
			return(1);
		}
	}
	SetX(Original,pObj);
	return(0);
}

private func MoveItems(iX){
	if(!iX || Speed == 0) return(0);
	//above
	var OnTop = ObjectsOnTop();
	if(OnTop && GetLength(OnTop) != 0) 
	for(var i in OnTop){
		if(GetProcedure(i) == "FLOAT" || GetProcedure(i) == "ClIMB" || GetProcedure(i) == "HANGLE" || GetProcedure(i) == "FLIGHT") continue;
		var wasStuckBefore = Stuck(i);
		if(Contained(i)) continue;
		
		if(!wasStuckBefore)
			SetX(GetX(i)+iX,i);
		
		if(Stuck(i) && !wasStuckBefore)
			SetX(GetX(i)+(-iX/4),i);
		
		if(!ObjectOnConveyor(i)){
			SetXDir(GetXDir(i)+Speed/3,i);
		}
	}
	
	//below (Climbing)
	var Below = ObjectsBelow();
	if(Below && GetLength(Below) != 0) 
	for(var i in Below){
		var wasStuckBefore = Stuck(i);
		if(Contained(i)) continue;
		
		if(!wasStuckBefore)
			SetX(GetX(i)-iX,i);
		
		if(Stuck(i) && !wasStuckBefore)
			SetX(GetX(i)-(-iX/4),i);
		
		if(!ObjectBelowConveyor(i)){
			SetXDir(GetXDir(i)-Speed/3,i);
		}
	}
}

private func ObjectsOnTop(){
	return(FindObjects(Find_OnLine(-34,-10,34,-10),Find_Not(Find_Category(C4D_Structure)),Find_Not(Find_Category(C4D_StaticBack)), Find_Not(Find_ID(BRDG)), Find_Not(Find_ID(CNVY))));
}

private func ObjectsBelow(){
	return(FindObjects(Find_OnLine(-34,10,34,10),Find_FuncEqual("GetProcedure()","HANGLE")));
}

public func ObjectOnTop(pObj){
	//if(Contained(pObj)) return(0);
	if(GetAction() != "Movement") return(0);
	var OnTop = ObjectsOnTop();
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
		x -= 5;
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
		x += 5;
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