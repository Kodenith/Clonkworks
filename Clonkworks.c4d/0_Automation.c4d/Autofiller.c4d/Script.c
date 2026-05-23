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

public func CheckForSound(){
  if(!Parent || GetDamage() > 58){
    while(Contents()){
      Contents()->Exit();
    }
    Explode(30);
  }
  if(GetCon() < 100) return(0);

  if(GetAction() == "Idle" && CanMakeSound()){
      SetAction("Filling");
  }else if(GetAction() == "Filling" && !CanMakeSound()){
      SetAction("Idle");
  }
  
  DoBarrelKick();
}

public func DoBarrelKick(){
  for(var barl in FindObjects(Find_Container(this))){
    var kick = 0;
    if(!barl->~IsBarrel() && !barl->~IsCanister()) kick = 1;
    if(barl->~IsBarrel() && LocalN("iFillLevel",barl) == 200) kick = 1;
    if(barl->~IsCanister() && LocalN("iFillLevel",barl) == 50) kick = 1;

    if(kick) Exit(barl,GetCDir()*17,6);
  }
}

public func CanMakeSound(){
  var pass = 0;
  for(var i in FindObjects(Find_ID(DPIP))){
    if(GetActionTarget(1,i) == this) pass = 1;
  }

  if(!pass) return(0);
  if(Contents()) return(1);
}

protected func RejectCollect(idObj,pObj){
	if(GetCDir() == 0) return(1);
	if(GetCDir() == 1){
		if(GetX(pObj) < GetX()-6) return(0);
	}
	if(GetCDir() == -1){
		if(GetX(pObj) > GetX()+6) return(0);
	}
	
	return(1);
}

public func GeyserGasTo(){ return(1); }
public func GeyserLiquidTo(){ return(1); }

public func Collection(){
	Sound("Clonk");
}

func ChooseOrderItem(){
	var pClonk = Par(1);
	CreateMenu(GetID(),pClonk,this(),1);
	var x,i;
	while(x = GetDefinition(i++,C4D_Object)){
		if(!DefinitionCall(x,"IsAnvilProduct")) continue;
		if(!GetPlrKnowledge(GetController(pClonk),x)) continue;
		AddMenuItem("%s","SetFooItem",x,pClonk,0,pClonk);
	}
}

func AutoDepositHere(){ return(1); }

func AD_NeedItem(pStation){
	if(FindObject2(Find_ID(BARL),Find_Container(pStation))) return(BARL);
	if(FindObject2(Find_ID(GCAN),Find_Container(pStation))) return(BARL);
	return(0);
}

public func GetResearchBase(){ return(CNVY); }