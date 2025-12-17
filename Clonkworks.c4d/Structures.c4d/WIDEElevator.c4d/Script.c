/*-- Fahrstuhl --*/

#strict

#include CXEC
#include ELEV
#include EBS2

protected func Initialize()
{
  pCase = CreateObject(CELC,+0,+27,GetOwner());
  SetObjectOrder(this(), pCase);
  SetAction("LiftCase",pCase);
  return(1);
}
