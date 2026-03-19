#strict 2

protected func Initialize()
{
  SetPosition(0,0);
  return(1);
}
  
protected func CheckGlued(){
  //living things sink
  for(var i in FindObjects(Find_OCF(OCF_InLiquid),Find_OCF(OCF_Alive),Find_NoContainer())){
    if(i->GetMaterial() == Material("Glue") && !GetEffect("GlueSink",i)) AddEffect("GlueSink",i,109,4,this);
  }

  for(var i in FindObjects(Find_Func("TransformInGlue"))){
    if(i->GetMaterial() == Material("Glue")){
        ChangeDef(i->~InGlueDef(),i);
        i->ExtractMaterialAmount(0,0,Material("Glue"),15);
      }
  }
}

public func FxGlueSinkTimer(Target,EffectNum,Time){
  if(GetProcedure(Target) != "FLIGHT")
    if(Target->GetMaterial() != Material("Glue")) return(-1);
  if(InLiquid(Target)){
    var sinkAm = Time/38;
    SetYDir(GetYDir(Target)+sinkAm,Target);
  }
}