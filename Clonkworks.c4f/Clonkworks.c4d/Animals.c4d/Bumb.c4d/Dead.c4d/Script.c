/*-- Toter Wipf --*/

#strict

/* Verwesung */

private func Decaying()
{
  DoCon(-4);
}
  
/* Aktionen von au�en */  
  
public func Eat(pBy)
{
  pBy->~Feed(10);
  RemoveObject();
  return(1);
}

