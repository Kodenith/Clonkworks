/*-- Fundament --*/

#strict

/* Alle Fundamente leiten ihre Grundfunktionalität von BAS7 ab */

#include BAS7

private func BasementID() { return(EXDB); }
private func BasementWidth() { return(84); }

protected func Damage(iChange, iByPlayer)
{
  if(GetID() != BasementID() ) return(_inherited(iChange, iByPlayer) );
  if(GetDamage() < 1000) return(_inherited(iChange, iByPlayer) );

  CastObjects(ROCK,4,15,0,-5);
  RemoveObject();
  
  return(_inherited(iChange, iByPlayer) );
}