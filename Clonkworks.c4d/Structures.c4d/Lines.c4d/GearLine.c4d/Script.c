/*--- Zuflussrohr ---*/

#strict
protected func Initialize()
{
  Local(0) = 7;
  Local(1) = 2;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  if(Contained()) Exit();
  var Rotor = GetActionTarget(0);
  var MainBelt = GetActionTarget(1);
  if(GetID(MainBelt) != CNVY) return(0);
  var Total = AffectedConveyors();
  
  for(var i in Total){
	  i->SetConveyorSpeed(Rotor->GetRotorForce());
  }
}

public func AffectedConveyors(){
  var MainBelt = GetActionTarget(1);
  var Total = [];
  ArrayAddArray(Total,MainBelt->GetNeighboursLeft(7),1);
  ArrayAddArray(Total,MainBelt->GetNeighboursRight(7),1);
  return(Total);
}

public func IsConvSet(pObj){
	if(GetID(GetActionTarget(1)) != CNVY) return(0);
	var List = AffectedConveyors();
	if(InArray(pObj,List) != -1) return(1);
}

public func KitType(){ return(FNKT); }
