/*--- Zuflussrohr ---*/

#strict 3
protected func Initialize()
{
  if(!FindObject(IVWR)){
  Local(0) = 343;
  Local(1) = 2;
  }
	
  SetAction("Connect");  
  SetVertex(0, 0, GetX()); SetVertex(0, 1, GetY());
  SetVertex(1, 0, GetX()); SetVertex(1, 1, GetY());
  SetPosition(0, 0, this());
}

local OutputString;
local InputString;

func NoDisconnection(){ return(1); }

public func GetOutputValue(){
    var Source = GetActionTarget(0);
    return(Source->~OutputActive(OutputString));
}

public func ConnectedTo(){ return(InputString); }
public func ConnectedFrom(){ return(OutputString); }