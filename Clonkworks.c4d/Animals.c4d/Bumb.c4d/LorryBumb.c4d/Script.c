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
}