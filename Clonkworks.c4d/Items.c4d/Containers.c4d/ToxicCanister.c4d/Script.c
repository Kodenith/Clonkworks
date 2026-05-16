#include GCAN
#strict 2

public func CanisterGas() { return(GS_1); }
public func CanisterGasName() { return("Toxic"); }

global func GetCanisterType(iType)
{
  if(iType == GS_1) return(TCAN);
  return(_inherited(iType));
}

func Initialize(){
  iFillLevel=50;
  iFillType=GS_1;
}

public func CalcValue(){
  return(8+(iFillLevel/25));
}