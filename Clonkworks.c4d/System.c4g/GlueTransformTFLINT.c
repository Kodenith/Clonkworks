/*-- Neues Script --*/

#strict 2
#appendto TFLN
#appendto STFN

public func TransformInGlue(){
  if(GetID() == TFLN) return(1);
  if(GetID() == STFN) return(1);
}
public func InGlueDef(){
  if(GetID() == TFLN) return(TFLS);
  if(GetID() == STFN) return(STFS);
}