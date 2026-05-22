/*-- Geyser Pump --*/

#strict 2
#include BAS3
#include IO__

local Extract; //Either the string name of the material, or the id of the gas.
local Amount; //Amount of material or gas. if it runs out it stops extracting.
local IBmax; //Used as the max value for infobar.
local TempGey; //No, this doesnt temporarily store a gay person. it stores the geyser its placed on to be later deleted.

//wiring
public func InputList(){
  return(["Lock"]);
}

//this function returns a list of string that can be output.
//default is a single output.
public func OutputList(){
  return(["Extracted Material/Gas","Material/Gas Left"]);
}

//used by wire, check if a certain output is active
public func OutputActive(string OutputName){
    if(OutputName == "Extracted Material/Gas"){
      return(Extract);
    }

    if(OutputName == "Material/Gas Left"){
      return(Amount);
    }
}

//custom placement requirements
public func RejectConstruction(iX,iY,pBuilder){
	var LiquidGeyser = FindObject2(Find_AtPoint(GetX(pBuilder),GetY(pBuilder)),Find_ID(GEY1));
  var GasGeyser = FindObject2(Find_AtPoint(GetX(pBuilder),GetY(pBuilder)),Find_ID(GEY2));

  if(!LiquidGeyser && !GasGeyser){
    Message("<c ff0000>$Warning1$</c>",pBuilder);
    pBuilder->Sound("Error");
    return(1);
  }
	
	return(0);
}

protected func Construction(){
  var LiquidGeyser = FindObject2(Find_Or(Find_OnLine(-24,0,24,0),Find_OnLine(-24,-6,24,-6)),Find_ID(GEY1));
  var GasGeyser = FindObject2(Find_Or(Find_OnLine(-24,0,24,0),Find_OnLine(-24,-6,24,-6)),Find_ID(GEY2));

  if(LiquidGeyser){
    Extract = LocalN("Liquid",LiquidGeyser);
    Amount = RandomX(20,100)*200;  //now its random to be consistant with deposits.
    TempGey = LiquidGeyser;
    SetX(GetX(TempGey));
  }else if(GasGeyser){
    Extract = LocalN("Gas",GasGeyser);
    Amount = RandomX(20,100)*50;
    TempGey = GasGeyser;
    SetX(GetX(TempGey));
  }else{
    Extract = 0;
    Amount = 0; //no data, machine does not function.
  }

  IBmax = Amount;

  return(_inherited());
}

func Initialize(){
  if(TempGey) RemoveObject(TempGey);
  SetGraphics("Shortcut",this,GetID(),1,1);
  AddEffect("Desc",this,1,0,this,GetID());
}

public func SetLightColor(){
  if(!Extract || !Amount) return(RGBa(255,255,255,255));
  if(GetType(Extract) == C4V_String){
    var R = GetMaterialVal("Color","Material",Material(Extract),0);
    var G = GetMaterialVal("Color","Material",Material(Extract),1);
    var B = GetMaterialVal("Color","Material",Material(Extract),2);

    return(RGBa(R,G,B));
  }

  return(Extract->~GasColor());
}

protected func Manage(){
  if(GetCon() < 100) return(0);
  //Manage action based on energy and data.
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  var CanPump = (Extract != 0 && Amount > 0 && EnergyCheck(10000) && GetCon() >= 100 && pDrainPipe && !InputActive("Lock"));
  if(CanPump){
    if(GetAction() == "Idle") SetAction("Pumping");
  }else{
    if(GetAction() != "Idle") SetAction("Idle");
  }

  SetClrModulation(SetLightColor(),this,1);
}

protected func PumpOut(){
  //pump out either liquid or gas. both are their own functions.
  if(GetType(Extract) == C4V_String){
    PumpLiquid();
    if(FindObject(EGY1)) Amount = IBmax;
  }else if(GetType(Extract) == C4V_C4ID){
    PumpGas();
    if(FindObject(EGY2)) Amount = IBmax;
  }

  if(GetPhase() == 19) Sound("GPump1");
  if(GetPhase() == 39) Sound("GPump2");

  if(!Amount){
    Sound("Discharge");
    Message("$Warning2$",this);
  }
}

//Liquid Pumping
protected func PumpLiquid(){
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  var pPumpTarget = GetActionTarget(1, pDrainPipe);

  if(!pDrainPipe) return(0);

  //Line kits simply drop pixels around.
  if(GetID(pPumpTarget) == LNKT){
    pPumpTarget->CastPXS(Extract,1,1);
    Amount--;
    return(0);
  }

  //containers have their barrels filled.
  if(GetOCF(pPumpTarget) & OCF_Container || pPumpTarget->~GeyserLiquidTo()){
      //check for barrels that aren't empty.
      var Set = 0;
      var BarrelType = GetBarrelType(Material(Extract));
      if(BarrelType){
        for(var i in FindObjects(Find_ID(BarrelType),Find_Container(pPumpTarget))){
          if(!i->BarrelIsFull()){
            i->BarrelDoFill(1,Material(Extract)+1);
            Amount--;
            Set=1;
            break;
          }

        }

      }else{  
        return(0);
      }

      if(Set) return(0);

      //otherwise fill an empty barrel
      var Barrel = FindContents(BARL,pPumpTarget);
      if(Barrel){
        ChangeDef(BarrelType,Barrel);
        Barrel->BarrelDoFill(1,Material(Extract));
      }
  }
}

//Gas Pumping
//i was too lazy to change variable names here :P
protected func PumpGas(){
  if(FrameCounter()%5 != 0) return(0); //Artificial slowdown
  var pDrainPipe = FindObject(DPIP,0,0,0,0,0,"Connect",this());
  var pPumpTarget = GetActionTarget(1, pDrainPipe);

  if(!pDrainPipe) return(0);

  //Line kits simply spew gas.
  if(GetID(pPumpTarget) == LNKT){
    pPumpTarget->CreateObject(Extract);
    Amount--;
    return(0);
  }

  //containers have their canisters filled
  if(GetOCF(pPumpTarget) & OCF_Container || pPumpTarget->~GeyserGasTo()){
      //check for canisters that aren't empty
      var Set = 0;
      var BarrelType = GetCanisterType(Extract);
      if(BarrelType && BarrelType != GetID(this)){
        for(var i in FindObjects(Find_ID(BarrelType),Find_Container(pPumpTarget))){
          if(!i->IsCanisterFull()){
            i->CanisterDoFill(1,Extract);
            LocalN("iFillType",i) = Extract;
            Amount--;
            Set=1;
            break;
          }

        }

      }else{  
        return(0);
      }

      if(Set) return(0);

      //otherwise fill an empty canister
      var Barrel = FindContents(GCAN,pPumpTarget);
      if(Barrel){
        Barrel->ChangeDef(GetCanisterType(Extract));
        Barrel->CanisterDoFill(1,Extract);
        LocalN("iFillType",Barrel) = Extract;
      }
  }
}

public func GetResearchBase() { return(PUMP); }

func Damage(){
  if(GetDamage() > 100) Incinerate();
}

func Incineration(){
  //explode everything inside before burning.
  if(Amount != 0){
  if(GetType(Extract) == C4V_String){
    TempGey = CreateObject(GEY1,0,18);
    LocalN("Liquid",TempGey) = Extract;
    TempGey->DoDamage(200);
  }

  if(GetType(Extract) == C4V_C4ID){
    TempGey = CreateObject(GEY2,0,18);
    LocalN("Gas",TempGey) = Extract;
    TempGey->DoDamage(200);
    CastObjects(Extract,RandomX(80,180));
  }
  }

  if(basement) RemoveObject(basement);
  ChangeDef(DGPP);
}

/* INFOBAR */
public func InfobarTrigger(){
  if(Extract)
	  return(IB_Grab);
}

public func InfobarMax(){
	return(IBmax);
}

public func InfobarValue(){
	return(Amount);
}

public func InfobarColor(){
  if(GetType(Extract) == C4V_String){
    var R = GetMaterialVal("Color","Material",Material(Extract),0);
    var G = GetMaterialVal("Color","Material",Material(Extract),1);
    var B = GetMaterialVal("Color","Material",Material(Extract),2);

    return(RGBa(R,G,B));
  }

  return(Extract->~GasColor());
}

public func InfobarInfo(){
  var ExtractWhat;
  if(GetType(Extract) == C4V_String) ExtractWhat = Concat2(Format("{{%i}}",GetBarrelType(Material(Extract))),Extract);
  else if(GetType(Extract) == C4V_C4ID) ExtractWhat = Concat2(Format("{{%i}}",Extract),GetName(,Extract));

  if(Amount)
	return(Format("$Info$",ExtractWhat));
  else return("$Warning2$");
}

public func IsOilGeyser(){
  if(GetCon() < 100) return(0);
  if(Extract == "Oil" && Amount > 0) return(1);
}

public func IsNaturalGasGeyser(){
  if(GetCon() < 100) return(0);
  if(Extract == GS_2 && Amount > 0) return(1);
}

public func ControlThrow(pClonk){
  [$Fill$|Image=GCAN]
  var Canister = Contents(0,pClonk);
  if(GetID(Canister) != GCAN || GetType(Extract) != C4V_C4ID || Amount == 0) return(0);
  if(!GetCanisterType(Extract)) return(0);
  var AmountToGive = BoundBy(Amount,0,50);
  Amount -= AmountToGive;
  Canister->TransformCanister(AmountToGive,Extract);
  Sound("Fuse");
  return(1);
}


/* EFFECT
for displaying data in the description. no more, no less. */

public func FxDescInfo(target,effectnum){
  if(GetType(Extract) == C4V_String){
    var cId = RMMG; //random magic id, its a bunch of questionmarks.
    if(GetBarrelType(Material(Extract))) cId = GetBarrelType(Material(Extract)); //id fo a barrel if it exists

    return(Format("$TxtPumping$",cId,Extract));
  }else if(GetType(Extract) == C4V_C4ID){
    return(Format("$TxtPumping$",Extract,GetName(,Extract)));
  }

  return("$Warning2$");
}
