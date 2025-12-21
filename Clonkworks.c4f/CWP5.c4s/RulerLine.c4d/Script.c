/*--- Zuflussrohr ---*/

#strict

protected func Initialize()
{
  Local(0) = 13;
  Local(1) = 13;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);
  
  //break after exceeding 50 units.
  var Units = ObjectDistance(from,to)/100;
  var UnitsDecimal = ObjectDistance(from,to)%100;
  
  Message("%d.%d ut|%d/28 vert",to,Units,UnitsDecimal,GetVertexNum()-2);
}

public func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if (!fNoMsg) BreakMessage();
}

private func BreakMessage()
{
  var pPumpTarget = GetActionTarget(0);
  if (GetID(pPumpTarget) != FNKT)
    pPumpTarget = GetActionTarget(1);
  Message("$TxtLinebroke$", pPumpTarget);
}

public func KitType(){ return(FNKT); }
public func isLine(){ return(1); }

public func isOnLineTarget(pObj){
	var Targ = FindObject2(Find_NoContainer(),Find_OCF(OCF_LineConstruct),Find_AtPoint(AbsX(GetX(pObj)),AbsY(GetY(pObj))));
	if(!Targ) return(0);
	if(GetActionTarget(0) == Targ) return(1);
	if(GetActionTarget(1) == Targ) return(1);
	
	return(0);
}