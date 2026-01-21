/*-- Beton --*/

#strict 2

protected func Hit()
{
  CastPXS("Rock",100,30);
  RemoveObject();
  return(1);
}

public func IsMixerProduct(){ return(1); }