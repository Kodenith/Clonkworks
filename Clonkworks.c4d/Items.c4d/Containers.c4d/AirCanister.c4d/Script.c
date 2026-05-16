#include GCAN
#strict 2

public func CanisterGas() { return(GS_3); }
public func CanisterGasName() { return("Air"); }

global func GetCanisterType(iType)
{
  if(iType == GS_3) return(PCAN);
  return(_inherited(iType));
}

func Initialize(){
  iFillLevel=50;
  iFillType=GS_3;
}