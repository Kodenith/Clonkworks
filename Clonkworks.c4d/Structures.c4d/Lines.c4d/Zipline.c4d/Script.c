/*--- Zuflussrohr ---*/

#strict
protected func Initialize()
{
  Local(0) = 1;
  Local(1) = 2;
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

protected func Transfer()
{
  if(Contained()) Exit();
  var from = GetActionTarget(0);
  var to = GetActionTarget(1);
}

public func KitType(){ return(FNKT); }