/* GAS STORAGE by Kodenith */
#strict 2

//Changeable value, how much gas can it hold?
static const GCAN_MaxFill = 50;

//from barrel.
local iFillLevel;
local iFillType;
global func EmptyCanisterID() { return(GCAN); }

protected func SellTo()
{
  if (BarrelConfiguration() & 2) return(GetID());
  return(EmptyCanisterID());
}

public func GetAmount() { return(iFillLevel); }
public func CanisterGas() { return(0); }
public func CanisterGasName() { return(0); }
public func CanisterMaxFill() { return(GCAN_MaxFill); }
public func IsCanisterFull(){ return(iFillLevel == CanisterMaxFill()); }

public func CanisterDoFill(iChange,Type){
  if(Type != iFillType && iFillType) return(0);
  iFillLevel += iChange;
  if(iFillLevel > GCAN_MaxFill) iFillLevel = GCAN_MaxFill;
  if(GetID() == GCAN && GetCanisterType(Type)){
      ChangeDef(GetCanisterType(Type));
      iFillType = Type;
  }
  if(iFillLevel <= 0){
    if(GetID() != GCAN) ChangeDef(GCAN);
    iFillType = 0;
    iFillLevel = 0;
  }
  return(iFillLevel);
} 

protected func Hit()
{
  Sound("ClonkHit*");
  BarrelEject();
  return(1);
}

private func BarrelEject()
{
  if(!iFillType) return(0);
  CreateContents(iFillType,this,iFillLevel);
  CanisterDoFill(-GCAN_MaxFill);
  Sound("Fusing");

  iFillLevel = 0;
  iFillType = 0;
  return(1);  
}

public func IsAnvilProduct() { return(1); }
//end

protected func Release(){
  if(iFillLevel > GCAN_MaxFill) iFillLevel = GCAN_MaxFill;
  if(iFillLevel < 0) iFillLevel = 0;
  if(iFillLevel == 0 && GetID() != GCAN) ChangeDef(GCAN);
  if(Contents())
    Exit(Contents());
}

protected func Activate()
{
  [$TxtEject$]
  return(BarrelEject());
}

public func Incineration(){
    if(iFillType) CastObjects(iFillType,iFillLevel);
    Explode(10);
}

public func IncinerationEX(){
    if(iFillType) CastObjects(iFillType,iFillLevel);
    Explode(10);
}

//the code for this sucks, so i made a solution, excuse me for my horrid code
public func TransformCanister(iLevel,FillType){
  var Canist=GetCanisterType(FillType);
  if(!Canist) return(0);
  iFillLevel = iLevel;
  iFillType = FillType;
  ChangeDef(Canist);
  return(1);
}

public func GetResearchBase(){ return(GPUP); }