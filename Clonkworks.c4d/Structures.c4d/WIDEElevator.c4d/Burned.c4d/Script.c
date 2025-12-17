/*-- Verbrannter Fahrstuhl --*/

#strict

#include EBS2

local pCase;

protected func Incineration() { if(pCase) RemoveObject(pCase); }
protected func IncinerationEx() { if(pCase) RemoveObject(pCase); }
protected func Destruction() { if(pCase) RemoveObject(pCase); return(_inherited()); }
