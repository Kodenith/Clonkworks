/*-- Neues Script --*/

#strict 2
#include BAS1
#include CXEC

local Connector;

func Initialize() {
  SetEntrance(0);
  Connector = CreateObject(ZIPC);
  Connector->SetAction("Attach",this());
  Connector->SetActionData(256*0+0);
  return(1);
}

func ActivateEntrance(pObj){
	if(GetID(pObj) == CK5P) return(1);
	if(!EnergyCheck(200) || !Connector){
		return(0);
	}
	
	if(!FindObject2(Find_ID(ZIPL),Find_ActionTarget(Connector))){
		Message("$Err1$",this());
		Sound("Error");
			if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
		return(0);
	}
	
	var Line = FindObject2(Find_ID(ZIPL),Find_ActionTarget(Connector));
	if(GetID(GetActionTarget(1,Line)) == FNKT){
		Message("$Err1$",this());
		Sound("Error");
			if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
		return(0);
	}

	if(!FindContents(CK5P)){
		Message("$Err2$",this());
		Sound("Error");
			if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
		return(0);
	}
	
	//for now its a rock
	var Zip = CreateObject(ZIPT,0,2);
	LocalN("Line",Zip) = Line;
	var Rope = FindContents(CK5P);
	Rope->ConnectObjects(Zip,pObj);
	
	if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
	return(0);
}

public func ALKConnectType(){
	if(FindObject2(Find_ID(ZIPL),Find_ActionTarget(Connector)) && Par(0) == 0) return(0);
	return([ZIPL]);
}

public func Recollection(){
	for(var zip in FindObjects(Find_ID(ZIPL))){
		if(GetActionTarget(0,zip) == this()){
			SetActionTargets(Connector,GetActionTarget(1,zip),zip);
		}else if(GetActionTarget(1,zip) == this()){
			SetActionTargets(GetActionTarget(0,zip),Connector,zip);
		}
		
		if(GetActionTarget(1,zip) == Connector && GetActionTarget(0,zip) == Connector){
			var kit = CreateObject(FNKT);
			SetActionTargets(GetActionTarget(0,zip),kit,zip);
			//kit->Enter(pClonk);
		}
	}
}

public func ControlUp(pClonk){
	[$Disc$|Image=ZIPL]
	var zip;
	if(zip = FindObject2(Find_ID(ZIPL), Find_ActionTarget(Connector))){
		if(GetID(GetActionTarget(1,zip)) != FNKT) 	
			CreateObject(FNKT);
		RemoveObject(zip);
		Sound("Connect");
	}
	return(1);
}

func Destruction(){
	if(Connector)
	RemoveObject(Connector);
}

func Incineration(){
		if(Connector)
	RemoveObject(Connector);
}