/*-- Neues Script --*/

#strict 2
#include LWPF

public func UpdateGraphic() {
  SetAction("Attach");
}

protected func UpdateRotation() {
  var lorry = GetActionTarget();
  // Lore ist verschwunden -> entfernen
  if (!lorry) RemoveObject();
  // Sonst Rotation anpassen
  else SetR(GetR(lorry));
  
  //push out all objects inside
  var oth;
  while(oth = FindOtherContents(BUMB,lorry)){
	  Exit(oth,0,-5);
	  Fling(oth,RandomX(-3,3),RandomX(-1,-3));
  }
}

func Incineration(){
  var oth;
  var lorry = GetActionTarget();
  if(oth = FindContents(BUMB,lorry)) Incinerate(oth);
}