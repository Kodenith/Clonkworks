#strict 2
local Filter;

func Hit(){
  Sound("WoodHit*");
}

func IsAnvilProduct(){ return(1); }

public func Activate(pHaz){
  [$TxtUse$]
  if(GetID(pHaz) != CLNK){
    Message("$Warn1$",pHaz);
    pHaz->Sound("CommandFailure1");
    return(1);
  }

  if(GetAction(pHaz) != "Walk"){
    Message("$Warn2$",pHaz,GetName(pHaz));
    pHaz->Sound("CommandFailure1");
    return(1);
  }

  MorphTo(HZMC,pHaz);
  SetXDir(0,pHaz);
  pHaz->SetAction("PutOnArmor");
  LocalN("Filter",pHaz) = Filter;
  RemoveObject();
  return(1);
}