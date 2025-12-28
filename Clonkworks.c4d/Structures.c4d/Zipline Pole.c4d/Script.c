/*-- Neues Script --*/

#strict 2
#include BAS1
#include CXEC

func Initialize() {
  SetEntrance(0);
  return(1);
}

func ActivateEntrance(pObj){
	if(GetID(pObj) == CK5P) return(1);
	if(!EnergyCheck(200)){
		return(0);
	}
	
	if(!FindObject2(Find_ID(ZIPL),Find_ActionTarget(this()))){
		Message("$Err1$",this());
		Sound("Error");
			if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
		return(0);
	}
	
	var Line = FindObject2(Find_ID(ZIPL),Find_ActionTarget(this()));
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
	
	for(var i in FindObjects(Find_ID(ZIPT))){
		if(LocalN("Dragged",i) == pObj) return(0);
	}
	
	var Zip = CreateObject(ZIPT,0,16);
	LocalN("Line",Zip) = Line;
	LocalN("memorizedCount",Zip) = GetVertexNum(Line);
	LocalN("Dragged",Zip) = pObj;
	var Rope = FindContents(CK5P);
	Rope->ConnectObjects(Zip,pObj);
	
	if(GetCommand(pObj,0) == "Enter")
		pObj->FinishCommand(,true);
	return(0);
}

public func ALKConnectType(){
	if(FindObject2(Find_ID(ZIPL),Find_ActionTarget(this())) && Par(0) == 0) return(0);
	return([ZIPL]);
}