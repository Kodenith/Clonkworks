/*-- Neues Script --*/

#strict 2
#include FLTG
#include FTG3
#include FTGN

func Initialize() {
  Amount = 0;
  if(!Filter) Filter = GOLD;
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

func RejectCollect(pId,pObj){
	var Left;
	if(GetX(pObj) < GetX()) Left = true;
	Enter(this(),pObj);
	Sound("Clonk");
	
	var SideExit;
	if(Amount && !FilterPath) SideExit = true;
	if(Amount && FilterPath) SideExit = false;
	if(!Amount && !FilterPath) SideExit = false;
	if(!Amount && FilterPath) SideExit = true;
	
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
	
	Amount--;
	if(Amount < 0) Amount = 0;
	
	return(1);
}

func ControlUpDouble(pObj){
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

public func ContextTurn(pClonk){
	[$TxtTurn$|Image=SG01:1|Condition=IsBuilt]
	AddCommand(pClonk,"Call",this(),0,0,0,0,"ControlUpDouble");
	AddCommand(pClonk,"Grab",this());
}

public func SelectableGate(){
	return(1);
}