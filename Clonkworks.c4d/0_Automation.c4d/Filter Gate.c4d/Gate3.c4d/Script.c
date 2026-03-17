/*-- Neues Script --*/

#strict 2
#include FLTG

private func Move(){
	SetFilterGraph();
	
	if(!AdditionalObject){
		OnChange();
	}
	
	if(GetY(AdditionalObject) < GetY()+20){
		SetY(GetY()+20,AdditionalObject);
		AdditionalObject->SetAction("Stop");
		AdditionalObject->SetComDir(COMD_Stop);
		SetXDir(0,AdditionalObject);
		SetYDir(0,AdditionalObject);
	}
}

func RejectCollect(pId,pObj){
	var Left;
	if(GetX(pObj) < GetX()) Left = true;
	Enter(this(),pObj);
	Sound("Clonk");
	
	var SideExit;
	if(FilterCompatibleItem(pObj) && !FilterPath) SideExit = true;
	if(FilterCompatibleItem(pObj) && FilterPath) SideExit = false;
	if(!FilterCompatibleItem(pObj) && !FilterPath) SideExit = false;
	if(!FilterCompatibleItem(pObj) && FilterPath) SideExit = true;
	
	TickOut();
	if(SideExit){
		var Item = pObj;
		if(Left) Exit(Item,14,12,,2);
		else Exit(Item,-14,12,,-2);
		if(GetDefFragile(GetID(Item))){
			SetXDir(0,Item);
			SetYDir(0,Item);
		}
	}else{
		Enter(AdditionalObject,pObj);
		pObj->Exit();
	}
	
	return(1);
}

func ControlThrow(pObj){
	[$TxtTurn$|Image=SG01:1]
	if(GetProcedure(pObj) != "PUSH") return(0);
	if(GetActionTarget(0,pObj) != this());
	Sound("SignalClick");
	if(!FilterPath){
		FilterPath = true;
		Message("$TxtTurnDown$",this());
	}else{
		FilterPath = false;
		Message("$TxtTurnLeft$|$TxtTurnRight$",this());
	}
}

public func ContextDoTurn(pClonk){
	[$TxtTurn$|Image=SG01:1|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlThrow");
	AddCommand(pClonk,"Grab",this());
}
public func SelectableGate(){
	return(1);
}

func Initialize() {
  if(!Filter)   Filter = 6;
  SetFilterGraph();
  if(GetID() == FLTG) ChangeDef(FTG1,this());
  
  OnChange();
  return(_inherited());
}

func OnChange(){
	AdditionalObject = CreateObject(G3P_);
	SetY(GetY(AdditionalObject)+30,AdditionalObject);
	var Connection = CreateObject(DPLI);
	LocalN("Source",AdditionalObject) = this();
	Connection->Init(this(),AdditionalObject);
}

func ControlUpSingle(pClonk){
	AdditionalObject->SetAction("Move");
	AdditionalObject->SetComDir(COMD_Up);
}

func ControlDownSingle(pClonk){
	AdditionalObject->SetAction("Move");
	AdditionalObject->SetComDir(COMD_Down);
}

func ControlDig(pClonk){
	AdditionalObject->SetAction("Stop");
	AdditionalObject->SetComDir(COMD_Stop);
	SetXDir(0,AdditionalObject);
	SetYDir(0,AdditionalObject);
}