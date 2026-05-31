#strict

public func Activate(caster, real_caster) {
  //SLog(caster); SLog(real_caster);
  var field = CreateObject(DTX_);
  if(GetCategory(caster) & C4D_Structure()){
    LocalN("Size",field) *= 2;
  }
  RemoveObject();
  return(1);
}


public func GetSpellClass(object pMage) { return(AIR1); }
public func GetSpellCombo(pMage) { return ("615"); } // (1: Backward; 2: Down; 3: Forward; 4: Throw; 5: Jump; 6: Dig)
