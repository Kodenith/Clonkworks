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

local Extension;

protected func Initialize() {
	//TargetSpeed = 60;
	Deaccel = 1;
	RequireRotor = 1;
  // Im Freien erzeugte Brückensegmente sind verankert
  if (!Contained())
    if(!ControlLeftDouble() && !ControlRightDouble()) Lock(1, 1);
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
	
	//desnow
	//if(!FindObject(STSN)) DigFreeRect(GetX()-43,GetY()-7,86,2); <- this lags!
	
	//above
	var OnTop = ObjectsOnTop();
	if(OnTop && GetLength(OnTop) != 0) 
	for(var i in OnTop){
		if(ObjectOnOtherConveyor(i) && ObjectNumber() < ObjectNumber(ObjectOnOtherConveyor(i))) continue;
		if((GetCategory(i) & C4D_Living) && FindObject(WKCV)) continue;
		if(i->~IgnoreConveyor()) continue;
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
		if((GetCategory(i) & C4D_Living) && FindObject(WKCV)) continue;
		var wasStuckBefore = Stuck(i);
		if(Contained(i)) continue;
		
		if(!wasStuckBefore)
			SetX(GetX(i)-iX,i);
		
		if(Stuck(i) && !wasStuckBefore)
			SetX(GetX(i)-(-iX/4),i);
		
		if(!ObjectBelowConveyor(i)){
			i->SetAction("Tumble");
			SetXDir(GetXDir(i)-Speed/3,i);
		}
	}
}

private func ObjectsOnTop(){
	var Objects = FindObjects(Find_OnLine(-42,-10,41,-10),Find_Not(Find_Category(C4D_Structure)),Find_Not(Find_Category(C4D_StaticBack)), Find_Not(Find_ID(BRDG)), Find_Not(Find_Func("IsConveyor")),Find_Not(Find_OCF(OCF_CrewMember)),Find_NoContainer(),Find_Not(Find_Func("IsTree")));
	var Clonks =  FindObjects(Find_OnLine(-34,-10,34,-10),Find_OCF(OCF_CrewMember),Find_NoContainer());
	var Trees =  FindObjects(Find_OnLine(-18,-10,18,-10),Find_Func("IsTree"),Find_Category(C4D_Vehicle),Find_NoContainer());
	ArrayAddArray(Objects,Clonks,1);
	ArrayAddArray(Objects,Trees,1);
	return(Objects);
}

private func ObjectsBelow(){
	return(FindObjects(Find_OnLine(-35,10,35,10),Find_FuncEqual("GetProcedure()","HANGLE"),Find_NoContainer()));
}

public func ObjectOnTop(pObj){
	//if(Contained(pObj)) return(0);
	if(GetAction() != "Movement") return(0);
	var OnTop = ObjectsOnTop();
	if(InArray(pObj,OnTop) != -1) return(1);
}

global func ObjectOnConveyor(pObj){
	for(var i in FindObjects(Find_Func("IsConveyor"))){
		if(i->ObjectOnTop(pObj)) return(1);
	}
	return(0);
}

public func ObjectOnOtherConveyor(pObj){
	for(var i in FindObjects(Find_Func("IsConveyor"),Find_Exclude(this()))){
		if(i->ObjectOnTop(pObj)) return(i);
	}
	return(0);
}

public func ObjectBelow(pObj){
	//if(Contained(pObj)) return(0);
	if(GetAction() != "Movement") return(0);
	var Below = FindObjects(Find_OnLine(-38,10,38,10),Find_FuncEqual("GetProcedure()","HANGLE"),Find_NoContainer());
	if(InArray(pObj,Below) != -1) return(1);
}

global func ObjectBelowConveyor(pObj){
	for(var i in FindObjects(Find_Func("IsConveyor"))){
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
		x -= 43;
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
		x += 43;
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
/*
public func ALKConnectType(){
	if(Par(0) == 0) return(0);
	for(var i in FindObjects(Find_ID(GERL))){
		if(i->IsConvSet(this())) return(0);
	}
	return([GERL]);
}
*/

//new gear line connection func
public func IsRotorTarget(){ return(1); }

//packaging
public func ControlUpDouble(pClonk){
	[$TxtPackage$]
	if(GetProcedure(pClonk) != "PUSH") return(0);
	if(IsLocked()) return(0);
	CreateObject(PCNV)->DoDamage(GetDamage());
	Sound("Connect");
	RemoveObject();
}

func NotLocked(){ return(!IsLocked()); }

public func ContextPackage(pClonk){
	[$TxtPackage$|Image=PCNV|Condition=NotLocked]
	AddCommand(pClonk,"Call",this(),pClonk,0,0,38*999,"ControlUpDouble");
	AddCommand(pClonk,"Grab",this());
}

//snap
public func ControlLeftDouble(pClonk){
	[$TxtSnapLeft$]
	if(IsLocked()) return(0);
	var Neighbour;
	if(Neighbour = FindObject2(Find_Func("IsLocked"),Find_ID(GetID()),Find_OnLine(-95,0,-86,0),Find_NoContainer())){
		SetPosition(GetX(Neighbour)+(Neighbour->ConveyorWidth()-1),GetY(Neighbour));
		Lock(0,1);
		return(1);
	}
}

public func ControlRightDouble(pClonk){
	[$TxtSnapRight$]
	if(IsLocked()) return(0);
	var Neighbour;
	if(Neighbour = FindObject2(Find_Func("IsLocked"),Find_ID(GetID()),Find_OnLine(86,0,95,0),Find_NoContainer())){
		SetPosition(GetX(Neighbour)-(Neighbour->ConveyorWidth()-1),GetY(Neighbour));
		Lock(0,1);
		return(1);
	}
}

//malfunction
public func Malfunction(){
	Speed = 0;
}

//detachment
public func CanBeDetached(){ return(is_locked && !Extension); }
public func OnDetach(){
	Release(0,1);
}

//cleanup
func Clean(){
	if(!IsLocked()) return(0);
	DigFreeRect(GetX()-43,GetY()-10,86,8);
}

//new funcs for better conveyor logic
public func IsConveyor(){ return(1); }
public func ConveyorWidth(){ return(GetDefWidth(GetID())); }