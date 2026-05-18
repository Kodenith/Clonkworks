/*-- Neues Script --*/

#strict 3
#include IO__
#include CXEC

public func WireFrom(){ return(0); }
public func GeyserLiquidTo(){ return(1); }

//this function returns a list of string that can be output.
//default is a single output.
public func InputList(){
  return(["Sprinkle"]);
}

func Damage(){
  if(GetDamage() > 50 && !Locked) Explode(16);
}
func GetResearchBase(){ return(CC1_); }

local Barrel;

func Initialize(){
  Barrel = CreateContents(BARL);
}

func Update(){
  if(GetCon() < 100) return(nil);
  if(!Barrel) Barrel = CreateContents(BARL);

  var amt = Barrel->GetAmount();
  if(!amt) amt = 0;

  if(amt <= 0 && GetID(Barrel) != BARL) ChangeDef(BARL,Barrel);
  if(InputActive("Sprinkle")){
    if(GetAction() == "Idle" && EnergyCheck(500) && Barrel->GetAmount()) SetAction("Sprinkle");
    TryDrain();
  }else{
    SetAction("Idle");
  }
}

func TryDrain(){
  var SourcePipe = FindObject(SPIP,0,0,0,0,0,"Connect",this());
  if(!SourcePipe) return(nil);
  var PumpSource = GetActionTarget(1, SourcePipe);
  if (!PumpSource || (GetID(PumpSource)!=LNKT)) return(nil);

  var BarrelMaterial = Barrel->~BarrelMaterial();
  var SourceMaterial = GetBarrelType(GetMaterial(AbsX(GetX(PumpSource)),AbsY(GetY(PumpSource))));
  if(!SourceMaterial) return(nil);

  var mat = GetMaterial(AbsX(GetX(PumpSource)),AbsY(GetY(PumpSource)));
  if(BarrelMaterial != -1 && BarrelMaterial != mat) return(nil);
  var Amount = Barrel->GetAmount();
  if(!Amount) Amount = 0;
  var ToExtract = BoundBy(1,0,200-Amount);
  if(ToExtract <= 0) return(nil);
  var Extracted = ExtractMaterialAmount(AbsX(GetX(PumpSource)),AbsY(GetY(PumpSource)),mat,ToExtract);

  if(BarrelMaterial == -1) ChangeDef(GetBarrelType(mat),Barrel);
  Barrel->BarrelDoFill(Extracted,mat);
}

func DoSprinkling(){
  if(GetPhase() == 0 && !Barrel->GetAmount()) return(SetAction("Idle"));
  if(!EnergyCheck(500)) return(SetAction("Idle"));
  if(Barrel->BarrelMaterial() == Material("Oil")) return(DoOilBurn());

  var Times = 15;
  if(Barrel->~BarrelMaterial() == -1) return(nil);
  while(Times--){
    if(Barrel->GetAmount() == nil || Barrel->GetAmount() <= 0) break;
    var mat = Barrel->~BarrelMaterial();
    if(mat == Material("Ice")) return(0);
    LocalN("iFillLevel",Barrel)--;
    CastPXS(MaterialName(mat),1,50);
  }
}

func DoOilBurn(){
  if(Random(28)) return(nil);
  if(!Barrel->GetAmount()) return(nil);
  var FireSize = 2*BoundBy(Barrel->GetAmount(),10,RandomX(35,50));
  LocalN("iFillLevel",Barrel)-=FireSize/2;

  var Flame = CreateObject(FLAM);
  Fling(Flame,RandomX(-4,4),RandomX(-2,-7));
  Flame->SetCon(FireSize);
  return(1);
}