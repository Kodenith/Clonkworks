/*-- Fundament --*/

#strict

local basement;
local pFather;
local iDynBasWidth;

/* BAS7 stellt die Grundfunktionalität für alle Fundament-Objekte sowie 
   abgeleitete Gebäude-Objekte zur Verfügung. */

private func BasementID() { return(DBAS); }
private func BasementWidth() { return(26); }
private func BuildingWidth() { return(50); }

/*-- Fundament-Objekte --*/

/* Fertigstellung */

protected func Initialize()
{
  if(GetID() != BasementID() ) return(0);
  Schedule("DynBasCreateBasements()",1,0);
  SetAction("Consolidate");
  MoveOutClonks();
  return(_inherited() );
}

private func Consolidated()
{
  if(GetID() != BasementID() ) return(0);
  
  // Das Fundament ist fertig und abgesunken: in den Hintergrund (fixieren)
  SetCategory(1);
}

public func Consolidate()
{
  return(SetCategory(1));
}

/* Schaden / Zerstörung */

protected func Damage(iChange, iByPlayer)
{
  if(GetID() != BasementID() ) return(_inherited(iChange, iByPlayer) );
  if(GetDamage() < 150) return(_inherited(iChange, iByPlayer) );

  CastObjects(ROCK,4,15,0,-5);
  RemoveObject();
  
  return(_inherited(iChange, iByPlayer) );
}
  
/* Feststeckende Clonks und andere Objekte rauswerfen */

private func MoveOutClonks() {
  var pObj;
  if(GetID()!=BasementID()) return(0);
  while(pObj=FindObject(0,-BasementWidth()/2-10,-15,BasementWidth()+20,25,OCF_Collectible() | OCF_Grab() | OCF_Alive(),0,0,NoContainer(),pObj))
    if(!DefinitionCall(GetID(pObj),"IsBridge"))
      if (~GetCategory(pObj) & C4D_Structure)
        MoveOutClonk(pObj);
}
  
private func MoveOutClonk(object pObj) {
  if(GetID()!=BasementID()) return(0);
  while(Stuck(pObj) && Inside(GetY(pObj)-GetY(),-15,+5))
    SetPosition(GetX(pObj),GetY(pObj)-1,pObj);  
}
  
/*-- Gebäude-Objekte --*/

/* Fundament */

protected func Construction()
{
  if(GetID() == BasementID() ) return(0);
  if(!basement) basement = CreateObject(BasementID(), ( (BuildingWidth()/2) * (-1)) +13, 8, GetOwner() );
  basement->DynBasTakeWidth(BuildingWidth());
  return(_inherited() );
}
  
protected func Destruction()
{
  if(GetID() == BasementID() ) return(KillChildren());
  if(basement) RemoveObject(basement);
  return(_inherited() );
}



/* Neue Funktionen */

public func DynBasCreateBasements()
{
  if(pFather) return;
  var basecount;
  var counter;
  var child;
  basecount = iDynBasWidth/26;
  counter = 1;
  while(counter != basecount) {
  child = CreateObject(BasementID(), 26*counter, 4, GetOwner() );
  child->DynBasSetFather(this);
  counter++;
  }
  child = CreateObject(BasementID(), iDynBasWidth -26, 4, GetOwner() );
  child->DynBasSetFather(this);
  return(1);
}

public func DynBasSetFather()
{
  pFather = Par(0);
  return(1);
}

public func DynBasTakeWidth()
{
  iDynBasWidth = Par(0);
  return(1);
}

public func KillChildren()
{
  if(pFather) return;
  for(var pObj in FindObjects(Find_ID(GetID()),Find_Distance(iDynBasWidth)  ))
    if(LocalN("pFather",pObj) == this) RemoveObject(pObj);
  return(1);
}

