/*-- Neues Script --*/

#strict 2

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

func Initialize(){
    Collector = CreateObject(CVS2,0,30);
    SetAction("Shaft",Collector);
}

local Collector;
func Move(){
  if(!Collector || !Parent || OnFire(Parent)){
	while(Contents(0,Collector)){
		Contents(0,Collector)->Exit();
	}
	RemoveObject(Collector);
    RemoveObject();
  }

  if(Contents(0,Collector)){
    Enter(this(),Contents(0,Collector));
    Contents()->Exit(,,5);
  }

}

func GetResearchBase(){ return(CNVY); }