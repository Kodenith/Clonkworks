#include GCAN
#strict 2

public func CanisterGas() { return(GS_2); }
public func CanisterGasName() { return("Natural"); }

global func GetCanisterType(iType)
{
  if(iType == GS_2) return(NCAN);
  return(_inherited(iType));
}

func Initialize(){
  iFillLevel=50;
  iFillType=GS_2;
}

public func CalcValue(){
  return(9+(iFillLevel/3));
}

public func FurnaceFuelValue(){
		if(this)
			return LocalN("iFillLevel")/10;
		else 
			return 20;
}

func BurnUp(pConsumer) {
 ChangeDef(GCAN,this);
 return 1;
}

public func Sale(plr) 
{
  var obj;
  if (obj = FindObject (GAEX, 50 - GetX (), 49 - GetY ()))
    obj -> GAEX::PlayerHasSold (plr, this ());
}

public func Purchase(int plr, object buyobj) 
{
  var obj;
  if (obj = FindObject (GAEX, 50 - GetX (), 49 - GetY ()))
    obj -> GAEX::PlayerHasBought (plr, this ());
}
