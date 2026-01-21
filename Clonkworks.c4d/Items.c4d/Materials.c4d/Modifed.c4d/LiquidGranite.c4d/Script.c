/*-- Flüssig-Granit --*/

#strict 2

protected func Hit()
{
  CastPXS("Granite", 150, 30);
  RemoveObject();
  return(1);
}

public func IsMixerProduct(){ return(1); }
public func GetResearchBase() { return(CNCR); }